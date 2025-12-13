// 右值引用 移动语义和完美转发
#include<iostream>
#include<vector>
// #include "Widget.h"

//  理解std::move和std::forward
//  move本身并不会移动变量
//  原理是move把表达式转为右值
//  从而让重载决议倾向选择移动构造/移动赋值
//  如Widget w1 = "test"; Widget w2 = std::move(w1); 这种情况就是移动构造 因为w1是个左值
//  w2 = std::move(w1); 是移动赋值
//  Widget w1 = "test", w2; w2 = w1; 拷贝赋值 因为w2已经是一个对象了 只是没有赋初始值

//  判定是构造还是赋值: 
//  行内出现类型名在声明如: T x = ...;/ T x(...);->构造
//  否则x = ...;->赋值

//  std::move简介
//  move的简单实现
template<class T>
constexpr std::remove_reference_t<T>&& move(T&& t) noexcept {
  return static_cast<std::remove_reference_t<T>&&>(t);
}

class Annotation {
public:
  explicit Annotation(const std::string s)
  : value(std::move(s)){};  //s并非移动 还是被复制进value的 因为s被事先声明为const 强制转换前是左值const 转换后是右值const 常量性被保留了下来
  //  所以只会调用string的复制构造函数, 无法传给移动构造函数
  //  move涉及到改变对象 而const修饰的对象是不能改变的
  //  所以move不仅不实际移动任何东西 甚至都不保证经过强制类型转换的对象具备可移动的能力
  //  唯一能确定的就是结果会是个右值

private:
  std::string value;
};


//  std::forward简介
//  forward是有条件强制类型转换: 仅当其实参是使用右值完成初始化时 才会执行向右值类型的强制转换
//  forward可以知道传进的实参是左值还是右值
//  具体原理细节见item28

template<typename T>
void makeLogEntry(const std::string& s, const T& time) {
  std::cout << s << time << std::endl;
}

template<typename T>
void lonAndProcess(T&& param) {
  auto now = std::chrono::system_clock::now();
  makeLogEntry("Calling 'process'", now);
  process(std::forward<T>(param));
}

template<typename T>
void process(T& param) {
  std::cout << "lval input!!" << std::endl;
}

template<typename T>
void process(T&& w) {
  std::cout << "rval input!!" << std::endl;
}

template<class T>
void callProcess(T&& x) {              // 转发引用
    process(std::forward<T>(x));       // 有条件 forward
}

class Widget{};

int main(void) {
    // Widget w = Widget{};
    Widget w;
    callProcess(w);        // lval
    callProcess(Widget{}); // rval
}