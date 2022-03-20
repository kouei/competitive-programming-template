#include <bits/stdc++.h>

using namespace std;

const int N = 2000010; // Must be at least twice as long as the orginal string.

/**************** Suffix Automaton Fields Begin ****************/

int nodes_top = 1;  // Current max node index.
int last = 1;       // Last extended node.

int maxlen[N];      // Max length of substrings recorded in this state (node).
int parent[N];      // Points to parent node (aka. suffix link).
int child[N][26];   // Assume only lower case letters.

/***************** Suffix Automaton Fields End *****************/

/**************** Suffix Automaton Construction Begin ****************/

// Online suffix automaton construction algorithm
void extend(int c) {
    int p = last;
    int cur = ++nodes_top;
    
    maxlen[cur] = maxlen[p] + 1;

    while(p && !child[p][c]) {
        child[p][c] = cur;
        p = parent[p];
    }

    if(!p) {
        parent[cur] = 1;
    } else {
        int q = child[p][c];
        if(maxlen[q] == maxlen[p] + 1) {
            parent[cur] = q;
        } else {
            int nq = ++nodes_top;
            for(int i = 0; i < 26; ++i) {
                child[nq][i] = child[q][i];
            }
            parent[nq] = parent[q];
            maxlen[nq] = maxlen[p] + 1;
            parent[cur] = nq;
            parent[q] = nq;
            while(p && child[p][c] == q) {
                child[p][c] = nq;
                p = parent[p];
            }
        }
    }
    last = cur;
}
/***************** Suffix Automaton Construction End *****************/

int main() {
    string str = "aabab";

    for(char ch : str) {
        extend(ch - 'a');
    }

    for(int i = 1; i <= nodes_top; ++i) {
        printf("\"Node %d\" : {\n", i);
        printf("\t\"Max Substring Length\" : %d\n", maxlen[i]);
        printf("\t\"Parent Node\" : %d\n", parent[i]);
        printf("\t\"Child Nodes\" : {\n");
        for(int j = 0; j < 26; ++j) {
            if(child[i][j]) {
                printf("\t\t%c -> %d\n", 'a' + j, child[i][j]);
            }
        }
        printf("\t}\n");
        printf("}\n\n");
    }

    return 0;
}