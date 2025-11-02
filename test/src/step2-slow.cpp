#include <bits/stdc++.h>
using namespace std;

static int _ = []() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  return 0;
}();

struct Monster {
  int max_hp;
  int hp;
  bool is_aoe_triggered;

  void decrease_hp() { hp = max(0, hp - 1); }
};

vector<Monster> init_monters;

bool is_all_monsters_dead(const vector<Monster> &monsters) {
  for (auto m : monsters) {
    if (m.hp > 0) {
      return false;
    }
  }

  return true;
}

void print_monsters(const vector<Monster> &monsters, int pos = -1,
                    char left = '[', char right = ']') {
  for (int i = 0; i < (int)monsters.size(); ++i) {
    if (i == pos) {
      cout << left << monsters[i].hp << right << " ";
    } else {
      cout << monsters[i].hp << " ";
    }
  }

  cout << "\n";
}

bool trigger_aoe(vector<Monster> &monsters, bool is_print = false) {
  for (size_t i = 0; i < monsters.size(); ++i) {
    if (monsters[i].is_aoe_triggered) {
      continue;
    }

    if (monsters[i].hp > init_monters[i].hp / 2) {
      continue;
    }

    if (is_print) {
      print_monsters(monsters, (int)i, '(', ')');
    }

    monsters[i].is_aoe_triggered = true;

    for (auto &m : monsters)
      m.decrease_hp();

    return true;
  }

  return false;
}

int solve(vector<int> &a) {
  int n = a.size();
  if (n == 0)
    return 0;
  vector<pair<int, int>> enemies;
  for (int x : a) {
    int h = x / 2;
    enemies.emplace_back(h, x);
  }
  sort(enemies.begin(), enemies.end(),
       [](const pair<int, int> &a_, const pair<int, int> &b_) {
         return a_.first > b_.first;
       });

  int k = 0;
  long long sum_x = 0;
  vector<int> xs(n);
  for (int i = 0; i < n; ++i) {
    int h_i = enemies[i].first;
    xs[i] = max(0, h_i - k);
    sum_x += xs[i];
    k++;
  }
  for (int i = 0; i < n; ++i) {
    int total = xs[i] + k;
    int a_i = enemies[i].second;
    if (total < a_i) {
      sum_x += a_i - total;
    }
  }
  return sum_x;
}

int main() {
  vector<int> v;
  while (true) {
    int hp;
    if (cin >> hp) {
      v.push_back(hp);
    } else {
      break;
    }
  }

  int min_step = solve(v);
  cout << min_step << "\n\n";

  return 0;
}