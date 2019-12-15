#include "../util.h"

struct Point {
  int x;
  int y;
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

enum class Direction {
  kUp,
  kRight,
  kDown,
  kLeft,
};

void Move(Point& point, Direction direction) {
  switch (direction) {
    case Direction::kUp:
      point.y -= 1;
      break;
    case Direction::kRight:
      point.x += 1;
      break;
    case Direction::kDown:
      point.y += 1;
      break;
    case Direction::kLeft:
      point.x -= 1;
      break;
    default:
      cout << "Error, bad direction: " << static_cast<int>(direction) << endl;
      break;
  }
}

Direction Turn(Direction current, Direction turn) {
  if (turn == Direction::kRight) {
    switch (current) {
      case Direction::kUp:
        return Direction::kRight;
      case Direction::kRight:
        return Direction::kDown;
      case Direction::kDown:
        return Direction::kLeft;
      case Direction::kLeft:
        return Direction::kUp;
      default:
        cout << "Error, bad turn: " << endl;
        break;
    }
  } else {
    switch (current) {
      case Direction::kUp:
        return Direction::kLeft;
      case Direction::kRight:
        return Direction::kUp;
      case Direction::kDown:
        return Direction::kRight;
      case Direction::kLeft:
        return Direction::kDown;
      default:
        cout << "Error, bad turn: " << endl;
        break;
    }
  }
}

void Part1(vector<long> codes) {
  queue<long> inputs;
  queue<long> outputs;
  Machine machine(codes, &inputs, &outputs);

  unordered_set<Point, PointHash> white;
  unordered_set<Point, PointHash> any_color;
  Point current{0, 0};
  Direction direction = Direction::kUp;

  while(!machine.halted) {
    inputs.push(white.find(current) != white.end());

    machine.Step();
    if (outputs.front() == 1) {
      white.insert(current);
    } else {
      white.erase(current);
    }
    any_color.insert(current);
    outputs.pop();

    machine.Step();
    if (outputs.front()) {
      direction = Turn(direction, Direction::kRight);
    } else {
      direction = Turn(direction, Direction::kLeft);
    }
    outputs.pop();

    Move(current, direction);
  }

  cout << "Answer 1: " << any_color.size() << endl;
}

void Part2(vector<long> codes) {
  queue<long> inputs;
  queue<long> outputs;
  Machine machine(codes, &inputs, &outputs);

  unordered_set<Point, PointHash> white;
  unordered_set<Point, PointHash> any_color;
  Point current{0, 0};
  Direction direction = Direction::kUp;

  // Part 2 starts on a white square.
  white.insert(current);

  while(!machine.halted) {
    inputs.push(white.find(current) != white.end());

    machine.Step();
    if (outputs.front() == 1) {
      white.insert(current);
    } else {
      white.erase(current);
    }
    any_color.insert(current);
    outputs.pop();

    machine.Step();
    if (outputs.front()) {
      direction = Turn(direction, Direction::kRight);
    } else {
      direction = Turn(direction, Direction::kLeft);
    }
    outputs.pop();

    Move(current, direction);
  }

  int min_x = 0;
  int min_y = 0;
  int max_x = 0;
  int max_y = 0;

  for (const Point& p : white) {
    if (p.x < min_x) min_x = p.x;
    if (p.y < min_y) min_y = p.y;
    if (p.x > max_x) max_x = p.x;
    if (p.y > max_y) max_y = p.y;
  }

  cout << "Answer 2: " << endl;
  for (int y = 0; y <= max_y - min_y; y++) {
    for (int x = 0; x <= max_x - min_x; x++) {
      if (white.find({x, y}) != white.end()) {
        cout << "▇";
      } else {
        cout << "\u001b[30m";
        cout << "▇";
        cout << "\u001b[0m";
      }
    }
    cout << endl;
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

  Part1(codes);
  Part2(codes);

  return 0;
}
