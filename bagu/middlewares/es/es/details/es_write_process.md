# Elasticsearch写入流程完整指南

## 目录
1. [写入基础架构](#1-写入基础架构)
2. [增加索引流程](#2-增加索引流程)
3. [更新索引流程](#3-更新索引流程)
4. [删除索引流程](#4-删除索引流程)
5. [写入性能优化](#5-写入性能优化)
6. [写入一致性](#6-写入一致性)
7. [故障处理](#7-故障处理)
8. [最佳实践](#8-最佳实践)

## 1. 写入基础架构

### 1.1 整体架构
`````mermaid



graph TD
    A[客户端请求] --> B[协调节点]
    B --> C[主分片节点]
    C --> D[副本分片节点]
    C --> E[translog]
    C --> F[内存缓冲区]
    F --> G[segment]
    G --> H[磁盘]
    E --> H
`````

### 1.2 写入组件
`````mermaid



mindmap
  root((写入组件))
    协调层
      路由计算
      请求转发
      结果聚合
    数据层
      内存缓冲
      translog
      segment文件
    存储层
      文件系统
      磁盘缓存
      持久化存储
`````

## 2. 增加索引流程

### 2.1 增加流程图
`````mermaid



sequenceDiagram
    participant Client
    participant Coordinator
    participant Primary
    participant Replica
    participant Disk
    
    Client->>Coordinator: 发送索引请求
    Coordinator->>Coordinator: 计算路由
    Coordinator->>Primary: 转发到主分片
    Primary->>Primary: 验证文档
    Primary->>Primary: 写入内存缓冲
    Primary->>Primary: 写入translog
    Primary->>Replica: 复制操作
    Replica->>Replica: 写入内存缓冲
    Replica->>Replica: 写入translog
    Replica-->>Primary: 确认写入
    Primary-->>Coordinator: 确认完成
    Coordinator-->>Client: 返回结果
`````

### 2.2 增加索引代码
`````java



class IndexOperation {
    // 单文档索引
    void indexDocument() {
        // 1. 创建索引请求
        IndexRequest request = new IndexRequest("my_index")
            .id("1")
            .source(XContentType.JSON,
                "field1", "value1",
                "field2", "value2",
                "timestamp", new Date()
            )
            // 设置写一致性
            .setRefreshPolicy(WriteRequest.RefreshPolicy.WAIT_UNTIL)
            .timeout(TimeValue.timeValueSeconds(1));
            
        // 2. 同步执行
        IndexResponse response = client.index(request, RequestOptions.DEFAULT);
        
        // 3. 异步执行
        client.indexAsync(request, RequestOptions.DEFAULT, new ActionListener<IndexResponse>() {
            @Override
            public void onResponse(IndexResponse response) {
                // 处理成功
            }
            
            @Override
            public void onFailure(Exception e) {
                // 处理失败
            }
        });
    }
    
    // 批量索引
    void bulkIndex() {
        BulkRequest bulkRequest = new BulkRequest();
        
        // 添加多个文档
        bulkRequest.add(new IndexRequest("my_index")
            .id("1")
            .source(XContentType.JSON, "field", "value1"));
            
        bulkRequest.add(new IndexRequest("my_index")
            .id("2")
            .source(XContentType.JSON, "field", "value2"));
            
        // 设置批量操作参数
        bulkRequest.timeout(TimeValue.timeValueMinutes(2));
        bulkRequest.setRefreshPolicy(WriteRequest.RefreshPolicy.WAIT_UNTIL);
        
        // 执行批量操作
        BulkResponse bulkResponse = client.bulk(bulkRequest, RequestOptions.DEFAULT);
    }
}
`````

## 3. 更新索引流程

### 3.1 更新流程图
`````mermaid



graph TD
    A[更新请求] --> B[协调节点]
    B --> C[计算路由]
    C --> D[主分片]
    D --> E{文档存在?}
    E -->|是| F[读取当前版本]
    E -->|否| G[创建新文档]
    F --> H[合并更新]
    H --> I[写入新版本]
    I --> J[更新translog]
    J --> K[复制到副本]
    K --> L[响应客户端]
`````

### 3.2 更新索引代码
`````java



class UpdateOperation {
    // 单文档更新
    void updateDocument() {
        // 1. 脚本更新
        UpdateRequest scriptUpdate = new UpdateRequest("my_index", "1")
            .script(new Script(ScriptType.INLINE, "painless",
                "ctx._source.counter += params.count",
                Collections.singletonMap("count", 1)))
            .retryOnConflict(3);
            
        // 2. 部分文档更新
        UpdateRequest docUpdate = new UpdateRequest("my_index", "1")
            .doc(XContentType.JSON,
                "updated_field", "new_value",
                "update_time", new Date()
            )
            .docAsUpsert(true);
            
        // 3. 更新后获取
        UpdateRequest getUpdate = new UpdateRequest("my_index", "1")
            .doc("field", "value")
            .fetchSource(true);
            
        // 4. 执行更新
        UpdateResponse response = client.update(docUpdate, RequestOptions.DEFAULT);
    }
    
    // 按查询更新
    void updateByQuery() {
        UpdateByQueryRequest request = new UpdateByQueryRequest("my_index");
        
        // 设置查询条件
        request.setQuery(new TermQueryBuilder("status", "pending"));
        
        // 设置更新脚本
        request.setScript(
            new Script(
                ScriptType.INLINE,
                "painless",
                "ctx._source.status = 'processed'",
                Collections.emptyMap()
            )
        );
        
        // 设置并行
        request.setSlices(2);
        request.setBatchSize(1000);
        
        // 执行更新
        BulkByScrollResponse response = client.updateByQuery(
            request, RequestOptions.DEFAULT);
    }
}
`````

## 4. 删除索引流程

### 4.1 删除流程图
`````mermaid



graph TD
    A[删除请求] --> B[协调节点]
    B --> C[计算路由]
    C --> D[主分片]
    D --> E{文档存在?}
    E -->|是| F[标记删除]
    E -->|否| G[返回未找到]
    F --> H[更新translog]
    H --> I[复制到副本]
    I --> J[响应客户端]
    F --> K[后台清理]
`````

### 4.2 删除索引代码
`````java



class DeleteOperation {
    // 单文档删除
    void deleteDocument() {
        // 1. 创建删除请求
        DeleteRequest request = new DeleteRequest("my_index", "1")
            .timeout(TimeValue.timeValueSeconds(2));
            
        // 2. 执行删除
        DeleteResponse response = client.delete(request, RequestOptions.DEFAULT);
    }
    
    // 按查询删除
    void deleteByQuery() {
        DeleteByQueryRequest request = new DeleteByQueryRequest("my_index");
        
        // 设置查询条件
        request.setQuery(new RangeQueryBuilder("timestamp")
            .lt("now-7d"));
            
        // 设置并行
        request.setSlices(2);
        request.setBatchSize(1000);
        
        // 执行删除
        BulkByScrollResponse response = client.deleteByQuery(
            request, RequestOptions.DEFAULT);
    }
    
    // 删除索引
    void deleteIndex() {
        DeleteIndexRequest request = new DeleteIndexRequest("my_index");
        AcknowledgedResponse response = client.indices().delete(
            request, RequestOptions.DEFAULT);
    }
}
`````

## 5. 写入性能优化

### 5.1 性能优化策略
`````mermaid



mindmap
  root((性能优化))
    批量写入
      批量大小
      并行度
      重试策略
    缓冲设置
      刷新间隔
      缓冲大小
      合并策略
    事务日志
      持久化策略
      同步策略
      清理策略
`````

### 5.2 优化配置代码
`````java



class WriteOptimization {
    // 优化写入设置
    void optimizeWriteSettings() {
        // 1. 索引设置
        UpdateSettingsRequest settingsRequest = new UpdateSettingsRequest("my_index")
            .settings(Settings.builder()
                // 刷新间隔
                .put("index.refresh_interval", "30s")
                // 事务日志
                .put("index.translog.durability", "async")
                .put("index.translog.sync_interval", "5s")
                // 缓冲设置
                .put("index.indexing.slowlog.threshold.index.warn", "10s")
                .put("index.indexing.slowlog.threshold.index.info", "5s")
                // 合并设置
                .put("index.merge.scheduler.max_thread_count", 1)
                .put("index.merge.policy.max_merged_segment", "2gb")
            );
            
        // 2. 批量处理设置
        BulkProcessor.Builder builder = BulkProcessor.builder(
            (request, bulkListener) ->
                client.bulkAsync(request, RequestOptions.DEFAULT, bulkListener),
            new BulkProcessor.Listener() {
                @Override
                public void beforeBulk(long executionId, BulkRequest request) {
                    // 批量处理前
                }
                
                @Override
                public void afterBulk(long executionId, BulkRequest request,
                                    BulkResponse response) {
                    // 批量处理后
                }
                
                @Override
                public void afterBulk(long executionId, BulkRequest request,
                                    Throwable failure) {
                    // 处理失败
                }
            });
            
        // 配置批量处理器
        builder.setBulkActions(10000)
            .setBulkSize(new ByteSizeValue(5, ByteSizeUnit.MB))
            .setFlushInterval(TimeValue.timeValueSeconds(5))
            .setConcurrentRequests(2)
            .setBackoffPolicy(BackoffPolicy.exponentialBackoff(
                TimeValue.timeValueMillis(100), 3));
    }
}
`````

## 6. 写入一致性

### 6.1 一致性模型
`````mermaid



graph LR
    A[写入请求] --> B{一致性级别}
    B -->|one| C[主分片确认]
    B -->|all| D[所有分片确认]
    B -->|quorum| E[多数分片确认]
    
    C --> F[快速响应]
    D --> G[强一致性]
    E --> H[折中方案]
`````

### 6.2 一致性代码
`````java



class ConsistencyControl {
    // 一致性控制
    void consistencySettings() {
        // 1. 索引请求一致性
        IndexRequest indexRequest = new IndexRequest("my_index")
            .source("field", "value")
            // 等待主分片和副本分片
            .setWaitForActiveShards(2)
            // 写入策略
            .setRefreshPolicy(WriteRequest.RefreshPolicy.WAIT_UNTIL);
            
        // 2. 批量请求一致性
        BulkRequest bulkRequest = new BulkRequest()
            .add(new IndexRequest("my_index")
                .source("field", "value1"))
            .add(new IndexRequest("my_index")
                .source("field", "value2"))
            .setRefreshPolicy(WriteRequest.RefreshPolicy.WAIT_UNTIL)
            .timeout(TimeValue.timeValueMinutes(2));
    }
}
`````

## 7. 故障处理

### 7.1 故障处理流程
`````mermaid



graph TD
    A[写入失败] --> B{失败类型}
    B -->|超时| C[重试策略]
    B -->|节点失败| D[故障转移]
    B -->|资源不足| E[资源调整]
    
    C --> F[指数退避]
    D --> G[选择新节点]
    E --> H[优化设置]
`````

### 7.2 故障处理代码
`````java



class FailureHandling {
    // 故障处理
    void handleFailures() {
        // 1. 重试策略
        IndexRequest request = new IndexRequest("my_index")
            .source("field", "value")
            .timeout(TimeValue.timeValueSeconds(1))
            .retryOnConflict(3);
            
        // 2. 批量错误处理
        BulkRequest bulkRequest = new BulkRequest()
            .add(new IndexRequest("my_index").source("field", "value"))
            .timeout(TimeValue.timeValueMinutes(2));
            
        BulkResponse bulkResponse = client.bulk(bulkRequest, RequestOptions.DEFAULT);
        
        if (bulkResponse.hasFailures()) {
            for (BulkItemResponse bulkItemResponse : bulkResponse) {
                if (bulkItemResponse.isFailed()) {
                    BulkItemResponse.Failure failure = bulkItemResponse.getFailure();
                    // 处理失败
                }
            }
        }
    }
}
`````

## 8. 最佳实践

### 8.1 写入建议
1. **批量处理**
   - 使用批量API
   - 合理的批量大小
   - 并行处理

2. **性能优化**
   - 调整刷新间隔
   - 异步translog
   - 合理分片

3. **错误处理**
   - 重试机制
   - 错误记录
   - 监控告警

### 8.2 监控命令
`````bash



# 1. 查看写入状态
GET _cat/indices?v

# 2. 查看分片状态
GET _cat/shards?v

# 3. 查看段信息
GET _cat/segments?v

# 4. 查看写入性能
GET _nodes/stats/indices/indexing

# 5. 查看translog状态
GET _cat/recovery?v
`````

## 结语

本文档详细介绍了ES的写入流程，包括增加、更新和删除索引的详细过程，通过流程图、代码示例和实际命令全面展示了相关概念和实践方法。理解这些知识对于优化ES的写入性能至关重要。