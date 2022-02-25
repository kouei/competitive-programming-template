#include<bits/stdc++.h>
using namespace std;

static int _ = [](){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 0;
}();

/// <summary>
/// Create suffix array for str using SA-IS algorithm.
/// </summary>
/// <param name="s">Input string, each character is represented as a number in {1..K}. </param>
/// <param name="sa">An existing array to place the suffix array.</param>
/// <param name="K">Maximum character in the dictionary of str. The dictionary is represented as {1..K}.</param>
void createSuffixArray(vector<int> & s, vector<int> & sa, int K) {
    int n = s.size();
    s.push_back(0); // Add the smallest character in dictionary to the end of string.

    const int S_TYPE = 0;
    const int L_TYPE = 1;
    vector<int> t(n + 1);

    t[n] = S_TYPE;
    for(int i = n - 1; i >= 0; --i) {
        if(s[i] < s[i + 1]) {
            t[i] = S_TYPE;
        } else if(s[i] > s[i + 1]) {
            t[i] = L_TYPE;
        } else {
            t[i] = t[i + 1];
        }
    }

    vector<int> p1;
    p1.reserve(n + 1);

    for(int i = 1; i < n; ++i) {
        if(t[i] == S_TYPE && t[i - 1] == L_TYPE) {
            p1.push_back(i);
        }
    }
    p1.push_back(n);

    vector<int> l_count_for_each_bucket(K + 1);
    for(int i = 0; i < n + 1; ++i) {
        if(t[i] == L_TYPE) {
            l_count_for_each_bucket[s[i]] += 1;
        }
    }
}

int main() {
    string str = "ABRACADABRA";
    int n = str.size();
    vector<int> s(n);
    for(int i = 0; i < n; ++i) {
        s[i] = str[i] - 'A' + 1;
    }
    return 0;
}