## 2PC两阶段提交

先说分布式事务中的XA协议

在该协议中，有三个角色：

- AP（application）：应用服务
- TM（Transcation Manager）：事务管理器（全局事务管理）
- RM（Resource Manager）：资源管理器（数据库）

![](https://secure2.wostatic.cn/static/okm5a7cmBb7Bst3z6ZUY2X/image.png?auth_key=1732170784-facHD4RheKj5wGnX2seoDp-0-e711340832b4db4dedf1b62a8718cc52)

XA协议采用两阶段提交方式来管理分布式事务。XA接口提供资源管理器与事务管理器之间进行通信的标准接口。

两阶段提交的思路可以概括为：参与者将操作成败通知给协调者，再由协调者根据所有参与者的反馈情况决定各参与者是否要提交操作还是回滚操作。

![](https://secure2.wostatic.cn/static/ho3xKsSwaWRGNTFZnBi6uE/image.png?auth_key=1732170784-jVqz1CDw97LVFyXMjtvi9E-0-221697137cb7c3d3407816c9991ae77c)

- 准备阶段：事务管理器要求每个涉及到事务的数据库预提交(precommit)此操作，并反映是否可以提交
- 提交阶段：事务管理器要求每个数据库提交数据或者回滚数据

详细一点的介绍是：

- 准备阶段（Prepare Phase）：在这个阶段，事务协调者像所有的参与者发送一个准备请求，询问他们是否可以提交事务。如果参与者可以提交事务，则它们会将事务记录在本地的日志中，并返回一个“同意”给事务协调者。如果参与者无法提交事务，则会返回一个“拒绝”给事务协调者
- 提交阶段（Commit Phase）: 在这个阶段，如果所有参与者都返回了“同意”消息，则事务协调者像所有的参与者发送一个提交请求，要求他们提交事务。如果有任何一个参与者返回了“拒绝”消息，则事务协调者向所有参与者发送一个回滚请求，要求它们回滚事务。

这样做的优点：尽量保证了数据的强一致，实现成本较低。

缺点：

- 单点问题：事务管理器在整个流程中扮演角色很重要，如果宕机，比如在第一阶段已经完成。在第二阶段正准备提交的时候事务管理器宕机，资源管理器就会一直阻塞，导致数据库无法使用
- 同步阻塞：在准备就绪之后，资源管理器中的资源一直处于阻塞，直到提交完成，释放资源
- 数据不一致：两阶段提交协议虽然为分布式数据强一致性所设计，但仍然存在数据不一致的可能，比如在第二阶段中，假设协调者发出了事务commit的通知，但是由于网络问题该通知仅被一部分参与者收到并执行。其余的参与者则因为没有收到通知而一直处于阻塞状态，此时就产生了数据的不一致性

