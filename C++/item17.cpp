#include<iostream>
#include<vector>
#include<mutex>
#include<algorithm>
// 理解特种成员函数的生成机制
// 默认构造函数不需要传参
// 只要指定了一个要求传参的构造函数 就会组织编译器生成默认构造函数

struct T
{
    T(){};
    T(int){
        std::cout << "T(int)\n";
    }
    T(const T&) {
        std::cout << "T(const T&)\n";
    }
    T& operator=(T&&);
};


int main() {
    // T a = T(1); // 拷贝省略 只调用构造函数
    // T b;
    // b = T(1);
    std::vector<int> nums{0,1,2,3};
    std::vector<int> res(nums);


}