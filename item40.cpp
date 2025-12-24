// 对并发使用std::atomic 对特种内存使用volatile
#include<iostream>
#include<thread>
volatile int vi(0);

// 測試次數：次數越多，衝突機率越大
const int ITERATIONS = 100000;

// 選手 1: volatile (無用的防禦)
volatile int v_count = 0;

// 選手 2: atomic (真正的並發守護者)
std::atomic<int> a_count{0};

void task() {
    for (int i = 0; i < ITERATIONS; ++i) {
        // 這裡發生 RMW (Read-Modify-Write)
        v_count++; // 非原子操作，多線程會互相覆蓋
        a_count++; // 原子操作，保證正確
    }
}

int main(void) {
    // 对并发使用std::atomic
    // 啟動 10 個線程同時狂加
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(task);
    }
    for (auto& t : threads) t.join();

    std::cout << "理論正確值應該是: " << 10 * ITERATIONS << "\n";
    
    std::cout << "Volatile 結果: " << v_count;
    if (v_count < 10 * ITERATIONS) {
        std::cout << " (❌ 錯誤! 發生了數據競爭)\n";
    } else {
        std::cout << " (⚠️ 運氣好沒錯，但邏輯是不安全的)\n";
    }

    std::cout << "Atomic   結果: " << a_count << " (✅ 正確)\n";

    // 对特种内存使用volatile
    volatile int x = 1;
    x = 10;
    x = 20;
    auto y = x;
    y = x;
    /* 看起来很蠢对不对 因为前后赋值了两次 实际上编译器会将x = 10这句话优化掉
    只剩x=20, 在常规编程中这样的优化无伤大雅 但在涉及特种内存(传感器 无线电发射器等) 这样的优化是致命的
    假如你在读取温度 在auto y = x; y = x;这中间 可能温度已经变化过了 此时如果优化了auto y = x; 可能是致命的 因为少读取了一次温度
    同样 如果x对应无线电发射器的控制端口 则x=10可能是在发射第一次命令 x=20可能是第二次命令 如果优化掉一次就会少发射一次命令
    此时要用volatile修饰 避免编译器自作主张优化
    */
}