/*
```
题目
黑塔女士的普通攻击可以对一名敌人造成一点伤害。
此外，黑塔女士的天赋，会在敌人的生命值首次降低到初始值的二分之一(向下取整)时，立即发动追加攻击，对所有敌人造成一点伤害。
追加攻击造成的伤害可以继续触发追加攻击，且多个敌人的生命值同时下降到二分之一时，每个敌人都可以触发一次追加攻击。
但是请注意，每个敌人在被消灭前，最多只能触发一次追加攻击。
现在有许多敌人，黑塔女士应该如何用最少的普通攻击消灭所有敌人呢?

输入：
一个长度为n的数组a，表示有n名敌人，第i名敌人的初始生命值为a[i]。

数据范围：
0 < n < 100000
0 < a[i] < 1000000000

输出：
一个整数，表示消灭所有敌人所需要的最少普通攻击次数

示例1：
[5, 6]

示例1输出：
7

示例1解释：
对0号敌人进行3次普通攻击，对1号敌人进行4次普通攻击


示例2：
[1, 2, 3, 4, 5]

示例2输出：
1

示例2解释：
对0号敌人进行1次普通攻击


示例3：
[1, 3, 6]

示例3输出：
3

示例3解释：
对2号敌人进行3次普通攻击
```
*/

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