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

tuple<int, vector<int>> dfs() {

    int min_step = INT_MAX;
    vector<int> min_history;

    vector<tuple<int, vector<int>, vector<Monster>>> stk;
    stk.emplace_back(0, vector<int>{}, init_monters);

    while(!stk.empty()) {
        auto step = move(get<0>(stk.back()));
        auto history = move(get<1>(stk.back()));
        auto monsters = move(get<2>(stk.back()));
        stk.pop_back();

        if(is_all_monsters_dead(monsters)) {
            if (min_step > step) {
                min_step = step;
                min_history = move(history);
            }

            continue;
        }

        for(size_t i = 0; i < monsters.size(); ++i) {
            if (monsters[i].hp <= 0) {
                continue;
            }

            auto new_monsters = monsters;
            new_monsters[i].decrease_hp();
            while(trigger_aoe(new_monsters)) {}

            auto new_history = history;
            new_history.emplace_back(i);
            stk.emplace_back(step + 1, move(new_history), move(new_monsters));
        }
    }

    return { min_step, min_history };
}

int main() {
    while (true) {
        int hp;
        if (cin >> hp) {
            init_monters.push_back({hp, false});
        } else {
            break;
        }
    }

    auto result = dfs();
    auto min_step = get<0>(result);
    auto min_history = move(get<1>(result));

#if false
    cout << "Min Step = " << min_step << "\n\n";

    cout << "Init Monsters:" << "\n";
    print_monsters(init_monters);
    cout << "\n";

    auto monsters = init_monters;
    for (size_t i = 0; i < min_history.size(); ++i) {
        cout << "Step " << i + 1 << ":" << "\n";

        int pos = min_history[i];
        print_monsters(monsters, pos);
        

        monsters[pos].decrease_hp();

        if(trigger_aoe(monsters, true)) {
            print_monsters(monsters);
            while(trigger_aoe(monsters, true)) {
                print_monsters(monsters);
            }
        } else {
            print_monsters(monsters);
        }

        cout << "\n";
    }
#else
    cout << min_step << "\n\n";
#endif

    return 0;
}