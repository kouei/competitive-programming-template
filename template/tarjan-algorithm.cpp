// https://codeforces.com/contest/427/problem/C

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

const int MOD = 1e9 + 7;
const int MAXN = 1e5 + 10;
int N, M;
int cost[MAXN];
vector<vector<int>> neighbors;

int next_id;
int id[MAXN];
int lowlink[MAXN];
char on_stack[MAXN];

int stk_top;
int stk[MAXN];

int min_cost_top;
int min_cost[MAXN];
int min_cost_count[MAXN];


void dfs(int x) {
    id[x] = lowlink[x] = next_id++;
    stk[stk_top++] = x;
    on_stack[x] = 1;

    for(int y : neighbors[x]) {
        if(id[y] == -1) {
            dfs(y);
            lowlink[x] = min(lowlink[x], lowlink[y]);
        } else if(on_stack[y]) {
            lowlink[x] = min(lowlink[x], lowlink[y]);
        }
    }

    if(lowlink[x] == id[x]) {
        int cur_cost = INT_MAX;
        int cur_count = 0;

        int top;
        do {
            top = stk[--stk_top];
            on_stack[top] = 0;
            if(cur_cost > cost[top]) {
                cur_cost = cost[top];
                cur_count = 1;
            } else if(cur_cost == cost[top]) {
                cur_count += 1;
            }
        } while(top != x);

        min_cost[min_cost_top] = cur_cost;
        min_cost_count[min_cost_top] = cur_count;
        min_cost_top += 1;
    }
}

int main() {
    cin >> N;
    for(int i = 1; i <= N; ++i) {
        cin >> cost[i];
    }
    int n = N + 10;
    neighbors = vector<vector<int>>(n);

    cin >> M;
    for(int i = 0; i < M; ++i) {
        int a, b;
        cin >> a >> b;
        neighbors[a].push_back(b);
    }

    next_id = 1;
    memset(id, 0xff, sizeof(id[0]) * n);
    memset(lowlink, 0xff, sizeof(lowlink[0]) * n);
    memset(on_stack, 0, sizeof(on_stack[0]) * n);

    stk_top = 0;
    min_cost_top = 0;

    for(int i = 1; i <= N; ++i) {
        if(id[i] == -1) {
            dfs(i);
        }
    }

    int64_t res1 = 0;
    int res2 = 1;

    for(int i = 0; i < min_cost_top; ++i) {
        res1 += min_cost[i];
        res2 = (int64_t)res2 * min_cost_count[i] % MOD;
    }

    cout << res1 << " " << res2 << "\n";
    return 0;
}