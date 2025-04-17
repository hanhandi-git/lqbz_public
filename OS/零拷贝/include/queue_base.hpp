#pragma once
#include <string>
#include <cstddef>

class QueueBase {
public:
    /**
     * @brief 虚析构函数
     */
    virtual ~QueueBase() = default;

    /**
     * @brief 将消息写入队列
     * @param data 要写入的数据
     * @param size 数据大小
     * @return 是否写入成功
     */
    virtual bool enqueue(const char* data, size_t size) = 0;

    /**
     * @brief 从队列读取消息
     * @param buffer 接收数据的缓冲区
     * @param size 缓冲区大小
     * @return 实际读取的字节数，失败返回-1
     */
    virtual ssize_t dequeue(char* buffer, size_t size) = 0;
}; 