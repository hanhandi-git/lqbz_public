在 Kubernetes 中，**Endpoints** 是 Service 的核心组件之一，用于记录 Service 后端 Pod 的 IP 和端口信息。以下是 Kubernetes 中生成 Endpoints 的几种方式：

---

### 1. **自动生成的 Endpoints**
当创建 **Service** 时，Kubernetes 会根据 Service 的 `selector` 自动查找匹配的 Pod，并将这些 Pod 的 IP 和端口信息填充到 Endpoints 对象中。例如：
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
      targetPort: 9376
```
Kubernetes 会自动创建名为 `my-service` 的 Endpoints 对象，记录所有 `app=my-app` 的 Pod 的 IP 和端口。

---

### 2. **手动定义的 Endpoints**
如果 Service 没有指定 `selector`，Kubernetes 不会自动生成 Endpoints。此时可以手动创建 Endpoints 对象，将 Service 与特定的后端（如外部服务）关联。例如：
```yaml
apiVersion: v1
kind: Endpoints
metadata:
  name: my-service
subsets:
  - addresses:
      - ip: 192.168.1.100
    ports:
      - port: 9376
```
这种方式常用于将 Service 映射到集群外部的服务（如数据库、第三方 API 等）。

---

### 3. **通过 ExternalName Service 生成 Endpoints**
**ExternalName Service** 是一种特殊的 Service 类型，它通过 DNS CNAME 记录将 Service 映射到外部服务的域名，而不是直接生成 Endpoints。例如：
```yaml
apiVersion: v1
kind: Service
metadata:
  name: my-service
spec:
  type: ExternalName
  externalName: my.external.service.com
```
这种 Service 不会生成 Endpoints，而是通过 DNS 解析将流量转发到外部服务。

---

### 4. **通过 Headless Service 生成 Endpoints**
**Headless Service** 是一种没有 ClusterIP 的 Service，通常用于直接暴露 Pod 的 IP 地址。它仍然会生成 Endpoints，但不会进行负载均衡。例如：
```yaml
apiVersion: v1
kind: Service
metadata:
  name: my-service
spec:
  clusterIP: None
  selector:
    app: my-app
  ports:
    - protocol: TCP
      port: 80
      targetPort: 9376
```
Kubernetes 会生成 Endpoints，记录所有匹配 Pod 的 IP 和端口。

---

### 5. **通过 EndpointSlice 生成 Endpoints**
Kubernetes 1.16 引入了 **EndpointSlice**，它是 Endpoints 的扩展版本，支持更高效地管理大量后端 Pod。EndpointSlice 将 Endpoints 信息分片存储，适用于大规模集群。例如：
```yaml
apiVersion: discovery.k8s.io/v1
kind: EndpointSlice
metadata:
  name: my-service-abc123
  labels:
    kubernetes.io/service-name: my-service
addressType: IPv4
ports:
  - name: http
    protocol: TCP
    port: 80
endpoints:
  - addresses:
      - "10.1.2.3"
    conditions:
      ready: true
```
EndpointSlice 是 Endpoints 的增强版，支持更细粒度的管理和扩展。

---

### 总结
Kubernetes 中生成 Endpoints 的方式包括：
1. **自动生成的 Endpoints**（通过 Service 的 `selector`）
2. **手动定义的 Endpoints**（用于集群外部服务）
3. **ExternalName Service**（通过 DNS CNAME 映射外部服务，不生成 Endpoints）
4. **Headless Service**（直接暴露 Pod IP，生成 Endpoints）
5. **EndpointSlice**（Endpoints 的扩展版本，支持大规模集群）

这些方式可以满足不同场景的需求，灵活选择即可。