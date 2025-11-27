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
    
}

class Investment{

};

class Stock: public Investment {};
class Bond: public Investment {};
class RealEstate: public Investment {};

template<typename... Ts>
std::unique_ptr<Investment>
makeInvestment(Ts&&... params);
