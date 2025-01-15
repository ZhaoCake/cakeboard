# CakeBoard

CakeBoard 是一个基于信号包通信的FPGA仿真器框架。

## 项目目标
- 更可配置的虚拟FPGA开发板
- 支持更多样的外设
- 现代化的交互方式
- 可选的交互方式

## 特点
- 基于信号包的设备通信
- 精确的时钟控制
- 模块化的设备系统
- 灵活的配置机制

## 当前进展 (2025-1-14)

### 完成功能
1. 核心框架搭建
   - 时钟控制机制
   - 设备管理系统
   - 信号包通信架构

2. 构建系统
   - 主项目 Makefile
   - 示例项目构建脚本
   - Verilator 集成

3. 示例项目
   - 基本的 LED 控制演示
   - 时钟和复位信号处理

### 项目结构
```
cakeboard/
├── include/              # 公共头文件
│   └── cakeboard.h      # 核心头文件
├── src/                 # 源代码
│   └── cakeboard.cpp    # 核心实现
├── scripts/             # 构建脚本
│   └── cakeboard.mk     # 公共构建规则
├── example/             # 示例代码
│   ├── csrc/           # C++源码
│   ├── vsrc/           # Verilog源码
│   ├── constr/         # 引脚约束
│   └── Makefile        # 示例构建脚本
├── Makefile            # 主构建脚本
└── README.md           # 项目文档
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
make
./build/top
```

### 依赖项目
- Verilator (用于Verilog仿真)
- C++17 编译器
- Make 构建工具

### 待办事项
1. 实现引脚绑定功能
2. 添加设备抽象层
3. 实现可视化界面
4. 添加配置系统
5. 完善文档

## 贡献指南
欢迎提交 Issue 和 Pull Request。

## License
[待定]