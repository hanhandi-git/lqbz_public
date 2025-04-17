// batch.h
#pragma once
#include <boost/circular_buffer.hpp>
#include <vector>
#include "task.h"

class Batch {
public:
    explicit Batch(size_t size) : tasks_(size) {}
    
    void addTask(Task task) {
        tasks_.push_back(std::move(task));
    }
    
    const boost::circular_buffer<Task>& getTasks() const { return tasks_; }
    
    size_t size() const { return tasks_.size(); }

private:
    boost::circular_buffer<Task> tasks_;
};