// 万能引用重载的替代方案
#include<iostream>
#include<memory>
#include<vector>
#include<set>
#include<type_traits>
#include<utility>

// 1.放弃治疗 舍弃重载 将logAndAdd()改为logAndAddInt()之类
// 2.传递const T&类型的形参, 效率不够高但可行
// 3.传值
// 以上用法不推荐 故不再赘述

// 4.贴标签: true/false_type
// 原理: 利用is_integral()等可以判断变量类型的函数作为形参, 分别调用不同重载的函数
// 虽然int/string等类型很容易混淆 但true/false_type不会
class Widget {};
std::multiset<std::string> names;

// logAndAdd具体实现其一: remove_reference为false 即false_type
// 为什么用true/false_type而不是true/false(bool)?
// 因为true/false是运行期值 在编译阶段不会出现
// 而重载决议(即选择用哪个重载函数)是在编译期
// 当然也因为is_integral()返回的就是true/false_type
template<typename T>
void logAndAddImpl(T&& name, std::false_type) {
    // auto now = std::chrono::system_clock::now();
    names.emplace(std::forward<T>(name));
}

// logAndAdd具体实现其二: remove_reference为true 传入int
void logAndAddImpl(int idx, std::true_type) {
    // logAndAdd(nameFromIdx(idx));
    std::cout << "Int Type!\n";
}

// logAndAdd调用
template<typename T>
void logAndAdd(T&& name) {
    logAndAddImpl(
        std::forward<T>(name),
        std::is_integral<std::remove_reference_t<T>>()
    );
}

std::string nameFromIdx(int idx) {
    std::cout << "nameFromIdx" << idx << std::endl;
    return "Durgon";
};


// 5.对接受万能引用的模板施加限制(std::enable_if)
// 解决痛点: 标签无法解决构造函数被万能引用"霸占"的问题
// 因为构造函数没办法改名字 也不能随便增加参数 至少不能像logAndAdd那样再去调用另一个Impl函数
// enable_if: 字面意思 当满足某条件时才会启用万能引用模板 比如传入的变量不是Person类对象时才开启万能引用
// 这样就可以避免构造函数被万能引用霸占

// 相对的 功能好用 语法就比较复杂 阴阳平衡(笑)
class Person {
private:
    std::string _name;

public:
    // 我们想指定的是 当T是Person以外的类型时才启用该模板构造函数 此处需要引入std::decay
    // decay可以移除T类型带有的所有引用 const或是volatile等奇奇怪怪的修饰词
    // 意味着Person Person& const Person const Person& Person&&等你能想到的组合 都能被视作Person
    // C++14出现了enable_if_t和decay_t来代替::type 时代发展真是太好了
    template<typename T,
             typename = typename std::enable_if_t<
                !std::is_base_of<Person,std::decay_t<T>>::value    // 开关 我知道 太特么丑了 我衷心希望我以后的代码里不会出现这坨东西(尽量) 我的编译器甚至都看不出哪个尖角号是哪个尖角号
                // 这里要和前面的is_integral结合一下 毕竟判断是否是整形才是我们最开始的目的 我们在构造函数多义词上花了太多时间
                &&
                !std::is_integral<std::remove_reference_t<T>>::value>>
                explicit Person(T&& n)
                : _name(std::forward<T> (n)){
                    // 在客户代码尝试从一个无法构造出std::string类型的对象的类型出发来创建一个Person对象时报出该错误
                    // 据说错误信息会来得很晚 没有实践
                    static_assert(
                        std::is_constructible<std::string, T>::value,
                        "Paramater n can't be used to construct a std::string"
                    );
                };

                explicit Person(int n)
                : _name(nameFromIdx(n)){};
};

// 还有一件事: SpecialPerson and Person
// 如果用Person(基类)的构造函数去构造子类对象的话 因为enable_if只会判断对象是不是Person
// 所以SpecialPerson的构造函数(传入SpecialPerson对象)依然会被基类的万能引用模板吃掉 真是太操蛋了
// 这里要引入std::is_base_of来拯救世界 用法如下
// std::is_base_of<T1, T2> 如果T2是由T1派生而来 即SpecialPerson and Person
// 则::value为真, 同样的因为任何类都可以看作是自身的派生 所以
// std::is_base_of<T, T>::value也为真 这样可以完美取缔is_same

int main(void) {
    // std::string name = "Durgon";
    // logAndAdd(name);
    int idx = 1;
    logAndAdd(1);
}