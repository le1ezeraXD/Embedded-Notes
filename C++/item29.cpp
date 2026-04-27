// 假定移动操作不存在 成本高 未使用
// 应该是想说移动操作不是在什么情况下性能都出色
#include<iostream>
#include<memory>
#include<vector>
#include<array>
// #include "Widget.h"

int main(void) {
    // Widget w;
    // vector可以做到移动指针 这样就可以在常数时间内移动容器内的全部内容
    // 因为vector实际上是在堆上开辟一块内存
    // 在概念上 只需持有一个指涉到存放容器内容的堆内存的指针
    // 所以只要移动指针 理论上就可以达到移动容器的效果 
    std::vector<int> v1{1,2,3,4,5};
    auto v2 = std::move(v1);            // 此时v1置空 v2接管v1原来内容

    // 但array对象不可以 因为其内容数据是直接存储在对象内的
    // 本质上是一个带有stl接口的内建数组
    // 所以无论是移动还是复制操作 对array对象来说都需要线性时间的复杂度 即O(n)
    std::array<int,100> aw1;
    for (int i = 0; i < aw1.size(); ++i) aw1[i] = i;        // 将数据放入aw1
    auto aw2 = std::move(aw1);                              // 移动aw1到aw2 此操作需要线性时间 需要一个个将aw1的元素移动进aw2

    // 作为对比 string类对象提供的是常数时间的移动和线性时间的复制
    // 但并非它的移动比复制更快
    // 许多string的实现都采用了小型字符串优化
    // 小型字符串(即长度小于某某的字符串)会存放在string对象的某个缓冲区内 而不像vector一样使用堆上的内存
    // 所以就不存在移动指针的操作了(因为没有指针) 此时移动和复制一样需要线性时间
    std::string s("Night Haze");
    auto s2 = std::move(s);

    // 还有 即是是支持快速移动的类型如vector 假如要求移动时没有加上noexcept声明 则依旧可能执行复制而非移动
    // 底层的复制操作只有在已知移动操作绝对不会有问题的前提下才会使用移动将其替换


}