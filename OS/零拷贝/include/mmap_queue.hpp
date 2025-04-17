#pragma once
#include "queue_base.hpp"
#include <string>
#include <cstddef>
#include <sys/types.h>

class MmapQueue : public QueueBase {
public:
    MmapQueue(const std::string& data_file, size_t max_size);
    ~MmapQueue();

    bool enqueue(const char* data, size_t size) override;
    ssize_t dequeue(char* buffer, size_t size) override;

private:
    int data_fd_;        // 数据文件描述符
    char* buffer_;       // 内存映射指针
    size_t write_pos_;   // 写入位置
    size_t read_pos_;    // 读取位置
    size_t max_size_;    // 最大容量
}; 