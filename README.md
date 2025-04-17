# 编程学习与实践仓库

<p align="center">
  <img src="https://img.shields.io/badge/language-C%2B%2B%2FGo-blue.svg" alt="Language">
  <img src="https://img.shields.io/badge/category-algorithms%2Fdata%20structures-brightgreen.svg" alt="Category">
  <img src="https://img.shields.io/badge/status-active-success.svg" alt="Status">
</p>

## 📖 项目简介

这是一个综合性的编程学习与实践仓库，包含了算法题解、数据结构实现、系统设计资料以及编程语言学习路径。本仓库旨在为开发者提供一站式的学习资源，帮助提升编程技能和系统设计能力。

## 🗂️ 内容组织

### 1. 算法题解 (hot100)

收集了LeetCode热门100题的详细解析，每道题目都包含:
- 问题描述
- 解题思路
- 多种实现方法
- 代码实现
- 复杂度分析

### 2. 数据结构实现 (cpp_basic/simple_stl)

手写实现了常用的STL容器:
- Vector: 动态数组
- Queue: 队列
- Stack: 栈
- HashTable: 哈希表
- DoublyLinkedList: 双向链表


### 3. Go语言学习 (go_learn)

为C++开发者设计的Go语言学习路径:
- 基础知识
- 中级特性
- 高级应用
- 实战项目
- C++与Go的对比分析

### 4. 系统设计 (system_design)

系统设计相关的学习资料:
- 系统设计原则
- 常见面试题
- 架构设计模式
- 分布式系统概念

### 5. 向量检索技术 (cpp_tech_stack/faiss)

探索现代向量检索技术:
- FAISS库使用
- 向量相似度搜索原理
- 高效索引结构实现

## 🚀 使用指南

每个子目录都包含独立的项目和文档，可以根据需要选择性地学习:

```bash
# 编译STL实现
cd cpp_basic/simple_stl
./build.sh

# 运行算法题测试
cd hot100/{题号}
g++ -std=c++11 test.cpp -o test
./test

# 查看Go学习路径
cd go_learn
cat README.md
```

## 💡 特色亮点

- **全面覆盖**: 从基础算法到高级系统设计，满足不同层次的学习需求
- **实用导向**: 所有实现都包含详细注释和测试用例
- **性能优化**: 关注代码效率，提供优化思路和技巧
- **跨语言学习**: 支持C++和Go语言的对比学习

## 📈 学习路径建议

1. 先学习基础数据结构 (cpp_basic)
2. 练习算法题 (hot100)
3. 了解性能优化技巧 (OS/Cache friendly code)
4. 学习系统设计原则 (system_design)
5. 探索新语言 (go_learn)

## 🔗 参考资源

- [LeetCode](https://leetcode.com/)
- [C++ 参考文档](https://en.cppreference.com/)
- [Go 官方文档](https://golang.org/doc/)
- [系统设计入门](https://github.com/donnemartin/system-design-primer)

---

**欢迎贡献和提出建议!** 如果您有任何问题或想法，请提交issue或pull request。
