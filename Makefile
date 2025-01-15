BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I$(INCLUDE_DIR)
AR = ar
ARFLAGS = rcs

LIB = $(BUILD_DIR)/libcakeboard.a
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean

all: $(LIB)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	@mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) $@ $^

clean:
	rm -rf $(BUILD_DIR) 