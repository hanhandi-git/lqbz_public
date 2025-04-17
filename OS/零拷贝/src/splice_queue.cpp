#include "splice_queue.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdexcept>
#include <cstring>

SpliceQueue::SpliceQueue(const std::string& data_file, size_t max_size)
    : write_pos_(0), read_pos_(0), max_size_(max_size) {
    // 打开或创建数据文件
    data_fd_ = open(data_file.c_str(), O_RDWR | O_CREAT, 0644);
    if (data_fd_ < 0) {
        throw std::runtime_error("Failed to open data file");
    }
    
    // 设置文件大小
    if (ftruncate(data_fd_, max_size) < 0) {
        close(data_fd_);
        throw std::runtime_error("Failed to set file size");
    }

    // 创建管道
    if (pipe(pipe_fds_) < 0) {
        close(data_fd_);
        throw std::runtime_error("Failed to create pipe");
    }
}

SpliceQueue::~SpliceQueue() {
    if (data_fd_ >= 0) {
        close(data_fd_);
    }
    close(pipe_fds_[0]);
    close(pipe_fds_[1]);
}

bool SpliceQueue::enqueue(const char* data, size_t size) {
    if (write_pos_ + size > max_size_) {
        return false;
    }

    const size_t PIPE_BUF_SIZE = 65536; // 64KB批次
    size_t remaining = size;
    size_t offset = 0;

    while (remaining > 0) {
        // 计算这次要处理的数据量
        size_t batch_size = std::min(remaining, PIPE_BUF_SIZE);

        // 写入管道
        ssize_t written = write(pipe_fds_[1], data + offset, batch_size);
        if (written < 0) {
            return false;
        }

        // 使用splice从管道写入文件
        loff_t file_offset = write_pos_ + offset;
        ssize_t spliced = splice(pipe_fds_[0], nullptr, 
            data_fd_, &file_offset, written, SPLICE_F_MOVE);
        
        if (spliced < 0) {
            return false;
        }

        remaining -= spliced;
        offset += spliced;
    }

    write_pos_ += size;
    return true;
}

ssize_t SpliceQueue::dequeue(char* buffer, size_t size) {
    if (read_pos_ >= write_pos_) {
        return 0;
    }

    // 计算可读取的数据量
    size_t available = write_pos_ - read_pos_;
    size_t to_read = std::min(size, available);
    size_t total_read = 0;
    
    const size_t PIPE_BUF_SIZE = 65536; // 64KB批次
    
    while (total_read < to_read) {
        size_t remaining = to_read - total_read;
        size_t batch_size = std::min(remaining, PIPE_BUF_SIZE);

        // 使用splice从文件读取到管道
        loff_t offset = read_pos_ + total_read;
        ssize_t spliced = splice(data_fd_, &offset,
            pipe_fds_[1], nullptr, batch_size, SPLICE_F_MOVE);
        
        if (spliced < 0) {
            return -1;
        }

        // 从管道读取到buffer
        ssize_t read_bytes = read(pipe_fds_[0], buffer + total_read, spliced);
        if (read_bytes < 0) {
            return -1;
        }

        total_read += read_bytes;
    }

    read_pos_ += total_read;
    return total_read;
} 