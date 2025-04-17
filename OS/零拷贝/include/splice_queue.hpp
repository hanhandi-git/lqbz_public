#pragma once
#include "queue_base.hpp"
#include <string>

class SpliceQueue : public QueueBase {
public:
    SpliceQueue(const std::string& data_file, size_t max_size);
    ~SpliceQueue();

    bool enqueue(const char* data, size_t size) override;
    ssize_t dequeue(char* buffer, size_t size) override;

private:
    int data_fd_;        // 数据文件描述符
    int pipe_fds_[2];    // 管道文件描述符
    size_t write_pos_;   // 写入位置
    size_t read_pos_;    // 读取位置
    size_t max_size_;    // 最大容量
}; 