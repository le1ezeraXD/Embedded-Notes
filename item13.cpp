#include<iostream>
#include<vector>
#include<algorithm>

template<typename T>
using ConstIterT = std::vector<int>::const_iterator;

template <class C>
auto cbegin(const C& container)->decltype(std::begin(container)) {
    return std::begin(container);
}
int main(void) {
    std::vector<int> values;

    // const iterator is most safety than iterator
    // if u want to change the value, use the iterator
    // if not, use const
    std::vector<int>::const_iterator it = 
        std::find(values.cbegin(), values.cend(), 1983);
    values.insert(it, 1998);
}