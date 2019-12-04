#include "../util.h"

int Part1(vector<int>& numbers) {
  int result = 0;
  for (int number : numbers) {
    result += (number / 3) - 2;
  }
  return result;
}

int Part2(vector<int>& numbers) {
  int result = 0;
  for (int number : numbers) {
    int temp = number;
    while (true) {
      temp = (temp / 3) - 2;
      if (temp <= 0) break;
      result += temp;
    }
  }
  return result;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Missing file" << endl;
    return -1;
  }

  const char* filename = argv[1];
  vector<int> numbers;
  for (string& line : ReadLines(filename)) {
    numbers.push_back(std::stoi(line));
  }

  cout << "Answer 1: " << Part1(numbers) << endl;
  cout << "Answer 2: " << Part2(numbers) << endl;

  return 0;
}
