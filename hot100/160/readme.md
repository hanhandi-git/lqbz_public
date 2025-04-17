# 相交链表

## 问题链接
[LeetCode 160. 相交链表](https://leetcode.com/problems/intersection-of-two-linked-lists/)

## 问题描述
给你两个单链表的头节点 headA 和 headB,请你找出并返回两个单链表相交的起始节点。如果两个链表不存在相交节点,返回 null。

题目数据保证整个链式结构中不存在环。

注意,函数返回结果后,链表必须保持其原始结构。

## 解题思路
1. 使用双指针法解决此问题:
   - 创建两个指针 pA 和 pB,分别指向两个链表的头节点
   - 当 pA 到达链表末尾时,将它重定向到链表 B 的头节点
   - 当 pB 到达链表末尾时,将它重定向到链表 A 的头节点
   - 当 pA 和 pB 相遇时,即为相交节点
   
2. 原理分析:
   - 假设链表 A 的长度为 a,链表 B 的长度为 b
   - 相交部分的长度为 c
   - 则 pA 走过的距离为 a + (b-c),pB 走过的距离为 b + (a-c)
   - 当两个指针相遇时,它们走过的距离相等,即为相交点

## 代码实现
```cpp
class Solution {
public:
ListNode getIntersectionNode(ListNode headA, ListNode headB) {
if (!headA || !headB) return nullptr;
ListNode pA = headA;
ListNode pB = headB;
while (pA != pB) {
pA = pA ? pA->next : headB;
pB = pB ? pB->next : headA;
}
return pA;
}
};
```

## 复杂度分析
- 时间复杂度：O(n)，其中 n 是两个链表的总长度
- 空间复杂度：O(1)，只使用了两个指针