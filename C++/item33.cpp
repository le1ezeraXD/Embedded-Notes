// 对auto&&类型的形参使用decltype 以std::forward之
#include<iostream>
#include<vector>
#include<memory>

class Widget {
public:
    
};

int main(void) {
    // 因为传递给lambda的参数param是用auto修饰的
    // 所以在forward<???>处要用decltype来获取变量的类型
    // decltype: 是左值就返回左值 是右值就返回右值
    auto func = [&](auto&& param...) {
        return std::forward<decltype(param)>(param);
    };
}
