TOPNAME = top
NXDC_FILES = constr/top.nxdc
INC_PATH ?=

VERILATOR = verilator
VERILATOR_CFLAGS += -MMD --build -cc  \
				-O3 --x-assign fast --x-initial fast --noassert

BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)

# 检查CAKEBOARD_HOME环境变量
ifeq ($(CAKEBOARD_HOME),)
$(error Please set CAKEBOARD_HOME environment variable)
endif

# 创建构建目录
$(shell mkdir -p $(BUILD_DIR))

# 添加 cakeboard 库的头文件路径
INC_PATH += $(CAKEBOARD_HOME)/include

# 添加 cakeboard 库的链接路径和库名
LDFLAGS += -L$(CAKEBOARD_HOME)/build -lcakeboard

# project source
VSRCS = $(shell find $(abspath ./vsrc) -name "*.v")
CSRCS = $(shell find $(abspath ./csrc) -name "*.c" -or -name "*.cc" -or -name "*.cpp")

# 自动生成的设备绑定代码
AUTO_BIND_CPP = $(BUILD_DIR)/auto_bind.cpp

# 确保生成的代码存在
$(AUTO_BIND_CPP): devices/pin_map.json
	@mkdir -p $(dir $@)
	python3 $(CAKEBOARD_HOME)/scripts/pin_bind_gen.py $< $@

# 添加自动生成的代码到源文件列表（移到前面）
CSRCS += $(AUTO_BIND_CPP)

# 确保在编译前生成绑定代码和库
$(BIN): $(AUTO_BIND_CPP) $(VSRCS) $(CSRCS) $(CAKEBOARD_ARCHIVE)
	@rm -rf $(OBJ_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) \
		--top-module $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CXXFLAGS)) \
		$(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN))

# rules for CakeBoard
include $(CAKEBOARD_HOME)/scripts/cakeboard.mk

# rules for verilator
INCFLAGS = $(addprefix -I, $(INC_PATH))
CXXFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\""

default: $(BIN)

all: default

run: $(BIN)
	@$^

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(AUTO_BIND_CPP)

.PHONY: default all clean run 