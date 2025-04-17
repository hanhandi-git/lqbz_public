在 Kubernetes 中，`podPort`、`nodePort` 和 `targetPort` 是与服务（Service）相关的三个不同的概念，它们共同工作以实现网络通信。以下是每个术语的详细解释和它们之间的区别：

1. **targetPort**：
   - `targetPort` 是 Service 定义中指向 Pod 内部容器端口的端口。
   - 当 Service 接收到流量时，它会将流量转发到 `targetPort` 指定的端口，即 Pod 内部容器监听的端口。
   - `targetPort` 可以是数字形式，也可以是字符串形式（如果以字符串形式指定，它必须与 Pod 定义中的容器端口名称匹配）。
   - 如果没有指定 `targetPort`，Kubernetes 默认使用 `podPort`（即 Service 定义中的 `port`）。

2. **podPort**（通常指的是 Service 定义中的 `port`）：
   - `podPort` 是 Service 定义中声明的端口，它是 Service 监听的端口。
   - 这个端口是集群内部通信时使用的端口，即集群内的 Pod 通过这个端口访问 Service。
   - `podPort` 仅在 Service 定义中使用，不直接与 Pod 通信。
   - 通常，`podPort` 和 `targetPort` 是相同的，但它们可以不同，如果 Service 需要在不同的端口上接收流量，但将流量转发到 Pod 的不同端口。

3. **nodePort**：
   - `nodePort` 是 Service 定义中的一个可选配置，它允许从集群外部访问 Service。
   - 当 Service 类型设置为 `NodePort` 时，Kubernetes 会在每个节点上打开一个静态端口（`nodePort`），从而允许外部流量通过这个端口访问 Service。
   - `nodePort` 是一个数字，范围从 30000 到 32767。
   - 外部用户可以通过 `<NodeIP>:nodePort` 或 `<ClusterIP>:nodePort` 访问 Service，其中 `<NodeIP>` 是任意节点的 IP 地址，`<ClusterIP>` 是 Service 的虚拟 IP 地址。

总结区别：
- `targetPort` 是 Pod 内部容器监听的端口，Service 将流量转发到这个端口。
- `podPort` 是 Service 监听的端口，用于集群内部通信（在 Service 定义中与 `port` 相同）。
- `nodePort` 是在每个节点上打开的端口，用于从集群外部访问 Service。

以下是一个简化的示例，说明它们之间的关系：
```yaml
apiVersion: v1
kind: Service
metadata:
  name: my-service
spec:
  type: NodePort
  ports:
    - port: 80  # podPort，Service 监听的端口
      targetPort: 8080  # targetPort，流量转发到 Pod 的这个端口
      nodePort: 30080  # nodePort，外部访问的端口
  selector:
    app: my-app
```
在这个例子中，Service 监听集群内部的 80 端口（`podPort`），并将流量转发到 Pod 的 8080 端口（`targetPort`）。同时，外部用户可以通过 `<NodeIP>:30080` 访问这个 Service（`nodePort`）。
