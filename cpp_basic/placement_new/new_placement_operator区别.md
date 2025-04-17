### 10.1 new 相关操作对比

```mermaid
graph TD
    A[new 表达式] --> B[分配内存<br>operator new]
    B --> C[构造对象<br>placement new]
    
    D[内存管理方式] --> E[new]
    D --> F[operator new]
    D --> G[placement new]
    
    E -->|完整过程| H[1. 分配内存<br>2. 构造对象]
    F -->|仅分配| I[分配内存]
    G -->|仅构造| J[在已有内存上构造]
```

### 10.2 三种方式的使用场景

```mermaid
classDiagram
    class New {
        +分配内存
        +构造对象
        +一般使用
        Example: T* p = new T();
    }
    
    class OperatorNew {
        +仅分配内存
        +不调用构造函数
        +底层内存管理
        Example: void* p = operator new(sizeof(T));
    }
    
    class PlacementNew {
        +在已有内存上构造
        +不分配新内存
        +对象池优化
        Example: new (ptr) T();
    }
```

### 10.3 内存管理流程

```mermaid
sequenceDiagram
    participant New
    participant OperatorNew
    participant PlacementNew
    participant Memory
    
    New->>OperatorNew: 1. 调用分配内存
    OperatorNew->>Memory: 2. 分配内存块
    Memory-->>New: 3. 返回内存地址
    New->>PlacementNew: 4. 在内存上构造
    PlacementNew->>Memory: 5. 构造对象
```

### 10.4 主要特点对比：

1. **new 表达式**
   - 完整的对象创建过程
   - 自动处理内存分配和构造
   - 最常用的对象创建方式

2. **operator new**
   - 仅负责内存分配
   - 类似于 malloc
   - 可以重载以自定义内存分配策略

3. **placement new**
   - 在已分配的内存上构造对象
   - 不进行内存分配
   - 用于优化内存管理和对象池实现

### 10.5 使用示例：

```cpp
// new 表达式
T* p1 = new T();

// operator new
void* memory = operator new(sizeof(T));

// placement new
T* p2 = new (memory) T();
```

### 10.6 析构对应关系：

```mermaid
graph LR
    A[new] -->|对应| B[delete]
    C[operator new] -->|对应| D[operator delete]
    E[placement new] -->|对应| F[显式析构 + operator delete]
```