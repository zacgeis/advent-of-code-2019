#include "../util.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Missing file" << endl;
    return -1;
  }

  const char* filename = argv[1];
  vector<int> pixels;
  for (const string& line : ReadLines(filename)) {
    for (char c : line) {
      pixels.push_back(c - '0');
    }
  }

  const int width = 25;
  const int height = 6;
  int layer = width * height;
  int layer_count = pixels.size() / layer;

  int smallest_zeros = -1;
  int result = 0;
  for (int i = 0; i < layer_count; i++) {
    int zeros = 0;
    int ones = 0;
    int twos = 0;
    int offset = i * layer;
    for (int j = 0; j < layer; j++) {
      switch (pixels[offset + j]) {
        case 0:
          zeros++;
          break;
        case 1:
          ones++;
          break;
        case 2:
          twos++;
          break;
        default:
          cout << "Found bad number" << endl;
          break;
      }
    }
    if (smallest_zeros == -1 || zeros < smallest_zeros) {
      smallest_zeros = zeros;
      result = ones * twos;
    }
  }

  cout << "Answer 1: " << result << endl;

  array<int, width * height> picture;
  for (int i = 0; i < layer; i++) {
    picture[i] = 2;
  }
  for (int i = 0; i < layer_count; i++) {
    int offset = i * layer;
    for (int j = 0; j < layer; j++) {
      if (picture[j] != 2) continue;
      picture[j] = pixels[offset + j];
    }
  }

  cout << "Answer 2: " << endl;
  for (int row = 0; row < height; row++) {
    for (int cell = 0; cell < width; cell++) {
      if (picture[cell + (row * width)] == 0) {
        cout << "\u001b[30m";
        cout << "▇";
        cout << "\u001b[0m";
      } else {
        cout << "▇";
      }
    }
    cout << endl;
  }

  return 0;
}
