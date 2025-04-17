# 如何通过ES实现分布式事务

## 1. ES 事务特性

### 1.1 基本特性
Elasticsearch 本身不支持传统意义上的 ACID 事务，但它提供了以下机制来保证数据一致性：

- 单文档操作的原子性
- 乐观并发控制
- 版本控制机制
- 写一致性控制

### 1.2 写一致性级别
ES提供了多种写一致性级别：

```json
PUT /orders/_doc/1?consistency=quorum
{
  "order_id": "1",
  "status": "pending"
}
```

- one：只要主分片可用就写入（默认）
- all：所有分片（主分片和副本分片）都可用才写入
- quorum：大多数分片可用才写入

## 2. 实现分布式事务的方案

### 2.1 乐观锁方案

#### 2.1.1 基本实现
利用 ES 的版本控制机制实现：

```json
// 第一步：获取文档
GET /orders/_doc/1

// 第二步：更新文档（带版本号）
PUT /orders/_doc/1?version=1&version_type=external
{
  "order_id": "1",
  "status": "completed",
  "update_time": "2024-03-20T10:00:00Z"
}
```

#### 2.1.2 处理并发冲突
```java
public boolean updateWithRetry(String orderId, OrderStatus newStatus, int maxRetries) {
    for (int i = 0; i < maxRetries; i++) {
        try {
            // 获取当前版本
            GetResponse response = client.get(new GetRequest("orders", orderId)).get();
            long version = response.getVersion();
            
            // 尝试更新
            UpdateRequest request = new UpdateRequest("orders", orderId)
                .version(version)
                .doc(jsonBuilder()
                    .startObject()
                        .field("status", newStatus.toString())
                        .field("update_time", new Date())
                    .endObject());
                    
            client.update(request).get();
            return true;
        } catch (VersionConflictEngineException e) {
            // 版本冲突，重试
            continue;
        }
    }
    return false;
}
```

### 2.2 Two-Phase Commit (2PC)

#### 2.2.1 完整实现流程

1. 准备阶段
```json
// 创建事务记录
PUT /transaction/_doc/tx_1
{
  "transaction_id": "tx_1",
  "status": "preparing",
  "participants": [
    {
      "service": "order_service",
      "status": "preparing",
      "operation": "create_order",
      "data": {
        "order_id": "order_1",
        "amount": 100
      }
    },
    {
      "service": "inventory_service",
      "status": "preparing",
      "operation": "reduce_stock",
      "data": {
        "product_id": "prod_1",
        "quantity": 1
      }
    }
  ],
  "start_time": "2024-03-20T10:00:00Z",
  "timeout": "30s"
}
```

2. 提交阶段
```json
// 更新事务状态为committing
PUT /transaction/_doc/tx_1/_update
{
  "doc": {
    "status": "committing",
    "participants": [
      {
        "service": "order_service",
        "status": "committed",
        "operation": "create_order"
      },
      {
        "service": "inventory_service",
        "status": "committed",
        "operation": "reduce_stock"
      }
    ],
    "commit_time": "2024-03-20T10:00:01Z"
  }
}
```

#### 2.2.2 超时处理
```java
@Scheduled(fixedRate = 10000)
public void checkTimeoutTransactions() {
    SearchRequest searchRequest = new SearchRequest("transaction");
    SearchSourceBuilder sourceBuilder = new SearchSourceBuilder();
    sourceBuilder.query(QueryBuilders.boolQuery()
        .must(QueryBuilders.termQuery("status", "preparing"))
        .must(QueryBuilders.rangeQuery("start_time")
            .lt("now-30s")));
    
    SearchResponse response = client.search(searchRequest);
    for (SearchHit hit : response.getHits()) {
        String txId = hit.getId();
        rollbackTransaction(txId);
    }
}
```

### 2.3 TCC (Try-Confirm-Cancel) 方案

#### 2.3.1 详细实现

1. Try 阶段：预留资源
```json
// 预留库存
POST /inventory/_doc
{
  "reservation_id": "res_1",
  "product_id": "prod_1",
  "quantity": 1,
  "status": "trying",
  "expire_time": "2024-03-20T10:30:00Z",
  "transaction_id": "tx_1"
}

// 创建订单预留
POST /orders/_doc
{
  "order_id": "order_1",
  "status": "trying",
  "products": [
    {
      "product_id": "prod_1",
      "quantity": 1,
      "reservation_id": "res_1"
    }
  ],
  "transaction_id": "tx_1",
  "expire_time": "2024-03-20T10:30:00Z"
}
```

2. Confirm 阶段：确认资源
```java
@Transactional
public void confirm(String transactionId) {
    // 1. 确认库存预留
    UpdateRequest inventoryUpdate = new UpdateRequest("inventory", "res_1")
        .doc(jsonBuilder()
            .startObject()
                .field("status", "confirmed")
            .endObject());
    client.update(inventoryUpdate);
    
    // 2. 确认订单
    UpdateRequest orderUpdate = new UpdateRequest("orders", "order_1")
        .doc(jsonBuilder()
            .startObject()
                .field("status", "confirmed")
            .endObject());
    client.update(orderUpdate);
}
```

3. Cancel 阶段：取消预留
```java
@Transactional
public void cancel(String transactionId) {
    // 1. 取消库存预留
    DeleteRequest inventoryDelete = new DeleteRequest("inventory", "res_1");
    client.delete(inventoryDelete);
    
    // 2. 取消订单
    UpdateRequest orderUpdate = new UpdateRequest("orders", "order_1")
        .doc(jsonBuilder()
            .startObject()
                .field("status", "cancelled")
            .endObject());
    client.update(orderUpdate);
}
```

#### 2.3.2 资源预留超时处理
```java
@Scheduled(fixedRate = 5000)
public void cleanupExpiredReservations() {
    // 查找过期的预留
    SearchRequest searchRequest = new SearchRequest("inventory");
    searchRequest.source(new SearchSourceBuilder()
        .query(QueryBuilders.boolQuery()
            .must(QueryBuilders.termQuery("status", "trying"))
            .must(QueryBuilders.rangeQuery("expire_time").lt("now"))));
    
    SearchResponse response = client.search(searchRequest);
    for (SearchHit hit : response.getHits()) {
        String reservationId = hit.getId();
        String transactionId = hit.getSourceAsMap().get("transaction_id").toString();
        cancel(transactionId);
    }
}
```

## 4. 事务监控与管理

### 4.1 事务状态监控
```json
GET /transaction/_search
{
  "size": 0,
  "aggs": {
    "status_stats": {
      "terms": {
        "field": "status"
      }
    },
    "avg_duration": {
      "avg": {
        "script": {
          "source": "if (doc['commit_time'] != null && doc['start_time'] != null) { return doc['commit_time'].value.toInstant().toEpochMilli() - doc['start_time'].value.toInstant().toEpochMilli() }"
        }
      }
    }
  }
}
```

### 4.2 性能监控
```json
GET /_nodes/stats/indices/translog
```

### 4.3 告警配置
```java
@Scheduled(fixedRate = 30000)
public void monitorTransactionHealth() {
    // 检查长时间未完成的事务
    SearchResponse response = client.search(new SearchRequest("transaction")
        .source(new SearchSourceBuilder()
            .query(QueryBuilders.boolQuery()
                .must(QueryBuilders.termQuery("status", "preparing"))
                .must(QueryBuilders.rangeQuery("start_time").lt("now-5m")))));
                
    if (response.getHits().getTotalHits().value > 0) {
        alertService.sendAlert("Found " + response.getHits().getTotalHits().value + " stuck transactions");
    }
}
```

## 5. 最佳实践建议

1. 事务边界控制
   - 控制事务粒度
   - 避免长事务
   - 设置合理的超时时间

2. 性能优化
   - 使用批量操作
   - 合理设置刷新间隔
   - 优化索引设计

3. 可靠性保证
   - 实现幂等性
   - 做好日志记录
   - 完善的补偿机制

4. 监控告警
   - 实时监控事务状态
   - 及时发现异常
   - 自动化处理

通过以上详细的实现方案和最佳实践，可以在 ES 中构建可靠的分布式事务系统。选择合适的方案并做好监控和维护，是确保系统稳定运行的关键。
