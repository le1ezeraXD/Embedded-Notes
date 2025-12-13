// 区分万能引用和右值引用
// &&不全是右值引用的意思
#include<iostream>
#include<vector>
#include<memory>

class Widget{};

// 如果看到了T&& 却没有其设计的类型推导 那么就是个右值引用
void f(Widget&& param);             // 右值引用
Widget&& var1 = Widget();           // 右值引用
template<typename T>                // 右值引用
void f(std::vector<T>&& param){};   // 虽然有模板T 但是指定了形参类型为vector 所以是右值引用

// 万能引用的相同之处: 涉及到类型推导(T, auto)
template<typename T>                // 非右值引用 典型的万能引用
void f(T&& param);
auto&& var2 = var1;                 // 非右值引用 也是万能引用

// 但不是每个形参为T&&的模板都是万能引用
// 因为push_back作为MyVector的一部分 如果不存在特定的vector实例 则它也无从存在
// 所以该实例的具体类型会直接决定push_back的传入参数类型
template<class T, class Allocator = std::allocator<T>>
class MyVector {
public:
    // 尽管它写了T&& 但类型仍受Myvector限制
    void push_back(T&& x) {};

    // 此时涉及类型推导
    // 因为Args独立于类的形参T存在
    // 所以每次调用时都会进行推导
    template <class... Args>
    void emplace_back(Args&&... args){};
};

int main(void) {
    f(std::vector<int> {1,2,3});
}