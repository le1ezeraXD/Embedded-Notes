#include<iostream>
#include<vector>
#include<math.h>

using namespace std;

class Widget{};

void processWidget(Widget& w, bool b) {
    cout << "Hello World!" << endl;
}
double calcEpsilon() { return M_PI;};

int main() {
    Widget w;
    vector<bool> features(const Widget& w);
    // cout << features.size() << endl;
    // bool highPriority = features(w)[5];
    // processWidget(w, highPriority);
    auto ep = static_cast<float>(calcEpsilon());
    return 0;


}