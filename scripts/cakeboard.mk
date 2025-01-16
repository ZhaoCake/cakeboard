# Makefile for CakeBoard

# Get Verilator include directory
VERILATOR_ROOT = $(shell verilator -V | grep VERILATOR_ROOT | head -1 | awk '{print $$3}')

# CakeBoard archive
CAKEBOARD_ARCHIVE = $(CAKEBOARD_HOME)/build/libcakeboard.a

# C++ compilation flags
CXXFLAGS += -std=c++17 \
	-I$(CAKEBOARD_HOME)/include \
	-I$(VERILATOR_ROOT)/include \
	-I$(VERILATOR_ROOT)/include/vltstd \
	-DTOP_NAME="\"V$(TOPNAME)\""

# Link flags
LDFLAGS += -lpthread

# Build CakeBoard library if not exists
$(CAKEBOARD_ARCHIVE):
	@echo "Building CakeBoard library..."
	@$(MAKE) -C $(CAKEBOARD_HOME)

# Make sure the archive is always checked
.PHONY: $(CAKEBOARD_ARCHIVE) 