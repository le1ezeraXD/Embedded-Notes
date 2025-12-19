// 使用初始化捕获将对象移入闭包
#include<memory>
#include<iostream>
#include<vector>

class Widget {
public:
    void configure() { std::cout << "Widget Configured!\n"; }
};


int main(void) {
    auto pw = std::make_unique<Widget>();
    // C++14初始化捕获

    // 关键在这一步 左边的pw是lambda内部变量名
    // 右边std::move()中的pw是外部变量 将它move进lambda
    auto func = [pw = std::move(pw)]() {
        if (pw) {
            pw->configure();
        }
    };

    // 此时外部的pw因为被移入lambda 所以此时置空
    if (!pw) std::cout << "外部pw已置空!\n";

    std::vector<double> data2 = {1.1,2.2,3.3};
    // lambda默认的operator()是const修饰的 所以不能在lambda内修改move进的变量
    // 解决办法是在lambda后添加mutable来"解锁"
    auto func2 = [data2 = std::move(data2)] () mutable {
        data2.push_back(4.4);
        std::cout << "Data size: " << data2.size() << "\n";
    };

    // lambda内部拥有Widget指针
    func();
    func2();
}