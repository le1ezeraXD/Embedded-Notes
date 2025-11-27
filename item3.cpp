#include<iostream>
#include<vector>
#include<deque>

using namespace std;

template<typename Container, typename Index>
decltype(auto)
authAndAccess(Container& c, Index i)
{
    return c[i];
}
 
int main() {
    // const int i = 0;
    // decltype(i) t = 1;
    // cout << t << endl;
    deque<int> d;
    authAndAccess(d,5) = 10;
    int x = 0;
    int& rx = x;
    const int cx = x;

    decltype(auto) a1 = x;      // int
    decltype(auto) a2 = (x);    // int&
    decltype(auto) a3 = rx;     // int&
    decltype(auto) a4 = (rx);   // int&
    decltype(auto) a5 = cx;     // const int
    decltype(auto) a6 = (cx);   // const int&

    decltype(x) b1 = x;         // int
    decltype((x)) b2 = x;       // int&

    return 0;
}