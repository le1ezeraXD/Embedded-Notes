#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
#include <string>

// shared_ptr: 没有特定的shared_ptr拥有该对象
// 取而代之的是多个shared_ptr实例共享对同一对象的所有权
// 当最后一个shared_ptr被销毁时, 该对象才会被释放

// shared_ptr的引用计数机制:
// 每个shared_ptr实例都维护一个引用计数器, 记录有多少个shared_ptr实例共享对同一对象的所有权
// 当创建一个新的shared_ptr实例时, 引用计数器增加1
// 当一个shared_ptr实例被销毁或重置时, 引用计数器减少1
// 当引用计数器变为0时, 该对象被释放

// 一个用来观察生命周期的测试类
struct Widget {
    std::string name;
    Widget(const std::string& n) : name(n) {
        std::cout << "Widget(" << name << ") constructed\n";
    }
    ~Widget() {
        std::cout << "Widget(" << name << ") destructed\n";
    }
};

// 极简版 shared_ptr
template<typename T>
class MySharedPtr {
private:
    // 控制块：只放强引用计数
    struct ControlBlock {
        long use_count;
        ControlBlock() : use_count(1) {}
    };

    T* ptr;                // 指向实际对象
    ControlBlock* ctrl;    // 指向控制块

    // 释放当前持有的引用
    void release() {
        if (ctrl) {
            ctrl->use_count--;
            std::cout << "[release] use_count -> " << ctrl->use_count << "\n";

            if (ctrl->use_count == 0) {
                std::cout << "[release] deleting managed object\n";
                delete ptr;
                delete ctrl;
            }
            ptr = nullptr;
            ctrl = nullptr;
        }
    }

public:
    // 默认构造：空指针
    MySharedPtr() : ptr(nullptr), ctrl(nullptr) {
        std::cout << "MySharedPtr() default constructed (null)\n";
    }

    // 从裸指针构造
    explicit MySharedPtr(T* p) : ptr(p), ctrl(nullptr) {
        if (p) {
            ctrl = new ControlBlock();
            std::cout << "MySharedPtr(T*) constructed, use_count = "
                      << ctrl->use_count << "\n";
        } else {
            std::cout << "MySharedPtr(T*) constructed with null pointer\n";
        }
    }

    // 拷贝构造
    MySharedPtr(const MySharedPtr& other)
        : ptr(other.ptr), ctrl(other.ctrl) {
        if (ctrl) {
            ctrl->use_count++;
            std::cout << "MySharedPtr copy-constructed, use_count = "
                      << ctrl->use_count << "\n";
        } else {
            std::cout << "MySharedPtr copy-constructed from null\n";
        }
    }

    // 拷贝赋值
    MySharedPtr& operator=(const MySharedPtr& other) {
        if (this == &other) return *this; // 防自赋值

        // 先释放自己当前的引用
        release();

        // 再接手别人的控制块和指针
        ptr = other.ptr;
        ctrl = other.ctrl;
        if (ctrl) {
            ctrl->use_count++;
            std::cout << "MySharedPtr copy-assigned, use_count = "
                      << ctrl->use_count << "\n";
        } else {
            std::cout << "MySharedPtr copy-assigned from null\n";
        }
        return *this;
    }

    // 移动构造函数
    MySharedPtr(MySharedPtr&& other) noexcept
        : ptr(other.ptr), ctrl(other.ctrl) {
        other.ptr = nullptr;
        other.ctrl = nullptr;
        std::cout << "MySharedPtr move-constructed\n";
    }

    // 析构
    ~MySharedPtr() {
        std::cout << "~MySharedPtr() called\n";
        release();
    }

    // 一些辅助接口
    long use_count() const {
        return ctrl ? ctrl->use_count : 0;
    }

    T* get() const { return ptr; }

    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
};


// 测试用 main
int main() {
    {
        std::cout << "===== scope 1 =====\n";
        MySharedPtr<Widget> sp1(new Widget("A"));

        {
            std::cout << "----- scope 2 -----\n";
            MySharedPtr<Widget> sp2 = sp1;     // 拷贝构造
            std::cout << "sp1.use_count() = " << sp1.use_count() << "\n";
            std::cout << "sp2.use_count() = " << sp2.use_count() << "\n";

            {
                std::cout << "..... scope 3 .....\n";
                MySharedPtr<Widget> sp3(sp2);  // 再拷贝一次
                std::cout << "sp1.use_count() = " << sp1.use_count() << "\n";
                std::cout << "sp2.use_count() = " << sp2.use_count() << "\n";
                std::cout << "sp3.use_count() = " << sp3.use_count() << "\n";
            } // 这里 sp3 析构，计数 -1

            std::cout << "after scope 3, sp1.use_count() = " << sp1.use_count() << "\n";
        } // 这里 sp2 析构，计数 -1

        std::cout << "after scope 2, sp1.use_count() = " << sp1.use_count() << "\n";
    } // 这里 sp1 析构，计数变 0，删对象 + 控制块

    std::cout << "===== end of main =====\n";

    class Widget {};
    // auto pw = new Widget;
    // std::shared_ptr<Widget> sp1(pw);    // 正确用法，shared_ptr管理裸指针
    // std::shared_ptr<Widget> sp2(pw);    // 错误用法，两个shared_ptr管理同一裸指针
    std::shared_ptr<Widget> sp1(new Widget); // 正确用法
    std::shared_ptr<Widget> sp2 = sp1;       // 正确用法，共享所有权

    return 0;
}
