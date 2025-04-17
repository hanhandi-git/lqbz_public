cmake_minimum_required(VERSION 3.10)
project(CacheFriendlyExtension)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加编译选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")
endif()

# 添加include目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加可执行文件
add_executable(list_test tests/list_test.cpp)
add_executable(tree_test tests/tree_test.cpp)
add_executable(hash_test tests/hash_test.cpp)
add_executable(string_test tests/string_test.cpp)

# 链接必要的库
if(UNIX)
    target_link_libraries(list_test pthread)
    target_link_libraries(tree_test pthread)
    target_link_libraries(hash_test pthread)
    target_link_libraries(string_test pthread)
endif() 