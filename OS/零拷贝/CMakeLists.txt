cmake_minimum_required(VERSION 3.10)
project(zero_copy_queue)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加源文件
add_library(queue_lib
    src/sendfile_queue.cpp
    src/mmap_queue.cpp
    src/splice_queue.cpp
)

# 添加性能测试可执行文件
add_executable(queue_test test/queue_test.cpp)
target_link_libraries(queue_test queue_lib)

# 添加持久化测试可执行文件
add_executable(persistence_writer test/persistence_writer.cpp)
target_link_libraries(persistence_writer queue_lib)

add_executable(persistence_reader test/persistence_reader.cpp)
target_link_libraries(persistence_reader queue_lib) 