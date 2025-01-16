#include "devices/led.h"
#include <cstring>
#include <bitset>
#include <iostream>
#include <iomanip>

namespace cakeboard {

LedDevice::LedDevice(const std::string& id, const LedConfig& cfg)
    : Device(id), config(cfg) {
    // 初始化LED状态矩阵
    led_states.resize(config.rows * config.cols, 0);
}

void LedDevice::update() {
    // 从所有绑定的信号更新LED状态
    size_t current_led = 0;
    
    for (const auto& binding : signal_bindings) {
        if (binding.ptr) {
            // 读取信号值 - 使用 uint32_t 来确保能处理更大的位宽
            uint32_t value = *static_cast<uint32_t*>(binding.ptr);
            
            // 如果指定了位域，提取相应的位
            if (binding.high_bit >= 0) {
                uint32_t mask = ((1U << (binding.high_bit - binding.low_bit + 1)) - 1) << binding.low_bit;
                value = (value & mask) >> binding.low_bit;
            }
            
            // 更新对应的LED状态
            for (int i = 0; i < binding.width && current_led < led_states.size(); ++i) {
                led_states[current_led++] = (value >> i) & 0x1;
            }
        }
    }

    // 显示LED状态
    std::cout << "\033[2J\033[H";  // 清屏并移动光标到开始位置
    std::cout << "LED Matrix Status:\n\n";
    
    const char* labels[] = {"Display LED:", "Debug LED: ", "Status LED:"};
    for (int row = 0; row < config.rows; ++row) {
        std::cout << labels[row] << " ";
        for (int col = 0; col < config.cols; ++col) {
            bool state = getLedState(row, col);
            std::cout << (state ? "●" : "○") << " ";
        }
        // 添加十六进制值显示
        uint8_t value = 0;
        for (int col = 0; col < 8; ++col) {
            if (getLedState(row, col)) {
                value |= (1 << col);
            }
        }
        std::cout << " [0x" << std::hex << std::setw(2) << std::setfill('0') 
                 << static_cast<int>(value) << std::dec << "]";
        std::cout << "\n";
    }
    std::cout << std::flush;
}

void LedDevice::reset() {
    std::fill(led_states.begin(), led_states.end(), 0);
}

void LedDevice::handleSignal(const std::shared_ptr<SignalPacket>& packet) {
    // LED设备暂时不需要处理信号
}

void LedDevice::bindSignal(const std::string& signal_name, void* signal_ptr, 
                          int width, int low_bit, int high_bit) {
    // 添加新的信号绑定
    signal_bindings.push_back(SignalBinding{
        .ptr = signal_ptr,
        .width = width,
        .low_bit = low_bit,
        .high_bit = high_bit
    });
}

} // namespace cakeboard 