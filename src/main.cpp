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
    int cur_hp;
};

vector<Monster> monsters;

int solve() {
    int n = static_cast<int>(monsters.size());

    int res = 0;
    for(auto & m : monsters) {
        if (m.cur_hp > n) {
            res += m.cur_hp - n;
            m.cur_hp = n;
        }
    }

    auto is_less = [](Monster a, Monster b) {
        int half_a_dis = a.cur_hp - a.max_hp / 2;
        int half_b_dis = b.cur_hp - b.max_hp / 2;
        return half_a_dis < half_b_dis;
    };

    std::sort(monsters.begin(), monsters.end(), is_less);

    int aoe = 0;
    for(auto & m : monsters) {
        m.cur_hp -= aoe;
        if(m.cur_hp > m.max_hp / 2) {
            res += m.cur_hp - m.max_hp / 2;
        }

        aoe += 1;
    }

    return res;
}

int main() {
    while (true) {
        int hp;
        if (cin >> hp) {
            monsters.push_back({hp, hp});
        } else {
            break;
        }
    }

    int res = solve();
    cout << res << "\n\n";
    return 0;
}