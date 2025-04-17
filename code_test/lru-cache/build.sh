#!/bin/bash

# 编译程序
g++ -std=c++11 -g lru_cache_test.cpp -o lru_cache_test
g++ -std=c++11 -g lru_cache_extension.cpp -o lru_cache_extension

# 尝试设置 core dump 文件的位置和命名格式
if [ -w /proc/sys/kernel/core_pattern ]; then
    echo "$(pwd)/core.%e.%p" | sudo tee /proc/sys/kernel/core_pattern
else
    echo "Warning: Unable to set core dump pattern. Core dumps may not be generated in the current directory."
fi

# 尝试设置无限制的 core dump 大小
if ulimit -c unlimited > /dev/null 2>&1; then
    echo "Successfully set unlimited core dump size."
else
    echo "Warning: Unable to set unlimited core dump size. Core dumps may be limited in size or disabled."
fi

# 设置环境变量，让 core dump 文件生成在当前目录
export TMPDIR=.

echo "Build complete. Core dumps will be generated if possible."
