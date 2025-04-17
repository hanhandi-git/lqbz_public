#include "mmap_queue.hpp"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdexcept>
#include <cstring>

MmapQueue::MmapQueue(const std::string& data_file, size_t max_size)
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

    // 映射文件到内存
    buffer_ = static_cast<char*>(mmap(nullptr, max_size, 
        PROT_READ | PROT_WRITE, MAP_SHARED, data_fd_, 0));
    
    if (buffer_ == MAP_FAILED) {
        close(data_fd_);
        throw std::runtime_error("Failed to mmap file");
    }
}

MmapQueue::~MmapQueue() {
    if (buffer_ != MAP_FAILED) {
        munmap(buffer_, max_size_);
    }
    if (data_fd_ >= 0) {
        close(data_fd_);
    }
}

bool MmapQueue::enqueue(const char* data, size_t size) {
    if (write_pos_ + size > max_size_) {
        return false;
    }

    // 直接复制到映射内存
    memcpy(buffer_ + write_pos_, data, size);
    write_pos_ += size;
    return true;
}

ssize_t MmapQueue::dequeue(char* buffer, size_t size) {
    if (read_pos_ >= write_pos_) {
        return 0;
    }

    // 计算可读取的数据量
    size_t available = write_pos_ - read_pos_;
    size_t to_read = std::min(size, available);

    // 从映射内存复制到buffer
    memcpy(buffer, buffer_ + read_pos_, to_read);
    read_pos_ += to_read;
    
    return to_read;
} 