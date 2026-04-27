#include<iostream>
#include<vector>

using namespace std;

class Widget {
    private:
        int x;
        int y;
    public:
        Widget(int x, int y) {
            this->x = x;
            this->y = y;
        }
};

int main() {
    vector<int> tmp = {1,2,3};
    vector<int> res{1,2,3};
    Widget w();     // w is a function, not a val
}