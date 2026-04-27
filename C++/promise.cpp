// promise demo
#include<memory>
#include<future>
#include<iostream>
#include<thread>

// promise相当于任务启动器
// 函数接收一个promise类型的实参
void worker(std::promise<int> p) {
    std::cout << "工頭: 開始搬磚...\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    int result = 42;
    std::cout << "工頭: 磚搬完了，把結果塞進 Promise！\n";
    // 
    p.set_value(result);
}

int main(void) {
    std::promise<int> p;
    auto f = p.get_future();

    std::thread t(worker, std::move(p));
    std::cout << "老闆: 在辦公室等結果...\n";
    int value = f.get();
    std::cout << "老闆: 拿到結果了: " << value << "\n";
    t.join();

}