# Elasticsearch查询与聚合性能优化指南

## 目录
1. [查询基础架构](#1-查询基础架构)
2. [查询性能优化](#2-查询性能优化)
3. [聚合基础架构](#3-聚合基础架构)
4. [聚合性能优化](#4-聚合性能优化)
5. [缓存机制](#5-缓存机制)
6. [性能监控](#6-性能监控)
7. [最佳实践](#7-最佳实践)

## 1. 查询基础架构

### 1.1 查询执行流程
`````mermaid
graph TD
    A[查询请求] --> B[协调节点]
    B --> C[解析查询]
    C --> D[重写查询]
    D --> E[计算分片]
    E --> F[分发查询]
    F --> G[执行查询]
    G --> H[合并结果]
    H --> I[返回结果]
`````

### 1.2 查询类型分类
`````mermaid
mindmap
  root((查询类型))
    叶子查询
      term
      terms
      range
      prefix
    复合查询
      bool
      dis_max
      function_score
    全文查询
      match
      match_phrase
      query_string
    地理查询
      geo_point
      geo_shape
`````

### 1.3 查询构建代码
`````java
class QueryBuilder {
    // 构建Bool查询
    BoolQueryBuilder buildBoolQuery() {
        return QueryBuilders.boolQuery()
            .must(QueryBuilders.matchQuery("title", "elasticsearch"))
            .filter(QueryBuilders.termQuery("status", "active"))
            .should(QueryBuilders.rangeQuery("price").gte(100).lte(200))
            .minimumShouldMatch(1);
    }
    
    // 构建复合查询
    DisMaxQueryBuilder buildDisMaxQuery() {
        return QueryBuilders.disMaxQuery()
            .add(QueryBuilders.termQuery("name", "john"))
            .add(QueryBuilders.termQuery("name.keyword", "john"))
            .tieBreaker(0.7f);
    }
    
    // 构建函数评分查询
    FunctionScoreQueryBuilder buildFunctionScoreQuery() {
        return QueryBuilders.functionScoreQuery(
            QueryBuilders.matchQuery("description", "apple"),
            ScoreFunctionBuilders.exponentialDecayFunction("price", 100, 200)
        );
    }
}
`````

## 2. 查询性能优化

### 2.1 查询优化流程
`````mermaid
graph LR
    A[原始查询] --> B[查询分析]
    B --> C[重写优化]
    C --> D[缓存利用]
    D --> E[执行优化]
    
    B --> F[性能分析]
    F --> G[慢查询分析]
    G --> H[优化建议]
`````

### 2.2 查询优化代码
`````java
class QueryOptimizer {
    // 优化查询构建
    SearchSourceBuilder optimizeQuery(SearchSourceBuilder source) {
        // 1. 设置合适的分页
        source.from(0).size(20);
        
        // 2. 选择需要的字段
        source.fetchSource(new String[]{"title", "price"}, null);
        
        // 3. 添加过滤器
        source.postFilter(QueryBuilders.termQuery("status", "active"));
        
        // 4. 设置路由
        source.routing("user123");
        
        return source;
    }
    
    // 性能分析
    void analyzeQuery(SearchRequest request) {
        // 1. 开启分析
        request.source().profile(true);
        
        // 2. 执行查询
        SearchResponse response = client.search(request, RequestOptions.DEFAULT);
        
        // 3. 获取分析结果
        Map<String, ProfileShardResult> profileResults = response.getProfileResults();
    }
}
`````

### 2.3 查询重写优化
`````mermaid
graph TD
    A[原始查询] --> B{是否可重写}
    B -->|是| C[重写规则]
    B -->|否| D[原样执行]
    C --> E[规则1:词条优化]
    C --> F[规则2:范围优化]
    C --> G[规则3:布尔优化]
    E --> H[优化后查询]
    F --> H
    G --> H
`````

## 3. 聚合基础架构

### 3.1 聚合执行流程
`````mermaid
sequenceDiagram
    participant Client
    participant Coordinator
    participant Shard
    participant Aggregator
    
    Client->>Coordinator: 聚合请求
    Coordinator->>Shard: 分发请求
    Shard->>Aggregator: 执行聚合
    Aggregator-->>Shard: 返回结果
    Shard-->>Coordinator: 返回分片结果
    Coordinator->>Coordinator: 合并结果
    Coordinator-->>Client: 返回最终结果
`````

### 3.2 聚合类型分类
`````mermaid
mindmap
  root((聚合类型))
    指标聚合
      avg
      sum
      min/max
      cardinality
    桶聚合
      terms
      date_histogram
      range
    管道聚合
      derivative
      cumulative_sum
      bucket_script
    矩阵聚合
      matrix_stats
`````

### 3.3 聚合构建代码
`````java
class AggregationBuilder {
    // 构建复杂聚合
    void buildComplexAggregation() {
        SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
        
        // 1. Terms聚合
        TermsAggregationBuilder termsAgg = AggregationBuilders
            .terms("by_category")
            .field("category")
            .size(10);
            
        // 2. 嵌套的Avg聚合
        AvgAggregationBuilder avgAgg = AggregationBuilders
            .avg("avg_price")
            .field("price");
        termsAgg.subAggregation(avgAgg);
        
        // 3. Date Histogram聚合
        DateHistogramAggregationBuilder dateAgg = AggregationBuilders
            .dateHistogram("by_month")
            .field("date")
            .calendarInterval(DateHistogramInterval.MONTH);
            
        // 4. 管道聚合
        DerivativePipelineAggregationBuilder derivativeAgg = PipelineAggregatorBuilders
            .derivative("price_derivative", "avg_price");
        dateAgg.subAggregation(derivativeAgg);
        
        searchSourceBuilder.aggregation(termsAgg);
        searchSourceBuilder.aggregation(dateAgg);
    }
}
`````

## 4. 聚合性能优化

### 4.1 聚合优化策略
`````mermaid
graph TD
    A[聚合优化] --> B[数据结构优化]
    A --> C[执行优化]
    A --> D[内存优化]
    
    B --> E[字段类型选择]
    B --> F[Doc Values]
    
    C --> G[分片优化]
    C --> H[缓存利用]
    
    D --> I[Circuit Breaker]
    D --> J[内存限制]
`````

### 4.2 聚合优化代码
`````java
class AggregationOptimizer {
    // 优化聚合执行
    SearchSourceBuilder optimizeAggregation() {
        SearchSourceBuilder source = new SearchSourceBuilder();
        
        // 1. 设置分片大小
        source.size(0);  // 不需要hits
        
        // 2. 添加过滤
        source.query(QueryBuilders.boolQuery()
            .filter(QueryBuilders.rangeQuery("date")
                .gte("now-1M")));
                
        // 3. 优化Terms聚合
        TermsAggregationBuilder termsAgg = AggregationBuilders
            .terms("by_category")
            .field("category.keyword")  // 使用keyword
            .collectMode(SubAggCollectionMode.BREADTH_FIRST)  // 优化收集模式
            .size(10);
            
        // 4. 设置分片大小
        source.aggregation(termsAgg);
        
        return source;
    }
}
`````

### 4.3 内存优化流程
`````mermaid
graph LR
    A[聚合请求] --> B{内存检查}
    B -->|超限| C[熔断]
    B -->|正常| D[执行聚合]
    D --> E{内存使用}
    E -->|高| F[优化策略]
    E -->|低| G[继续执行]
    F --> H[减少精度]
    F --> I[限制范围]
`````

## 5. 缓存机制

### 5.1 缓存层次
`````mermaid
graph TD
    A[缓存系统] --> B[查询缓存]
    A --> C[过滤器缓存]
    A --> D[请求缓存]
    
    B --> E[Node Query Cache]
    C --> F[Node Filter Cache]
    D --> G[Shard Request Cache]
`````

### 5.2 缓存管理代码
`````java
class CacheManager {
    // 配置缓存
    void configureCaching() {
        // 1. 索引级别缓存设置
        Settings settings = Settings.builder()
            .put("index.queries.cache.enabled", true)
            .put("index.requests.cache.enable", true)
            .build();
            
        // 2. 请求级别缓存控制
        SearchSourceBuilder source = new SearchSourceBuilder()
            .profile(true)
            .requestCache(true);
            
        // 3. 清除缓存
        ClearIndicesCacheRequest clearCache = new ClearIndicesCacheRequest()
            .queryCache(true)
            .requestCache(true);
    }
}
`````

## 6. 性能监控

### 6.1 监控指标
`````mermaid
mindmap
  root((性能监控))
    查询性能
      查询延迟
      查询吞吐量
      查询失败率
    聚合性能
      聚合时间
      内存使用
      缓存命中率
    系统资源
      CPU使用率
      内存使用率
      磁盘IO
`````

### 6.2 监控代码
`````java
class PerformanceMonitor {
    // 监控查询性能
    void monitorQueryPerformance() {
        // 1. 获取节点统计
        NodesStatsRequest nodesStats = new NodesStatsRequest();
        nodesStats.indices(true).os(true).process(true);
        
        // 2. 获取索引统计
        IndicesStatsRequest indicesStats = new IndicesStatsRequest();
        indicesStats.all();
        
        // 3. 分析结果
        NodesStatsResponse nodesResponse = client.nodes().stats(nodesStats, RequestOptions.DEFAULT);
        IndicesStatsResponse indicesResponse = client.indices().stats(indicesStats, RequestOptions.DEFAULT);
    }
    
    // 监控聚合性能
    void monitorAggregationPerformance() {
        // 1. 开启分析
        SearchSourceBuilder source = new SearchSourceBuilder()
            .profile(true)
            .aggregation(AggregationBuilders.terms("test").field("field"));
            
        // 2. 执行并分析
        SearchResponse response = client.search(new SearchRequest().source(source), RequestOptions.DEFAULT);
        Map<String, ProfileShardResult> profileResults = response.getProfileResults();
    }
}
`````

## 7. 最佳实践

### 7.1 查询优化建议
1. **查询设计**
   - 使用过滤器替代查询
   - 避免使用脚本
   - 合理使用分页

2. **性能优化**
   - 使用路由
   - 优化映射
   - 利用缓存

3. **资源控制**
   - 设置超时
   - 限制深度分页
   - 控制返回字段

### 7.2 聚合优化建议
1. **数据结构**
   - 使用keyword类型
   - 开启doc_values
   - 控制精度

2. **执行优化**
   - 使用过滤器
   - 设置合适的size
   - 选择适当的收集模式

3. **内存管理**
   - 监控内存使用
   - 设置断路器
   - 优化聚合深度

### 7.3 性能优化命令示例
`````bash
# 1. 优化查询设置
PUT my_index/_settings
{
  "index": {
    "refresh_interval": "30s",
    "number_of_replicas": 1,
    "search.idle.after": "30s"
  }
}

# 2. 优化映射设置
PUT my_index/_mapping
{
  "properties": {
    "my_field": {
      "type": "keyword",
      "doc_values": true,
      "eager_global_ordinals": true
    }
  }
}

# 3. 监控查询性能
GET _cat/thread_pool/search?v
GET _nodes/stats/indices/search
GET _nodes/stats/breaker
`````

## 结语

本文档详细介绍了ES的查询与聚合性能优化，通过流程图、代码示例和实际命令全面展示了相关概念和实践方法。理解并应用这些知识对于构建高性能的ES应用至关重要。

