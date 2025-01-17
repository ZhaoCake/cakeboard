#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// 导出的C接口函数声明
int getLedRows();
int getLedCols();
bool getLedState(int row, int col);
int getSwitchRows();
int getSwitchCols();
bool getSwitchState(int row, int col);
bool updateSwitchState(int row, int col, bool state);

#ifdef __cplusplus
}

// 全局设备指针声明
namespace cakeboard {
    class LedDevice;
    class SwitchDevice;
    extern LedDevice* current_led_device;
    extern SwitchDevice* current_switch_device;
}
#endif