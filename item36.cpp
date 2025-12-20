// 如果异步是必要的 则指定std::launch::async
#include<future>
#include<memory>
#include<iostream>

// 痛点: async具有两种模式:
// 1: std::launch::async(激进模式): 和thread相同 在定义的一瞬间就启动
// 2: std::launch::deferred(延迟模式): 在call f.get()或f.wait()时才会启动 而且占用主线程
// 目标: 即标题 如果必须要执行异步 强制使用std::launch::async

using namespace std::literals;

void f() {std::this_thread::sleep_for(5s);}     // f睡眠五秒后返回

// auto fut = std::async(f);                       // 假装异步运行f

// C++11版本 将std::launch::async作为启动策略的写法
// 嗯 比我想的要复杂一些
template<typename F, typename... Ts>
inline
// std::future<typename std::result_of<F(Ts...)>::type>    // C++11写法
auto                                                       // C++14 再次感叹时代进步真好
reallyAsync(F&& f, Ts&&... params) {
    return std::async(std::launch::async,
                        std::forward<F>(f),
                        std::forward<Ts>(params)...);
}

auto fut = reallyAsync(f);

int main(void) {
    // 如果fut选择了async模式 这段代码没有问题
    // 如果fut选择deferred模式 则fut永远不会运行
    // 循环也永远不会终止
    // while (fut.wait_for(100ms) != 
    //         std::future_status::ready)
    // {
    //     std::cout << "Thread Running!" << std::endl;
    // }

    // 避免该种缺陷的方式是校验std::async返回的期望值 确定任务是否被推迟
    // 调用一个超时为0的wait_for即可
    if (fut.wait_for(0s) == std::future_status::deferred) {
        std::cout << "任务被推迟了!" << std::endl;
    } else {
        while (fut.wait_for(1000ms) != std::future_status::ready) {
            std::cout << "任务运行中..." << std::endl;
        }
        std::cout << "Mission Accomplished!" << std::endl;
    }
    
}