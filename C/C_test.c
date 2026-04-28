/**
 * @file embedded_c_deep_dive.c
 * @brief 嵌入式 C 語言底層特性演示
 * @details 涵蓋靜態變量、溢出邏輯、硬體寄存器模擬及狀態機實現
 * @author Liu Shucheng (Liu Ji)
 */

#include <stdio.h>
#include <stdint.h>

// --- 1. 枚舉與狀態機定義 ---
typedef enum {
    SYS_IDLE = 0,
    SYS_ACTIVE,
    SYS_FAULT
} SystemMode_t;

// --- 2. Const Volatile 模擬硬體寄存器 ---
// 模擬一個唯讀的硬體狀態位，軟體不可修改，但硬體可能隨時改變其值
const volatile uint8_t HW_STATUS_REG = 0x01;

/**
 * @brief 演示 static 變量的持久性
 */
void static_variable_demo() {
    static int call_count = 0; // 只在編譯期初始化一次，存於靜態區
    int local_var = 0;        // 每次進函數都會在棧上重新分配並賦值為 0
    
    call_count++;
    local_var++;
    
    printf("[Static Demo] Call Count (Static): %d, Local Var (Auto): %d\n", 
            call_count, local_var);
}

/**
 * @brief 演示有符號與無符號整型的溢出行為
 */
void integer_overflow_demo() {
    // 無符號溢出：255 + 1 -> 0
    uint8_t u_val = 255;
    printf("[Overflow] Unsigned 255 + 1 = %u\n", (uint8_t)(u_val + 1));

    // 有符號溢出：127 + 1 -> -128 (基於二進制補碼邏輯)
    int8_t s_val = 127;
    printf("[Overflow] Signed 127 + 1 = %d\n", (int8_t)(s_val + 1));
}

/**
 * @brief 演示 Switch-Case 配合 Enum 的狀態管理
 */
void state_machine_demo(SystemMode_t mode) {
    switch (mode) {
        case SYS_IDLE:
            printf("[State] System is in IDLE mode.\n");
            break;
        case SYS_ACTIVE:
            printf("[State] System is ACTIVE, processing data...\n");
            break;
        case SYS_FAULT:
            printf("[State] FAULT detected! Watchdog may reset soon.\n");
            break;
        default:
            printf("[State] Unknown mode.\n");
            break;
    }
}

int main() {
    printf("--- Embedded C Concept Demo Start ---\n\n");

    // 1. Static 測試
    for (int k = 0; k < 3; k++) {
        static_variable_demo();
    }

    // 2. 溢出測試
    printf("\n");
    integer_overflow_demo();

    // 3. Const Volatile 說明
    printf("\n[Register Demo] HW_STATUS_REG current value: %u\n", HW_STATUS_REG);
    // HW_STATUS_REG = 0x02; // 如果取消這行注釋，編譯會報錯，體現 const 的保護作用

    // 4. 狀態機測試
    printf("\n");
    state_machine_demo(SYS_ACTIVE);

    printf("\n--- Demo End ---\n");
    return 0;
}