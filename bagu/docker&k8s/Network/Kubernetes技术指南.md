# Kubernetes技术指南

# 第一部分：Pod端口暴露方案指南

## 背景说明
在 Kubernetes 中，Pod 可以监听特定的端口，但这些端口默认情况下只对集群内部的其他 Pod 和服务可见。如果你希望本机（即宿主机）也能够访问 Pod 监听的端口，你需要进行一些配置。

## 常用方案

### 1. 使用 hostPort
你可以在 Pod 的定义中使用 hostPort 字段，将容器端口映射到宿主机的端口。
```yaml
apiVersion: v1
kind: Pod
metadata:
  name: my-pod
spec:
  containers:
  - name: my-container
    image: my-image
    ports:
    - containerPort: 80
      hostPort: 8080
```

在这个例子中，容器内的 80 端口被映射到宿主机的 8080 端口。这样，你就可以通过 `http://<宿主机IP>:8080` 访问 Pod。

### 2. 使用 NodePort
如果你使用的是 Service 资源，可以配置 NodePort 类型的 Service，将 Pod 的端口暴露在宿主机的一个特定端口上。
```yaml
apiVersion: v1
kind: Service
metadata:
  name: my-service
spec:
  selector:
    app: my-app
  ports:
  - protocol: TCP
    port: 80
    targetPort: 80
    nodePort: 30007
  type: NodePort
```

在这个例子中，Pod 的 80 端口被映射到宿主机的 30007 端口。你可以通过 `http://<宿主机IP>:30007` 访问 Pod。

### 3. 使用 LoadBalancer
如果你在云环境中运行 Kubernetes，可以使用 LoadBalancer 类型的 Service，云提供商会自动为你分配一个外部 IP 地址。
```yaml
apiVersion: v1
kind: Service
metadata:
  name: my-service
spec:
  selector:
    app: my-app
  ports:
  - protocol: TCP
    port: 80
    targetPort: 80
  type: LoadBalancer
```

### 4. 使用 kubectl port-forward
如果你只是想在本地开发环境中临时访问 Pod，可以使用 kubectl port-forward 命令：

```bash
kubectl port-forward pod/my-pod 8080:80
```

这个命令会将 Pod 的 80 端口转发到本地的 8080 端口，你可以通过 `http://localhost:8080` 访问 Pod。

## 方案对比

| 方案 | 适用场景 | 优点 | 缺点 |
|------|----------|------|------|
| hostPort | 开发测试环境 | 配置简单，直接映射 | 不适合生产环境，可能造成端口冲突 |
| NodePort | 测试环境和小规模生产环境 | 配置简单，支持负载均衡 | 端口范围限制(30000-32767) |
| LoadBalancer | 云环境生产系统 | 自动负载均衡，高可用 | 需要云服务支持，可能产生额外费用 |
| port-forward | 本地开发调试 | 简单快捷，临时使用 | 仅供本地访问，不支持生产环境 |

## 端口暴露方案总结
选择哪种端口暴露方案主要取决于以下因素：
- 使用环境（开发、测试还是生产）
- 基础设施支持情况（是否在云环境）
- 访问需求（临时访问还是长期服务）
- 运维要求（是否需要负载均衡、高可用等）

# 第二部分：Kubernetes面试要点大纲

## 一、基础概念篇
### 1. 核心组件
- etcd的作用与原理
- API Server的职责
- Controller Manager的工作机制
- Scheduler的调度策略
- Kubelet的功能与工作流程

### 2. 基础资源对象
- Pod的生命周期
- ReplicaSet vs Deployment
- StatefulSet使用场景
- DaemonSet应用实例
- Job和CronJob的区别

### 3. 网络模型
- Pod网络通信原理
- Service类型与区别
- Ingress工作机制
- CNI插件对比（Calico/Flannel）
- DNS服务原理

## 二、实战应用篇
### 1. 部署管理
- 滚动更新策略
- 回滚机制
- 配置管理（ConfigMap/Secret）
- 资源限制（Resource Quota）
- 健康检查机制

### 2. 存储方案
- PV/PVC工作机制
- StorageClass动态供应
- 各类存储方案对比
- 数据备份策略
- 持久化最佳实践

### 3. 安全机制
- RBAC权限控制
- ServiceAccount使用
- Security Context配置
- Network Policy实现
- 密钥管理

## 三、架构设计篇
### 1. 高可用方案
- Master节点高可用
- etcd集群部署
- 关键组件备份
- 灾难恢复方案
- 多集群管理

### 2. 性能优化
- 资源分配优化
- HPA自动扩缩容
- 调度优化策略
- 网络性能调优
- 监控指标优化

### 3. 成本控制
- 资源利用率优化
- 弹性伸缩策略
- 成本监控方案
- 资源超卖策略
- 混部方案

## 四、问题排查篇
### 1. 故障诊断
- Pod启动失败排查
- 网络连通性问题
- 性能问题定位
- 日志收集分析
- 常见错误码解析

### 2. 监控告警
- Prometheus部署配置
- 关键指标监控
- 告警规则制定
- Grafana展示
- 日志管理方案

### 3. 运维工具
- kubectl高级用法
- helm包管理
- 运维自动化工具
- 集群升级策略
- 备份恢复工具

## 五、场景设计篇
### 1. 微服务架构
- 服务发现机制
- 负载均衡策略
- 配置中心设计
- 服务网格选型
- 灰度发布方案

### 2. 有状态服务
- 数据库部署方案
- 消息队列架构
- 缓存集群设计
- 分布式存储
- 会话管理

### 3. DevOps实践
- CI/CD流水线
- 镜像管理策略
- 环境隔离方案
- 发布策略
- 自动化测试

## 六、实际经验篇
### 1. 生产实践
- 大规模集群管理
- 问题排查案例
- 性能优化经验
- 架构演进历程
- 运维自动化实践

### 2. 最佳实践
- 容器镜像策略
- 资源规划方法
- 备份恢复流程
- 安全加固方案
- 监控告警策略

### 3. 发展趋势
- 云原生新技术
- 容器运行时演进
- 混合云方案
- Serverless架构
- 边缘计算实践

## 面试准备建议

根据不同岗位的要求，建议重点关注以下内容：

1. 基础岗位：重点掌握第一、二部分
2. 高级岗位：深入理解第三、四部分
3. 架构岗位：精通第五、六部分
4. 运维岗位：侧重第二、四部分
5. 开发岗位：侧重第一、五部分 