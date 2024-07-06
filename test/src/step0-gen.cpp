// generating a random sequence of distinct elements
#include <bits/stdc++.h>
using namespace std;

std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

int rand(int start, int end) {
    std::uniform_int_distribution<int> distribution(start, end);
    return distribution(generator);
}

int main(int argc, char* argv[]) {
    int n = 5;
    for(int i = 0; i < n; ++i) {
        int x = rand(1, 10);
        printf("%d ", x);
    }
    puts("");
}