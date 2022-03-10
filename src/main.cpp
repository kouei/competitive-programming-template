#include <bits/stdc++.h>

using namespace std;

static int _ = [](){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 0;
}();

const int N = 2000010; // twice as long as the orginal string.

struct Node {
    int maxlen; // max substring len of this state.
    int parent; // link to parent node (aka. suffix link).
    int child[26]; // child pointers.
};
Node nodes[N];
int nodes_top = 1;
int last = 1;

int64_t f[N];

void extend(int c) {
    int p = last;
    int np = ++nodes_top;
    f[np] = 1;

    nodes[np].maxlen = nodes[p].maxlen + 1;

    while(p && !nodes[p].child[c]) {
        nodes[p].child[c] = np;
        p = nodes[p].parent;
    }

    if(!p) {
        nodes[np].parent = 1;
    } else {
        int q = nodes[p].child[c];
        if(nodes[q].maxlen == nodes[p].maxlen + 1) {
            nodes[np].parent = q;
        } else {
            int nq = ++nodes_top;
            nodes[nq] = nodes[q], nodes[nq].maxlen = nodes[p].maxlen + 1;
            nodes[q].parent = nodes[np].parent = nq;
            while(p && nodes[p].child[c] == q) {
                nodes[p].child[c] = nq;
                p = nodes[p].parent;
            }
        }
    }
    last = np;
}

int64_t ans;
int head[N];
int ver[N];
int nxt[N];
int idx = 1;

void add(int a, int b) {
    ver[++idx] = b;
    nxt[idx] = head[a];
    head[a] = idx;
}


void dfs(int x) {
    for(int i = head[x]; i; i = nxt[i]) {
        dfs(ver[i]);
        f[x] += f[ver[i]];
    }

    if(f[x] > 1) {
        ans = max(ans, f[x] * nodes[x].maxlen);
    }
}


int main() {
    string str;
    cin >> str;

    for(char ch : str) {
        extend(ch - 'a');
    }

    for(int i = 2; i <= nodes_top; ++i) {
        add(nodes[i].parent, i);
    }
    dfs(1);
    cout << ans << "\n";
    return 0;
}