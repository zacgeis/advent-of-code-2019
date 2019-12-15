#include "../util.h"

int Gcd(int x, int y) {
  x = abs(x);
  y = abs(y);
  int smaller = x;
  if (y < smaller) smaller = y;
  int result = -1;
  for (int i = 1; i <= smaller; i++) {
    if (x % i == 0 && y % i == 0) {
      result = i;
    }
  }
  return result;
}

vector<tuple<int, int>> Find(vector<int>& map, int width, int height, int obj_x, int obj_y) {
  vector<tuple<int, int>> result;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (x == obj_x && y == obj_y) continue;
      if (!map[x + (y * width)]) continue;
      int rise = y - obj_y;
      int run = x - obj_x;
      if (rise == 0 || run == 0) {
        if (run == 0) {
          if (rise < 0) rise = -1;
          else rise = 1;
        } else {
          if (run < 0) run = -1;
          else run = 1;
        }
      } else {
        int gcd = Gcd(rise, run);
        if (gcd != -1) {
          rise = rise / gcd;
          run = run / gcd;
        }
      }
      bool blocked = false;
      int temp_x = obj_x;
      int temp_y = obj_y;
      while (true) {
        temp_x += run;
        temp_y += rise;
        // Not blocked
        if (temp_x == x && temp_y == y) {
          result.push_back({x, y});
          break;
        }
        // Blocked
        if (map[temp_x + (temp_y * width)]) {
          break;
        }
      }
    }
  }
  return result;
}

double angle(tuple<int, int> origin, tuple<int, int> point) {
  double a = atan2(get<1>(point) - get<1>(origin), get<0>(point) - get<0>(origin));
  if (a < 0) {
    a += 2 * M_PI;
  }
  return fmod(a + M_PI / 2, 2 * M_PI);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Missing file" << endl;
    return -1;
  }

  const char* filename = argv[1];
  int width = 0;
  int height = 0;
  vector<int> map;
  for (const string& line : ReadLines(filename)) {
    width = 0;
    for (char c : line) {
      map.push_back(c == '#');
      width++;
    }
    height++;
  }

  int max_count = 0;
  int max_x = 0;
  int max_y = 0;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (map[x + (y * width)]) {
        // cout << x << "," << y << ": " << Count(map, width, height, x, y) << endl;
        int count = Find(map, width, height, x, y).size();
        if (count > max_count) {
          max_x = x;
          max_y = y;
          max_count = count;
        }
      }
    }
  }

  int destroyed = 0;
  int last_x = 0;
  int last_y = 0;
  bool done = false;
  while (!done) {
    vector<tuple<int, int>> targets = Find(map, width, height, max_x, max_y);
    sort(targets.begin(), targets.end(), [max_x, max_y](tuple<int, int> t1, tuple<int, int> t2) {
      return angle({max_x, max_y}, t1) < angle({max_x, max_y}, t2);
    });
    for (auto [x, y] : targets) {
      map[x + (y * width)] = 0;
      last_x = x;
      last_y = y;
      destroyed++;
      if (destroyed > 199) {
        done = true;
        break;
      }
    }
  }

  cout << "Answer 1: " << max_count << endl;
  cout << "Answer 2: " << last_x * 100 + last_y << endl;

  return 0;
}

