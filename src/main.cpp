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

vector<int> buildTypeMap(const vector<int> & data);
bool isLMSChar(int offset, const vector<int> & typemap);
bool lmsSubstringsAreEqual(const vector<int> & str, const vector<int> & typemap, int offsetA, int offsetB);
vector<int> findBucketSizes(const vector<int> & str, int alphabetSize);
vector<int> findBucketHeads(const vector<int> & bucketSizes);
vector<int> findBucketTails(const vector<int> & bucketSizes);
vector<int> makeSuffixArrayByInducedSorting(const vector<int> & str, int alphabetSize);
vector<int> guessLMSSort(const vector<int> & str, const vector<int> & bucketSizes, const vector<int> & typemap);
void induceSortL(const vector<int> & str, vector<int> & guessedSuffixArray, const vector<int> & bucketSizes, const vector<int> & typemap);
void induceSortS(const vector<int> & str, vector<int> & guessedSuffixArray, const vector<int> & bucketSizes, const vector<int> & typemap);
tuple<vector<int>, int, vector<int>> summariseSuffixArray(const vector<int> & str, const vector<int> & guessedSuffixArray, const vector<int> & typemap);
vector<int> makeSummarySuffixArray(const vector<int> & summaryString, int summaryAlphabetSize);
vector<int> accurateLMSSort(const vector<int> str, const vector<int> & bucketSizes, const vector<int> & typemap, const vector<int> & summarySuffixArray, const vector<int> & summarySuffixOffsets);
vector<int> encode(const string & s);

vector<int> buildTypeMap(const vector<int> & data) {
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

bool lmsSubstringsAreEqual(const vector<int> & str, const vector<int> & typemap, int offsetA, int offsetB) {
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

vector<int> findBucketSizes(const vector<int> & str, int alphabetSize) {
    vector<int> res(alphabetSize);
    for(int ch : str) {
        res[ch] += 1;
    }
    return res;
}

vector<int> findBucketHeads(const vector<int> & bucketSizes) {
    int offset = 1;
    vector<int> res(bucketSizes.size());
    for(int i = 0; i < bucketSizes.size(); ++i) {
        res[i] = offset;
        offset += bucketSizes[i];
    }
    return res;
}

vector<int> findBucketTails(const vector<int> & bucketSizes) {
    int offset = 1;
    vector<int> res(bucketSizes.size());
    for(int i = 0; i < bucketSizes.size(); ++i) {
        offset += bucketSizes[i];
        res[i] = offset - 1;
    }
    return res;
}

vector<int> makeSuffixArrayByInducedSorting(const vector<int> & str, int alphabetSize) {
    // Compute the suffix array of 'string' with the SA-IS algorithm.
    
    // Classify each character of the string as S_TYPE or L_TYPE
    vector<int> typemap = buildTypeMap(str);
    // We'll be slotting suffixes into buckets according to what
    // character they start with, so let's precompute that info now.
    vector<int> bucketSizes = findBucketSizes(str, alphabetSize);
    
    // Use a simple bucket-sort to insert all the LMS suffixes into
    // approximately the right place the suffix array.
    vector<int> guessedSuffixArray = guessLMSSort(str, bucketSizes, typemap);
    // Slot all the other suffixes into guessedSuffixArray, by using
    // induced sorting. This may move the LMS suffixes around.
    induceSortL(str, guessedSuffixArray, bucketSizes, typemap);
    induceSortS(str, guessedSuffixArray, bucketSizes, typemap);
    
    // Create a new string that summarises the relative order of LMS
    // suffixes in the guessed suffix array.
    auto temp = summariseSuffixArray(str, guessedSuffixArray, typemap);
    vector<int> summaryString = move(get<0>(temp));
    int summaryAlphabetSize = move(get<1>(temp));
    vector<int> summarySuffixOffsets = move(get<2>(temp));
    
    // Make a sorted suffix array of the summary string.
    vector<int> summarySuffixArray = makeSummarySuffixArray(summaryString, summaryAlphabetSize);
    
    // Using the suffix array of the summary string, determine exactly
    // where the LMS suffixes should go in our final array.
    vector<int> result = accurateLMSSort(str, bucketSizes, typemap, summarySuffixArray, summarySuffixOffsets);
    
    // ...and once again, slot all the other suffixes into place with
    // induced sorting.
    induceSortL(str, result, bucketSizes, typemap);
    induceSortS(str, result, bucketSizes, typemap);
    
    return result;
}

vector<int> guessLMSSort(const vector<int> & str, const vector<int> & bucketSizes, const vector<int> & typemap) {
    // Make a suffix array with LMS-substrings approximately right.
    
    // Create a suffix array with room for a pointer to every suffix of
    // the string, including the empty suffix at the end.
    vector<int> guessedSuffixArray(str.size() + 1, -1);

    vector<int> bucketTails = findBucketTails(bucketSizes);

    // Bucket-sort all the LMS suffixes into their appropriate bucket.
    for(int i = 0; i < str.size(); ++i) {
        if(!isLMSChar(i, typemap)) {
            // Not the start of an LMS suffix
            continue;
        }

        // Which bucket does this suffix go into?
        int bucketIndex = str[i];
        // Add the start position at the tail of the bucket...
        guessedSuffixArray[bucketTails[bucketIndex]] = i;
        // ...and move the tail pointer down.
        bucketTails[bucketIndex] -= 1;
    }

    // The empty suffix is defined to be an LMS-substring, and we know
    // it goes at the front.
    guessedSuffixArray[0] = str.size();

    return guessedSuffixArray;
}

void induceSortL(const vector<int> & str, vector<int> & guessedSuffixArray, const vector<int> & bucketSizes, const vector<int> & typemap) {
    // Slot L-type suffixes into place.
    
    vector<int> bucketHeads = findBucketHeads(bucketSizes);
    
    // For each cell in the suffix array....
    for(int i = 0; i < guessedSuffixArray.size(); ++i) {
        if(guessedSuffixArray[i] == -1) {
            // No offset is recorded here.
            continue;
        }
    
        // We're interested in the suffix that begins to the left of
        // the suffix this entry points at.
        int j = guessedSuffixArray[i] - 1;
        if(j < 0) {
            // This entry in the suffix array is the suffix that begins
            // at the start of the string, offset 0. Therefore there is
            // no suffix to the left of it, and j is out of bounds of
            // the typemap.
            continue;
        }

        if(typemap[j] != L_TYPE) {
            // We're only interested in L-type suffixes right now.
            continue;
        }
    
        // Which bucket does this suffix go into?
        int bucketIndex = str[j];
        // Add the start position at the head of the bucket...
        guessedSuffixArray[bucketHeads[bucketIndex]] = j;
        // ...and move the head pointer up.
        bucketHeads[bucketIndex] += 1;
    }
}

void induceSortS(const vector<int> & str, vector<int> & guessedSuffixArray, const vector<int> & bucketSizes, const vector<int> & typemap) {
    // Slot S-type suffixes into place.
    
    vector<int> bucketTails = findBucketTails(bucketSizes);
    
    for(int i = guessedSuffixArray.size() - 1; i >= 0; --i) {
        int j = guessedSuffixArray[i] - 1;
        if(j < 0) {
            // This entry in the suffix array is the suffix that begins
            // at the start of the string, offset 0. Therefore there is
            // no suffix to the left of it, and j is out of bounds of
            // the typemap.
            continue;
        }

        if(typemap[j] != S_TYPE) {
            // We're only interested in S-type suffixes right now.
            continue;
        }
    
        // Which bucket does this suffix go into?
        int bucketIndex = str[j];
        // Add the start position at the tail of the bucket...
        guessedSuffixArray[bucketTails[bucketIndex]] = j;
        // ...and move the tail pointer down.
        bucketTails[bucketIndex] -= 1;
    }
}

tuple<vector<int>, int, vector<int>> summariseSuffixArray(const vector<int> & str, const vector<int> & guessedSuffixArray, const vector<int> & typemap) {
    // Construct a 'summary string' of the positions of LMS-substrings.

    // We will use this array to store the names of LMS substrings in
    // the positions they appear in the original string.
    vector<int> lmsNames(str.size() + 1, -1);
    
    // Keep track of what names we've allocated.
    int currentName = 0;
    
    // We know that the first LMS-substring we'll see will always be
    // the one representing the empty suffix, and it will always be at
    // position 0 of suffixOffset.
    lmsNames[guessedSuffixArray[0]] = currentName;

    // Where in the original string was the last LMS suffix we checked?
    int lastLMSSuffixOffset = guessedSuffixArray[0];
    
    // For each suffix in the suffix array...
    for(int i = 1; i < guessedSuffixArray.size(); ++i) {
        // ...where does this suffix appear in the original string?
        int suffixOffset = guessedSuffixArray[i];
    
        // We only care about LMS suffixes.
        if(!isLMSChar(suffixOffset, typemap)) {
            continue;
        }
    
        // If this LMS suffix starts with a different LMS substring
        // from the last suffix we looked at....
        if(!lmsSubstringsAreEqual(str, typemap, lastLMSSuffixOffset, suffixOffset)) {
            // ...then it gets a new name
            currentName += 1;
        }
    
        // Record the last LMS suffix we looked at.
        lastLMSSuffixOffset = suffixOffset;
    
        // Store the name of this LMS suffix in lmsNames, in the same
        // place this suffix occurs in the original string.
        lmsNames[suffixOffset] = currentName;
    }
    
    // Now lmsNames contains all the characters of the suffix string in
    // the correct order, but it also contains a lot of unused indexes
    // we don't care about and which we want to remove. We also take
    // this opportunity to build summarySuffixOffsets, which tells
    // us which LMS-suffix each item in the summary string represents.
    // This will be important later.
    vector<int> summarySuffixOffsets;
    vector<int> summaryString;

    for(int index = 0; index < lmsNames.size(); ++index) {
        int name = lmsNames[index];
        if(name == -1) {
            continue;
        }
        summarySuffixOffsets.push_back(index);
        summaryString.push_back(name);
    }
    
    // The alphabetically smallest character in the summary string
    // is numbered zero, so the total number of characters in our
    // alphabet is one larger than the largest numbered character.
    int summaryAlphabetSize = currentName + 1;
    
    return make_tuple(summaryString, summaryAlphabetSize, summarySuffixOffsets);
}

vector<int> makeSummarySuffixArray(const vector<int> & summaryString, int summaryAlphabetSize) {
    // Construct a sorted suffix array of the summary string.

    if(summaryAlphabetSize == summaryString.size()) {
        // Every character of this summary string appears once and only
        // once, so we can make the suffix array with a bucket sort.
        vector<int> summarySuffixArray(summaryString.size() + 1, -1);
    
        // Always include the empty suffix at the beginning.
        summarySuffixArray[0] = summaryString.size();
    
        for(int x = 0; x < summaryString.size(); ++x) {
            int y = summaryString[x];
            summarySuffixArray[y + 1] = x;
        }

        return summarySuffixArray;
    } else {
        // This summary string is a little more complex, so we'll have
        // to use recursion.
        return makeSuffixArrayByInducedSorting(summaryString, summaryAlphabetSize);
    }
}

vector<int> accurateLMSSort(const vector<int> str, const vector<int> & bucketSizes, const vector<int> & typemap, const vector<int> & summarySuffixArray, const vector<int> & summarySuffixOffsets) {
    // Make a suffix array with LMS suffixes exactly right.
    
    // A suffix for every character, plus the empty suffix.
    vector<int> suffixOffsets(str.size() + 1, -1);
    
    // As before, we'll be adding suffixes to the ends of their
    // respective buckets, so to keep them in the right order we'll
    // have to iterate through summarySuffixArray in reverse order.
    vector<int> bucketTails = findBucketTails(bucketSizes);
    for(int i = summarySuffixArray.size() - 1; i > 1; --i) {
        int stringIndex = summarySuffixOffsets[summarySuffixArray[i]];
    
        // Which bucket does this suffix go into?
        int bucketIndex = str[stringIndex];
        // Add the suffix at the tail of the bucket...
        suffixOffsets[bucketTails[bucketIndex]] = stringIndex;
        // ...and move the tail pointer down.
        bucketTails[bucketIndex] -= 1;
    }

    // Always include the empty suffix at the beginning.
    suffixOffsets[0] = str.size();
    
    return suffixOffsets;
}

void removeTheEmptySuffix(vector<int> & sa) {
    for(int i = 1; i < sa.size(); ++i) {
        sa[i - 1] = sa[i];
    }
    sa.pop_back();
}

vector<int> encode(const string & s) {
    vector<int> res(s.size());
    for(int i = 0; i < s.size(); ++i) {
        res[i] = s[i] - 'A' + 1;
    }
    return res;
}


int main() {
    string s;
    cin >> s;
    cout << s << "\n";

    vector<int> encoded_s = encode(s);
    for(int i : encoded_s) {
        cout << i << " ";
    }
    cout << "\n";
    
    vector<int> sa = makeSuffixArrayByInducedSorting(encoded_s, 27);
    removeTheEmptySuffix(sa);
    
    for(int i = 0; i < sa.size(); ++i) {
        cout << sa[i] << "\t" << s.substr(sa[i]) << "\n";
    }
    return 0;
}