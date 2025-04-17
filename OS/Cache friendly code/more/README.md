# 缓存友好的数据结构实现与性能测试

本项目实现了几种常见数据结构的缓存友好版本,并与标准库实现进行了性能对比。

## 实现概述

### 1. 哈希表实现 (CacheAwareHashTable)

缓存优化策略:
- 使用分块存储,每个块大小对齐到缓存行
- 批量查找时对键值进行排序以优化访问模式
- 使用预取指令提前加载数据

```mermaid
flowchart TD
    A[输入键值对] --> B{计算哈希值}
    B --> C[定位到对应块]
    C --> D{查找空槽位}
    D -->|找到| E[插入数据]
    D -->|未找到| F[创建新块]
    F --> D
```

### 2. 链表实现 (CacheAwareList)

缓存优化策略:
- 使用内存池管理节点
- 节点按块分配并对齐到缓存行
- 每块存储多个节点以提高空间局部性

```mermaid
flowchart TD
    A[新节点请求] --> B{内存池是否有空闲}
    B -->|是| C[从内存池分配]
    B -->|否| D[分配新的节点块]
    D --> E[初始化内存池]
    E --> C
    C --> F[返回节点]
```

### 3. 字符串处理 (CacheAwareStringMatcher)

缓存优化策略:
- 分块处理文本数据
- 使用局部变量存储模式串
- 预取下一个数据块
- 实现了KMP和Boyer-Moore算法的缓存优化版本

```mermaid
flowchart TD
    A[输入文本和模式串] --> B[分块读取文本]
    B --> C[预取下一块]
    C --> D[模式匹配]
    D --> E{是否匹配}
    E -->|是| F[记录位置]
    E -->|否| G[移动窗口]
    F --> G
    G --> B
```

### 4. B+树实现 (CacheAwareBPlusTree)

缓存优化策略:
- 节点大小对齐到缓存行
- 使用预取指令加载子节点
- 优化节点内数据布局

```mermaid
flowchart TD
    A[查找/插入] --> B[定位到根节点]
    B --> C[预取子节点]
    C --> D{是否叶子节点}
    D -->|否| E[二分查找子节点]
    D -->|是| F[在叶子节点中查找]
    E --> C
```

## 性能测试结果

| 数据结构 | 标准库实现 | 缓存优化版本 | 性能提升 |
|---------|-----------|------------|---------|
| 哈希表 | 21176ms | 8527ms | 2.48x |
| 链表 | 2633ms | 621ms | 4.24x |
| B+树 | 128126ms | 37509ms | 3.42x |

字符串匹配算法性能对比:

| 算法 | 耗时(ms) | vs std::search |
|------|----------|----------------|
| std::search | 38 | 1.00x |
| 优化KMP | 226 | 0.17x |
| 优化BM | 1 | 38.00x |

## 结论分析

1. 缓存友好的实现在大多数场景下都能带来显著的性能提升
2. 链表的优化效果最明显,主要得益于内存池和块式分配策略
3. B+树通过调整节点大小和预取获得了3.42倍的性能提升
4. Boyer-Moore算法的缓存优化版本性能最好,比标准搜索快38倍
5. 基数排序的缓存优化版本性能不理想,可能需要进一步改进

## 构建与运行

```bash
# 构建项目
./build.sh

# 运行测试
./build/hash_test
./build/list_test
./build/string_test
./build/tree_test
```

## 注意事项

1. 性能测试结果与硬件环境相关,不同平台可能有所差异
2. 缓存优化版本可能会消耗更多内存
3. 需要编译器支持预取指令(__builtin_prefetch)
4. 建议在开启优化选项(-O3)的情况下编译运行 

## 设计图解

### 类图

```mermaid
classDiagram
    class CacheAwareHashTable~K,V~ {
        -vector<Chunk*> buckets
        -size_t size_
        +insert(K key, V value)
        +find(K key, V& value)
        +batchLookup(vector<K> keys)
        +size() size_t
        -hash(K key) size_t
    }
    
    class CacheAwareStringMatcher {
        +findAll(string text, string pattern) vector<int>
        +findAllBM(string text, string pattern) vector<int>
        -computePrefixFunction(string pattern) vector<int>
        -preprocessBadCharacter(char* pattern, int size, array<int,256>& badChar)
    }
    
    class CacheAwareStringSorter {
        +radixSort(vector<string>& strings)
    }

    class Chunk {
        +array<K,8> keys
        +array<V,8> values
        +array<bool,8> occupied
        +Chunk* next
    }

    CacheAwareHashTable *-- Chunk
```

### 哈希表批量查找时序图

```mermaid
sequenceDiagram
    participant Client
    participant HashTable
    participant Chunk
    
    Client->>HashTable: batchLookup(keys)
    activate HashTable
    
    HashTable->>HashTable: 对keys计算哈希值并排序
    
    loop 对每个key
        HashTable->>Chunk: 查找key
        activate Chunk
        
        loop 遍历Chunk
            Chunk->>Chunk: 检查槽位
        end
        
        Chunk-->>HashTable: 返回查找结果
        deactivate Chunk
        
        HashTable->>HashTable: 保存结果
    end
    
    HashTable-->>Client: 返回results
    deactivate HashTable
```

### 字符串匹配流程图

```mermaid
flowchart TB
    Start[开始] --> Init[初始化结果集]
    Init --> PreProcess[预处理模式串]
    PreProcess --> ReadChunk[读取文本块]
    
    ReadChunk --> Prefetch[预取下一块]
    Prefetch --> Match{进行匹配}
    
    Match -->|匹配成功| Record[记录位置]
    Record --> NextPos[移动到下一位置]
    
    Match -->|不匹配| NextPos
    
    NextPos --> ChunkEnd{块结束?}
    ChunkEnd -->|否| Match
    ChunkEnd -->|是| TextEnd{文本结束?}
    
    TextEnd -->|否| ReadChunk
    TextEnd -->|是| End[结束]
```

### 缓存访问优化示意图

```mermaid
graph LR
    subgraph CPU
        L1[L1 Cache]
        L2[L2 Cache]
        L3[L3 Cache]
    end
    
    subgraph Memory
        M1[Chunk 1]
        M2[Chunk 2]
        M3[Chunk 3]
    end
    
    L1 --> L2
    L2 --> L3
    L3 --> Memory
    
    M1 -->|预取| L1
    M2 -->|预取| L1
    M3 -->|预取| L1
```

## 关键优化策略

### 1. 数据布局优化
```mermaid
graph TD
    A[数据结构] --> B[按缓存行对齐]
    A --> C[分块存储]
    A --> D[局部性优化]
    
    B --> E[减少伪共享]
    C --> F[提高缓存命中]
    D --> G[减少缓存失效]
```

### 2. 访问模式优化
```mermaid
graph TD
    A[访问优化] --> B[预取指令]
    A --> C[批量处理]
    A --> D[排序访问]
    
    B --> E[减少等待时间]
    C --> F[提高吞吐量]
    D --> G[优化内存访问]
``` 