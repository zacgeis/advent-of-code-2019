#include "../util.h"

struct Point {
  int x;
  int y;
  int z;
};

struct Moon {
  Point pos;
  Point vel;
};

unsigned long Gcd(long x, long y) {
  x = abs(x);
  y = abs(y);
  long smaller = x;
  if (y < smaller) smaller = y;
  long result = -1;
  for (long i = 1; i <= smaller; i++) {
    if (x % i == 0 && y % i == 0) {
      result = i;
    }
  }
  return result;
}

unsigned long Lcm(long x, long y) {
  return (x * y) / Gcd(x, y);
}

void Step(array<Moon, 4>& moons) {
  for (int m1 = 0; m1 < moons.size(); m1++) {
    for (int m2 = m1 + 1; m2 < moons.size(); m2++) {
      if (moons[m1].pos.x > moons[m2].pos.x) {
        moons[m1].vel.x -= 1;
        moons[m2].vel.x += 1;
      } else if (moons[m1].pos.x < moons[m2].pos.x) {
        moons[m1].vel.x += 1;
        moons[m2].vel.x -= 1;
      }
      if (moons[m1].pos.y > moons[m2].pos.y) {
        moons[m1].vel.y -= 1;
        moons[m2].vel.y += 1;
      } else if (moons[m1].pos.y < moons[m2].pos.y) {
        moons[m1].vel.y += 1;
        moons[m2].vel.y -= 1;
      }
      if (moons[m1].pos.z > moons[m2].pos.z) {
        moons[m1].vel.z -= 1;
        moons[m2].vel.z += 1;
      } else if (moons[m1].pos.z < moons[m2].pos.z) {
        moons[m1].vel.z += 1;
        moons[m2].vel.z -= 1;
      }
    }
  }

  for (int m = 0; m < moons.size(); m++) {
    Moon& moon = moons[m];
    moon.pos.x += moon.vel.x;
    moon.pos.y += moon.vel.y;
    moon.pos.z += moon.vel.z;
  }
}

void Part1(array<Moon, 4> moons) {
  for (int i = 0; i < 1000; i++) {
    Step(moons);
  }

  int answer = 0;
  for (Moon& moon : moons) {
    int pot = abs(moon.pos.x) + abs(moon.pos.y) + abs(moon.pos.z);
    int ken = abs(moon.vel.x) + abs(moon.vel.y) + abs(moon.vel.z);
    answer += pot * ken;
  }
  cout << "Answer 1: " << answer << endl;
}

array<array<int, 4>, 6> Splits(array<Moon, 4>& moons) {
  array<array<int, 4>, 6> splits;
  for (int m = 0; m < moons.size(); m++) {
    Moon& moon = moons[m];
    splits[0][m] = moon.pos.x;
    splits[1][m] = moon.vel.x;
    splits[2][m] = moon.pos.y;
    splits[3][m] = moon.vel.y;
    splits[4][m] = moon.pos.z;
    splits[5][m] = moon.vel.z;
  }
  return splits;
}

void Part2(array<Moon, 4> moons) {
  array<int, 3> cycles = {0, 0, 0};
  array<array<int, 4>, 6> origin = Splits(moons);
  int i = 0;
  while (true) {
    Step(moons);
    array<array<int, 4>, 6> splits = Splits(moons);
    int found = 0;
    for (int s = 0; s < splits.size(); s += 2) {
      if (cycles[s / 2] != 0) {
        found++;
        continue;
      }
      bool match = true;
      for (int x = 0; x < moons.size(); x++) {
        if (origin[s][x] != splits[s][x] || origin[s + 1][x] != splits[s + 1][x]) {
          match = false;
          break;
        }
      }
      if (match) {
        cycles[s / 2] = i + 1;
      }
    }
    if (found == cycles.size()) break;
    i++;
  }
  unsigned long answer = cycles[0];
  for (int i = 1; i < cycles.size(); i++) {
    answer = Lcm(answer, cycles[i]);
  }
  cout << "Answer 2: " << answer << endl;
}

int main(int argc, char** argv) {
  array<Moon, 4> moons;
  moons[0] = {{-1, -4, 0}, {0, 0, 0}};
  moons[1] = {{4, 7, -1}, {0, 0, 0}};
  moons[2] = {{-14, -10, 9}, {0, 0, 0}};
  moons[3] = {{1, 2, 17}, {0, 0, 0}};

  Part1(moons);
  Part2(moons);

  return 0;
}
