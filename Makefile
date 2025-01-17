BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include

CXX = g++
AR = ar
# 始终启用 -fPIC，这样静态库的目标文件也可以用于动态库
CXXFLAGS = -std=c++17 -Wall -Wextra -I$(INCLUDE_DIR) -fPIC
# 添加运行时库链接
LDFLAGS = -shared -Wl,--no-undefined

# 同时生成静态库和动态库
STATIC_LIB = $(BUILD_DIR)/libcakeboard.a
SHARED_LIB = $(BUILD_DIR)/libcakeboard.so
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean

all: $(STATIC_LIB) $(SHARED_LIB)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(STATIC_LIB): $(OBJS)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $^

$(SHARED_LIB): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) -o $@ $^ -lstdc++

clean:
	rm -rf $(BUILD_DIR) 