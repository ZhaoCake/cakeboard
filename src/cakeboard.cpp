/**
 * @file cakeboard.cpp
 * @brief CakeBoard 仿真器核心实现
 * @details 实现了时钟控制、设备管理和信号调度等核心功能
 */

#include "cakeboard.h"
#include <stdarg.h>
#include <algorithm>

namespace cakeboard {

CakeBoard& CakeBoard::getInstance() {
    static CakeBoard instance;
    return instance;
}

void CakeBoard::init() {
    // 只进行基本初始化
}

void CakeBoard::quit() {
    devices.clear();
    signalQueue.clear();
}

void CakeBoard::update() {
    processSignals();  // 处理信号
    updateDevices();   // 更新设备状态
}

void CakeBoard::updateDevices() {
    for(auto& device : devices) {
        device->update();
    }
}

void CakeBoard::processSignals() {
    // 将信号分发给对应的设备
    for(const auto& signal : signalQueue) {
        for(auto& device : devices) {
            if(device->getId() == signal->deviceId) {
                device->handleSignal(signal);
                break;
            }
        }
    }
    signalQueue.clear();
}

void CakeBoard::addDevice(std::shared_ptr<Device> device) {
    devices.push_back(device);
}

void CakeBoard::removeDevice(const std::string& deviceId) {
    devices.erase(
        std::remove_if(devices.begin(), devices.end(),
            [&](const auto& device) { return device->getId() == deviceId; }),
        devices.end()
    );
}

void CakeBoard::sendSignal(const std::shared_ptr<SignalPacket>& packet) {
    signalQueue.push_back(packet);
}

void CakeBoard::bindPin(void* signal, int len, ...) {
    va_list args;
    va_start(args, len);
    // TODO: 实现引脚绑定逻辑
    va_end(args);
}

} // namespace cakeboard 