// #include<vector>
// #include<string>
#include<memory>
#pragma once

class Widget {
public:
    Widget();
    ~Widget();

    Widget(Widget&& rhs);
    Widget& operator=(Widget&& rhs);

    void dosomething();
private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
