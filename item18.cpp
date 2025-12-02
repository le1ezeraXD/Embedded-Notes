#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>

// 第四章 智能指针
// 四种智能指针: std::auto_ptr, unique_ptr, shared_ptr, weak_ptr
int main(void) {
    int *p = (int*)malloc(sizeof(int));
    *p = 1;
    std::cout << *p << "\n";
    free(p);
    Investment i;
    int* p = new int(10);
    delete p;

    // *p = 5; // 未定义行为: 使用了悬空指针
    std::unique_ptr<int> up(new int(20));
    up.reset(); // 释放原有资源
    up.reset(new int(30)); // 重新分配资源

    
    
}

class Investment{

};

class Stock: public Investment {};
class Bond: public Investment {};
class RealEstate: public Investment {};

