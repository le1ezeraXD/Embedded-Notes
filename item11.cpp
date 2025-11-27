#include<iostream>
#include<vector>
#include<set>
#include<unordered_set>
#include<numeric>

class basic_ios {
public:
    bool isLucky(int number) {return number == 6;}
    template<typename T>
    bool isLucky(T) = delete;
};

struct MyType
{
    MyType() = default;
    MyType(const MyType&) = delete;
    MyType operator = (const MyType&) = delete;
};


int main(void) {
    basic_ios b;
    MyType a;
    int n = 26;
    int cnt[n] = {0};
    std::cout << cnt[2] << std::endl;
    // if (b.isLucky('a')) std::cout << "is Lucky!" << std::endl;
    // if (b.isLucky(true)) std::cout << "is Lucky!" << std::endl;
    std::vector<int> res = {1,1,2,3,5};
    std::unordered_set<int> s(res.begin(), res.end());
    for (auto n : s) std::cout << n << "\n";
    int sum = std::reduce(res.begin(), res.end());
}