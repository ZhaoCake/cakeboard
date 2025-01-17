#!/bin/bash
CAKEBOARD_HOME=$(realpath ..)
export LD_LIBRARY_PATH=$CAKEBOARD_HOME/build:$LD_LIBRARY_PATH

# 运行程序
./build/top