#include<string>
#include<vector>
#include<iostream>
#include "Widget.h"

struct Widget::Impl {
    std::string name;
    std::vector<double>data;
};

Widget::Widget()
    : pImpl(std::make_unique<Impl>())
{}

Widget::~Widget() = default;
Widget::Widget(Widget&& rhs) = default;
Widget& Widget::operator=(Widget&& rhs) = default;

void Widget::dosomething() {
    std::cout << "Fuck you!" << std::endl;
}

