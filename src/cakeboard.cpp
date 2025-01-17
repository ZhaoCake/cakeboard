/**
 * @file cakeboard.cpp
 * @brief CakeBoard 仿真器核心实现
 * @details 实现了时钟控制、设备管理和信号调度等核心功能
 */

#include "cakeboard.h"
#include "devices/led.h"
#include "devices/switch.h"
#include <algorithm>

namespace cakeboard {
    // 全局设备指针定义
    LedDevice* current_led_device = nullptr;
    SwitchDevice* current_switch_device = nullptr;
}

namespace cakeboard {

void CakeBoard::quit() {
    // 清理设备和指针
    devices.clear();
    signalQueue.clear();
    current_led_device = nullptr;
    current_switch_device = nullptr;
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
    // 保存设备并设置相应的全局指针
    devices.push_back(device);
    
    // 根据设备类型设置全局指针
    if (auto led = std::dynamic_pointer_cast<LedDevice>(device)) {
        current_led_device = led.get();
    }
    else if (auto sw = std::dynamic_pointer_cast<SwitchDevice>(device)) {
        current_switch_device = sw.get();
    }
}

void CakeBoard::removeDevice(const std::string& deviceId) {
    // 移除设备前清除相应的全局指针
    auto it = std::find_if(devices.begin(), devices.end(),
        [&](const auto& dev) { return dev->getId() == deviceId; });
        
    if (it != devices.end()) {
        if (auto led = std::dynamic_pointer_cast<LedDevice>(*it)) {
            if (current_led_device == led.get()) {
                current_led_device = nullptr;
            }
        }
        else if (auto sw = std::dynamic_pointer_cast<SwitchDevice>(*it)) {
            if (current_switch_device == sw.get()) {
                current_switch_device = nullptr;
            }
        }
        devices.erase(it);
    }
}

void CakeBoard::sendSignal(const std::shared_ptr<SignalPacket>& packet) {
    signalQueue.push_back(packet);
}

} // namespace cakeboard 

// 导出的C接口实现
extern "C" {

int getLedRows() {
    return cakeboard::current_led_device ? cakeboard::current_led_device->getConfig().rows : 0;
}

int getLedCols() {
    return cakeboard::current_led_device ? cakeboard::current_led_device->getConfig().cols : 0;
}

bool getLedState(int row, int col) {
    return cakeboard::current_led_device ? cakeboard::current_led_device->getLedState(row, col) : false;
}

int getSwitchRows() {
    return cakeboard::current_switch_device ? cakeboard::current_switch_device->getConfig().rows : 0;
}

int getSwitchCols() {
    return cakeboard::current_switch_device ? cakeboard::current_switch_device->getConfig().cols : 0;
}

bool getSwitchState(int row, int col) {
    return cakeboard::current_switch_device ? cakeboard::current_switch_device->getSwitchState(row, col) : false;
}

bool updateSwitchState(int row, int col, bool state) {
    if (!cakeboard::current_switch_device) return false;
    
    if (row < 0 || row >= cakeboard::current_switch_device->getConfig().rows ||
        col < 0 || col >= cakeboard::current_switch_device->getConfig().cols) {
        return false;
    }
    
    cakeboard::current_switch_device->setSwitchState(row, col, state);
    return true;
}

} // extern "C" 