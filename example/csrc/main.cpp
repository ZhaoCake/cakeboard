#include <cakeboard.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <thread>

#include "Vtop.h"

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

void print_signals() {
    std::cout << std::hex << std::setfill('0')
              << "led = 0x" << std::setw(2) << (int)dut.led
              << " | sw = 0x" << std::setw(2) << (int)dut.sw
              << std::dec << std::endl;
}

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
    
    reset(10, clock);
    dut.sw = 0;

    int cycle_count = 0;
    
    try {
        while(1) {
            board.update();
            
            // 每1000个周期随机改变一次开关值
            if(cycle_count % 1000 == 0) {
                dut.sw = rand() & 0xFF;
            }
            
            clock.tick();
            
            // 每100个周期打印一次信号值
            if(++cycle_count >= 100) {
                print_signals();
                cycle_count = 0;
            }
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