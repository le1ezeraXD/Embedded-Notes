// 避免依万能引用类型进行重载
#include<iostream>
#include<vector>
#include<memory>

// 如我在item25踩过的坑
class Widget{
private:
    std::string _name;

public:
    // 此处接收一个右值
    // 将其move进成员变量_name中 以减小开销
    void setName(std::string&& name) {
        std::cout << "rValue!\n";
        _name = std::move(name);
    }

    // 此处为万能引用
    // 将接收进的name forward给成员变量_name
    template<typename T>
    void setName(T&& name) {
        std::cout << "l/rValue\n";
        _name = std::forward<T>(name);
    }
};

class Person {
private:
    std::string _name;
    auto nameFromIdx(int idx) { return "Name";};
public:
    template<typename T>
    explicit Person(T&& n)
    : _name(std::forward<T>(n)){}

    explicit Person(int idx)
    : _name(nameFromIdx(idx)){}

    Person(const Person& rhs){};    // 编译器自动生成特种函数
    Person(Person&& rhs){};         // 编译器自动生成特种函数
    Person(){};
};

class SpecialPerson: public Person {
public:
    SpecialPerson(const SpecialPerson& rhs) // 复制构造函数
    : Person(rhs){} // 调用的是基类的完美转发构造函数

    SpecialPerson(SpecialPerson&& rhs)      // 移动构造函数
    : Person(std::move(rhs)){}  // 调用的是基类的完美转发构造函数
    // 但实际无法通过编译 因为不能用SpecialPerson对象去setName
};

int main(void) {
    // 问题在于 万能引用能接收的实参实在太多 可能会覆盖我们希望调用右值引用的情况 如:
    Widget w;
    w.setName("Name_L");                    // 理想情况下应输出rValue 触发右值引用
    std::string n = "Name_R";
    w.setName(n);                           // 理想情况下应输出l/rValue 触发万能引用
    // 实则不然 两次都会触发万能引用
    // 因为传入"Name_L"时会退化为const *p, 被万能引用抓到了
    // 如果想触发右值应该这样写
    w.setName(std::string("Name_L!"));      // 太蠢了 而且太长

    // Person类也是一样 但会有更可怕的事情发生
    Person p("Nancy");
    // Person p2(p);   // 看似可以通过编译 实际上连复制构造函数都会被万能引用吸走 变成用Person对象去setName
    // 原因是编译器自动生成的移动和复制构造函数 一个要求const修饰 一个要求右值Person
    // 而传入p严格意义上不符合其中的任何一个
    // 所以会被万能引用吸走
    // 所以应该
    const Person cp("Nancy");   // 变成常量
    Person p2(cp);              // 此时触发复制构造函数
    // 并且 C++重载决议规则中提到
    // 若一个模板实例化函数和一个非函数模板(即常规函数)具备相同的匹配程度
    // 优先选用常规函数
    
}