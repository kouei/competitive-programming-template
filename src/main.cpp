#include<bits/stdc++.h>
using namespace std;

static int _ = [](){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return 0;
}();

const int S_TYPE = 0;
const int L_TYPE = 1;

vector<int> buildTypeMap(const string & data) {
    // Builds a map marking each suffix of the data as S_TYPE or L_TYPE.

    // The map should contain one more entry than there are characters
    // in the string, because we also need to store the type of the
    // empty suffix between the last character and the end of the
    // string.
    vector<int> res(data.size() + 1);
    int n = res.size();

    // The empty suffix after the last character is S_TYPE
    res[n - 1] = S_TYPE;

    // If this is an empty string...
    if(data.empty()) {
        // ...there are no more characters, so we're done.
        return res;
    }

    // The suffix containing only the last character must necessarily
    // be larger than the empty suffix.
    res[n - 2] = L_TYPE;

    // Step through the rest of the string from right to left.
    for(int i = n - 3; i >= 0; --i) {
        if(data[i] > data[i + 1]) {
            res[i] = L_TYPE;
        } else if(data[i] < data[i + 1]) {
            res[i] = S_TYPE;
        } else {
            res[i] = res[i + 1];
        }
    }

    return res;
}

bool isLMSChar(int offset, const vector<int> & typemap) {
    // Returns true if the character at offset is a left-most S-type.
    return offset > 0 &&
           typemap[offset] == S_TYPE &&
           typemap[offset - 1] == L_TYPE;
}

bool lmsSubstringsAreEqual(const string & str, const vector<int> & typemap, int offsetA, int offsetB) {
    // Return True if LMS substrings at offsetA and offsetB are equal.
    // No other substring is equal to the empty suffix.
    if(offsetA == str.size() || offsetB == str.size()) {
        return false;
    }

    int i = 0;
    while(true) {
        bool aIsLMS = isLMSChar(i + offsetA, typemap);
        bool bIsLMS = isLMSChar(i + offsetB, typemap);

        // If we've found the start of the next LMS substrings...
        if(i > 0 && aIsLMS && bIsLMS) {
            // ...then we made it all the way through our original LMS
            // substrings without finding a difference, so we can go
            // home now.
            return true;
        }

        if(aIsLMS != bIsLMS) {
            // We found the end of one LMS substring before we reached
            // the end of the other.
            return false;
        }

        if(str[i + offsetA] != str[i + offsetB]) {
            // We found a character difference, we're done.
            return false;
        }

        i += 1;
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