/**************************Example Part Begin**************************/
// pb_ds tree_order_statistics example
// https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/testsuite/ext/pb_ds/example/tree_order_statistics.cc
/***************************Example Part End***************************/

/**************************Header Part Begin**************************/
#include <bits/stdc++.h>
//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
//using namespace __gnu_pbds;

// static int _ = []()
// {
//     ios_base::sync_with_stdio(false);
//     cin.tie(nullptr);
//     cout.tie(nullptr);
//     return 0;
// }();
/***************************Header Part End***************************/

void createSuffixArray(const string &str, vector<int> &sa, vector<int> &LCP);
void makeSuffixArray(const vector<int> &s, vector<int> &SA, int n, int K);
int assignNames(const vector<int> &s, vector<int> &s12, vector<int> &SA12,
                int n0, int n12, int K);
void radixPass(const vector<int> &in, vector<int> &out,
               const vector<int> &s, int offset, int n, int K);
void radixPass(const vector<int> &in, vector<int> &out,
               const vector<int> &s, int n, int K);
void computeS12(vector<int> &s12, vector<int> &SA12,
                int n12, int K12);
void computeS0(const vector<int> &s, vector<int> &s0,
               vector<int> &SA0, const vector<int> &SA12,
               int n0, int n12, int K);
void merge(const vector<int> &s, const vector<int> &s12,
           vector<int> &SA, const vector<int> &SA0, const vector<int> &SA12,
           int n, int n0, int n12, int t);
int getIndexIntoS(const vector<int> &SA12, int t, int n0);
bool leq(int a1, int a2, int b1, int b2);
bool leq(int a1, int a2, int a3, int b1, int b2, int b3);
bool suffix12IsSmaller(const vector<int> &s, const vector<int> &s12,
                       const vector<int> &SA12, int n0, int i, int j, int t);
void makeLCPArray(vector<int> &s, const vector<int> &sa, vector<int> &LCP);

/// <summary>
/// This function does 2 things:
/// 1. Fill in the suffix array for string str.
/// 2. Fill in the LCP array (Longest Common Prefix) for string str.
/// </summary>
/// <param name="str">Input string.</param>
/// <param name="sa">An existing array to place the suffix array.</param>
/// <param name="LCP">An existing array to place the LCP information.</param>
/// <exception cref="invalid_argument">Thrown when the size of str, sa and LCP are not the same.</exception>
void createSuffixArray(const string &str, vector<int> &sa, vector<int> &LCP)
{
    if (sa.size() != str.length() || LCP.size() != str.length())
        throw invalid_argument{"Mismatched vector sizes"};

    int N = str.length();

    vector<int> s(N + 3);  // 可以理解为往原字符串末尾填充了3个'\0'字符，填充什么字符不重要，只要保证填充的字符是当前字典里最小的字符就行
    vector<int> SA(N + 3); // 同上

    for (int i = 0; i < N; ++i)
        s[i] = str[i];

    // 这里假设字典里最大的字符为250，对于ASCII码可显示字符来说，这个值够了
    // 当然，如果你想得到一个比250更低更精确的数字，你可以先对原始字符串进行离散化
    makeSuffixArray(s, SA, N, 250);

    for (int i = 0; i < N; ++i)
        sa[i] = SA[i];

    makeLCPArray(s, sa, LCP);
}

/// <summary>
/// find the suffix array SA of s[0..n-1] in {1..K}∧n
/// requires s[n] = s[n+1] = s[n+2] = 0, n>=2
/// </summary>
/// <param name="s">Input string, each character is represented as a number in {1..K}.</param>
/// <param name="SA">An existing array to place the suffix array.</param>
/// <param name="n">Length of s.</param>
/// <param name="K">Maximum character in dictionary. The dictionary is represented as {1..K}.</param>
void makeSuffixArray(const vector<int> &s, vector<int> &SA, int n, int K)
{
    int n0 = (n + 2) / 3;  // s0的长度，可以理解为((n - 0) + 2) / 3，因为s0在开头没有跳过任何字符，这里的+2是为了实现除以3之后向上取整
    int n1 = (n + 1) / 3;  // s1的长度，可以理解为((n - 1) + 2) / 3，因为s1在开头跳过了1个字符，这里的+2是为了实现除以3之后向上取整
    int n2 = n / 3;        // s2的长度，可以理解为((n - 2) + 2) / 3，因为s2在开头跳过了2个字符，这里的+2是为了实现除以3之后向上取整
    int t = n0 - n1;       // 1 iff n%3 == 1
    int n12 = n1 + n2 + t; // 相当于int n12 = n0 + n2，为什么是这样？见94行开始的注释

    vector<int> s12(n12 + 3);  // 由于s12这个三元字符串后续可能会需要递归求解后缀数组，所以它也要像s一样，在末尾填充3个空字符（由于是空的三元字符，所以这里是填充3个"\0\0\0"三元空字符）
    vector<int> SA12(n12 + 3); // 同上
    vector<int> s0(n0);
    vector<int> SA0(n0);

    // generate positions in s for items in s12
    // the "+t" adds a dummy mod 1 suffix if n%3 == 1
    // at that point, the size of s12 is n12

    // 这里的i < n + t造成的实际效果是：如果t == 1（即n % 3 == 1），那么s12里会多插一个三元字符（这个字符会分配给s1），其值为"\0\0\0"（即3个填充字符）
    // 为什么要这么做？这是因为当n % 3 == 1时，最后一个字符会属于一个s0里的三元字符，而这个三元字符的后面没有对应的来自于s1的三元字符
    // 这给后面的merge步骤带来了不便，因为merge步骤假设每个s0三元字符的后面都有一个s1三元字符，他们俩共享两个字符：
    // 比如：abcd，
    // s0三元字符：abc
    // s1三元字符：bcd
    // 他们共享bc：a(bc)d
    // 上述分析说明了为什么n12 = n0 + n2 而不是 n12 = n1 + n2
    for (int i = 0, j = 0; i < n + t; ++i)
        if (i % 3 != 0)   // 如果i % 3 == 1，那么此时三元字符(s[i],s[i+1],s[i+2])属于s1；同理如果i % 3 == 2，那么便属于s2
            s12[j++] = i; // 注意，这里的i代表的不是s[i]，而是(s[i],s[i+1],s[i+2])这个三元字符的起始位置

    int K12 = assignNames(s, s12, SA12, n0, n12, K);

    computeS12(s12, SA12, n12, K12);         // 注意，这里传入的是K12
    computeS0(s, s0, SA0, SA12, n0, n12, K); // 注意，这里传入的是K
    merge(s, s12, SA, SA0, SA12, n, n0, n12, t);
}

// Assigns the new supercharacter names.
// At end of routine, SA will have indices into s, in sorted order
// and s12 will have new character names
// Returns the number of names assigned; note that if
// this value is the same as n12, then SA is a suffix array for s12.
int assignNames(const vector<int> &s, vector<int> &s12, vector<int> &SA12,
                int n0, int n12, int K)
{
    // radix sort the new character trios
    // 这里使用了一个小技巧，就是把SA12临时拉过来充当输出缓冲区（即out）
    // 之所以可以这么做，是因为算法进行到此处时，SA12还没有真正被用到
    // 这里另外用到的一个小技巧是，让s12和S12轮流充当in和out，这样就不用额外分配in和out的空间
    radixPass(s12, SA12, s, 2, n12, K);
    radixPass(SA12, s12, s, 1, n12, K);
    radixPass(s12, SA12, s, 0, n12, K);

    // find lexicographic names of triples
    int name = 0;
    int c0 = -1, c1 = -1, c2 = -1;

    // 经过了radix sort以后，现在SA12里包含了已经排序好的三元字符所对应的下标（以三元字符中的第一个字符在原始数组中的下标来表示）
    // 所以在接下来的这个for循环里，我们只需要顺序的扫描一遍，就可以知道一共有多少个不同的三元字符（记到name里）
    // 并且根据这个三元字符的第一个字符的下标模3的余数，来判断这个三元字符属于S1还是S2，然后将name（代表新的字典里，这个三元字符的编号）分配给这个三元字符
    for (int i = 0; i < n12; ++i)
    {
        if (s[SA12[i]] != c0 || s[SA12[i] + 1] != c1 || s[SA12[i] + 2] != c2)
        {
            ++name;
            c0 = s[SA12[i]];
            c1 = s[SA12[i] + 1];
            c2 = s[SA12[i] + 2];
        }

        // 这里的SA12[i] / 3是把三元字符的起始字符在原字符串中的下标换算成三元字符串中的下标
        if (SA12[i] % 3 == 1)
            s12[SA12[i] / 3] = name; // S1
        else
            s12[SA12[i] / 3 + n0] = name; // S2
    }

    return name;
}

// stably sort in[0..n-1] with indices into s that has keys in 0..K
// into out[0..n-1]; sort is relative to offset into s
// uses counting radix sort
void radixPass(const vector<int> &in, vector<int> &out,
               const vector<int> &s, int offset, int n, int K)
{
    vector<int> count(K + 2); // counter array

    for (int i = 0; i < n; ++i)
        ++count[s[in[i] + offset] + 1]; // count occurrences

    for (int i = 1; i <= K + 1; ++i) // compute exclusive sums
        count[i] += count[i - 1];

    for (int i = 0; i < n; ++i)
        out[count[s[in[i] + offset]]++] = in[i]; // sort
}

// stably sort in[0..n-1] with indices into s that has keys in 0..K
// into out[0..n-1]
// uses counting radix sort
void radixPass(const vector<int> &in, vector<int> &out,
               const vector<int> &s, int n, int K)
{
    radixPass(in, out, s, 0, n, K);
}

// Compute the suffix array for s12, placing result into SA12
void computeS12(vector<int> &s12, vector<int> &SA12,
                int n12, int K12)
{
    // 如果新的名字的数量跟老的名字的数量一样，那么意味着每个三元字符都是独一无二的，
    // 如果一个字符串的每个字符都是独一无二的，此时求这个字符串的后缀数组及其简单。
    // 因为此时，一个后缀的首字母直接决定了这个后缀在所有后缀中的字典序
    // 这也是下面这个for循环所做的事情。
    if (K12 == n12) // if unique names, don’t need recursion
        for (int i = 0; i < n12; ++i)
            SA12[s12[i] - 1] = i;
    else
    {
        // 当进到else里来的时候，可以肯定K12 < n12。
        // 这是因为K12 > n12这种情况是不存在的。
        // 比如，n12 = 10，意味着字符串本身才包含10个字符，
        // 假如K12 = 15，意味着字典里有15个字符从1号一直到15号，
        // 但是你想，即使n12中每个字符都各不相同，仍然意味着字典里有5个字符没用上，
        // 那它们存在有什么意义？
        // 所以必然有K12 <= n12，
        // 而上面的if排除了K12 == n12，
        // 所以此时必然有K12 < n12
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
        for (int i = 0; i < n12; ++i)
            s12[SA12[i]] = i + 1;
    }
}

void computeS0(const vector<int> &s, vector<int> &s0,
               vector<int> &SA0, const vector<int> &SA12,
               int n0, int n12, int K)
{
    for (int i = 0, j = 0; i < n12; ++i)
        // 通过SA12[i] < n0我们跳过了所有S2中的后缀，因为我们只打算根据S1后缀找到对应的S0后缀。
        // （还记得在makeSuffixArray这个函数中我们特意引入一个t变量吗？就是为了S1和S0一一对应）
        if (SA12[i] < n0)
            // 有了上面的那行注释之后，现在你知道了，此时下面的SA12[i]必然指向某个S1中的后缀在s12中的位置
            // 这里3 * SA12[i]就可以得到S0中三元字符的起始字符在原字串中的下标。
            // 想想s12的原始形态长什么样子？必然是下面这种样子：
            // 1,4,7,10 ... （S1结束，跟着开始S2） 2,5,8,11 ...
            // 假如S12[i] == 2，
            // 那么3 * SA12[i] == 6,
            // 此时，仔细想一想，
            // s12[2]是不是等于7？（并且根据上面那个if，这一定是一个S1里的后缀噢）
            // 那如果S1里的某个后缀的开头的三元字符的开头字符如果是s中7这个位置的字符
            // 那么对应S0的后缀的开头的三元字符的开头字符是不是就是s中6这个位置的字符？
            s0[j++] = 3 * SA12[i];

    // 由于每个S0里的后缀，都相当于这个后缀在s中的首字符 + 其后的一个S1的后缀。
    // 因此，由于上面构造s0的时候，我们是按照SA12[i]的顺序构造的，所以相当于已经按S1后缀排过序了
    // 那么接下来我们只需要对这个所谓的s中的首字符排序，就相当于对S0里的后缀排序了，也就得到了S0的后缀数组
    radixPass(s0, SA0, s, n0, K); // 这里相当于radixPass(s0, SA0, s, 0, n0, K); （即传入offset=0）
}

// merge sorted SA0 suffixes and sorted SA12 suffixes
void merge(const vector<int> &s, const vector<int> &s12,
           vector<int> &SA, const vector<int> &SA0, const vector<int> &SA12,
           int n, int n0, int n12, int t)
{
    int p = 0, k = 0;

    while (t != n12 && p != n0)
    {
        int i = getIndexIntoS(SA12, t, n0); // S12
        int j = SA0[p];                     // S0

        if (suffix12IsSmaller(s, s12, SA12, n0, i, j, t))
        {
            SA[k++] = i;
            ++t;
        }
        else
        {
            SA[k++] = j;
            ++p;
        }
    }

    while (p < n0)
        SA[k++] = SA0[p++];
    while (t < n12)
        SA[k++] = getIndexIntoS(SA12, t++, n0);
}

// 下面这个函数的作用是将S1或S2中的下标，换算成s中的下标
int getIndexIntoS(const vector<int> &SA12, int t, int n0)
{
    if (SA12[t] < n0)
        return SA12[t] * 3 + 1;
    else
        return (SA12[t] - n0) * 3 + 2;
}

bool leq(int a1, int a2, int b1, int b2)
{
    return a1 < b1 || (a1 == b1 && a2 <= b2);
}

// True if [a1 a2] <= [b1 b2 b3]
bool leq(int a1, int a2, int a3, int b1, int b2, int b3)
{
    return a1 < b1 || (a1 == b1 && leq(a2, a3, b2, b3));
}

bool suffix12IsSmaller(const vector<int> &s, const vector<int> &s12,
                       const vector<int> &SA12, int n0, int i, int j, int t)
{
    if (SA12[t] < n0) // s1 vs s0; can break tie after 1 character
        return leq(s[i], s12[SA12[t] + n0],
                   s[j], s12[j / 3]);
        // 上面那句等价于下面这句：
        // return make_tuple(s[i], s12[SA12[t] + n0]) <= make_tuple(s[j], s12[j / 3]);
    else // s2 vs s0; can break tie after 2 characters
        return leq(s[i], s[i + 1], s12[SA12[t] - n0 + 1],
                   s[j], s[j + 1], s12[j / 3 + n0]);
        // 上面那句等价于下面这句：
        // return make_tuple(s[i], s[i + 1], s12[SA12[t] - n0 + 1]) <= make_tuple(s[j], s[j + 1], s12[j / 3 + n0]);

}

/*
* Create the LCP array from the suffix array
* s is the input array populated from 0..N-1, with available pos N
* sa is an already-computed suffix array 0..N-1
* LCP is the resulting LCP array 0..N-1
*/
void makeLCPArray(vector<int> &s, const vector<int> &sa, vector<int> &LCP)
{
    int N = sa.size();
    vector<int> rank(N);

    s[N] = -1;
    for (int i = 0; i < N; ++i)
        rank[sa[i]] = i; // rank[sa[i]]表示，从sa[i]这个位置开始的后缀，在所有后缀中的排名

    int h = 0; // h是当前已经匹配的公共前缀长度
    for (int i = 0; i < N; ++i)
        if (rank[i] > 0)
        {
            int j = sa[rank[i] - 1];

            // 下面的while循环是计算排名为rank[i]的后缀和排名为rank[i] - 1的后缀的公共前缀长度
            while (s[i + h] == s[j + h])
                ++h;

            LCP[rank[i]] = h;
            if (h > 0)
                --h; // 这里--h是为了下一次循环做准备，因为当++i的时候，相当于跳过了1个公共前缀字符，于是h就相应地减1
        }
}

int main()
{
    int t;
    scanf("%d", &t);
    getchar();

    string str;
    while(true) {
        int ch = getchar();
        if(ch == EOF || ch == '\n') break;
        str += ch;
    }
    int n = str.size();
    vector<int> sa(n);
    vector<int> LCP(n);
    createSuffixArray(str, sa, LCP);

    int cur_pos = -1;
    int count = 0;
    int max_count = -1;
    int max_pos = -1;
    for(int i = n - 1; i >= 0; --i) {
        if(cur_pos == -1) {
            if(sa[i] + t <= n) {
                cur_pos = sa[i];
                count = 1;
            }
        }

        if(LCP[i] >= t) {
            count += 1;
            if(max_count <= count) {
                max_count = count;
                max_pos = cur_pos;
            }
        } else {
            cur_pos = -1;
            count = 0;
        }

    }

    cout << str.substr(max_pos, t) << " " << max_count << "\n";

    // for(int i : sa) {
    //     cout << str.substr(i) << "\n";
    // }
    // cout << "\n";
    // for(int i : LCP) cout << i << " ";
    // cout << "\n";

    return 0;
}