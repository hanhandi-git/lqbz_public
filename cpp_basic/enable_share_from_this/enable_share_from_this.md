# std::enable_shared_from_this 使用指南

## 1. 基本概念

`std::enable_shared_from_this` 是一个模板基类，它允许你从类的成员函数中安全地获取 `this` 指针对应的 `shared_ptr`。当一个类继承自 `enable_shared_from_this` 时，它可以通过调用 `shared_from_this()` 来获取指向自身的 `shared_ptr`。

## 2. 使用场景

主要用于以下场景：
- 在类的成员函数中需要返回 this 的 shared_ptr
- 异步操作中需要传递 this 对象的 shared_ptr
- 需要确保对象生命周期的回调函数中

## 3. 基本语法

```cpp
#include <memory>

class MyClass : public std::enable_shared_from_this<MyClass> {
public:
    std::shared_ptr<MyClass> getShared() {
        return shared_from_this();
    }
};
```

## 4. 使用示例

### 4.1 异步回调示例

```cpp
class AsyncTask : public std::enable_shared_from_this<AsyncTask> {
public:
    void start() {
        // 传递 shared_ptr 给异步操作
        asyncOperation(shared_from_this());
    }
    
private:
    void asyncOperation(std::shared_ptr<AsyncTask> self) {
        // 异步操作...
    }
};

// 正确的使用方式
void usage() {
    auto task = std::make_shared<AsyncTask>();
    task->start();
}
```

### 4.2 错误使用示例

```cpp
class Wrong {
    // 错误：直接在栈上创建对象
    Wrong w;
    auto ptr = w.shared_from_this(); // 将抛出 bad_weak_ptr 异常
    
    // 错误：在构造函数中调用
    Wrong() {
        auto ptr = shared_from_this(); // 将抛出 bad_weak_ptr 异常
    }
};
```

## 5. 注意事项

1. 必须先创建对象的 shared_ptr，才能调用 shared_from_this()
```cpp
// 正确方式
auto ptr = std::make_shared<MyClass>();
auto same_ptr = ptr->getShared();

// 错误方式
MyClass obj;
auto ptr = obj.getShared(); // 将抛出异常
```

2. 不要在构造函数中调用 shared_from_this()

3. 避免循环引用
```cpp
class Parent : public std::enable_shared_from_this<Parent> {
    std::shared_ptr<Child> child;
};

class Child {
    std::weak_ptr<Parent> parent; // 使用 weak_ptr 避免循环引用
};
```

4. enable_shared_from_this 是一个公有继承的基类

## 6. 实现原理

enable_shared_from_this 内部维护了一个 weak_ptr，当创建类对象的 shared_ptr 时，这个 weak_ptr 会自动绑定。调用 shared_from_this() 时，实际上是通过这个 weak_ptr 创建新的 shared_ptr。

## 7. 最佳实践

1. 总是使用 make_shared 创建对象
2. 如果类需要共享 this 指针，就继承 enable_shared_from_this
3. 在异步操作中传递 this 时，使用 shared_from_this()
4. 注意避免循环引用，适当使用 weak_ptr