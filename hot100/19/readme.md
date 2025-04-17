# 删除链表的倒数第N个节点

## 问题链接
[LeetCode 19. 删除链表的倒数第N个节点](https://leetcode.com/problems/remove-nth-node-from-end-of-list/)

## 问题描述
给你一个链表，删除链表的倒数第 n 个结点，并且返回链表的头结点。

示例:
```
输入：head = [1,2,3,4,5], n = 2
输出：[1,2,3,5]

输入：head = [1], n = 1
输出：[]

输入：head = [1,2], n = 1
输出：[1]
```

## 解题思路
这是一个链表操作问题,可以通过以下步骤解决:

1. 使用快慢指针法:
   - 设置一个dummy节点指向head
   - 快指针先移动n步
   - 然后快慢指针同时移动
   - 当快指针到达末尾时,慢指针的下一个节点就是要删除的节点
2. 特殊情况处理:
   - 链表为空
   - 删除的是头节点
   - n大于链表长度
3. 使用dummy节点可以统一处理所有情况,避免对头节点的特殊处理

## 代码实现
```cpp
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode* dummy = new ListNode(0);
        dummy->next = head;
        ListNode* fast = dummy;
        ListNode* slow = dummy;
        
        // 快指针先移动n步
        for (int i = 0; i < n; i++) {
            fast = fast->next;
        }
        
        // 同时移动快慢指针
        while (fast->next != nullptr) {
            fast = fast->next;
            slow = slow->next;
        }
        
        // 删除目标节点
        slow->next = slow->next->next;
        
        ListNode* result = dummy->next;
        delete dummy;
        return result;
    }
};
```

## 复杂度分析
- 时间复杂度：O(N),其中N是链表的长度
  - 只需要一次遍历
- 空间复杂度：O(1)
  - 只使用了常数个指针

## 关键点
1. 使用dummy节点简化边界情况处理
2. 快慢指针的使用技巧
3. 正确计算快慢指针的距离
4. 注意内存管理,防止内存泄漏
5. 链表节点的删除操作要注意指针更新顺序
