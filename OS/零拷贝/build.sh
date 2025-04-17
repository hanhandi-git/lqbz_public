#!/bin/bash
# 使用方法:
# - 运行性能测试: ./build.sh
# - 运行持久化测试: ./build.sh persistence
# 清理旧的构建目录
if [ -d "build" ]; then
    echo "Cleaning old build directory..."
    rm -rf build
fi

# 创建新的构建目录
echo "Creating build directory..."
mkdir build
cd build

# 运行CMake配置
echo "Configuring with CMake..."
cmake .. || {
    echo "CMake configuration failed"
    exit 1
}

# 编译项目
echo "Building project..."
make || {
    echo "Build failed"
    exit 1
}

# 运行测试
if [ "$1" = "persistence" ]; then
    echo -e "\nRunning persistence tests..."
    echo "Step 1: Writing test data..."
    ./persistence_writer
    echo -e "\nStep 2: Reading and verifying test data..."
    ./persistence_reader
else
    echo -e "\nRunning queue performance tests..."
    ./queue_test
fi

cd .. 