#pragma once
#include "cakeboard.h"
#include <vector>

namespace cakeboard {

struct SwitchConfig {
    int rows;
    int cols;
    std::vector<int> signal_widths;  // 每个信号的位宽
};

class SwitchDevice : public Device {
public:
    SwitchDevice(const std::string& id, const SwitchConfig& config);
    
    void update() override;
    void reset() override;
    void handleSignal(const std::shared_ptr<SignalPacket>& packet) override;

    // 绑定信号（支持位域）
    void bindSignal(const std::string& signal_name, void* signal_ptr, 
                   int width, int low_bit = 0, int high_bit = -1);

    // 获取开关状态
    bool getSwitchState(int row, int col) const {
        int idx = row * config.cols + col;
        return (idx < switch_states.size()) ? switch_states[idx] : false;
    }

    // 设置开关状态
    void setSwitchState(int row, int col, bool state) {
        int idx = row * config.cols + col;
        if (idx < switch_states.size()) {
            switch_states[idx] = state;
        }
    }

    // 获取设备配置
    const SwitchConfig& getConfig() const { return config; }

private:
    SwitchConfig config;
    std::vector<uint8_t> switch_states;  // 开关状态矩阵
    struct SignalBinding {
        void* ptr;
        int width;
        int low_bit;
        int high_bit;
    };
    std::vector<SignalBinding> signal_bindings;
};

} // namespace cakeboard 