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

// Macro to accelerate child array copy
#define copy_child(child_a,child_b) { \
    int * ptr_a = (child_a); \
    int * ptr_b = (child_b); \
    ptr_a[0] =  ptr_b[0] ; \
    ptr_a[1] =  ptr_b[1] ; \
    ptr_a[2] =  ptr_b[2] ; \
    ptr_a[3] =  ptr_b[3] ; \
    ptr_a[4] =  ptr_b[4] ; \
    ptr_a[5] =  ptr_b[5] ; \
    ptr_a[6] =  ptr_b[6] ; \
    ptr_a[7] =  ptr_b[7] ; \
    ptr_a[8] =  ptr_b[8] ; \
    ptr_a[9] =  ptr_b[9] ; \
    ptr_a[10] = ptr_b[10]; \
    ptr_a[11] = ptr_b[11]; \
    ptr_a[12] = ptr_b[12]; \
    ptr_a[13] = ptr_b[13]; \
    ptr_a[14] = ptr_b[14]; \
    ptr_a[15] = ptr_b[15]; \
    ptr_a[16] = ptr_b[16]; \
    ptr_a[17] = ptr_b[17]; \
    ptr_a[18] = ptr_b[18]; \
    ptr_a[19] = ptr_b[19]; \
    ptr_a[20] = ptr_b[20]; \
    ptr_a[21] = ptr_b[21]; \
    ptr_a[22] = ptr_b[22]; \
    ptr_a[23] = ptr_b[23]; \
    ptr_a[24] = ptr_b[24]; \
    ptr_a[25] = ptr_b[25]; \
}

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
            copy_child(child[nq], child[q]);
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