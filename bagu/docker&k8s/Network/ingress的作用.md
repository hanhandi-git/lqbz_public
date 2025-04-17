# Kubernetes Ingress详解

## 什么是Ingress
Ingress是Kubernetes中的一个API对象，作为集群的流量入口，管理外部对集群内服务的访问。它提供：
- HTTP/HTTPS路由
- 负载均衡
- SSL终止
- 基于名称的虚拟主机等功能

## 工作原理
1. Ingress Controller监听Ingress资源的变化
2. 根据Ingress规则配置反向代理
3. 将外部请求转发到集群内的相应服务

## 基本架构
```plaintext
Internet --> Ingress Controller --> Ingress Rules --> Services --> Pods
```

## 配置示例

### 1. 基本HTTP路由
```yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: my-ingress
  annotations:
    nginx.ingress.kubernetes.io/rewrite-target: /
spec:
  rules:
  - host: myapp.example.com    # 域名
    http:
      paths:
      - path: /               # URL路径
        pathType: Prefix
        backend:
          service:
            name: my-service  # 后端服务
            port:
              number: 80      # 服务端口
```

### 2. 多域名路由
```yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: multi-host-ingress
spec:
  rules:
  - host: app1.example.com
    http:
      paths:
      - path: /
        pathType: Prefix
        backend:
          service:
            name: app1-service
            port:
              number: 8080
  - host: app2.example.com
    http:
      paths:
      - path: /
        pathType: Prefix
        backend:
          service:
            name: app2-service
            port:
              number: 8080
```

### 3. 启用TLS
```yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: tls-ingress
spec:
  tls:
  - hosts:
    - secure.example.com
    secretName: tls-secret
  rules:
  - host: secure.example.com
    http:
      paths:
      - path: /
        pathType: Prefix
        backend:
          service:
            name: secure-service
            port:
              number: 443
```

## 常见使用场景

### 1. 路径重写
```yaml
metadata:
  annotations:
    nginx.ingress.kubernetes.io/rewrite-target: /$2
spec:
  rules:
  - http:
      paths:
      - path: /api(/|$)(.*)
        pathType: Prefix
        backend:
          service:
            name: api-service
            port:
              number: 80
```

### 2. 会话亲和性
```yaml
metadata:
  annotations:
    nginx.ingress.kubernetes.io/affinity: "cookie"
    nginx.ingress.kubernetes.io/session-cookie-name: "route"
    nginx.ingress.kubernetes.io/session-cookie-expires: "172800"
```

### 3. 限速配置
```yaml
metadata:
  annotations:
    nginx.ingress.kubernetes.io/limit-rps: "10"
    nginx.ingress.kubernetes.io/limit-connections: "5"
```

## 注意事项

### 1. 性能考虑
- 合理配置资源限制
- 避免过多的路由规则
- 使用适当的负载均衡算法

### 2. 安全建议
- 总是启用TLS
- 配置适当的访问控制
- 定期更新Ingress Controller
- 监控异常访问

### 3. 维护建议
- 做好日志收集
- 监控Ingress Controller状态
- 定期检查证书有效期
- 备份Ingress配置

## 常见问题排查

### 1. 访问失败
- 检查Ingress Controller是否正常运行
- 验证DNS解析是否正确
- 确认后端服务是否可用
- 查看Ingress Controller日志

### 2. 性能问题
- 检查资源使用情况
- 优化路由规则
- 调整超时设置
- 考虑使用缓存

### 3. 配置错误
```bash
# 检查Ingress状态
kubectl describe ingress my-ingress

# 查看Ingress Controller日志
kubectl logs -n ingress-nginx deploy/ingress-nginx-controller

# 验证服务可达性
kubectl get svc
```

## 最佳实践

1. **规划设计**
   - 合理规划域名和路径
   - 预估流量和容量
   - 制定备份恢复策略

2. **安全配置**
   - 启用TLS 1.2/1.3
   - 配置WAF规则
   - 实施访问控制

3. **监控告警**
   - 监控关键指标
   - 设置合理的告警阈值
   - 建立应急响应机制

## 总结
Ingress是K8s中管理外部访问的重要组件，通过合理配置可以：
- 集中管理集群入口流量
- 提供灵活的路由规则
- 实现负载均衡
- 管理SSL证书
- 优化访问性能
