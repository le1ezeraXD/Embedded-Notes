#include<iostream>
#include<vector>
#include<algorithm>

int f(int x) noexcept{
    return 'aka';
}; // f whill not throw error C++11 style
int f(int x) throw();  // C++98 style

int main(void) {
    int a = f(2);
    std::cout << a << std::endl;
}