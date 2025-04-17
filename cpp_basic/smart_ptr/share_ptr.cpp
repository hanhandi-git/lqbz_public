#include <iostream>
#include <vector>
#include <unordered_set>

template <typename T>
class shared_ptr {
private:
    T* ptr;
    int* ref_count;
    void release() {
        if (ref_count) {
            --(*ref_count);
            if (*ref_count == 0) {
                delete ptr;
                delete ref_count;
            }
            ptr = nullptr;
            ref_count = nullptr;
        }
    }

public:
    shared_ptr() : ptr(nullptr), ref_count(nullptr) {}
    shared_ptr(T* p) : ptr(p), ref_count(new int(1)) {}
    shared_ptr(const shared_ptr& other) : ptr(other.ptr), ref_count(other.ref_count) {
        if (ref_count) {
            ++(*ref_count);
        }
    }
    ~shared_ptr() {
        release();
    }
    shared_ptr& operator=(const shared_ptr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            if (ref_count) {
                ++(*ref_count);
            }
        }
        return *this;
    }

    T* get() const {
        return ptr;
    }

    int use_count() const {
        return ref_count ? *ref_count : 0;
    }

    void reset() {
        release();
    }

    void reset(T* p) {
        release();
        ptr = p;
        ref_count = new int(1);
    }

    T& operator*() const {
        return *ptr;
    }

    T* operator->() const {
        return ptr;
    }
};