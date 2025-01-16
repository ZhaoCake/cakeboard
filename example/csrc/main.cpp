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
        : period(std::chrono::nanoseconds(period_ns)),
          start_time(std::chrono::steady_clock::now()) {
        next_time = start_time;
    }

    void tick() {
        // 等待到下一个时钟周期
        auto now = std::chrono::steady_clock::now();
        if (now < next_time) {
            std::this_thread::sleep_until(next_time);
        }
        next_time += period;

        // 执行时钟周期
        dut.clk = 1;
        dut.eval();
        dut.clk = 0;
        dut.eval();

        // 更新并显示仿真时间
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - start_time).count();
        std::cout << "\rSimulation time: " << elapsed << "s" << std::flush;
    }

private:
    std::chrono::nanoseconds period;
    std::chrono::steady_clock::time_point next_time;
    std::chrono::steady_clock::time_point start_time;
};

static void reset(int n, ClockControl& clock) {
    dut.rstn = 0;
    while (n-- > 0) clock.tick();
    dut.rstn = 1;
}

int main() {
    auto& board = cakeboard::CakeBoard::getInstance();
    board.init();
    
    // 使用较低的频率，比如100KHz
    constexpr int SIMULATION_FREQ = 100000;  // 100KHz
    ClockControl clock(1000000000 / SIMULATION_FREQ);
    
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
        std::cerr << "\nError: " << e.what() << std::endl;
        board.quit();
        return 1;
    }
    
    board.quit();
    return 0;
} 