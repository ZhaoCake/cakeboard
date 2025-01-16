/**
 * @file cakeboard.h
 * @brief CakeBoard 仿真器核心头文件
 */

#pragma once
#include <memory>
#include <vector>
#include <chrono>
#include <cstdint>
#include <string>

namespace cakeboard {

// 前向声明
class Device;

/**
 * @brief 信号包基类
 */
struct SignalPacket {
    uint64_t timestamp;    ///< 信号产生的时间戳(微秒)
    std::string deviceId;  ///< 目标设备的标识符
    virtual ~SignalPacket() = default;
};

/**
 * @brief CakeBoard仿真器核心类
 */
class CakeBoard {
public:
    static CakeBoard& getInstance();
    
    /**
     * @brief 初始化仿真器
     */
    void init();
    
    /**
     * @brief 清理资源
     */
    void quit();
    
    /**
     * @brief 更新设备状态
     */
    void update();
    
    /**
     * @brief 添加设备到仿真器
     * @param device 设备智能指针
     * @note 设备ID必须唯一
     */
    void addDevice(std::shared_ptr<Device> device);
    
    /**
     * @brief 从仿真器移除设备
     * @param deviceId 设备标识符
     */
    void removeDevice(const std::string& deviceId);
    
    /**
     * @brief 绑定信号到引脚
     * @param signal 信号指针
     * @param len 信号长度
     * @param ... 引脚编号列表(从高位到低位)
     */
    void bindPin(void* signal, int len, ...);
    
    /**
     * @brief 发送信号包
     * @param packet 信号包智能指针
     * @note 信号包会在下一个更新周期被处理
     */
    void sendSignal(const std::shared_ptr<SignalPacket>& packet);

    // 仿真器默认频率（50MHz）
    static constexpr int SIMULATION_FREQ = 50000000;

private:
    CakeBoard() = default;
    ~CakeBoard() = default;
    CakeBoard(const CakeBoard&) = delete;
    CakeBoard& operator=(const CakeBoard&) = delete;

    void updateDevices();
    void processSignals();
    
    std::vector<std::shared_ptr<Device>> devices;
    std::vector<std::shared_ptr<SignalPacket>> signalQueue;
};

/**
 * @brief 设备基类
 * @details 所有仿真设备都需要继承此类并实现相应接口
 */
class Device {
public:
    /**
     * @brief 构造函数
     * @param id 设备标识符
     * @note id必须唯一
     */
    Device(const std::string& id) : deviceId(id) {}
    virtual ~Device() = default;
    
    /**
     * @brief 更新设备状态
     * @details 每帧调用一次,用于更新设备内部状态
     */
    virtual void update() = 0;
    
    /**
     * @brief 重置设备状态
     * @details 将设备恢复到初始状态
     */
    virtual void reset() = 0;
    
    /**
     * @brief 处理接收到的信号
     * @param packet 信号包智能指针
     * @details 处理发送给本设备的信号
     */
    virtual void handleSignal(const std::shared_ptr<SignalPacket>& packet) = 0;
    
    /**
     * @brief 获取设备标识符
     * @return const std::string& 设备ID
     */
    const std::string& getId() const { return deviceId; }

protected:
    /**
     * @brief 发送信号到其他设备
     * @param packet 信号包智能指针
     * @details 派生类可以调用此方法发送信号
     */
    void sendSignal(const std::shared_ptr<SignalPacket>& packet) {
        CakeBoard::getInstance().sendSignal(packet);
    }

private:
    std::string deviceId;  ///< 设备标识符
};

} // namespace cakeboard 