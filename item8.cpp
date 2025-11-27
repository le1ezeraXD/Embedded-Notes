#include<iostream>
#include<vector>

using namespace std;

// Summary: do not use 0/NULL to false, u should use nullptr
int main() {
    // 0 is int, not point
    // if C++ find a zero, define it to nullptr
    // but actually, zero is a int type not point
    void f(int);
    void f(bool);
    void f(void*);

    f(0);       // call f(int), not f(void*);
    // f(NULL);    // can not compile, will f(int), never f(void*)
}
