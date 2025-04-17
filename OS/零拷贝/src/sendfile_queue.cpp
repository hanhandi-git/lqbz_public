#include "sendfile_queue.hpp"
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdexcept>

SendfileQueue::SendfileQueue(const std::string& data_file, size_t max_size)
    : write_pos_(0), read_pos_(0), max_size_(max_size) {
    // 打开或创建数据文件
    data_fd_ = open(data_file.c_str(), O_RDWR | O_CREAT, 0644);
    if (data_fd_ < 0) {
        throw std::runtime_error("Failed to open data file");
    }
    
    // 设置文件大小
    ftruncate(data_fd_, max_size);
}

SendfileQueue::~SendfileQueue() {
    if (data_fd_ >= 0) {
        close(data_fd_);
    }
}

bool SendfileQueue::enqueue(const char* data, size_t size) {
    if (write_pos_ + size > max_size_) {
        return false;
    }

    ssize_t written = write(data_fd_, data, size);
    if (written < 0) {
        return false;
    }

    write_pos_ += written;
    return true;
}

ssize_t SendfileQueue::dequeue(char* buffer, size_t size) {
    if (read_pos_ >= write_pos_) {
        return 0;
    }

    // 创建临时文件用于sendfile
    char temp_path[] = "/tmp/queue_XXXXXX";
    int temp_fd = mkstemp(temp_path);
    if (temp_fd < 0) {
        return -1;
    }

    // 使用sendfile将数据从数据文件复制到临时文件
    off_t offset = read_pos_; // 创建off_t类型的偏移量
    ssize_t sent = sendfile(temp_fd, data_fd_, &offset, size);
    if (sent < 0) {
        close(temp_fd);
        unlink(temp_path);
        return -1;
    }

    // 更新读取位置
    read_pos_ = offset;

    // 从临时文件读取数据到buffer
    lseek(temp_fd, 0, SEEK_SET);
    ssize_t read_bytes = read(temp_fd, buffer, sent);

    close(temp_fd);
    unlink(temp_path);
    
    return read_bytes;
} 