#include "../util.h"

struct Env {
  int pc;
  bool halted;
  queue<int>* inputs;
  queue<int>* outputs;
  vector<int> codes;
};

int Access(vector<int>& codes, int mode, int i) {
  if (mode == 0) {
    return codes[codes[i]];
  }
  return codes[i];
}

void Run(Env& env) {
  vector<int>& codes = env.codes;
  int i = env.pc;
  bool yield = false;
  while (!yield) {
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
        if (env.inputs->empty()) {
          yield = true;
          break;
        }
        codes[codes[i + 1]] = env.inputs->front();
        env.inputs->pop();
        i += 2;
        break;
      case 4:
        env.outputs->push(Access(codes, mode1, i + 1));
        i += 2;
        yield = true;
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
        yield = true;
        env.halted = true;
        break;
      default:
        return;
    }
  }
  env.pc = i;
}

int Part2(vector<int>& codes, array<int, 5>& phases, int depth) {
  if (depth == 5) {
    array<Env, 5> envs;
    array<queue<int>, 5> inputs;

    for (int i = 0; i < 5; i++) {
      queue<int>* input;
      queue<int>* output;
      if (i == 0) {
        input = &inputs[4];
      } else {
        input = &inputs[i - 1];
      }
      if (i == 4) {
        output = &inputs[0];
      } else {
        output = &inputs[i + 1];
      }
      envs[i] = {
        0,
        false,
        input,
        output,
        codes,
      };
    }

    for (int i = 0; i < 5; i++) {
      // First input is the phase.
      inputs[i].push(phases[i]);
    }
    // First amp gets 0 after it's phase
    inputs[0].push(0);

    while (true) {
      bool did_run = false;
      for (int i = 0; i < 5; i++) {
        Env& current_env = envs[i];
        if (current_env.halted) {
          continue;
        }
        Run(current_env);
        did_run = true;
      }
      if (!did_run) break;
    }

    // amp e outputs to amp a
    int result = inputs[0].front();
    return result;
  }

  int max = 0;
  for (int i = 5; i <= 9; i++) {
    bool skip = false;
    for (int j = 0; j < depth; j++) {
      if (phases[j] == i) {
        skip = true;
        break;
      }
    }
    if (skip) continue;
    phases[depth] = i;
    int result = Part2(codes, phases, depth + 1);
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
  cout << "Answer 2: " << Part2(codes, phases, 0) << endl;

  return 0;
}
