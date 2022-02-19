#include <bits/stdc++.h>
using namespace std;

/***************************Template Part Begin***************************/

const int NBytes = 1024 * 1024;
char _buf[NBytes];
int * buf = reinterpret_cast<int*>(_buf);
int top = 0;

template <class T>
struct Mallocator
{
    using value_type = T;
    Mallocator() noexcept {}

    template<class U> Mallocator(const Mallocator<U> & rhs) noexcept {}
    template<class U> bool operator==(const Mallocator<U> & rhs) const noexcept { return true; }
    template<class U> bool operator!=(const Mallocator<U> & rhs) const noexcept { return !(*this == rhs); }
    T * allocate(const size_t n) const;
    void deallocate(T * const p, size_t) const noexcept;
};

template <>
int * Mallocator<int>::allocate(const size_t n) const
{
    if (n == 0) {
        return nullptr;
    } else {
        int * res = &buf[top];
        top += n;
        return res;
    }
}

template<>
void Mallocator<int>::deallocate(int * const p, size_t) const noexcept
{
    // Add your deallocation logic here.
    // Leave it blank if you don't want to deallocate.
}

template<typename T>
using MyVector = vector<T, Mallocator<T>>;

/***************************Template Part End***************************/

int main() {
    MyVector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    for(int i : v) {
        cout << i << "\n";
    }
    return 0;
}