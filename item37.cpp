// 使std::thread类型对象在所有路径皆不可联结
#include<iostream>
#include<vector>
#include<memory>
#include<future>

constexpr int tenMillion = 10'000'000;
// 可连结线程:
// 以异步方式已运行或可运行的线程
// 只要创建了一个std::thread并给了任务 预设就是Joinable的
void doWork1() {};
std::thread t(doWork1);      // t手里握着一个正在跑的线程 此时t Joinable


// 不可连结线程:
// 默认构造的std::thread 即创建了但是没给具体任务的线程
std::thread t_();
// 已移动的线程
std::thread t1(doWork1);
std::thread t2 = std::move(t1); // 此时t1 Unjoinable(已被move走 置空); t2 Joinable;
// 已join过的线程
// t.join();
// 已Detach过的线程
// t.detach();

// 什么时候需要使用thread而不是async:
// 1.修改线程优先级
// 假设有一个音频播放器
// 要保证音频播放线程优先级最高
void audioTask() {};
// std::async无能为力
// 因为它返回的是std::future 无法提供native_handle接口 无法修改优先级
auto f = std::async(audioTask);

// std::thread可以做到
std::thread t3(audioTask);
auto handle = t3.native_handle();    // 拿到"底层身份证"句柄
// 此时可以调用系统API修改优先级
// 伪代码 因为我不知道macOS的系统API怎么写
// SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);
// t.join();

// 2.绑定CPU核心
// 将某线程绑定CPU核心 目的是利用CPU L1/L2缓存 避免核心切换带来的性能损耗
// 原理同样是async无法获得句柄 自然也无法绑定CPU

// 3.设置线程堆栈大小
// async使用系统默认堆栈大小
// 但thread可以自己设置

// 总结: 凡是C++没有提供但操作系统提供了的线程功能 都用thread

// 为什么用thread是不太安全的:
// 假设一种情况 当执行了thread对象的析构函数 但是它仍处于可联结状态(即还没有join也没有detach的线程, 简单说就是还没有让它结束的线程)
// 此时程序会崩溃 因为thread对象都已经被杀掉了 但它的任务还在继续跑 这违反直觉
// 站在内存的角度看 假设以上情况真实发生 此时thread对象被杀掉 但它的任务doWork()还在内存里运行着
// 在之后的某个时刻 有一个函数f的调用 使用了一部分或全部线程doWork占用过的内存
// 当f运行时 doWork发起的lambda式依然在异步执行 在原先的栈上修改一些内容 但此时这个栈已经属于f了
// 那么对于f来说 自己占有的栈帧上的内容会莫名其妙的改变
// 非常的扯淡
// 所以此时要回到标题 在使用thread类型对象时 确保从它定义的作用域出去的任何路径 都要是不可联结状态
// 即 执行析构函数前一定保证它join过
// 最好的方式就是自己在析构函数中写一个join或是detach

class ThreadRAII {
public:
    enum class DtorAction {join, detach};
    ThreadRAII(std::thread&& t, DtorAction a)
    : action(a), t(std::move(t)){}

    ~ThreadRAII() {
        if (t.joinable()) {
            if (action == DtorAction::join) {
                t.join();
            }
            else {
                t.detach();
            }
        }
    }
    ThreadRAII(ThreadRAII&&) = default;
    ThreadRAII& operator=(ThreadRAII&&) = default;

    std::thread& get() {return t;}
private:
    DtorAction action;
    std::thread t;
};

bool doWork(std::function<bool(int)> filter,
            int maxVal = tenMillion)
{
    std::vector<int> goodVals;
    ThreadRAII t(
        std::thread([&filter, maxVal, &goodVals] {
            for (auto i = 0; i <= maxVal; ++i) {if (filter(i)) goodVals.push_back(i);}
        }),
        ThreadRAII::DtorAction::join
    );

    auto nh = t.get().native_handle();

    if (true) {             // 这里简写condition
        t.get().join();
        return true;
    }
    return false;
}

int main(void) {

}