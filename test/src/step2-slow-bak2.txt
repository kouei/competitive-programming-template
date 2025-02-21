#include<bits/stdc++.h>
using namespace std;

#pragma GCC optimize("O2,unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")

static int _ = [](){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 0;
}();

struct Monster {
    int max_hp;
    int hp;
    bool is_aoe_triggered;

    void decrease_hp() {
        hp = max(0, hp - 1);
    }
};

vector<Monster> init_monters;

bool is_all_monsters_dead(const vector<Monster> & monsters) {
    for(auto m : monsters) {
        if(m.hp > 0) {
            return false;
        }
    }

    return true;
}

void print_monsters(const vector<Monster> & monsters, int pos = -1, char left = '[', char right = ']') {
    for(int i = 0; i < (int)monsters.size(); ++i) {
        if(i == pos) {
            cout << left << monsters[i].hp << right << " ";
        } else {
            cout << monsters[i].hp << " ";
        }
    }

    cout << "\n";
}

bool trigger_aoe(vector<Monster> & monsters, bool is_print = false) {
    for(size_t i = 0; i < monsters.size(); ++i) {
        if (monsters[i].is_aoe_triggered) {
            continue;
        }

        if (monsters[i].hp > init_monters[i].hp / 2) {
            continue;
        }

        if(is_print) {
            print_monsters(monsters, (int)i, '(', ')');
        }

        monsters[i].is_aoe_triggered = true;

        for(auto & m : monsters) m.decrease_hp();


        return true;
    }

    return false;
}

int solve() {

    vector<int> index;
    for(int i = 0; i < (int)init_monters.size(); ++i) {
        index.push_back(i);
    }

    int min_step = INT_MAX;
    do {
        int step = 0;
        auto monsters = init_monters;

        for(int i : index) {
            if(monsters[i].is_aoe_triggered) {
                continue;
            }

            if(monsters[i].hp <= monsters[i].max_hp / 2) {
                continue;
            }

            step += monsters[i].hp - monsters[i].max_hp / 2;
            monsters[i].hp = monsters[i].max_hp / 2;

            while(trigger_aoe(monsters)) {}
        }

        for(const auto & m : monsters) {
            step += m.hp;
        }

        min_step = min(min_step, step);
    } while(next_permutation(index.begin(), index.end()));

    return min_step;
}

int main() {
    while (true) {
        int hp;
        if (cin >> hp) {
            init_monters.push_back({hp, hp, false});
        } else {
            break;
        }
    }

    int min_step = solve();
    cout << min_step << "\n\n";
    
    return 0;
}