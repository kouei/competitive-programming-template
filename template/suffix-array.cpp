/*******************************Description*******************************/

// This file demonstrate how to construct suffix array in linear time.

/*******************************Description*******************************/

#include <bits/stdc++.h>
using namespace std;

/***************************Template Part Begin***************************/

void createSuffixArray(const string & str, vector<int> & sa, vector<int> & LCP);
void makeSuffixArray(const vector<int> & s, vector<int> & SA, int n, int K);
int assignNames(const vector<int> & s, vector<int> & s12, vector<int> & SA12,
                int n0, int n12, int K);
void radixPass(const vector<int> & in, vector<int> & out,
               const vector<int> & s, int offset, int n, int K);
void computeS12(vector<int> & s12, vector<int> & SA12, int n12, int K12);
void computeS0(const vector<int> & s, vector<int> & s0,
               vector<int> & SA0, const vector<int> & SA12,
               int n0, int n12, int K);
void merge(const vector<int> & s, const vector<int> & s12,
           vector<int> & SA, const vector<int> & SA0, const vector<int> & SA12,
           int n, int n0, int n12, int t);
int getIndexIntoS(const vector<int> & SA12, int t, int n0);
bool suffix12IsSmaller(const vector<int> & s, const vector<int> & s12,
                       const vector<int> & SA12, int n0, int i, int j, int t);
void makeLCPArray(vector<int> & s, const vector<int> & a, vector<int> & LCP);

/// <summary>
/// This function does 2 things:
/// 1. Fill in the suffix array for string str.
/// 2. Fill in the LCP array (Longest Common Prefix) for string str.
/// </summary>
/// <param name="str">Input string.</param>
/// <param name="sa">An existing array to place the suffix array.</param>
/// <param name="LCP">LCP[i] means the length of the longest common prefix between suffix that starts at sa[i] and suffix that starts at sa[i - 1].</param>
/// <exception cref="invalid_argument">Thrown when the size of str, sa and LCP are not the same.</exception>
void createSuffixArray(const string & str, vector<int> & sa, vector<int> & LCP)
{
    assert(sa.size() == str.length() && LCP.size() == str.length());

    int N = str.length();

    vector<int> s(N + 3);  // Padding three '\0' characters in the end. Padding character Must be the smallest character in the dictionary.
    vector<int> SA(N + 3); // Same as the above.

    for (int i = 0; i < N; ++i) {
        s[i] = str[i];
    }

    // Here we assume the largest character in dictionary is 250.
    // For ASCII, it is large enough.
    // If you want to get something more accurate,
    // you can perform discretization on str first.
    makeSuffixArray(s, SA, N, 250);

    for (int i = 0; i < N; ++i) {
        sa[i] = SA[i];
    }

    makeLCPArray(s, sa, LCP);
}

/// <summary>
/// find the suffix array SA of s[0..n-1] in {1..K}∧n
/// requires s[n] = s[n+1] = s[n+2] = 0, n>=2
/// </summary>
/// <param name="s">Input string, each character is represented as a number in {1..K}.</param>
/// <param name="SA">Suffix array of s.</param>
/// <param name="n">Length of s.</param>
/// <param name="K">Maximum character in the dictionary of s. The dictionary is represented as {1..K}.</param>
void makeSuffixArray(const vector<int> &s, vector<int> &SA, int n, int K)
{
    // Here we will construct S0, S1 and S2 from s.
    // S0, S1 and S2 will be made up by tri-characters.
    // Example:
    // s = ABRACADABRA, Len(s) = 11
    // S0 = (ABR)(ACA)(DAB)(RA), Len(S0) = 4
    // S1 = A(BRA)(CAD)(ABR)(A), Len(S1) = 4, notice that we skip 1 character (A) at the beginning.
    // S2 = AB(RAC)(ADA)(BRA), Len(S2) = 3, notice that we skip 2 characters (AB) at the beginning.

    // Length of S0. Think of it as ((n - 0) + 2) / 3.
    // "- 0" because we didn't skip any character at the beginning.
    // "+ 2" because we want to round up when dividing 3.
    int n0 = (n + 2) / 3;
    int n1 = (n + 1) / 3;  // Same as the above
    int n2 = n / 3;        // Same as the above
    int t = n0 - n1;       // 1 iff n%3 == 1
    
    // Think of it as "int n12 = n0 + n2".
    // We do this because we want S1 to have as many characters as S0.
    // The effect of doing this is the same as adding the smallest tri-character ('\0','\0','\0') to the end of S1.
    // This is convenient when we do the "merge" step.
    int n12 = n1 + n2 + t;

    // Since we may recursively calculate suffix array for s12 as well.
    // Padding 3 characters in the end as we did to str.
    vector<int> s12(n12 + 3);
    vector<int> SA12(n12 + 3); // Same as the above
    vector<int> s0(n0);
    vector<int> SA0(n0);

    
    for (int i = 0, j = 0; i < n + t; ++i) {
        // if i % 3 == 1, then tri-character(s[i],s[i+1],s[i+2]) belongs to S1.
        // if i % 3 == 2, then tri-character(s[i],s[i+1],s[i+2]) belongs to S2.
        if (i % 3 != 0) {
            s12[j++] = i; // Here s12[j++] denotes the start position for tri-character (s[i],s[i+1],s[i+2]) (which is i).
        }
    }

    int K12 = assignNames(s, s12, SA12, n0, n12, K);

    computeS12(s12, SA12, n12, K12); // notice here we are passing K12
    computeS0(s, s0, SA0, SA12, n0, n12, K); // notice here we are passing K
    merge(s, s12, SA, SA0, SA12, n, n0, n12, t);
}

/// <summary>
/// Assigns the new supercharacter names.
/// At end of routine, SA will have indices into s, in sorted order
/// and s12 will have new character names
/// Returns the number of names assigned; note that if
/// this value is the same as n12, then SA is a suffix array for s12.
/// </summary>
/// <param name="s">Input string, each character is represented as a number in {1..K}.</param>
/// <param name="s12">Concatenation of S1 & S2, which are made by tri-characters.</param>
/// <param name="SA12">Suffix array of s12.</param>
/// <param name="n0">Length of S0.</param>
/// <param name="n12">Length of S12.</param>
/// <param name="K">Maximum character in the dictionary of s. The dictionary is represented as {1..K}.</param>
int assignNames(const vector<int> & s, vector<int> & s12, vector<int> & SA12,
                int n0, int n12, int K) {
    // Radix sort all tri-characters.
    // Since SA12 has not been used yet,
    // here we treat SA12 as the output buffer in radix sort.
    // Here we also treat s12 and SA12 as input buffer and output buffer alternately.
    radixPass(s12, SA12, s, 2, n12, K);
    radixPass(SA12, s12, s, 1, n12, K);
    radixPass(s12, SA12, s, 0, n12, K);

    // Find lexicographic names of tri-characters
    int name = 0;
    auto last_tri_character = make_tuple(-1, -1, -1);
    // After radix sort, now SA12 contains sorted starting index of all tri-characters in S12.
    // In the next for loop we will count how many unique tri-characters are there and record the number in "name".
    for (int i = 0; i < n12; ++i) {
        auto cur_tri_character = make_tuple(s[SA12[i]], s[SA12[i] + 1], s[SA12[i] + 2]);
        // A new tri-character has been found.
        if (last_tri_character != cur_tri_character) {
            ++name;
            last_tri_character = cur_tri_character;
        }

        // SA12[i] is the index of the tri-character in s
        // SA12[i] / 3 is the index of the tri-character in S1 or S2
        if (SA12[i] % 3 == 1) {
            s12[SA12[i] / 3] = name; // S1
        } else {
            // "+ n0" means all S2 tri-characters come after S1
            s12[SA12[i] / 3 + n0] = name; // S2
        }
    }
    return name;
}

/// <summary>
/// Stably sort in[0..n-1] with indices into s that has keys in {0..K} into out[0..n-1]; 
/// Sort is relative to offset into s.
/// Uses counting radix sort.
/// </summary>
/// <param name="in">Input buffer, each element is an index into s.</param>
/// <param name="out">Output buffer.</param>
/// <param name="s">Input string, each character is represented as a number in {1..K}.</param>
/// <param name="offset">Offset adding to index in s.</param>
/// <param name="n">Length of s.</param>
/// <param name="K">Maximum character in the dictionary of s. The dictionary is represented as {1..K}.</param>
void radixPass(const vector<int> & in, vector<int> & out,
               const vector<int> & s, int offset, int n, int K) {
    vector<int> count(K + 2); // counter array

    for (int i = 0; i < n; ++i) {
        ++count[s[in[i] + offset] + 1]; // count occurrences
    }

    for (int i = 1; i <= K + 1; ++i) {// compute exclusive sums
        count[i] += count[i - 1];
    }

    for (int i = 0; i < n; ++i) {
        out[count[s[in[i] + offset]]++] = in[i]; // sort
    }
}

/// <summary>
/// Compute the suffix array for s12, placing result into SA12.
/// </summary>
/// <param name="s12">Concatenation of S1 & S2, which are made by tri-characters.</param>
/// <param name="SA12">Suffix array of s12.</param>
/// <param name="n12">Length of S12.</param>
/// <param name="K12">Maximum character in the dictionary of s12. The dictionary is represented as {1..K12}.</param>
void computeS12(vector<int> & s12, vector<int> & SA12, int n12, int K12)
{
    // If the number of new names equals to the number of characters in S12,
    // it means every tri-character in S12 is unique.
    // In this case we don't need recursive call to calculate the suffix array of S12,
    // because the start character of a suffix directly decides its relative position among all suffix.
    if (K12 == n12) {
        for (int i = 0; i < n12; ++i) {
            SA12[s12[i] - 1] = i;
        }
    } else {
        // Note that we will always have K12 <= n12.
        // If K12 > n12, it means some characters in the dictionary are not used in S12.
        // This is impossible in our algorithm.
        // Hence we always have K12 <= n12.
        // And since we enter this "else" block, it means K12 != n12.
        // So we have K12 < n12 now. (Which means there are duplicate characters in S12)
        makeSuffixArray(s12, SA12, n12, K12);

        // 如何理解下面这个for循环：
        // 可以思考一个问题：假如已知某个字符串的后缀数组，能否倒推原始字符串？
        // 直觉告诉你肯定不行，那如果再限定这个字符串里的每个字符都各不相同呢？
        // 此时，假如后缀数组的第0号元素是7，
        // 那么意味着字典序排名第1的后缀开始于下标7，于是我们可以确定原字符串下标7位置的那个字符一定是1（相当于0+1）
        // 那如果后缀数组的第1号元素是3呢？于是我们可以断定原字符串下标3位置的那个字符一定是2（相当于1+1）
        // 为什么要做这一步？是为了后面computeS0更方便
        // 这样处理之后，相当于每个后缀都起始于一个独一无二的字符，那如果我们想比较这些后缀的相对大小只需要比较它们的首字母即可
        // store unique names in s12 using the suffix array
        for (int i = 0; i < n12; ++i) {
            s12[SA12[i]] = i + 1;
        }
    }
}

/// <summary>
/// Compute the suffix array for S0, placing result into SA0.
/// </summary>
/// <param name="s">Input string, each character is represented as a number in {1..K}.</param>
/// <param name="s0">S0, which is made by tri-characters.</param>
/// <param name="SA0">Suffix array of s0.</param>
/// <param name="SA12">Suffix array of s12.</param>
/// <param name="n0">Length of S0.</param>
/// <param name="n12">Length of S12.</param>
/// <param name="K">Maximum character in the dictionary of s. The dictionary is represented as {1..K}.</param>
void computeS0(const vector<int> &s, vector<int> &s0,
               vector<int> &SA0, const vector<int> &SA12,
               int n0, int n12, int K) {
    for (int i = 0, j = 0; i < n12; ++i) {
        if (SA12[i] < n0) { // We only want to use S1 to calculate suffix array of S0.
            // SA12[i] is index into s12.
            // Hence, 3 * SA12[i] is index into s.
            // Hence, s0[j++] is the index (into s) of the starting character of the tri-character in S0.
            s0[j++] = 3 * SA12[i];
        }
    }
    // Suppose suffix0 is a suffix in S0,
    // suffix0 = x + suffix1,
    // where x is a character in s and suffix1 is a suffix in S1.
    // Since we have already sorted all suffix1.
    // To sort all suffix0, we only need to compare the x now.
    radixPass(s0, SA0, s, 0, n0, K); // pass offset = 0 to only compare the x.
}

/// <summary>
/// Merge sorted SA0 suffixes and sorted SA12 suffixes.
/// </summary>
/// <param name="s">Input string, each character is represented as a number in {1..K}.</param>
/// <param name="s12">Concatenation of S1 & S2, which are made by tri-characters.</param>
/// <param name="SA">Suffix array of s.</param>
/// <param name="SA0">Suffix array of s0.</param>
/// <param name="SA12">Suffix array of s12.</param>
/// <param name="n">Length of s.</param>
/// <param name="n0">Length of S0.</param>
/// <param name="n12">Length of S12.</param>
/// <param name="t">Equals to n0 - n1, it will be 1 if n % 3 == 1.</param>
void merge(const vector<int> &s, const vector<int> &s12,
           vector<int> &SA, const vector<int> &SA0, const vector<int> &SA12,
           int n, int n0, int n12, int t) {

    int p = 0, k = 0;

    // If t == 1, it means n % 3 == 1.
    // Remember that in function "makeSuffixArray" we pad the smallest tri-character ('\0','\0','\0') to the end of S1.
    // Hence this tri-character will show up in SA12[0], because the suffix starting at this tri-character will surely be the smallest among all suffix.
    // Hence we start from SA12[1] to skip this tri-character because it is not in our original string s.
    while (t != n12 && p != n0) {
        int i = getIndexIntoS(SA12, t, n0); // S12
        int j = SA0[p];                     // S0

        if (suffix12IsSmaller(s, s12, SA12, n0, i, j, t)) {
            SA[k++] = i;
            ++t;
        } else {
            SA[k++] = j;
            ++p;
        }
    }

    while (p < n0) {
        SA[k++] = SA0[p++];
    } while (t < n12) {
        SA[k++] = getIndexIntoS(SA12, t++, n0);
    }
}

/// <summary>
/// Convert index into S12 to index into s.
/// </summary>
/// <param name="SA12">Suffix array of s12.</param>
/// <param name="n0">Length of S0.</param>
/// <param name="t">Index into SA12.</param>
int getIndexIntoS(const vector<int> & SA12, int t, int n0) {
    if (SA12[t] < n0) {
        return SA12[t] * 3 + 1;
    } else {
        return (SA12[t] - n0) * 3 + 2;
    }
}

/// <summary>
/// Check whether a suffix from S12 is smaller than a suffix from S0.
/// </summary>
/// <param name="s">Input string, each character is represented as a number in {1..K}.</param>
/// <param name="s12">Concatenation of S1 & S2, which are made by tri-characters.</param>
/// <param name="SA12">Suffix array of s12.</param>
/// <param name="n0">Length of S0.</param>
/// <param name="i">Start index into s of a S1 suffix.</param>
/// <param name="j">Start index into s of a S0 suffix.</param>
/// <param name="t">Index into SA12.</param>
bool suffix12IsSmaller(const vector<int> &s, const vector<int> &s12,
                       const vector<int> &SA12, int n0, int i, int j, int t) {
    // For a S1 suffix, if its index into s12 is x,
    // then the S2 suffix right after this S1 suffix has a index equals to x + n0

    // Likewise, for a S2 suffix, if its index into s12 is y,
    // then the S1 suffix right before this S2 suffix has a index equals to y - n0

    // Explanation:
    // If the start index (into s) of a S1 suffix is p1, then p1 % 3 == 1.
    // If the start index (into s) of a S2 suffix that is right after the above S1 suffix is p2, then p2 % 3 == 2.
    // We also have p2 == p1 + 1.
    // Hence, p1 / 3 == p2 / 3.
    // In function "assignNames", we do the following:
    // s12[p1 / 3] = name;
    // s12[p2 / 3 + n0] = name;

    if (SA12[t] < n0) {// s1 vs s0; can break tie after 1 character
        return make_pair(s[i], s12[SA12[t] + n0]) <= make_pair(s[j], s12[j / 3]);
    } else {// s2 vs s0; can break tie after 2 characters
        // As we all know, S0, S1 and S2 show up alternately like the following:
        // S0,S1,S2,S0,S1,S2...
        // S0,S1,[S2],(S0),S1,S2...
        // If we SA[t] is the index of [S2] into s12,
        // to get the index of (S0), we use SA12[t] - n0 + 1
        return make_tuple(s[i], s[i + 1], s12[SA12[t] - n0 + 1]) <= make_tuple(s[j], s[j + 1], s12[j / 3 + n0]);
    }
}

/// <summary>
/// Create the LCP array from the suffix array
/// s is the input array populated from 0..N-1, with available pos N
/// sa is an already-computed suffix array 0..N-1
/// LCP is the resulting LCP array 0..N-1
/// </summary>
/// <param name="s">Input string, each character is represented as a number in {1..K}.</param>
/// <param name="sa">An existing array to place the suffix array.</param>
/// <param name="LCP">LCP[i] means the length of the longest common prefix between suffix that starts at sa[i] and suffix that starts at sa[i - 1].</param>
void makeLCPArray(vector<int> &s, const vector<int> &sa, vector<int> &LCP) {
    int N = sa.size();
    vector<int> rank(N);

    s[N] = -1;
    for (int i = 0; i < N; ++i) {
        rank[sa[i]] = i; // rank[sa[i]] means the rank of the suffix starting at sa[i].
    }

    int h = 0; // current length of common prefix.
    for (int i = 0; i < N; ++i) {
        if (rank[i] > 0) {
            int j = sa[rank[i] - 1]; // starting index of the suffix that has rank[i] - 1.

            // Compute common prefix length of the suffix which has rank[i] and the suffix which has rank[i] - 1.
            while (s[i + h] == s[j + h]) {
                ++h;
            }

            LCP[rank[i]] = h;
            if (h > 0) {
                 // when we enter the next loop, we are actually skipping 1 character at the beginning (which is already matched).
                 // Hence we minus 1 from h.
                --h;
            }
        }
    }
}

/***************************Template Part End***************************/

int main() {
    string str = "ABRACADABRA";
    int n = str.size();
    vector<int> sa(n);
    vector<int> LCP(n);
    createSuffixArray(str, sa, LCP);

    cout << "String: " << str << "\n\n";
    cout << "LCP" << "\t" << "Suffix" << "\n";
    for(int i = 0; i < n; ++i) {
        cout << LCP[i] << "\t" << str.substr(sa[i]) << "\n";
    }
    return 0;
}