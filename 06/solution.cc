#include "../util.h"

struct Object {
  string id;
  Object* center;
  int distance;
  bool visited;
  vector<Object*> orbits;
};

Object& FindOrCreate(unordered_map<string, Object>& objects, string id) {
  auto it = objects.find(id);
  if (it == objects.end()) {
    it = get<0>(objects.insert({id, {id, nullptr, 0, false, {}}}));
  }
  return it->second;
}

int Count(Object& object) {
  Object* current = object.center;
  int count = 0;
  while (current != nullptr) {
    current = current->center;
    count++;
  }
  return count;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Missing file" << endl;
    return -1;
  }

  const char* filename = argv[1];
  unordered_map<string, Object> objects;
  for (const string& line : ReadLines(filename)) {
    const vector<string>& parts = Split(line, ")");
    Object& center = FindOrCreate(objects, parts[0]);
    Object& orbit = FindOrCreate(objects, parts[1]);
    center.orbits.push_back(&orbit);
    orbit.center = &center;
  }

  int sum = 0;
  for (auto it = objects.begin(); it != objects.end(); it++) {
    int count = Count(it->second);
    sum += count;
  }
  cout << "Answer 1: " << sum << endl;

  auto object_you = objects.find("YOU");
  queue<Object*> search;
  search.push(&object_you->second);

  int transfers = -1;
  while (!search.empty()) {
    Object* current = search.front();
    search.pop();
    if (current->visited) {
      continue;
    }
    current->visited = true;
    if (current->id == "SAN") {
      transfers = current->distance - 2;
      break;
    }
    if (current->center != nullptr) {
      current->center->distance = current->distance + 1;
      search.push(current->center);
    }
    for (Object* orbit : current->orbits) {
      orbit->distance = current->distance + 1;
      search.push(orbit);
    }
  }
  cout << "Answer 2: " << transfers << endl;

  return 0;
}
