#include<iostream>
#include<vector>
#include<array>

// constexpr function
// if input constexpr variable, the function will return a constexpr result
// if input variable is not constexpr, then will return a not constexpr result

constexpr
int pow(int base, int exp) noexcept {
    return exp == 0 ? 1 : base * pow(base, exp-1);
}

class Point {
public:
    constexpr Point(double xVal = 0, double yVal = 0) noexcept
    : x(xVal), y(yVal)
    {}

    constexpr double xValue() const noexcept {return x;}
    constexpr double yValue() const noexcept {return y;}

    void setX(double newX) noexcept {x = newX;}
    void setY(double newY) noexcept {y = newY;}
private:
    double x, y;
};

int main(void) {
    // constexpr int n = 10;
    // std::vector<int> res(n,0);
    // const int sz = 10;
    // constexpr auto arraySize1 = sz;
    // std::array<int, arraySize1> data1;
    // constexpr int arraySize2 = 10;
    // std::array<int, arraySize2> data2;
    constexpr Point p1(9.4, 27.7);
    constexpr Point p2(28.8, 5.3);  // both are the constexpr variable
    
}