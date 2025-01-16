#!/usr/bin/env python3
import json
import sys

def generate_signal_bind_code(f, dev_id, sig_name, sig_cfg, indent="    "):
    """生成单个信号的绑定代码"""
    if 'verilog_signals' in sig_cfg:
        # 处理多信号映射
        for v_sig, bit_range in sig_cfg['verilog_signals'].items():
            high, low = bit_range
            width = high - low + 1
            f.write(f'{indent}// 绑定 {sig_name}[{high}:{low}] 到 {v_sig}\n')
            f.write(f'{indent}{dev_id}->bindSignal("{sig_name}", '
                   f'&top->{v_sig}, {width}, {low}, {high});\n')
    else:
        # 简单信号映射
        v_sig = sig_cfg['verilog_signal']
        f.write(f'{indent}// 绑定 {sig_name} 到 {v_sig}\n')
        f.write(f'{indent}{dev_id}->bindSignal("{sig_name}", '
               f'&top->{v_sig}, {sig_cfg["width"]});\n')

def generate_bind_code(pin_map, output_file):
    with open(output_file, 'w') as f:
        # 生成头文件
        f.write('#include "cakeboard.h"\n')
        f.write('#include "devices/led.h"\n')
        f.write('#include "Vtop.h"\n\n')

        # 声明绑定函数 - 移除 extern "C"，因为这是C++代码
        f.write('void bind_all_devices(TOP_NAME* top) {\n')
        
        # 创建设备实例
        for dev_id, dev_config in pin_map.items():
            if dev_config['type'] == 'led_matrix':
                # 创建LED设备配置
                f.write(f'    cakeboard::LedConfig {dev_id}_cfg{{\n')
                f.write(f'        .rows = {dev_config["rows"]},\n')
                f.write(f'        .cols = {dev_config["cols"]},\n')
                f.write('        .signal_widths = {')
                
                # 计算总位宽
                total_width = 0
                for sig_cfg in dev_config["signals"].values():
                    if 'verilog_signals' in sig_cfg:
                        for bit_range in sig_cfg['verilog_signals'].values():
                            high, low = bit_range
                            total_width += high - low + 1
                    else:
                        total_width += sig_cfg['width']
                
                f.write(f'{total_width}')
                f.write('}\n    };\n\n')
                
                # 创建设备实例
                f.write(f'    auto {dev_id} = std::make_shared<cakeboard::LedDevice>(\n')
                f.write(f'        "{dev_id}",\n')
                f.write(f'        {dev_id}_cfg);\n\n')
                
                # 绑定信号
                for sig_name, sig_cfg in dev_config['signals'].items():
                    if sig_cfg['direction'] == 'in':
                        generate_signal_bind_code(f, dev_id, sig_name, sig_cfg)
                
                f.write(f'    cakeboard::CakeBoard::getInstance().addDevice({dev_id});\n\n')
        
        f.write('}\n')

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: pin_bind_gen.py <pin_map.json> <output.cpp>")
        sys.exit(1)
        
    with open(sys.argv[1]) as f:
        pin_map = json.load(f)
    
    generate_bind_code(pin_map, sys.argv[2]) 