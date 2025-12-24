// 对于一次性事件通信 请考虑使用void期值
#include<iostream>
#include<future>
#include<thread>

// 线程之间通信

// 方法一: 条件变量 (传统，最罗嗦)
std::mutex cv_m;
std::condition_variable cv;
bool cv_flag = false; // 共享数据

void cvWaitTask() {
    std::unique_lock<std::mutex> lk(cv_m);
    std::cout << "[CV] Waiting for signal...\n";
    // 必须传入 lambda 防止虚假唤醒
    // wait 会自动解锁 lk 进入睡眠，醒来后自动重新加锁
    cv.wait(lk, []{ return cv_flag; }); 
    std::cout << "[CV] Signal received! Running...\n";
}

void cvSignalTask() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        // 只需要短时间持有锁来修改 flag
        std::lock_guard<std::mutex> lk(cv_m);
        cv_flag = true;
        std::cout << "[CV] Sending signal!\n";
    } // 出了作用域锁自动释放，避免拿着锁通知导致唤醒的线程立刻又阻塞
    cv.notify_one();
}
// 方法二: 原子标志位(轮询等待)
std::atomic<bool> flag(false);
void waitTask() {
    std::cout << "Waiting for Flag == true...\n";
    while (!flag);
    std::cout << "Flag is true!!!\n";
}

void signalTask() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Message from signalTask!! Change Flag to true!\n";
    flag = true;
}

// 方法三: std::promise<void>
void detectTask(std::promise<void>&& p) {
    std::cout << "Detecting..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Detected!!!\n";
    p.set_value();
}

void reactTask(std::shared_future<void> f) {
    // std::cout << "Waiting for detect...\n";
    f.wait();
    std::cout << "Run!!!\n";
}

int main() {
    // 方法一 demo
    std::thread t1(cvWaitTask);
    std::thread t2(cvSignalTask);
    t1.join();
    t2.join();

    // 方法二 demo
    std::thread t3(waitTask);
    std::thread t4(signalTask);

    t3.join();
    t4.join();

    // 方法三 demo
    std::promise<void> p;

    auto temp_f = p.get_future().share();       // share会创建shared_future对象 此时所有线程都可复制它并呼叫get 结果不会消失 适合两个及以上线程使用

    std::thread detector(detectTask, std::move(p));       // 接收一个promise对象 准备用来set_value(发令)
    std::thread runner1(reactTask, temp_f);   // 接收future对象 准备wait等待指令
    std::thread runner2(reactTask, temp_f);
    std::thread runner3(reactTask, temp_f);

    detector.join();
    runner1.join();
    runner2.join();
    runner3.join();

}