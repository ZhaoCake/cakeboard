#pragma once
#include "cakeboard.h"
#include <vector>

namespace cakeboard {

struct LedConfig {
    int rows;
    int cols;
    std::vector<int> signal_widths;  // 每个信号的位宽
};

class LedDevice : public Device {
public:
    LedDevice(const std::string& id, const LedConfig& config);
    
    void update() override;
    void reset() override;
    void handleSignal(const std::shared_ptr<SignalPacket>& packet) override;

    // 绑定信号（支持位域）
    void bindSignal(const std::string& signal_name, void* signal_ptr, 
                   int width, int low_bit = 0, int high_bit = -1);

    // 获取LED状态
    bool getLedState(int row, int col) const {
        int idx = row * config.cols + col;
        return (idx < led_states.size()) ? led_states[idx] : false;
    }

    // 获取设备配置
    const LedConfig& getConfig() const { return config; }

private:
    LedConfig config;
    std::vector<uint8_t> led_states;  // LED状态矩阵
    struct SignalBinding {
        void* ptr;
        int width;
        int low_bit;
        int high_bit;
    };
    std::vector<SignalBinding> signal_bindings;
};

} // namespace cakeboard 