#include "../util.h"

int Part1(vector<int>& original_codes, int noun, int verb) {
  vector<int> codes = original_codes;
  codes[1] = noun;
  codes[2] = verb;
  int i = 0;
  bool halt = false;
  while (!halt) {
    switch (codes[i]) {
      case 1:
        codes[codes[i + 3]] = codes[codes[i + 1]] + codes[codes[i + 2]];
        i += 4;
        break;
      case 2:
        codes[codes[i + 3]] = codes[codes[i + 1]] * codes[codes[i + 2]];
        i += 4;
        break;
      case 99:
        halt = true;
        break;
      default:
        cout << "Invalid op code" << endl;
        return -1;
    }
  }

  return codes[0];
}

void Part2(vector<int>& original_codes, int target) {
  for (int noun = 0; noun <= 99; noun++) {
    for (int verb = 0; verb <= 99; verb++) {
      int result = Part1(original_codes, noun, verb);
      if (target == result) {
        cout << "Answer 2: noun: " << noun << ", verb: " << verb << endl;
        return;
      }
    }
  }
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Missing file" << endl;
    return -1;
  }

  const char* filename = argv[1];
  vector<int> codes;
  for (const string& line : ReadLines(filename)) {
    for (const string& s : Split(line, ",")) {
      codes.push_back(std::stoi(s));
    }
  }

  cout << "Answer 1: " << Part1(codes, 12, 2) << endl;
  Part2(codes, 19690720);

  return 0;
}
