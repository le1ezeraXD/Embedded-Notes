// 对变化多端的线程句柄析构函数行为保持关注
#include<future>
#include<iostream>
#include<chrono>
#include<memory>
#include<thread>

void doWork() {
    // 使用标准库的 sleep，跨平台且单位明确
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "  [Work Done]\n"; 
}


int main(void) {
    std::cout << "Thread Start\n";
    // 由async启动的任务 级别最高 以下称t1
    // 如果在t1还未执行完时就析构了async返回的future对象
    // 系统会阻止这一行为 t1会继续保持运行到结束(执行一次隐式join) 称为阻塞
    std::async(std::launch::async, doWork);
    std::async(std::launch::async, doWork);
    std::async(std::launch::async, doWork);
    // 以上三个线程 因为没有变量承接 所以都会落到阻塞状态中
    // 如果这样写:
    auto t1 = std::async(std::launch::async, doWork);
    auto t2 = std::async(std::launch::async, doWork);
    auto t3 = std::async(std::launch::async, doWork);
    // 则三个线程会并行运行 因为用tx承接了创建出来的future对象 没有析构自然就不会阻塞

    // 其他情况 即由promise packaged_tast/deferred创建的任务 以下称t2
    // 同上 在t2未结束时析构
    // 会执行一次隐式detach 对象仍会被析构 但任务依旧在后台运行 无阻塞
    {
        // 1. 创建一个打包任务
        std::packaged_task<void()> task(doWork);
        
        // 2. 获取对应的 future (这就是你要析构的对象)
        std::future<void> f = task.get_future();
        
        // 3. 必须手动开一个线程去跑这个 task
        // 注意：根据 Item 37，这里的 thread t 必须 detach 或 join，否则 t 析构会崩
        // 为了演示 future 的析构不阻塞，我们把线程 detach (放生)
        std::thread t(std::move(task));
        t.detach(); 

        std::cout << "主线程: 准备析构 packaged_task 的 future...\n";
    }

    // 特殊情况 被推迟的任务
    // 使用deferred创建了任务 但是没有呼叫get就析构了
    // 此时任务根本没有开始执行 也就无所谓阻塞
    std::async(std::launch::deferred, doWork);
    std::async(std::launch::deferred, doWork);
    std::async(std::launch::deferred, doWork);
    std::cout << "Thread Over\n";
}