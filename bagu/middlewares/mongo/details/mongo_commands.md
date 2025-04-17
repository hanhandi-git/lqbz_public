# MongoDB 常用命令指南

## 目录
- [1. 数据库操作命令](#1-数据库操作命令)
  - [1.1 数据库管理](#11-数据库管理)
  - [1.2 集合操作](#12-集合操作)
  - [1.3 索引管理](#13-索引管理)
- [2. 文档操作命令](#2-文档操作命令)
  - [2.1 查询操作](#21-查询操作)
  - [2.2 更新操作](#22-更新操作)
  - [2.3 删除操作](#23-删除操作)
- [3. 聚合操作命令](#3-聚合操作命令)
  - [3.1 聚合管道](#31-聚合管道)
  - [3.2 聚合函数](#32-聚合函数)
  - [3.3 聚合优化](#33-聚合优化)
- [4. 管理命令](#4-管理命令)
  - [4.1 用户管理](#41-用户管理)
  - [4.2 备份恢复](#42-备份恢复)
  - [4.3 监控诊断](#43-监控诊断)


## 1. 数据库操作命令

### 1.1 数据库管理

```mermaid

graph TD
    A[数据库管理] --> B[创建数据库]
    A --> C[切换数据库]
    A --> D[删除数据库]
    
    B --> B1[use database]
    B --> B2[db.createCollection]
    
    C --> C1[show dbs]
    C --> C2[db.stats]
    
    D --> D1[db.dropDatabase]
    D --> D2[验证删除]
```

数据库管理命令示例：
```javascript


// 1. 数据库基本操作
// 显示所有数据库
show dbs

// 创建/切换数据库
use mydb

// 显示当前数据库
db

// 删除数据库
db.dropDatabase()

// 查看数据库状态
db.stats()

// 2. 数据库管理操作
// 查看数据库版本
db.version()

// 查看数据库命令帮助
db.help()

// 查看数据库服务器状态
db.serverStatus()

// 3. 数据库维护操作
// 修复数据库
db.repairDatabase()

// 设置数据库参数
db.setProfilingLevel(2)

// 获取数据库参数
db.getProfilingLevel()
```

### 1.2 集合操作

```mermaid
graph LR
    A[集合操作] --> B[创建集合]
    A --> C[管理集合]
    A --> D[删除集合]
    
    B --> B1[显式创建]
    B --> B2[隐式创建]
    B --> B3[验证规则]
    
    C --> C1[重命名]
    C --> C2[统计信息]
    C --> C3[压缩]
    
    D --> D1[删除集合]
    D --> D2[清空数据]
    D --> D3[验证删除]
```

集合操作命令示例：
```js

// 1. 集合创建
// 显式创建集合
db.createCollection("users", {
    capped: true,
    size: 1048576,
    max: 1000,
    validator: {
        $jsonSchema: {
            bsonType: "object",
            required: ["name", "email"],
            properties: {
                name: {
                    bsonType: "string",
                    description: "must be a string"
                },
                email: {
                    bsonType: "string",
                    pattern: "^.+@.+$",
                    description: "must be a valid email"
                }
            }
        }
    }
})

// 查看所有集合
show collections

// 2. 集合管理
// 重命名集合
db.users.renameCollection("newUsers")

// 获取集合统计信息
db.users.stats()

// 获取集合大小
db.users.dataSize()

// 3. 集合维护
// 压缩集合
db.runCommand({compact: "users"})

// 验证集合
db.users.validate()

// 删除集合
db.users.drop()
```

### 1.3 索引管理

```mermaid
graph TD
    A[索引管理] --> B[创建索引]
    A --> C[管理索引]
    A --> D[删除索引]
    
    B --> B1[单字段索引]
    B --> B2[复合索引]
    B --> B3[特殊索引]
    
    C --> C1[查看索引]
    C --> C2[重建索引]
    C --> C3[索引统计]
    
    D --> D1[删除单个]
    D --> D2[删除全部]
    D --> D3[验证删除]
```

索引管理命令示例：
```javascript
// 1. 创建索引
// 创建单字段索引
db.users.createIndex({ username: 1 })

// 创建复合索引
db.users.createIndex({ 
    email: 1, 
    createDate: -1 
})

// 创建唯一索引
db.users.createIndex(
    { email: 1 },
    { unique: true }
)

// 创建部分索引
db.users.createIndex(
    { age: 1 },
    {
        partialFilterExpression: {
            age: { $gt: 18 }
        }
    }
)

// 创建文本索引
db.articles.createIndex(
    { content: "text" },
    {
        weights: {
            content: 10
        },
        name: "ContentIndex"
    }
)

// 2. 索引管理
// 查看集合索引
db.users.getIndexes()

// 查看索引大小
db.users.totalIndexSize()

// 重建索引
db.users.reIndex()

// 获取索引信息
db.users.aggregate([
    { $indexStats: {} }
])

// 3. 删除索引
// 删除指定索引
db.users.dropIndex("index_name")

// 删除所有索引
db.users.dropIndexes()

// 验证索引使用情况
db.users.find({ username: "test" }).explain("executionStats")
```

## 2. 文档操作命令

### 2.1 查询操作

```mermaid
graph LR
    A[查询操作] --> B[基本查询]
    A --> C[高级查询]
    A --> D[查询优化]
    
    B --> B1[精确匹配]
    B --> B2[条件查询]
    B --> B3[字段投影]
    
    C --> C1[正则匹配]
    C --> C2[数组查询]
    C --> C3[嵌套查询]
    
    D --> D1[查询计划]
    D --> D2[索引使用]
    D --> D3[性能分析]
```

查询操作命令示例：
```javascript
// 1. 基本查询
// 查找所有文档
db.users.find()

// 查找特定条件
db.users.find({ age: { $gt: 18 } })

// 字段投影
db.users.find(
    { age: { $gt: 18 } },
    { username: 1, email: 1, _id: 0 }
)

// 2. 高级查询
// 正则表达式查询
db.users.find({
    email: /.*@gmail\.com$/
})

// 数组查询
db.users.find({
    tags: { $all: ["mongodb", "database"] }
})

// 嵌套文档查询
db.users.find({
    "address.city": "Beijing"
})

// 存在性查询
db.users.find({
    phone: { $exists: true }
})

// 3. 查询优化
// 使用limit限制结果
db.users.find().limit(10)

// 使用skip进行分页
db.users.find().skip(20).limit(10)

// 使用sort排序
db.users.find().sort({
    age: -1,
    username: 1
})

// 使用hint指定索引
db.users.find({
    age: { $gt: 18 }
}).hint({ age: 1 })

// 查询计划分析
db.users.find({
    age: { $gt: 18 },
    email: /.*@gmail\.com$/
}).explain("executionStats")
```

### 2.2 更新操作

```mermaid
graph TD
    A[更新操作] --> B[单文档更新]
    A --> C[多文档更新]
    A --> D[特殊更新]
    
    B --> B1[替换更新]
    B --> B2[字段更新]
    B --> B3[嵌套更新]
    
    C --> C1[批量更新]
    C --> C2[条件更新]
    C --> C3[原子更新]
    
    D --> D1[数组更新]
    D --> D2[字段操作]
    D --> D3[更新选项]
```


更新操作命令示例：
```javascript

// 1. 单文档更新
// 替换整个文档
db.users.replaceOne(
    { _id: ObjectId("...") },
    {
        username: "newname",
        email: "new@example.com",
        age: 25
    }
)

// 更新特定字段
db.users.updateOne(
    { username: "oldname" },
    {
        $set: { 
            email: "updated@example.com",
            lastModified: new Date()
        }
    }
)

// 嵌套文档更新
db.users.updateOne(
    { _id: ObjectId("...") },
    {
        $set: {
            "address.city": "Shanghai",
            "address.postcode": "200001"
        }
    }
)

// 2. 多文档更新
// 批量更新
db.users.updateMany(
    { age: { $lt: 18 } },
    {
        $set: { status: "minor" },
        $inc: { ageGroup: 1 }
    }
)

// 条件更新
db.users.updateMany(
    { 
        lastLogin: { 
            $lt: new Date(Date.now() - 30*24*60*60*1000) 
        }
    },
    {
        $set: { status: "inactive" }
    }
)

// 3. 特殊更新操作
// 数组操作
db.users.updateOne(
    { username: "test" },
    {
        $push: { 
            tags: {
                $each: ["tag1", "tag2"],
                $sort: 1
            }
        }
    }
)

// 删除字段
db.users.updateMany(
    { status: "inactive" },
    {
        $unset: { 
            temporaryField: "",
            "metadata.obsolete": "" 
        }
    }
)

// 更新或插入(upsert)
db.users.updateOne(
    { email: "new@example.com" },
    {
        $set: {
            username: "newuser",
            createDate: new Date()
        }
    },
    { upsert: true }
)

### 2.3 删除操作

```mermaid
graph LR
    A[删除操作] --> B[单文档删除]
    A --> C[多文档删除]
    A --> D[删除优化]
    
    B --> B1[精确删除]
    B --> B2[条件删除]
    B --> B3[删除确认]
    
    C --> C1[批量删除]
    C --> C2[条件删除]
    C --> C3[删除验证]
    
    D --> D1[索引使用]
    D --> D2[批量处理]
    D --> D3[性能监控]
```

删除操作命令示例：
```javascript

// 1. 单文档删除
// 删除特定文档
db.users.deleteOne({ _id: ObjectId("...") })

// 条件删除
db.users.deleteOne({
    username: "testuser",
    status: "inactive"
})

// 删除并返回文档
db.users.findOneAndDelete({
    email: "old@example.com"
})

// 2. 多文档删除
// 批量删除
db.users.deleteMany({
    lastLogin: {
        $lt: new Date(Date.now() - 365*24*60*60*1000)
    }
})

// 条件批量删除
db.users.deleteMany({
    $or: [
        { status: "inactive" },
        { verified: false }
    ]
})

// 删除集合中所有文档
db.users.deleteMany({})

// 3. 删除优化
// 使用索引的删除
db.users.deleteMany({
    email: /.*@oldomain\.com$/
}).hint({ email: 1 })

// 批量删除带进度显示
const batchSize = 1000;
let deleted = 0;
while (true) {
    const result = db.users.deleteMany(
        { status: "inactive" },
        { limit: batchSize }
    );
    deleted += result.deletedCount;
    if (result.deletedCount < batchSize) break;
    print(`Deleted ${deleted} documents`);
}

// 删除前备份关键数据
db.users.find(
    { status: "to_delete" },
    { email: 1, username: 1 }
).forEach(doc => {
    db.deletedUsers.insertOne({
        originalId: doc._id,
        email: doc.email,
        deleteDate: new Date()
    });
});
```

## 3. 聚合操作命令

### 3.1 聚合管道

```mermaid
graph TD
    A[聚合管道] --> B[管道阶段]
    A --> C[管道操作]
    A --> D[管道优化]
    
    B --> B1[$match]
    B --> B2[$group]
    B --> B3[$project]
    
    C --> C1[$sort]
    C --> C2[$limit]
    C --> C3[$lookup]
    
    D --> D1[内存限制]
    D --> D2[索引使用]
    D --> D3[性能优化]
```


聚合管道操作示例：
```javascript

// 1. 基本聚合操作
// 简单分组统计
db.orders.aggregate([
    { 
        $match: { 
            status: "completed",
            orderDate: {
                $gte: new Date("2024-01-01")
            }
        }
    },
    {
        $group: {
            _id: "$customerId",
            totalOrders: { $sum: 1 },
            totalAmount: { $sum: "$amount" },
            avgOrderValue: { $avg: "$amount" }
        }
    },
    {
        $sort: { totalAmount: -1 }
    }
])

// 2. 复杂聚合操作
// 多表关联统计
db.orders.aggregate([
    {
        $lookup: {
            from: "customers",
            localField: "customerId",
            foreignField: "_id",
            as: "customerInfo"
        }
    },
    {
        $unwind: "$customerInfo"
    },
    {
        $group: {
            _id: {
                customerId: "$customerId",
                customerName: "$customerInfo.name"
            },
            orderCount: { $sum: 1 },
            totalSpent: { $sum: "$amount" },
            lastOrder: { $max: "$orderDate" }
        }
    },
    {
        $project: {
            _id: 0,
            customerId: "$_id.customerId",
            customerName: "$_id.customerName",
            orderStats: {
                count: "$orderCount",
                total: "$totalSpent",
                average: { 
                    $divide: ["$totalSpent", "$orderCount"] 
                }
            },
            lastOrderDate: "$lastOrder"
        }
    }
])

// 3. 高级聚合功能
// 时间维度分析
db.orders.aggregate([
    {
        $match: {
            orderDate: {
                $gte: new Date("2024-01-01"),
                $lt: new Date("2025-01-01")
            }
        }
    },
    {
        $group: {
            _id: {
                year: { $year: "$orderDate" },
                month: { $month: "$orderDate" },
                day: { $dayOfMonth: "$orderDate" }
            },
            dailyOrders: { $sum: 1 },
            dailyRevenue: { $sum: "$amount" },
            uniqueCustomers: { 
                $addToSet: "$customerId" 
            }
        }
    },
    {
        $addFields: {
            date: {
                $dateFromParts: {
                    year: "$_id.year",
                    month: "$_id.month",
                    day: "$_id.day"
                }
            },
            uniqueCustomerCount: {
                $size: "$uniqueCustomers"
            }
        }
    },
    {
        $sort: { date: 1 }
    }
])

```
### 3.2 聚合函数

```mermaid

graph LR
    A[聚合函数] --> B[算术函数]
    A --> C[日期函数]
    A --> D[字符串函数]
    
    B --> B1[$sum/$avg]
    B --> B2[$min/$max]
    B --> B3[$multiply/$divide]
    
    C --> C1[$dateToString]
    C --> C2[$dateDiff]
    C --> C3[$dateAdd]
    
    D --> D1[$concat]
    D --> D2[$substr]
    D --> D3[$toLower/$toUpper]
```

聚合函数示例：
```javascript

// 1. 算术函数
// 销售统计分析
db.sales.aggregate([
    {
        $group: {
            _id: "$product",
            totalQuantity: { $sum: "$quantity" },
            averagePrice: { $avg: "$price" },
            revenue: {
                $sum: { 
                    $multiply: ["$quantity", "$price"] 
                }
            },
            minPrice: { $min: "$price" },
            maxPrice: { $max: "$price" },
            priceRange: {
                $subtract: [
                    { $max: "$price" },
                    { $min: "$price" }
                ]
            }
        }
    },
    {
        $project: {
            _id: 0,
            product: "$_id",
            stats: {
                quantity: "$totalQuantity",
                revenue: { $round: ["$revenue", 2] },
                averagePrice: { 
                    $round: ["$averagePrice", 2] 
                },
                priceRange: { 
                    $round: ["$priceRange", 2] 
                }
            }
        }
    }
])

// 2. 日期函数
// 时间分析
db.events.aggregate([
    {
        $group: {
            _id: {
                year: { $year: "$timestamp" },
                month: { $month: "$timestamp" },
                dayOfWeek: { $dayOfWeek: "$timestamp" }
            },
            count: { $sum: 1 },
            firstEvent: { $min: "$timestamp" },
            lastEvent: { $max: "$timestamp" },
            duration: {
                $dateDiff: {
                    startDate: { $min: "$timestamp" },
                    endDate: { $max: "$timestamp" },
                    unit: "hour"
                }
            }
        }
    },
    {
        $project: {
            period: {
                $dateToString: {
                    format: "%Y-%m-%d",
                    date: "$firstEvent"
                }
            },
            weekDay: {
                $switch: {
                    branches: [
                        { case: { $eq: ["$_id.dayOfWeek", 1] }, then: "Sunday" },
                        { case: { $eq: ["$_id.dayOfWeek", 2] }, then: "Monday" },
                        { case: { $eq: ["$_id.dayOfWeek", 3] }, then: "Tuesday" },
                        { case: { $eq: ["$_id.dayOfWeek", 4] }, then: "Wednesday" },
                        { case: { $eq: ["$_id.dayOfWeek", 5] }, then: "Thursday" },
                        { case: { $eq: ["$_id.dayOfWeek", 6] }, then: "Friday" },
                        { case: { $eq: ["$_id.dayOfWeek", 7] }, then: "Saturday" }
                    ]
                }
            },
            eventCount: "$count",
            durationHours: "$duration"
        }
    }
])


// 3. 字符串函数和聚合优化示例
```javascript


// 1. 字符串函数操作
// 文本处理和分析
db.articles.aggregate([
    {
        $project: {
            title: { $toUpper: "$title" },
            slug: {
                $toLower: {
                    $reduce: {
                        input: { 
                            $split: [
                                { $trim: { input: "$title" } },
                                " "
                            ]
                        },
                        initialValue: "",
                        in: {
                            $concat: [
                                "$$value",
                                { $cond: [
                                    { $eq: ["$$value", ""] },
                                    "",
                                    "-"
                                ]},
                                "$$this"
                            ]
                        }
                    }
                }
            },
            firstParagraph: {
                $substr: [
                    "$content",
                    0,
                    200
                ]
            },
            wordCount: {
                $size: {
                    $split: ["$content", " "]
                }
            },
            tags: {
                $map: {
                    input: "$tags",
                    as: "tag",
                    in: { $toLower: "$$tag" }
                }
            }
        }
    }
])

// 2. 聚合优化示例
// 使用索引优化
db.orders.aggregate([
    {
        $match: {
            orderDate: {
                $gte: new Date("2024-01-01")
            }
        }
    },
    {
        $sort: {
            orderDate: -1
        }
    },
    {
        $limit: 1000
    }
]).hint({ orderDate: 1 })

// 使用内存限制
db.orders.aggregate([
    {
        $group: {
            _id: "$customerId",
            totalOrders: { $sum: 1 }
        }
    }
], {
    allowDiskUse: true,
    maxTimeMS: 60000
})

### 3.3 聚合优化

```mermaid

graph TD
    A[聚合优化] --> B[性能优化]
    A --> C[内存管理]
    A --> D[执行计划]
    
    B --> B1[索引使用]
    B --> B2[管道顺序]
    B --> B3[批处理]
    
    C --> C1[内存限制]
    C --> C2[磁盘使用]
    C --> C3[结果缓存]
    
    D --> D1[计划分析]
    D --> D2[性能监控]
    D --> D3[优化建议]
```

聚合优化最佳实践：

1. **早期过滤**
```javascript

// 优化前
db.orders.aggregate([
    {
        $group: {
            _id: "$status",
            orders: { $push: "$$ROOT" }
        }
    },
    {
        $match: {
            "_id": "completed"
        }
    }
])

// 优化后
db.orders.aggregate([
    {
        $match: {
            status: "completed"
        }
    },
    {
        $group: {
            _id: "$status",
            orders: { $push: "$$ROOT" }
        }
    }
])
```

2. **索引利用**
```javascript

// 创建复合索引支持聚合
db.orders.createIndex({
    status: 1,
    orderDate: -1
})

// 利用索引的聚合查询
db.orders.aggregate([
    {
        $match: {
            status: "completed",
            orderDate: {
                $gte: new Date("2024-01-01")
            }
        }
    },
    {
        $sort: {
            orderDate: -1
        }
    }
]).explain("executionStats")
```

3. **内存优化**
```javascript

// 大数据集处理
db.logs.aggregate([
    {
        $match: {
            level: "ERROR"
        }
    },
    {
        $group: {
            _id: {
                hour: { $hour: "$timestamp" },
                day: { $dayOfMonth: "$timestamp" },
                month: { $month: "$timestamp" }
            },
            count: { $sum: 1 }
        }
    }
], {
    allowDiskUse: true,
    cursor: { batchSize: 1000 }
})
```

## 4. 管理命令

### 4.1 用户管理

```mermaid

graph LR
    A[用户管理] --> B[用户操作]
    A --> C[角色管理]
    A --> D[权限控制]
    
    B --> B1[创建用户]
    B --> B2[修改用户]
    B --> B3[删除用户]
    
    C --> C1[角色分配]
    C --> C2[角色创建]
    C --> C3[权限查看]
    
    D --> D1[授权管理]
    D --> D2[访问控制]
    D --> D3[安全审计]
```


用户管理命令示例：
```javascript



// 1. 用户操作命令
// 创建管理员用户
db.createUser({
    user: "admin",
    pwd: "secure_password",
    roles: [
        { role: "userAdminAnyDatabase", db: "admin" },
        { role: "readWriteAnyDatabase", db: "admin" }
    ],
    customData: {
        description: "System Administrator",
        department: "IT",
        createdAt: new Date()
    }
})

// 创建应用用户
db.createUser({
    user: "appUser",
    pwd: "app_password",
    roles: [
        { role: "readWrite", db: "appDatabase" },
        { role: "read", db: "reportDatabase" }
    ]
})

// 修改用户权限
db.grantRolesToUser(
    "appUser",
    [
        { role: "readWrite", db: "newDatabase" }
    ]
)

// 修改用户密码
db.changeUserPassword("appUser", "new_password")

// 查看用户信息
db.getUser("appUser")

// 查看所有用户
db.getUsers()

// 删除用户
db.dropUser("appUser")
```
### 4.2 备份恢复

```mermaid

graph TD
    A[备份恢复] --> B[备份操作]
    A --> C[恢复操作]
    A --> D[备份策略]
    
    B --> B1[全量备份]
    B --> B2[增量备份]
    B --> B3[热备份]
    
    C --> C1[完全恢复]
    C --> C2[时间点恢复]
    C --> C3[验证恢复]
    
    D --> D1[备份计划]
    D --> D2[备份监控]
    D --> D3[灾难恢复]
```

备份恢复命令示例：
```javascript

// 1. 备份操作
// 全量备份数据库
mongodump --db=mydb --out=/backup/mongodump-$(date +%Y-%m-%d)

// 备份特定集合
mongodump --db=mydb --collection=users \
    --out=/backup/users-$(date +%Y-%m-%d)

// 压缩备份
mongodump --db=mydb \
    --archive=/backup/mydb.$(date +%Y-%m-%d).gz \
    --gzip

// 带认证的备份
mongodump --uri="mongodb://username:password@localhost:27017/mydb" \
    --out=/backup/mongodump-$(date +%Y-%m-%d)

// 2. 恢复操作
// 恢复整个数据库
mongorestore --db=mydb \
    /backup/mongodump-2024-01-01/mydb/

// 恢复特定集合
mongorestore --db=mydb --collection=users \
    /backup/users-2024-01-01/mydb/users.bson

// 从压缩文件恢复
mongorestore --archive=/backup/mydb.2024-01-01.gz \
    --gzip

// 带数据清理的恢复
mongorestore --db=mydb \
    --drop /backup/mongodump-2024-01-01/mydb/
```
### 4.3 监控诊断

```mermaid

graph LR
    A[监控诊断] --> B[性能监控]
    A --> C[状态诊断]
    A --> D[问题排查]
    
    B --> B1[资源监控]
    B --> B2[操作监控]
    B --> B3[性能分析]
    
    C --> C1[服务状态]
    C --> C2[复制状态]
    C --> C3[分片状态]
    
    D --> D1[日志分析]
    D --> D2[慢查询]
    D --> D3[连接追踪]
```

监控诊断命令示例：
```javascript

// 1. 性能监控
// 服务器状态
db.serverStatus()

// 数据库状态
db.stats()

// 集合状态
db.users.stats()

// 操作统计
db.currentOp()

// 2. 性能分析
// 开启性能分析
db.setProfilingLevel(2)

// 查看性能数据
db.system.profile.find().sort({millis:-1}).limit(10)

// 查看慢查询
db.system.profile.find({
    millis: { $gt: 100 }
}).sort({
    millis: -1
})

// 3. 诊断命令
// 查看日志
db.adminCommand({ getLog: "global" })

// 连接信息
db.currentOp(true)

// 锁定状态
db.serverStatus().locks

// 内存使用
db.serverStatus().mem

// 4. 性能优化建议
// 索引使用分析
db.users.aggregate([
    { $indexStats: {} }
])

// 集合扫描分析
db.users.find({
    status: "active"
}).explain("executionStats")

// 系统性能检查脚本
function checkSystemPerformance() {
    print("=== 系统性能报告 ===");
    
    // 检查连接数
    const connections = db.serverStatus().connections;
    print(`当前连接数: ${connections.current}`);
    print(`可用连接数: ${connections.available}`);
    
    // 检查内存使用
    const mem = db.serverStatus().mem;
    print(`内存使用: ${mem.resident}MB`);
    
    // 检查慢查询
    const slowQueries = db.system.profile.count({
        millis: { $gt: 100 }
    });
    print(`慢查询数量: ${slowQueries}`);
    
    // 检查索引使用
    db.getCollectionNames().forEach(collection => {
        const stats = db[collection].stats();
        print(`\n集合 ${collection}:`);
        print(`文档数: ${stats.count}`);
        print(`索引数: ${stats.nindexes}`);
    });
}
```
