#include <iostream>
#include <thread>
#include <chrono>

int status_reg = 0;

volatile int volatile_status_reg = 0;

void wait_for_hardware_bad() {
    std::cout << "[Bad] Start waiting...\n";
    /*在编译器看来 status_reg是0 而且在while循环中没有人改它
    于是循环被优化成if status_reg == 0 while (true)...
    结果只读了一次内存 值就留存在CPU寄存器中
    就算内存里的值变了 CPU寄存器里的值还是0
    循环无限进行下去
    */
    while (status_reg == 0) {
        // waiting for hardware changed...
    }
}

void wait_for_hardware_good() {
    std::cout<< "[Good] Start waiting...\n";
    /*volatile视角
    该变量是volatile修饰的 禁止缓存到寄存器
    每次循环都去内存地址读取最新的值
    */
   while (volatile_status_reg == 0) {
    // 每次都重新读取内存
   }
   std::cout << "[Good] Hardware ready!\n";
}

// 模拟硬件行为的线程
void hardware_simulator() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "\n[Hardware] I am ready now! Changing memory...\n";

    // 模拟硬件直接修改内存
    status_reg = 1;
    volatile_status_reg = 1;
}

int main() {
    // 启动模拟硬件
    std::thread hw(hardware_simulator);
    hw.detach();        // 自己在后台跑

    wait_for_hardware_good();
    // wait_for_hardware_bad(); // 解开这行可能会卡死
}