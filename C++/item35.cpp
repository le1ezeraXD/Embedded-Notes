// 优先选用基于任务而非基于线程的程序设计
#include<iostream>
#include<vector>
#include<memory>
#include<thread>
#include<future>

int heavyComputation() {
    return 1+1;
}

int main(void) {
    // 旧时代Thread-based版本
    int result;

    std::thread t([&result]() {
        // 需要捕获引用
        // 如果这里throw异常 程序会直接崩溃
        result = heavyComputation();
    });

    t.join();   // 需手动等待线程结束
    std::cout << "Thread Result: " << result << "\n";

    // 新时代的Task-based版本
    // 不需要管线程管理 不需要管锁
    std::future<int> f = std::async(heavyComputation);

    // 做点别的

    try {
        std::cout << "Async result: " << f.get() << "\n";
    } catch(...) {
        std::cout << "Task Failed!\n";
    }

}