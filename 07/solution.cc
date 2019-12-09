#include "../util.h"

int Access(vector<int>& codes, int mode, int i) {
  if (mode == 0) {
    return codes[codes[i]];
  }
  return codes[i];
}

int Run(vector<int> codes, stack<int> inputs) {
  int output = -1;
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
        codes[codes[i + 1]] = inputs.top();
        inputs.pop();
        i += 2;
        break;
      case 4:
        output = Access(codes, mode1, i + 1);
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
        return -1;
    }
  }

  return output;
}

int Part1(vector<int>& codes, array<int, 5>& phases, int depth) {
  if (depth == 5) {
    int last_output = 0;
    for (int i = 0; i < 5; i++) {
      stack<int> inputs;
      inputs.push(last_output);
      inputs.push(phases[i]);
      last_output = Run(codes, inputs);
    }
    return last_output;
  }

  int max = 0;
  for (int i = 0; i <= 4; i++) {
    bool skip = false;
    for (int j = 0; j < depth; j++) {
      if (phases[j] == i) {
        skip = true;
        break;
      }
    }
    if (skip) continue;
    phases[depth] = i;
    int result = Part1(codes, phases, depth + 1);
    if (result > max) {
      max = result;
    }
  }
  return max;
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

  array<int, 5> phases = {0, 0, 0, 0, 0};
  cout << "Answer 1: " << Part1(codes, phases, 0) << endl;

  return 0;
}
