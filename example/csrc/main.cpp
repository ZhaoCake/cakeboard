#include <cakeboard.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>  // 为了使用rand()

// 这里假设Verilator生成的头文件在build目录下
#include "Vtop.h"

static TOP_NAME dut;

// 打印所有信号值(除了clk和rstn)
void print_signals() {
    std::cout << std::hex << std::setfill('0')
              << "led = 0x" << std::setw(2) << (int)dut.led
              << " | sw = 0x" << std::setw(2) << (int)dut.sw
              << std::dec << std::endl;
}

static void single_cycle() {
    dut.clk = 0;
    dut.eval();
    dut.clk = 1;
    dut.eval();
}

static void reset(int n) {
    dut.rstn = 0;  // 注意这里使用rstn(低电平有效)
    while (n-- > 0) single_cycle();
    dut.rstn = 1;
}

int main() {
    auto& board = cakeboard::CakeBoard::getInstance();
    board.init(100000000);  // 100MHz
    
    // 复位
    reset(10);

    // 初始化开关值
    dut.sw = 0;

    int cycle_count = 0;
    while(1) {
        board.update();
        
        // 每1000个周期随机改变一次开关值
        if(cycle_count % 1000 == 0) {
            dut.sw = rand() & 0xFF;  // 随机设置开关值
        }
        
        single_cycle();
        
        // 每100个周期打印一次信号值
        if(++cycle_count >= 100) {
            print_signals();
            cycle_count = 0;
        }
    }
    
    board.quit();
    return 0;
} 