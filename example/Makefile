TOPNAME = top
NXDC_FILES = constr/top.nxdc
VERILATOR_HOME ?= /usr/share/verilator
CAKEBOARD_HOME ?= ..

# 添加动态库路径
export LD_LIBRARY_PATH := $(CAKEBOARD_HOME)/build:$(LD_LIBRARY_PATH)

VERILATOR = verilator
VERILATOR_CFLAGS += -MMD --build -cc  \
				-O3 --x-assign fast --x-initial fast --noassert

BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)

# 编译选项
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I$(CAKEBOARD_HOME)/include \
           -I$(VERILATOR_HOME)/include -I$(BUILD_DIR) \
           -DTOP_NAME=Vtop
LDFLAGS = -L$(CAKEBOARD_HOME)/build -lcakeboard

# 源文件
VSRCS = $(shell find vsrc -name "*.v")
CSRCS = $(shell find csrc -name "*.cpp")

# 自动生成的设备绑定代码
AUTO_BIND_CPP = $(BUILD_DIR)/auto_bind.cpp

# 确保生成的代码存在
$(AUTO_BIND_CPP): devices/pin_map.json
	@mkdir -p $(dir $@)
	python3 $(CAKEBOARD_HOME)/scripts/pin_bind_gen.py $< $@

CSRCS += $(AUTO_BIND_CPP)

# 编译规则
$(BIN): $(AUTO_BIND_CPP) $(VSRCS) $(CSRCS)
	@rm -rf $(OBJ_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) \
		--top-module $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CXXFLAGS)) \
		$(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN))

default: $(BIN)

all: default

run: $(BIN)
	@./run.sh

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(AUTO_BIND_CPP)

.PHONY: default all clean run 