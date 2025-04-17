在 Kubernetes 中，Service 是用于暴露应用程序的核心组件，支持多种类型以满足不同的场景需求。以下是 Kubernetes 中常见的 Service 类型：

1. **ClusterIP**  
   这是默认的 Service 类型。它为服务分配一个集群内部的虚拟 IP 地址，仅允许集群内的其他组件访问。适用于内部服务间通信，比如微服务间调用。

2. **NodePort**  
   在每个节点（Node）上开放一个静态端口（默认范围 30000-32767），外部流量可以通过 `节点IP:端口` 访问服务。适用于开发测试环境或需要直接从集群外访问的场景。

3. **LoadBalancer**  
   通过云服务提供商的负载均衡器（如 AWS、GCP 等）分配外部 IP，自动将外部流量路由到 Service。适用于生产环境需对外暴露服务，但依赖云平台支持。

4. **ExternalName**  
   将服务映射到外部服务的 DNS 名称（如数据库、第三方 API），不创建代理或端口。适用于集成集群外部的服务。

5. **Headless Service**  
   通过设置 `clusterIP: None` 禁用 ClusterIP，直接暴露 Pod IP。DNS 查询返回所有 Pod 的 IP 地址。适用于有状态应用（如数据库集群）或需要直接访问 Pod 的场景。

### 总结
Kubernetes 中的 Service 类型包括：
• **ClusterIP**（默认，集群内部访问）
• **NodePort**（节点端口，外部访问）
• **LoadBalancer**（云平台负载均衡，外部访问）
• **ExternalName**（映射外部服务）
• **Headless Service**（直接暴露 Pod IP）

每种类型适用于不同的场景，选择合适的类型可以更好地满足应用需求。