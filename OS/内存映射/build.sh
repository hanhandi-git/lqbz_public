#!/bin/bash

# 创建编译目录
mkdir -p build

# 编译示例程序
g++ -std=c++17 example.cpp -o build/lru_cache_example -pthread

# 检查编译结果
if [ $? -eq 0 ]; then
    echo "编译成功！可执行文件位于 build/lru_cache_example"
    echo "运行方式: ./build/lru_cache_example"
else
    echo "编译失败！"
fi 