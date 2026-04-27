#include<iostream>
#include<memory>
#include<algorithm>

#include "Widget.h"


//  使用Pimpl习惯用法时 将特殊成员函数的定义放到实现文件中
int main(void) {
// 因为Widget的成员含有string vector等多种类型 这些类型的头文件必须存在 Widget才能通过编译
// 所以调用Widget的类也必须导入对应的头文件 会增加编译时间
    Widget w;
    w.dosomething();
    return 0;
}