#include<iostream>
#include<unordered_map>
#include<algorithm>
#include<memory>

//  优先使用make_unique和make_shared而非直接new

//  手搓make_unique
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&...params) {
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

//  temp class
class Widget{};

void processWidget(std::shared_ptr<Widget> spw, int priority);
int computePriority();

int main(void) {
    // processWidget(std::shared_ptr<Widget>(new Widget), computePriority());  //潜在的内存泄漏风险
    //  new Widget, shared_ptr(), computePriority()这三条在运行期执行顺序不定
    //  运行期 传递给函数的实参必须在函数调用被发起前完成评估求值
    //  如果new Widget先运行，computePriority()再被调用，而此时computePriority()出现了异常
    //  则shared_ptr()不会运行，new Widget出的Widget实例将永远不会被存储进shared_ptr, 此时内存泄漏
}