#include "../util.h"

const int lower = 147981;
const int upper = 691423;

vector<int> IntToVec(int i) {
  vector<int> result;
  while (i > 0) {
    result.push_back(i % 10);
    i /= 10;
  }
  reverse(result.begin(), result.end());
  return result;
}

int VecToInt(vector<int> v) {
  int result = 0;
  for (int i : v) {
    result += i;
    result *= 10;
  }
  return result / 10;
}

bool Adj(vector<int> v) {
  int last = 0;
  for (int i : v) {
    if (i == last) return true;
    last = i;
  }
  return false;
}

bool AdjUniq(vector<int> v) {
  bool has_double = false;
  int i = 0;
  int last = v[i];
  int count = 0;
  i++;
  while (i < v.size()) {
    if (v[i] == last) {
      count++;
    } else {
      if (count == 1) {
        return true;
      }
      last = v[i];
      count = 0;
    }
    i++;
  }
  return count == 1;
}

void Clear(vector<int>& v, int i, int c) {
  for (; i < v.size(); i++) {
    v[i] = c;
  }
}

int main(int argc, char** argv) {
  vector<int> current = IntToVec(lower);

  for (int i = 0; i < current.size() - 1; i++) {
    if (current[i] > current[i + 1]) current[i + 1] = current[i];
  }

  int count_1 = 0;
  int count_2 = 0;

  int last = current.size() - 1;
  int digit = last;
  while (true) {
    if (current[digit] < 9) {
      if (VecToInt(current) > upper) {
        break;
      }

      if (Adj(current)) {
        count_1++;
      }

      if (AdjUniq(current)) {
        count_2++;
      }

      current[digit]++;
      if (digit != last) {
        Clear(current, digit, current[digit]);
        digit = last;
      }
    } else {
      digit--;
    }
  }

  cout << "Answer 1: " << count_1 << endl;
  cout << "Answer 2: " << count_2 << endl;

  return 0;
}
