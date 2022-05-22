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

using i64 = int64_t;

void merge(int l1, int r1, int l2, int r2, int & l, int & r) {
    assert(r1 <= l2 || r2 <= l1);
    l = min(l1, l2);
    r = max(r1, r2);
}

int main() {
    int N;
    cin >> N;
    int l = 1;
    int r = 1;

    for(int i = 2; i <= N; ++i) {
        merge(l, r, i, i, l, r);
        merge(l, r, l + i, r + i, l, r);
    }

    cout << l << " " << r << "\n";
    return 0;
}