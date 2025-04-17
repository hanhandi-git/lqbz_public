#include <memory>

template<typename T>
class my_enable_shared_from_this {
protected:
    // 构造和析构函数
    my_enable_shared_from_this() noexcept = default;
    my_enable_shared_from_this(const my_enable_shared_from_this&) noexcept = default;
    my_enable_shared_from_this& operator=(const my_enable_shared_from_this&) noexcept = default;
    virtual ~my_enable_shared_from_this() = default;

public:
    // 获取shared_ptr
    std::shared_ptr<T> shared_from_this() {
        std::shared_ptr<T> p = weak_this.lock();
        if (!p) throw std::bad_weak_ptr();
        return p;
    }

    // const版本
    std::shared_ptr<const T> shared_from_this() const {
        std::shared_ptr<const T> p = weak_this.lock();
        if (!p) throw std::bad_weak_ptr();
        return p;
    }

protected:
    // 获取weak_ptr
    std::weak_ptr<T> weak_from_this() noexcept {
        return weak_this;
    }
    
    std::weak_ptr<const T> weak_from_this() const noexcept {
        return weak_this;
    }

private:
    // 用于存储this指针对应的控制块
    mutable std::weak_ptr<T> weak_this;
    
    // 允许shared_ptr访问weak_this
    template<typename U>
    friend class std::shared_ptr;
}; 