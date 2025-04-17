<think>
我需要详细解释什么时候应该使用 unique_ptr 而不是 shared_ptr，这是一个关于所有权语义和性能优化的重要问题。

unique_ptr 和 shared_ptr 的主要区别：
1. unique_ptr - 独占所有权，不可共享，轻量级
2. shared_ptr - 共享所有权，引用计数，开销较大

我将从以下几个角度来分析：
1. 独占所有权 vs 共享所有权
2. 性能考虑
3. 明确所有权语义
4. 常见使用场景
5. 实际代码示例

我会着重强调 unique_ptr 的优势，并提供具体的场景说明何时选择它而非 shared_ptr。
</think>

# 何时使用 unique_ptr 而非 shared_ptr

## 基本区别

`unique_ptr` 和 `shared_ptr` 有着根本的所有权语义差异：
- `unique_ptr`：**独占所有权**，一个对象只能有一个所有者
- `shared_ptr`：**共享所有权**，多个指针可以共同拥有同一个对象

## 使用 unique_ptr 的场景及理由

### 1. 明确的独占所有权关系

**何时使用**：
- 当资源只需要一个明确的所有者时
- 只有一个类/函数应该负责管理该资源的生命周期

**理由**：
- 清晰表达了"这个资源只属于我"的设计意图
- 防止资源意外共享导致的复杂依赖
- 避免不必要的引用计数开销

````cpp
// 文件处理器 - 文件句柄通常只需要一个所有者
class FileHandler {
private:
    std::unique_ptr<File> file; // 明确表示此类独占文件资源
};
````

### 2. 性能敏感场景

**何时使用**：
- 高频分配/释放对象的场景
- 性能关键路径
- 内存受限环境

**理由**：
- `unique_ptr` 几乎没有运行时开销，大小与原始指针相同
- 避免了 `shared_ptr` 的原子引用计数操作
- 没有额外的控制块内存分配

````cpp
// 游戏引擎中的粒子系统 - 可能有成千上万个粒子对象
for (int i = 0; i < 10000; ++i) {
    particles.push_back(std::unique_ptr<Particle>(new Particle()));
    // 比 shared_ptr 更高效，没有引用计数开销
}
````

### 3. 工厂函数返回值

**何时使用**：
- 函数创建并返回对象
- 调用者将成为返回对象的唯一所有者

**理由**：
- 明确转移所有权语义
- 强制调用者决定如何管理对象生命周期
- 避免不必要的引用计数

````cpp
std::unique_ptr<Widget> createWidget(int param) {
    return std::make_unique<Widget>(param);
    // 清晰表达"我创建了这个对象，但我不保留所有权"
}

// 调用者可以决定如何管理：
auto w = createWidget(42); // 接管所有权
processWidget(std::move(w)); // 转移所有权
````

### 4. 明确专属容器关系

**何时使用**：
- 容器需要独占其元素
- 元素不会被共享到容器外

**理由**：
- 表达"这些元素专属于此容器"的设计意图
- 容器销毁时自动清理所有元素

````cpp
class CompositeObject {
private:
    std::vector<std::unique_ptr<Component>> components;
    // 明确表示这些组件只属于此复合对象
};
````

### 5. 实现 PIMPL 模式

**何时使用**：
- 使用 Pointer to Implementation 模式封装实现细节
- 实现类通常只被一个接口类拥有

**理由**：
- 明确的一对一关系
- 避免不必要的引用计数
- 降低编译依赖

````cpp
// 头文件
class Engine {
public:
    Engine();
    ~Engine();
    // 其他公共API
private:
    class Impl;
    std::unique_ptr<Impl> pImpl; // 私有实现，只被此类拥有
};

// 实现文件
class Engine::Impl {
    // 实现细节
};

Engine::Engine() : pImpl(std::make_unique<Impl>()) {}
Engine::~Engine() = default; // unique_ptr自动清理Impl
````

### 6. 防止循环引用

**何时使用**：
- 对象之间有依赖关系，但不需要双向共享所有权
- 明确的父子或所有者被拥有者关系

**理由**：
- 避免 `shared_ptr` 可能导致的循环引用
- 更明确地表达所有权关系

````cpp
class Parent {
private:
    std::vector<std::unique_ptr<Child>> children;
    // 父对象明确拥有子对象，不允许子对象共享
};

class Child {
private:
    Parent* parent; // 原始指针，不参与所有权管理
};
````

## unique_ptr vs shared_ptr 决策流程

使用以下问题来判断：

1. 这个对象在其生命周期内需要多个所有者吗？
   - 是 → 使用 `shared_ptr`
   - 否 → 考虑使用 `unique_ptr`

2. 所有权需要在多个对象之间转移吗？
   - 是 → 使用 `unique_ptr` + `std::move`
   - 否，始终单一所有者 → 使用 `unique_ptr`
   - 否，需要多个同时拥有 → 使用 `shared_ptr`

3. 性能是关键因素吗？
   - 是，且不需要共享所有权 → 坚决使用 `unique_ptr`
   - 是，但需要共享所有权 → 权衡是否真的需要 `shared_ptr`

4. 代码表达的是什么所有权语义？
   - "我独自拥有这个资源" → `unique_ptr`
   - "我们共同拥有这个资源" → `shared_ptr`
   - "我只是使用这个资源" → 原始指针或引用

## 结论

`unique_ptr` 不仅是一个内存管理工具，更是一种设计工具，它帮助你清晰表达独占所有权意图。当资源只需要一个明确的所有者时，`unique_ptr` 应该是首选，只有在真正需要共享所有权时才使用 `shared_ptr`。

记住经典准则：**优先选择 unique_ptr，只在必要时才使用 shared_ptr**。这不仅能提高性能，更能创建更清晰、更易于理解和维护的代码。
