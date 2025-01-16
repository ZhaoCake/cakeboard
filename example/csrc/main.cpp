#include <cakeboard.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "Vtop.h"

// 声明绑定函数
void bind_all_devices(TOP_NAME* top);

static TOP_NAME dut;

// 时钟控制类
class ClockControl {
public:
    explicit ClockControl(int64_t period_ns) 
        : period(std::chrono::nanoseconds(period_ns)) {
        next_time = std::chrono::steady_clock::now();
    }

    void tick() {
        // 执行时钟周期
        dut.clk = 0;
        dut.eval();
        dut.clk = 1;
        dut.eval();
        
        // 更新下一个时钟周期的时间
        next_time += period;
        
        // 如果落后于目标时间，等待
        auto now = std::chrono::steady_clock::now();
        if (now < next_time) {
            std::this_thread::sleep_until(next_time);
        }
    }

private:
    std::chrono::nanoseconds period;
    std::chrono::steady_clock::time_point next_time;
};

static void reset(int n, ClockControl& clock) {
    dut.rstn = 0;
    while (n-- > 0) clock.tick();
    dut.rstn = 1;
}

int main() {
    auto& board = cakeboard::CakeBoard::getInstance();
    board.init();
    
    // 初始化时钟控制
    ClockControl clock(1000000000 / cakeboard::CakeBoard::SIMULATION_FREQ);
    
    // 绑定设备
    bind_all_devices(&dut);
    
    reset(10, clock);

    try {
        while(1) {
            board.update();
            clock.tick();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        board.quit();
        return 1;
    }
    
    board.quit();
    return 0;
} 