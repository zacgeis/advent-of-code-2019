#include "../util.h"

struct Op {
  char direction;
  int distance;
};

struct Point {
  int x;
  int y;
};

struct Path {
  int total_length;
  int distance;
  Point start;
  Point end;
};

void GeneratePaths(vector<Op>& wire, vector<Path>& hor, vector<Path>& ver) {
  Point previous_location;
  Point location{0, 0};
  int length = 0;
  for (Op& op : wire) {
    previous_location = location;
    switch (op.direction) {
      case 'U':
        location.y += op.distance;
        ver.push_back({
          .total_length = length,
          .distance = op.distance,
          .start = previous_location,
          .end = location,
        });
        break;
      case 'D':
        location.y -= op.distance;
        ver.push_back({
          .total_length = length,
          .distance = op.distance,
          .start = previous_location,
          .end = location,
        });
        break;
      case 'L':
        location.x -= op.distance;
        hor.push_back({
          .total_length = length,
          .distance = op.distance,
          .start = previous_location,
          .end = location,
        });
        break;
      case 'R':
        location.x += op.distance;
        hor.push_back({
          .total_length = length,
          .distance = op.distance,
          .start = previous_location,
          .end = location,
        });
        break;
      default:
        cout << "Unknown direction found" << endl;
        return;
    }
    length += op.distance;
  }
}

bool PathIntersect(Path& hor, Path& ver) {
  int lower_x = hor.start.x;
  int upper_x = hor.end.x;
  if (lower_x > upper_x) swap(lower_x, upper_x);
  if (ver.start.x >= lower_x && ver.start.x <= upper_x) {
    int lower_y = ver.start.y;
    int upper_y = ver.end.y;
    if (lower_y > upper_y) swap(lower_y, upper_y);
    if (hor.start.y >= lower_y && hor.start.y <= upper_y) {
      return true;
    }
  }
  return false;
}

int MinDistanceManh(vector<Path>& hors, vector<Path>& vers) {
  int min_distance = 0;

  for (int i = 0; i < hors.size(); i++) {
    for (int j = 0; j < vers.size(); j++) {
      Path& hor = hors[i];
      Path& ver = vers[j];
      if (PathIntersect(hor, ver)) {
        int distance = abs(hor.start.y) + abs(ver.start.x);
        if (min_distance == 0 || distance < min_distance) {
          min_distance = distance;
        }
      }
    }
  }

  return min_distance;
}

void Part1(array<vector<Op>, 2>& wires) {
  vector<Path> wire1hor;
  vector<Path> wire1ver;
  vector<Path> wire2hor;
  vector<Path> wire2ver;

  GeneratePaths(wires[0], wire1hor, wire1ver);
  GeneratePaths(wires[1], wire2hor, wire2ver);

  int min_distance1 = MinDistanceManh(wire1hor, wire2ver);
  int min_distance2 = MinDistanceManh(wire2hor, wire1ver);
  int min_distance = min_distance1;
  if (min_distance2 == 0 || min_distance2 < min_distance) {
    min_distance = min_distance2;
  }

  cout << "Answer 1: " << min_distance << endl;
}

int MinDistancePath(vector<Path>& hors, vector<Path>& vers) {
  int min_distance = 0;

  for (int i = 0; i < hors.size(); i++) {
    for (int j = 0; j < vers.size(); j++) {
      Path& hor = hors[i];
      Path& ver = vers[j];
      if (PathIntersect(hor, ver)) {
        int distance = (hor.total_length + abs(abs(ver.start.x) - abs(hor.start.x)))
          + (ver.total_length + abs(abs(hor.start.y) - abs(ver.start.y)));
        if (min_distance == 0 || distance < min_distance) {
          min_distance = distance;
        }
      }
    }
  }

  return min_distance;
}

void Part2(array<vector<Op>, 2>& wires) {
  vector<Path> wire1hor;
  vector<Path> wire1ver;
  vector<Path> wire2hor;
  vector<Path> wire2ver;

  GeneratePaths(wires[0], wire1hor, wire1ver);
  GeneratePaths(wires[1], wire2hor, wire2ver);

  int min_distance1 = MinDistancePath(wire1hor, wire2ver);
  int min_distance2 = MinDistancePath(wire2hor, wire1ver);
  int min_distance = min_distance1;
  if (min_distance2 == 0 || min_distance2 < min_distance) {
    min_distance = min_distance2;
  }

  cout << "Answer 2: " << min_distance << endl;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Missing file" << endl;
    return -1;
  }

  const char* filename = argv[1];
  int current_wire = 0;
  array<vector<Op>, 2> wires;
  for (const string& line : ReadLines(filename)) {
    vector<Op>& wire = wires[current_wire];
    for (const string& s : Split(line, ",")) {
      wire.push_back({
          .direction = s[0],
          .distance = std::stoi(s.substr(1, s.size() - 1)),
      });
    }
    current_wire++;
  }

  Part1(wires);
  Part2(wires);

  return 0;
}
