#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

vector<string> ReadLines(const char* filename) {
  vector<string> result;

  std::ifstream input(filename);
  string line;
  while (std::getline(input, line)) {
    result.push_back(line);
  }
  input.close();

  return result;
}

vector<string> Split(const string& input, const string& delim) {
  vector<string> result;

  size_t last_index = 0;
  size_t index = 0;
  while (true) {
    last_index = index;
    index = input.find(delim, index);
    if (index == string::npos) {
      result.push_back(input.substr(last_index, input.size() - last_index));
      break;
    }
    result.push_back(input.substr(last_index, index - last_index));
    index += delim.size();
  }

  return result;
}
