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

struct Point {
  int x;
  int y;
  int distance;
  bool operator==(const Point& p1) const {
    return this->x == p1.x && this->y == p1.y;
  }
};

struct PointHash {
  size_t operator()(const Point& p1) const {
    size_t hash = 7;
    hash = hash * 31 + p1.x;
    hash = hash * 31 + p1.y;
    return hash;
  }
};

class State {
  public:
    State(vector<long> codes) : machine(codes, &inputs, &outputs), point{0, 0, 0} {}

    State(const State& s) : machine(s.machine.codes, &inputs, &outputs), point(s.point) {}
    State(const State&& s) : machine(s.machine.codes, &inputs, &outputs), point(s.point) {}

    Point point;
    queue<long> inputs;
    queue<long> outputs;
    Machine machine;
};

void Solve(vector<long> codes) {
  State origin(codes);
  unordered_set<Point, PointHash> visited;
  unordered_set<Point, PointHash> open;
  queue<State> states;
  states.push(origin);

  int shortest_distance = -1;
  Point oxygen;

  while (!states.empty()) {
    State current = states.front();
    states.pop();

    visited.insert(current.point);

    for (int i = 1; i < 5; i++) {
      State new_state = current;

      switch (i) {
        case 1:
          new_state.point.y += 1;
          break;
        case 2:
          new_state.point.y -= 1;
          break;
        case 3:
          new_state.point.x -= 1;
          break;
        case 4:
          new_state.point.x += 1;
          break;
      }
      new_state.point.distance += 1;

      if (visited.find(new_state.point) != visited.end()) {
        continue;
      }

      new_state.inputs.push(i);
      new_state.machine.Step();
      int status = new_state.outputs.front();
      new_state.outputs.pop();

      if (status == 2) {
        if (shortest_distance == -1 || new_state.point.distance < shortest_distance) {
          shortest_distance = new_state.point.distance;
          oxygen = new_state.point;
        }
        open.insert(new_state.point);
      }
      if (status == 1) {
        states.push(new_state);
        open.insert(new_state.point);
      }
    }
  }

  cout << "Answer 1: " << shortest_distance << endl;

  queue<Point> spread;
  // reuse distance as time
  oxygen.distance = 0;
  spread.push(oxygen);
  int last_minute = 0;
  // erase all of the open squares
  while (!open.empty()) {
    Point current = spread.front();
    spread.pop();

    if (open.find(current) == open.end()) continue;

    Point north = current;
    north.y += 1;
    north.distance += 1;
    spread.push(north);

    Point east = current;
    east.x += 1;
    east.distance += 1;
    spread.push(east);

    Point south = current;
    south.y -= 1;
    south.distance += 1;
    spread.push(south);

    Point west = current;
    west.x -= 1;
    west.distance += 1;
    spread.push(west);

    last_minute = current.distance;
    open.erase(current);
  }

  cout << "Answer 2: " << last_minute << endl;
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

  Solve(codes);

  return 0;
}
