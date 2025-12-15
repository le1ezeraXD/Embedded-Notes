// 针对右值引用使用std::move
// 针对万能引用使用std::forward
#include<iostream>
#include<memory>
#include<vector>

class Widget{
private:
    std::string name;
    std::shared_ptr<int> p;

public:
    Widget(Widget&& rhs)
    : name(std::move(rhs.name)),
    p(std::move(rhs.p)){};
};

class item25{
private:
    std::string _name;

public:
    // 右值引用 必须用move 否则就浪费了
    // 因为name是右值
    void setName(std::vector<int>&& name) {
        std::cout << "rValue! Move!" << std::endl;
        _name = "Name Test";
    }

    // 涉及到类型推导 所以是万能引用
    // 此时要用forward 因为不确定是左值还是右值
    template<typename T>
    void setName(T&& name) {
        std::cout << "l/rValue! Forward!" << std::endl;
        _name = std::forward<T>(name);
    }

    // ROV(return value Optimization)
    // 当return的是个局部变量 而且类型与函数返回值类型相同时
    // 触发ROV 编译器会直接在调用函数的位置直接生成变量 既不移动也不拷贝
    std::string getName() {
        std::string res = _name;
        return res;
    }

};

int main(void) {
    item25 t;
    t.setName({1});
    std::cout << t.getName() << std::endl;;

    std::string name = "lValue Name";
    t.setName(name);
    std::cout << t.getName() << std::endl;;
}