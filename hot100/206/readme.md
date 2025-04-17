# 反转链表

## 问题链接
[LeetCode 206. Reverse Linked List](https://leetcode.com/problems/reverse-linked-list/)

## 问题描述
给你单链表的头节点 `head` ，请你反转链表，并返回反转后的链表。

## 解题思路
1. 使用迭代或递归的方法来反转链表。
2. 迭代方法：使用三个指针（prev, current, next）遍历链表，逐个反转节点的指向。
3. 递归方法：递归到链表末尾，然后在回溯过程中反转节点的指向。

## 代码实现
```cpp
// 迭代方法
class Solution {
public:
    ListNode reverseList(ListNode head) {
        ListNode prev = nullptr;
        ListNode curr = head;
        while (curr != nullptr) {
            ListNode nextTemp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextTemp;
        }
        return prev;
    }
};
// 递归方法
class Solution {
public:
    ListNode reverseList(ListNode head) {
        if (head == nullptr || head->next == nullptr) {
            return head;
        }
        ListNode p = reverseList(head->next);
        head->next->next = head;
        head->next = nullptr;
        return p;
    }
};
```
## 复杂度分析
- 时间复杂度：O(n)，其中 n 是链表的长度。
- 空间复杂度：
  - 迭代方法：O(1)
  - 递归方法：O(n)，由于递归调用栈的空间

## 扩展：其他相关问题
1. 反转链表的一部分
2. K 个一组翻转链表
3. 两两交换链表中的节点

这些变体问题都是基于反转链表的基本操作，但增加了一些额外的条件或复杂性。掌握基本的反转链表操作后，这些变体问题也能更容易地解决。