// lambda表达式
// 避免默认捕获模式
#include<iostream>
#include<memory>
#include<vector>
#include<functional>

// lambda按引用捕获会导致闭包包含指涉到局部变量的引用, 或者指涉到定义lambda式的作用域内的形参的引用
// 一旦lambda所创建的闭包越过了该局部变量或形参的生命期, 那么闭包内的引用就会空悬

// 加一个会覆盖x的函数
void smashStack() {
    int trash = 99999;
    std::cout << "Stack is smashed!" << std::endl;
}

// 错误的函数
std::function<void()> createPrinter() {
    int x = 100;                    // 局部变量 在该函数的栈空间中
    auto closure = [&]() {          // 定义lambda 使用[&]按引用捕获x
        std::cout << "Value: " << x << std::endl;
    };

    return closure;                 // 将该lambda回传出去

}                                   // 函数结束 变量x被销毁了

using FilterContainer = 
    std::vector<std::function<bool(int)>>;
FilterContainer filters;

void addDivisorFilter() {
    auto calc1 = 1; auto calc2 = 2;
    auto divisor = std::max(calc1, calc2);

    // 此时存在filters中的值是一个lambda变量
    // 当addDivisorFilter()函数结束之后 divisor变量便不存在了
    // 存放在filters中的lambda表达式就会抓空
    filters.emplace_back(
        [&](int value) { return value % divisor == 0; } // 危险!对divisor的指涉可能空悬!
    );
    // 解决之道(之一)是对divisor采用按值的默认捕获方式
    // 就该例来说 用值引用已足够 但值引用并非能完全解决这个问题
    filters.emplace_back(
        [=](int value) { return value % divisor == 0; }
    );
}

// 因为按值捕获了一个指针之后 在lambda创建的闭包中持有的是这个指针的副本
// 但并无办法阻止lambda之外的代码针对该指针实施delete所导致的指针空悬
// 比如存入一个指针 但在调用lambda之外的地方将这个指针delete掉 那么再运行lambda时照样会空悬
class Widget {
public:
    void addFilter() const;
private:
    int divisor = 1;
};

// 看起来可以工作 实际上[=]捕获的是Widget内所有成员变量
// 而Widget内的成员变量只有一个 就是this指针
// 所以代码应该长这样[this](int value) { return value % this->divisor == 0; }
// 为什么会出现空悬? 因为Widget的对象是会被销毁的 销毁后自然就不存在this指针了
void Widget::addFilter() const {
    filters.emplace_back(
        [=](int value) { return value % divisor == 0; }
    );
}

int main(void) {
    // lambda式常用于创建闭包并仅将其用作传递给函数的实参
    // {
    //     int x;
    //     auto c1 = [x](int y) { return x*y > 55; };

    //     auto c2 = c1;
    //     auto c3 = c2;
    // }
    auto myLambda = createPrinter();    // 虽然函数结束 但lambda还在
    smashStack();                       // 覆盖刚刚x的位置 加进来捣乱
    myLambda();                         // 灾难发生 lambda试图访问x 但x已经结束了

    Widget w;
    w.addFilter();
}