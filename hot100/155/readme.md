# 最小栈

## 问题链接
[LeetCode 155. 最小栈](https://leetcode.com/problems/min-stack/)

## 问题描述
设计一个支持 push、pop、top 和 getMin 操作的栈数据结构。

实现 MinStack 类:
- MinStack() 初始化堆栈对象。
- void push(int val) 将元素val推入堆栈。
- void pop() 删除堆栈顶部的元素。
- int top() 获取堆栈顶部的元素。
- int getMin() 获取堆栈中的最小元素。

所有操作都应该在 O(1) 时间复杂度内完成。

## 解题思路
1. 使用两个栈来实现最小栈:
   - 一个正常栈用于存储所有元素
   - 一个最小栈用于同步存储当前最小值
2. push操作:
   - 将元素压入正常栈
   - 如果最小栈为空或当前元素小于等于最小栈顶元素,则将其压入最小栈
3. pop操作:
   - 如果正常栈顶元素等于最小栈顶元素,则最小栈也要出栈
   - 正常栈执行出栈操作
4. top操作:
   - 直接返回正常栈的栈顶元素
5. getMin操作:
   - 直接返回最小栈的栈顶元素

## 代码实现
```cpp
class MinStack {
private:
    stack<int> s;      // 正常栈
    stack<int> minS;   // 最小栈
    
public:
    MinStack() {}
    
    void push(int val) {
        s.push(val);
        if (minS.empty() || val <= minS.top()) {
            minS.push(val);
        }
    }
    
    void pop() {
        if (s.top() == minS.top()) {
            minS.pop();
        }
        s.pop();
    }
    
    int top() {
        return s.top();
    }
    
    int getMin() {
        return minS.top();
    }
};
```

## 复杂度分析
- 时间复杂度：所有操作均为 O(1)
- 空间复杂度：O(N)，其中 N 是栈中元素的个数。需要额外的最小栈存储最小值。