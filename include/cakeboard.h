/**
 * @file cakeboard.h
 * @brief CakeBoard 仿真器核心头文件
 * @details 定义了仿真器的核心类和设备接口
 * 
 * CakeBoard 是一个基于信号包通信的FPGA仿真器框架。它提供了:
 * - 精确的时钟控制
 * - 设备管理机制
 * - 基于信号包的设备间通信
 * - 可扩展的设备接口
 * 
 * @author zhaocake
 * @date 2024
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
 * @details 所有设备间传递的信号都需要继承自此类
 */
struct SignalPacket {
    uint64_t timestamp;    ///< 信号产生的时间戳(微秒)
    std::string deviceId;  ///< 目标设备的标识符
    virtual ~SignalPacket() = default;
};

/**
 * @brief CakeBoard仿真器核心类
 * @details 负责管理时钟、设备和信号的调度
 */
class CakeBoard {
public:
    /**
     * @brief 获取单例实例
     * @return CakeBoard& 单例引用
     */
    static CakeBoard& getInstance();
    
    /**
     * @brief 初始化仿真器
     * @param target_hz 目标时钟频率(Hz)
     */
    void init(int target_hz = 100000000);
    
    /**
     * @brief 清理资源并关闭仿真器
     */
    void quit();
    
    /**
     * @brief 更新仿真器状态
     * @details 处理时钟、信号和设备更新,需要在主循环中持续调用
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

private:
    CakeBoard() = default;
    ~CakeBoard() = default;
    CakeBoard(const CakeBoard&) = delete;
    CakeBoard& operator=(const CakeBoard&) = delete;

    /**
     * @brief 获取当前时间(微秒)
     * @return uint64_t 当前时间戳
     */
    uint64_t getTimeUs();
    
    /**
     * @brief 更新所有设备状态
     */
    void updateDevices();
    
    /**
     * @brief 处理信号队列
     * @details 将累积的信号分发给对应的设备
     */
    void processSignals();
    
    std::vector<std::shared_ptr<Device>> devices;      ///< 设备列表
    std::vector<std::shared_ptr<SignalPacket>> signalQueue;  ///< 信号队列
    
    int targetHz{100000000};     ///< 目标频率
    uint64_t lastTickTime{0};    ///< 上次时钟更新时间
    int clocksPerFrame{1};       ///< 每帧时钟数
    int clockCounter{0};         ///< 时钟计数器
    
    static constexpr int TARGET_FPS = 60;  ///< 目标帧率
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