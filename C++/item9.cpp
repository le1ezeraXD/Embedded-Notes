#include<iostream>
#include<vector>

class Widget {};
// alias
template<typename T>
using Vec = std::vector<T>;

template<typename T>
struct is_Pointer {return static const bool false;};
template<typename T>
struct is_Pointer<T*> {return static const bool true;};

int main() {
    Vec<int> v;
    v.push_back(1);
    v.push_back(2);
    for (auto p : v) {
        std::cout << p << std::endl;
    }


}