# CakeBoard

CakeBoard 是一个基于信号包通信的FPGA仿真器框架。

## 项目目标
- 更可配置的虚拟FPGA开发板
- 支持更多样的外设
- 现代化的交互方式
- 可选的交互方式

## 致谢

本项目受到 [NVBoard](https://github.com/NJU-ProjectN/nvboard) 项目的启发。NVBoard 是一个优秀的虚拟FPGA开发板项目，为数字电路教学提供了很好的工具。CakeBoard 在以下方面借鉴了 NVBoard 的设计：

- 基于 Verilator 的仿真框架
- 设备抽象和管理机制
- 引脚绑定的概念

相比 NVBoard，CakeBoard 的改进：
- 使用 JSON 配置替代 nxdc 文件，提供更灵活的设备配置
- 支持可配置的设备矩阵
- 基于信号包的设备通信机制
- 现代化的 C++ 接口设计

感谢 NVBoard 团队为开源社区做出的贡献！

## 特点
- 基于信号包的设备通信
- 精确的时钟控制
- 模块化的设备系统
- 灵活的配置机制

## 当前进展 (2025-01-16)

### 完成功能
1. 核心框架搭建
   - 时钟控制机制
   - 设备管理系统
   - 信号包通信架构

2. 构建系统
   - 主项目 Makefile
   - 主项目编译成动态库被调用
   - 示例项目构建脚本
   - Verilator 集成

3. 示例项目
   - 基本的 LED 控制演示
   - 时钟和复位信号处理

4. LED设备实现
   - 添加了LED矩阵设备
   - 支持多行多列的LED显示
   - 实现了位域映射机制
   - 添加了终端可视化显示
   - 支持十六进制值显示

5. 开关设备实现
   - 添加了开关矩阵设备
   - 支持多行多列的开关配置
   - 实现了位域映射机制
   - 添加了终端可视化显示

6. 时钟控制优化
   - 降低仿真频率到100KHz以提高稳定性
   - 添加了仿真时间显示
   - 改进了时序精度

7. 设备配置系统
   - 添加了JSON格式的设备配置文件
   - 支持灵活的信号映射
   - 实现了自动代码生成
   - 支持复杂的位域映射

### 项目结构
```
cakeboard/
├── include/              # 公共头文件
│   ├── cakeboard.h      # 核心头文件
│   ├── device_factory.h # 设备工厂
│   └── devices/         # 设备头文件
│       ├── led.h       # LED设备
│       └── switch.h    # 开关设备
├── src/                 # 源代码
│   ├── cakeboard.cpp   # 核心实现
│   └── devices/        # 设备实现
│       ├── led.cpp    # LED设备
│       └── switch.cpp # 开关设备
├── scripts/            # 构建脚本
│   ├── cakeboard.mk   # 公共构建规则
│   └── pin_bind_gen.py # 引脚绑定生成器
├── example/           # 示例代码
│   ├── csrc/         # C++源码
│   ├── vsrc/         # Verilog源码
│   ├── constr/       # 引脚约束
│   └── devices/      # 设备配置
├── Makefile          # 主构建脚本
└── README.md         # 项目文档
```

### 使用方法

1. 设置环境变量：
```bash
export CAKEBOARD_HOME=/path/to/cakeboard
```

2. 构建核心库：
```bash
cd $CAKEBOARD_HOME
make
```

3. 运行示例：
```bash
cd example
make run
```

### 设备配置示例

在 devices/pin_map.json 中配置设备：
```json
{
    "led_matrix": {
        "type": "led_matrix",
        "rows": 3,
        "cols": 8,
        "signals": {
            "led": {
                "width": 24,
                "direction": "in",
                "mapping": "sequential",
                "verilog_signals": {
                    "led_display": [7, 0],
                    "debug_led": [15, 8],
                    "status_led": [23, 16]
                }
            }
        }
    }
}
```

### 依赖项目
- Verilator (用于Verilog仿真)
- C++17 编译器
- Make 构建工具
- Python3 (用于代码生成)

### 待办事项
1. 实现引脚绑定功能
2. 添加设备抽象层
3. 实现可视化界面
4. 添加配置系统
5. 完善文档
6. 添加键盘控制开关状态
7. 实现设备间的信号通信
8. 添加配置文件验证
9. 改进错误处理机制

## 贡献指南
欢迎提交 Issue 和 Pull Request。

## License
[待定]