#include "../util.h"

struct Comp {
  string name;
  long count;
};

struct Reaction {
  vector<Comp> inputs;
  Comp output;
};

vector<Comp> ParseComps(const string& raw_comps) {
  vector<Comp> comps;
  for (string& raw_comp : Split(raw_comps, ", ")) {
    vector<string> parts = Split(raw_comp, " ");
    comps.push_back({parts[1], stoi(parts[0])});
  }
  return comps;
}

void Produce(vector<Reaction>& reactions, unordered_map<string, long>& amounts, string name, long amount) {
  if (name == "ORE") {
    amounts["ORE"] += amount;
    amounts["ORE_TOTAL"] += amount;
    return;
  }

  Reaction reaction;
  for (Reaction& r : reactions) {
    if (r.output.name == name) {
      reaction = r;
      break;
    }
  }

  // how much do we need?
  long diff = amount - amounts[name];
  // we already have enough.
  if (diff <= 0) return;

  // how many times do we need to perform the reaction?
  long factor = diff / reaction.output.count;
  if (diff % reaction.output.count != 0) {
    factor += 1;
  }

  for (Comp& c : reaction.inputs) {
    c.count *= factor;
    if (amounts[c.name] < c.count) {
      Produce(reactions, amounts, c.name, c.count);
    }
    amounts[c.name] -= c.count;
  }

  amounts[reaction.output.name] += reaction.output.count * factor;
}

void Part1(vector<Reaction> reactions, unordered_map<string, long> amounts) {
  Produce(reactions, amounts, "FUEL", 1);
  cout << "Answer 1: " << amounts["ORE_TOTAL"] << endl;
}

void Part2(vector<Reaction> reactions, unordered_map<string, long> amounts) {
  long left = 0;
  long right = 100000000;
  while (true) {
    vector<Reaction> new_reactions = reactions;
    unordered_map<string, long> new_amounts = amounts;
    long factor = (right + left) / 2;
    Produce(new_reactions, new_amounts, "FUEL", factor);
    if (new_amounts["ORE_TOTAL"] > 1000000000000L) {
      right = factor;
    } else {
      left = factor;
    }
    if (right - left == 1) break;
  }
  cout << "Answer 2: " << left << endl;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Missing file" << endl;
    return -1;
  }

  const char* filename = argv[1];
  vector<Reaction> reactions;
  unordered_map<string, long> amounts;
  for (const string& line : ReadLines(filename)) {
    vector<string> parts = Split(line, " => ");
    reactions.push_back({ParseComps(parts[0]), ParseComps(parts[1])[0]});
  }

  for (Reaction& r : reactions) {
    for (Comp& c : r.inputs) {
      amounts[c.name] = 0;
    }
    amounts[r.output.name] = 0;
  }

  Part1(reactions, amounts);
  Part2(reactions, amounts);

  return 0;
}

