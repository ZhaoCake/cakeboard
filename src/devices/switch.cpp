#include "devices/switch.h"
#include <cstring>
#include <bitset>
#include <iostream>
#include <iomanip>

// 声明外部引用
extern cakeboard::SwitchDevice* current_switch_device;

namespace cakeboard {

SwitchDevice::SwitchDevice(const std::string& id, const SwitchConfig& cfg)
    : Device(id), config(cfg) {
    // 初始化开关状态矩阵
    switch_states.resize(config.rows * config.cols, 0);
}

void SwitchDevice::update() {
    // 更新所有绑定的信号
    size_t current_switch = 0;
    
    for (const auto& binding : signal_bindings) {
        if (binding.ptr) {
            // 计算当前信号的值
            uint32_t value = 0;
            for (int i = 0; i < binding.width && current_switch < switch_states.size(); ++i) {
                if (switch_states[current_switch++]) {
                    value |= (1U << i);
                }
            }
            
            // 如果指定了位域，将值放入正确的位置
            if (binding.high_bit >= 0) {
                uint32_t mask = ((1U << (binding.high_bit - binding.low_bit + 1)) - 1) << binding.low_bit;
                uint32_t old_value = *static_cast<uint32_t*>(binding.ptr);
                old_value &= ~mask;  // 清除原有位
                old_value |= (value << binding.low_bit) & mask;  // 设置新值
                *static_cast<uint32_t*>(binding.ptr) = old_value;
            } else {
                *static_cast<uint32_t*>(binding.ptr) = value;
            }
        }
    }

    // 显示开关状态
    std::cout << "\033[" << (config.rows + 5) << ";0H";  // 移动到LED显示下方
    std::cout << "Switch Matrix Status:\n\n";
    
    const char* labels[] = {"Input Switch:", "Mode Select:", "Config SW: "};
    for (int row = 0; row < config.rows; ++row) {
        std::cout << labels[row] << " ";
        for (int col = 0; col < config.cols; ++col) {
            bool state = getSwitchState(row, col);
            std::cout << (state ? "●" : "○") << " ";
        }
        // 添加十六进制值显示
        uint8_t value = 0;
        for (int col = 0; col < 8; ++col) {
            if (getSwitchState(row, col)) {
                value |= (1 << col);
            }
        }
        std::cout << " [0x" << std::hex << std::setw(2) << std::setfill('0') 
                 << static_cast<int>(value) << std::dec << "]";
        std::cout << "\n";
    }
    std::cout << std::flush;
}

void SwitchDevice::reset() {
    std::fill(switch_states.begin(), switch_states.end(), 0);
}

void SwitchDevice::handleSignal(const std::shared_ptr<SignalPacket>& packet) {
    // 开关设备暂时不需要处理信号
}

void SwitchDevice::bindSignal(const std::string& signal_name, void* signal_ptr, 
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