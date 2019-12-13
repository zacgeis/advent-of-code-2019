#include "../util.h"

struct Machine {
  Machine(vector<long> codes, queue<long>* inputs, queue<long>* outputs) :
    codes(codes), inputs(inputs), outputs(outputs) {}

  void ExpandMemory(long l) {
    while (l > 0 && codes.size() < l + 1) {
      codes.push_back(0);
    }
  }

  long GetAddress(int arg_offset) {
    int mode = 0;
    int pos = 10;
    for (int i = 0; i < arg_offset; i++) {
      pos *= 10;
    }
    mode = (codes[pc] / pos) % 10;

    long address = 0;
    if (mode == 0) {
      address = codes[pc + arg_offset];
    } else if (mode == 1) {
      address = pc + arg_offset;
    } else if (mode == 2) {
      address = relative_base + codes[pc + arg_offset];
    } else {
      cout << "Invalid mode detected" << endl;
    }

    ExpandMemory(address);
    return address;
  }

  long Access(int arg_offset) {
    return codes[GetAddress(arg_offset)];
  }

  void Store(int arg_offset, long value) {
    codes[GetAddress(arg_offset)] = value;
  }

  void Step() {
    bool yield = false;
    while (!yield) {
      int opcode = codes[pc] % 100;
      switch (opcode) {
        case 1:
          Store(3, Access(1) + Access(2));
          pc += 4;
          break;
        case 2:
          Store(3, Access(1) * Access(2));
          pc += 4;
          break;
        case 3:
          if (inputs->empty()) {
            cout << "No inputs to read" << endl;
            yield = true;
            break;
          }
          Store(1, inputs->front());
          inputs->pop();
          pc += 2;
          break;
        case 4:
          outputs->push(Access(1));
          pc += 2;
          yield = true;
          break;
        case 5:
          if (Access(1) != 0) {
            pc = Access(2);
          } else {
            pc += 3;
          }
          break;
        case 6:
          if (Access(1) == 0) {
            pc = Access(2);
          } else {
            pc += 3;
          }
          break;
        case 7:
          if (Access(1) < Access(2)) {
            Store(3, 1);
          } else {
            Store(3, 0);
          }
          pc += 4;
          break;
        case 8:
          if (Access(1) == Access(2)) {
            Store(3, 1);
          } else {
            Store(3, 0);
          }
          pc += 4;
          break;
        case 9:
          relative_base += Access(1);
          pc += 2;
          break;
        case 99:
          yield = true;
          halted = true;
          break;
        default:
          cout << "Bad op code" << endl;
          return;
      }
    }
  }

  long pc = 0;
  bool halted = false;
  long relative_base = 0;
  queue<long>* inputs;
  queue<long>* outputs;
  vector<long> codes;
};

void Solve(vector<long>& codes, long initial_input) {
  queue<long> inputs;
  queue<long> outputs;

  inputs.push(initial_input);

  Machine machine(codes, &inputs, &outputs);

  while(!machine.halted) {
    machine.Step();
  }

  cout << "Answer " << initial_input << ":" << endl;
  while (!outputs.empty()) {
    cout << "  " << outputs.front() << endl;
    outputs.pop();
  }
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Missing file" << endl;
    return -1;
  }

  const char* filename = argv[1];
  vector<long> codes;
  for (const string& line : ReadLines(filename)) {
    for (const string& s : Split(line, ",")) {
      codes.push_back(std::stol(s));
    }
  }

  Solve(codes, 1);
  Solve(codes, 2);

  return 0;
}
