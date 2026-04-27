// Item 34: 優先選用 Lambda，而非 std::bind
#include <iostream>
#include <functional>
#include <chrono>

using namespace std::chrono;
using namespace std::placeholders;

void setAlarm(steady_clock::time_point t, int volume) {
    if (steady_clock::now() > t) {
        std::cout << "Ding! Volume: " << volume << "\n";
    } else {
        std::cout << "Waiting...\n";
    }
}

int main() {
    auto oneHour = 1h;

    // --- 1. 痛苦的 std::bind ---
    // 陷阱：時間是在這裡計算的，不是在呼叫時計算的！
    auto bindFunc = std::bind(setAlarm, steady_clock::now() + oneHour, _1);

    // --- 2. 優雅的 Lambda ---
    // 時間計算被延遲到函數體內，邏輯正確
    auto lambdaFunc = [oneHour](int vol) {
        setAlarm(steady_clock::now() + oneHour, vol);
    };
    
    // --- 3. 呼叫 ---
    bindFunc(10);   // 可讀性差，行為可能不符預期
    lambdaFunc(10); // 清晰，正確
    
    std::cout << "Item 34 Completed. std::bind is dead.\n";
}