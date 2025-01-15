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

void CakeBoard::init(int target_hz) {
    targetHz = target_hz;
    clocksPerFrame = target_hz / TARGET_FPS;
    clockCounter = clocksPerFrame;
    lastTickTime = getTimeUs();
}

void CakeBoard::quit() {
    devices.clear();
    signalQueue.clear();
}

void CakeBoard::update() {
    if(--clockCounter < 0) {
        uint64_t now = getTimeUs();
        uint64_t elapsed = now - lastTickTime;
        
        if(elapsed == 0) return;
        
        // 动态调整每帧时钟数以维持目标频率
        int newClocksPerFrame = ((uint64_t)clocksPerFrame * 1000000) / 
                               (elapsed * TARGET_FPS);
        
        clockCounter += newClocksPerFrame;
        clocksPerFrame = newClocksPerFrame;
        
        // 每帧更新一次设备状态
        if(elapsed > 1000000 / TARGET_FPS) {
            lastTickTime = now;
            clockCounter = clocksPerFrame;
            
            processSignals();  // 先处理累积的信号
            updateDevices();   // 再更新设备状态
        }
    }
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
    packet->timestamp = getTimeUs();
    signalQueue.push_back(packet);
}

uint64_t CakeBoard::getTimeUs() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

void CakeBoard::bindPin(void* signal, int len, ...) {
    va_list args;
    va_start(args, len);
    // TODO: 实现引脚绑定逻辑
    va_end(args);
}

} // namespace cakeboard 