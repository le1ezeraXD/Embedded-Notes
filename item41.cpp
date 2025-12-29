// 针对可被复制的形参 在移动成本低且一定会被复制的前提下 考虑将其按值传递
#include<iostream>
#include<vector>

using namespace std;

// 亦有可取之处 但缺点是每次都要写两个函数 太过麻烦
class Widget_Overload {
public:
    void addName(const string& newName)
    { names.push_back(newName);}

    void addName(string&& newName)
    { names.push_back(move(newName));}
private:
    vector<string> names;
};

// 可能会导致其他方面的复杂性 作为模板 必须置于头文件中 当然demo里看不出
// 可能在对象代码中产生好几个函数 因为不仅针对左右值产生不同的实例化结果
// 针对string和可以转型为string的类型也会产生不同实例化结果
// 有些类型不能通用引用方式传递 参见item30 如大括号实例化的对象 或是可变长参数
class Widget_forward {
public:
    template<typename T>
    void addName(T&& newName)
    { names.push_back(forward<T>(newName));}
private:
    vector<string> names;
};

// 按值传递
// 当移动成本低的时候 愿意多一次移动操作(即复制)来换取代码的清晰
// 减少写重载函数 并避免模板报错
class Widget_By_Value {
public:
    void addName(string newName)
    { names.push_back(move(newName));}
private:
    vector<string> names;
};

// 何时一定会被复制:
// 例如在对类做实例化时, 向实例化的对象的成员变量容器中添加变量, 就像上面的names.push_back
// 此时如果传入左值 则会在形参处复制一份给形参
// 如果传入右值 则会直接触发移动构造 不涉及复制
// 所以使用按值传入并move()时 对于右值来讲 仅多花费一次复制的成本就可以使代码清晰
int main(void) {

}