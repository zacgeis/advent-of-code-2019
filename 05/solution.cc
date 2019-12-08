#include "../util.h"

int Access(vector<int>& codes, int mode, int i) {
  if (mode == 0) {
    return codes[codes[i]];
  }
  return codes[i];
}

void Part1(vector<int> codes) {
  int input = 1;
  int output = 0;
  int i = 0;
  bool halt = false;
  while (!halt) {
    int mode1 = (codes[i] / 100) % 10;
    int mode2 = (codes[i] / 1000) % 10;
    int opcode = codes[i] % 100;
    switch (opcode) {
      case 1:
        codes[codes[i + 3]] = Access(codes, mode1, i + 1) + Access(codes, mode2, i + 2);
        i += 4;
        break;
      case 2:
        codes[codes[i + 3]] = Access(codes, mode1, i + 1) * Access(codes, mode2, i + 2);
        i += 4;
        break;
      case 3:
        codes[codes[i + 1]] = input;
        i += 2;
        break;
      case 4:
        output = Access(codes, mode1, i + 1);
        if (output != 0) {
          cout << "Answer 1: " << output << endl;
        }
        i += 2;
        break;
      case 99:
        halt = true;
        break;
      default:
        cout << "Invalid op code: " << opcode << endl;
        return;
    }
  }
}

void Part2(vector<int> codes) {
  int input = 5;
  int i = 0;
  bool halt = false;
  while (!halt) {
    int mode1 = (codes[i] / 100) % 10;
    int mode2 = (codes[i] / 1000) % 10;
    int opcode = codes[i] % 100;
    switch (opcode) {
      case 1:
        codes[codes[i + 3]] = Access(codes, mode1, i + 1) + Access(codes, mode2, i + 2);
        i += 4;
        break;
      case 2:
        codes[codes[i + 3]] = Access(codes, mode1, i + 1) * Access(codes, mode2, i + 2);
        i += 4;
        break;
      case 3:
        codes[codes[i + 1]] = input;
        i += 2;
        break;
      case 4:
        cout << "Answer 2: " << Access(codes, mode1, i + 1) << endl;
        i += 2;
        break;
      case 5:
        if (Access(codes, mode1, i + 1) != 0) {
          i = Access(codes, mode2, i + 2);
        } else {
          i += 3;
        }
        break;
      case 6:
        if (Access(codes, mode1, i + 1) == 0) {
          i = Access(codes, mode2, i + 2);
        } else {
          i += 3;
        }
        break;
      case 7:
        if (Access(codes, mode1, i + 1) < Access(codes, mode2, i + 2)) {
          codes[codes[i + 3]] = 1;
        } else {
          codes[codes[i + 3]] = 0;
        }
        i += 4;
        break;
      case 8:
        if (Access(codes, mode1, i + 1) == Access(codes, mode2, i + 2)) {
          codes[codes[i + 3]] = 1;
        } else {
          codes[codes[i + 3]] = 0;
        }
        i += 4;
        break;
      case 99:
        halt = true;
        break;
      default:
        cout << "Invalid op code: " << opcode << endl;
        return;
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

  Part1(codes);
  Part2(codes);

  return 0;
}
