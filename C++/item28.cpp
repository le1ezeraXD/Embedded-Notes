// 理解引用折叠
#include<iostream>
#include<memory>
#include<vector>

// 实参在传递给函数模板时
// 推导出来的模板形参会将 **实参时左值还是右值** 的信息 编码到结果类型中
// 这个操作只有在实参被用以初始化的形参为万能引用时才会发生
template<typename T>
void func(T&& param){};

class Widget{};
Widget widgetFactory(){};     // 返回右值的函数

int main(void) {
    
    Widget w;                   // 变量(左值)
    func(w);                    // 调用func并传入左值 此时T推导类型为Widget&
    func(widgetFactory());      // 调用func并传入左值 此时T推导类型为Widget

    // 但func(w)这一行值得玩味 理论上C++不接受一个 **引用的引用** 变量 如
    // int x;
    // auto& &rx = x            // 错误 不可以声明引用的引用
    // 如果是func(w) 那么左值传递给万能引用的函数模板时 如果将T推导结果实例化 将会得到：
    // void func(Widget& && param); // 引用的引用!
    // 原因即是标题 引用折叠
    // 规则: 如果任一引用为左值引用 则结果为左值引用 否则结果为右值引用
    // 例: 有两种引用(左值和右值), 即有四种可能的引用(组合): 左左 右右 左右 右左
    // 所以只有右右引用的时候 才会被折叠为右值引用 其他时刻均折叠为左值引用
    auto&& w1 = w;                  // 展开后为Widget& && w1 = w; 折叠后为Widget& w1 = w;仍是左值 因为w的类型是Widget&
    auto&& w2 = widgetFactory();    // 以右值初始化w2 auto推导结果为Widget 展开后为Widget&& w2;

    // 综上所述 万能引用并非一种新的引用类型 其实它就是满足了下面两个条件的右值引用
    // 1.类型推导时会区别左值右值: auto或模板T都会推出&还是&&
    // 2。会发生引用折叠: 有左值引用即全是左值引用 只有当全是右值时才是右值引用

    
}