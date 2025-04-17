#pragma once
#include "queue_base.hpp"
#include <string>
#include <memory>

class SendfileQueue : public QueueBase {
public:
    /**
     * @brief 构造函数
     * @param data_file 数据文件路径
     * @param max_size 队列最大大小
     */
    SendfileQueue(const std::string& data_file, size_t max_size);
    ~SendfileQueue();

    bool enqueue(const char* data, size_t size) override;
    ssize_t dequeue(char* buffer, size_t size) override;

private:
    int data_fd_;        // 数据文件描述符
    size_t write_pos_;   // 写入位置
    size_t read_pos_;    // 读取位置
    size_t max_size_;    // 最大容量
}; 