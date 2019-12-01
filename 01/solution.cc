#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int part1(std::vector<int>& numbers) {
  int result = 0;
  for (int number : numbers) {
    result += (number / 3) - 2;
  }
  return result;
}

int part2(std::vector<int>& numbers) {
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
    std::cout << "Missing file" << std::endl;
    return -1;
  }

  const char* filename = argv[1];
  std::ifstream input(filename);

  std::vector<int> numbers;

  std::string line;
  while (std::getline(input, line)) {
    numbers.push_back(std::stoi(line));
  }
  input.close();

  std::cout << "Answer 1: " << part1(numbers) << std::endl;
  std::cout << "Answer 2: " << part2(numbers) << std::endl;

  return 0;
}
