1. 使用folly/boost的数据结构和工具:
- folly::MPMCQueue 用于阶段间通信
- folly::Future/Promise 用于异步任务
- folly::ThreadPoolExecutor 用于线程池管理
- boost::circular_buffer 用于batch缓存
- boost::lockfree::queue 用于无锁队列

2. Batch处理的设计:
- 需要一个batch collector来攒批
- 支持基于时间和数量的batch策略
- batch处理完成后需要拆分结果

3. 关键组件:
- BatchCollector: 负责攒批
- BatchProcessor: 处理一个batch
- Stage: 流水线的处理阶段
- Pipeline: 整体流程控制

4. 批处理策略:
- 基于数量(达到N个任务)
- 基于时间(超过T时间)
- 混合策略(先到者触发)


# UML 类图
```mermaid
classDiagram
    class Pipeline {
        -vector~Stage~ stages
        -ThreadPoolExecutor executor
        +addStage(Stage stage)
        +start()
        +stop()
        +submit(Task task)
    }
    
    class Stage {
        -string name
        -BatchCollector collector
        -BatchProcessor processor
        -MPMCQueue inputQueue
        -MPMCQueue outputQueue
        +process()
        +start()
        +stop()
    }
    
    class BatchCollector {
        -circular_buffer buffer
        -BatchStrategy strategy
        +collect(Task task)
        +isBatchReady()
        +getBatch()
    }
    
    class BatchProcessor {
        -function processor
        +process(Batch batch)
        +split(BatchResult result)
    }
    
    class BatchStrategy {
        -size_t maxSize
        -duration maxTime
        +shouldTrigger()
    }
    
    Pipeline --> Stage
    Stage --> BatchCollector
    Stage --> BatchProcessor
    BatchCollector --> BatchStrategy
```

```mermaid
graph TD
    A[提交任务] --> B[Stage收集任务]
    B --> C{是否满足batch条件?}
    C -- 否 --> B
    C -- 是 --> D[批量处理]
    D --> E[拆分结果]
    E --> F[传递到下一阶段]
```


```mermaid
sequenceDiagram
    participant Client
    participant Stage
    participant BatchCollector
    participant BatchProcessor
    
    Client->>Stage: submit(task)
    Stage->>BatchCollector: collect(task)
    BatchCollector->>BatchCollector: check batch condition
    
    alt batch ready
        BatchCollector->>BatchProcessor: process(batch)
        BatchProcessor->>BatchProcessor: split results
        BatchProcessor->>Stage: push results
    else not ready
        BatchCollector->>BatchCollector: store task
    end
```