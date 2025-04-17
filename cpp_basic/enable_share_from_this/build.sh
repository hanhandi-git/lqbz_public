#!/bin/bash

# 创建build目录
mkdir -p build

# 编译
g++ -std=c++11 -Wall -g example.cpp -o build/example

# 如果编译成功,输出提示
if [ $? -eq 0 ]; then
    echo "编译成功,可执行文件在 build/example"
else
    echo "编译失败"
fi 