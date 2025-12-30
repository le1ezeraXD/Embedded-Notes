// 考虑置入而非插入
#include <vector>
#include <iostream>
#include <string>
#include <regex>
// 如果参数类型与容器元素类型不匹配 如vector<string>和const char*
// 且不需要临时对象 优先使用emplace_back
// 如果参数已经是正确的类型 emplace_back与push_back性能相同
class Widget {
public:
    std::vector<std::string> v;

};

void killWidget(Widget* pWidget){};

int main(void) {
    std::vector<std::string> v;
    v.push_back("xyzzy");           // 此时编译器看到实参类型(const char [6] 与push_back接受的形参类型之间不匹配) 会生成临时string对象
    v.push_back(std::string("xyzzy"));  // 等价于这一句
    // 这个过程会先创造一个string类型的临时对象 没有名字 所以是个右值 第一次构造
    // 然后传进push_back的右值重载版本 绑定到右值引用形参x
    // 然后会在内存中为vector构造一个x的副本 这已经是第二次构造
    // 结果就是在vector内创建了一个新的对象(透过第一次构造的形参x副本) 这次是移动构造 因为x是右值
    // 当push_back返回时 临时对象被析构
    // 不难看出临时对象的构造和析构都是可以避免的 此时需要用到emplace_back
    v.emplace_back("xyzzy");        // 为什么它这么牛逼？因为它用了完美转发 只要你没有遇到完美转发的限制(item30) 它就是这么牛逼
    // 需要用到emplace_back的情况:
    // 欲添加的值时以构造而非赋值的方式加入容器 可以避免无谓的临时对象构造

    // 什么时候一定要用push_back?
    // 涉及到资源管理的情况
    // 例:
    std::vector<std::shared_ptr<Widget>> vs;        // 创建容器
    vs.push_back(std::shared_ptr<Widget>(new Widget, killWidget));         // 自定义删除器killWidget 此时是安全的 如果在push_back时发生内存不足的异常 临时对象会析构
                                                    // Widget资源也会在临时指针对象析构时被释放
    vs.emplace_back(new Widget, killWidget);        // 此时极为不安全 考虑同样内存不足的情况 new Widget裸指针被完美转发
                                                    // 但此时内存不足 却没有人能控制这个裸指针了 连带它所有的资源都发生泄漏

    // 另一种情况 是与带有explicit声明饰词的构造函数之间有互动
    // 假设为了表彰C++11对正则表达式的支持 构造了一个正则表达式对象的容器
    std::vector<std::regex> regexes;

    regexes.emplace_back(nullptr);          // 这玩意居然可以接收一个空指针 一看就很扯淡 人类不会写这种东西
    // std::regex r = nullptr;                 // 如果试图创造空指针的regex对象 会被编译器拒绝
    // regexes.push_back(nullptr);             // 如果使用push_back去添加空指针 也会被拒绝 但emplace_back可以这样做!
    // 原因是regex对象可以从字符串出发来构造 使得下面这种看起来就扯淡的代码是合法的
    std::regex upperCaseWord("[A-Z]+");     // 运行期成本会很高
    // 所以接受const char* 指针的regex构造函数以explicit声明
    // 然而在emplace_back调用过程中 却没有声称传递的是个regex对象
    // 取而代之的是向regex对象传递的是个构造函数实参 所以在编译器看来写了:
    std::regex r(nullptr);                  // 能编译

}