cmake_minimum_required(VERSION 3.10)
project(CacheFriendlyTests)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加编译选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")
endif()

# 查找NUMA库
find_library(NUMA_LIBRARY numa)
if(NOT NUMA_LIBRARY)
    message(FATAL_ERROR "未找到NUMA库")
endif()

# 添加可执行文件
add_executable(memory_test memory_access_test.cpp)
add_executable(matrix_test matrix_multiply_test.cpp)
add_executable(struct_test data_structure_test.cpp)
add_executable(false_sharing_test false_sharing_test.cpp)
add_executable(numa_test numa_latency_test.cpp)
add_executable(numa_allocation_test numa_allocation_test.cpp)
add_executable(numa_affinity_test numa_affinity_test.cpp)

# 链接必要的库
if(UNIX)
    target_link_libraries(memory_test pthread)
    target_link_libraries(matrix_test pthread)
    target_link_libraries(struct_test pthread)
    target_link_libraries(false_sharing_test pthread)
    target_link_libraries(numa_test pthread ${NUMA_LIBRARY})
    target_link_libraries(numa_allocation_test pthread ${NUMA_LIBRARY})
    target_link_libraries(numa_affinity_test pthread ${NUMA_LIBRARY})
endif() 

