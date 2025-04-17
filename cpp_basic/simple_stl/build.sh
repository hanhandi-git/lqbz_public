#!/bin/bash

# 编译选项
CXX="g++"
CXXFLAGS="-std=c++11 -Wall -Wextra"

# 编译所有容器实现
echo "Compiling vector..."
${CXX} ${CXXFLAGS} vector.cpp -o vector_test

echo "Compiling queue..."
${CXX} ${CXXFLAGS} queue.cpp -o queue_test

echo "Compiling stack..."
${CXX} ${CXXFLAGS} stack.cpp -o stack_test

echo "Compiling hashtable..."
${CXX} ${CXXFLAGS} hashtable.cpp -o hashtable_test

echo "Compiling doubly_linked_list..."
${CXX} ${CXXFLAGS} doubly_linked_list.cpp -o doubly_linked_list_test

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo "All compilations successful!"
    echo "Run the tests using:"
    echo "./vector_test"
    echo "./queue_test"
    echo "./stack_test"
    echo "./hashtable_test"
    echo "./doubly_linked_list_test"
else
    echo "Compilation failed!"
    exit 1
fi