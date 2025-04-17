指的是在一个分布式系统中，Consistency(一致性)、Availability(可用性)、Partition tolerance（分区容错性）这3个基本需求，最多只能同时满足其中2个。

![](https://secure2.wostatic.cn/static/bKFLswcyppU5U6S1wYps9h/image.png?auth_key=1732170629-cpruUehAW2aqkpuqHQ3upq-0-c74ad166c24a48ddeb12fae84e6f8204)

|选项|描述|
|-|-|
|Consistency（一致性）|指数据在多个副本之间能够保证一致的特性(严格的一致性)|
|Availability（可用性）|指系统提供的服务必须一直处在可用的状态，每次请求都能获取到非错的响应（不保证获得的数据为最新的数据）|
|Partition tolerance（分区容错性）|分布式系统在遇到任何网络分区故障的时候，仍然能够对外提供满足一致性和可用性的服务，除非整个网络环境都发生了变化。|


