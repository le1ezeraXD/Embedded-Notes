// 熟悉完美转发的失败情形
// 完美转发也不是完美的
#include<iostream>
#include<vector>
#include<memory>

class Widget{};

template<typename T>
void f(T&& param){};

// 完美转发的含义是我们不仅转发对象 也转发其显著特征
// 比如类型 左/右值 是否const或volation等
template<typename T>
void fwd(T&& param) {
    f(std::forward<T>(param));
};

// 一种拓展就是使得转发函数不只是模板 而是可变长形参模板 从而能够接受任意数量的实参
template<typename... Ts>
void fwd(Ts&&... params) {
    f(std::forward<Ts>(params)...);
};

// 以上例子中 如果对fwd()和f()传入同样的实参但是进行了不同的操作
// 则称完美转发失败

int main(void) {

}