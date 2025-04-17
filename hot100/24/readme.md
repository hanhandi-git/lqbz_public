# 两两交换链表中的节点

## 问题链接
[LeetCode 24. 两两交换链表中的节点](https://leetcode.com/problems/swap-nodes-in-pairs/)

## 问题描述
给你一个链表，两两交换其中相邻的节点，并返回交换后链表的头节点。你必须在不修改节点内部的值的情况下完成本题（即，只能进行节点交换）。

示例:
输入: head = [1,2,3,4]
输出: [2,1,4,3]

## 解题思路
这道题可以通过迭代或递归两种方式解决:

### 迭代法
1. 创建一个哑节点(dummy node)作为链表的头节点
2. 维护一个指针prev指向待交换的两个节点之前的节点
3. 对于每一对需要交换的节点:
   - 保存第一个节点node1和第二个节点node2
   - 调整指针完成交换:
     * prev->next = node2
     * node1->next = node2->next
     * node2->next = node1
   - 移动prev指针到下一对节点之前
4. 返回dummy.next作为交换后的链表头

### 递归法
1. 递归函数返回交换后的链表头
2. 基本情况:
   - 如果链表为空或只有一个节点,直接返回
3. 递归情况:
   - 保存第一个节点head和第二个节点next
   - head->next指向递归处理剩余节点的结果
   - next->next指向head
   - 返回next作为新的头节点

## 代码实现
```cpp
class Solution {
public:
    // 迭代解法
    ListNode* swapPairs(ListNode* head) {
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prev = &dummy;
        
        while (prev->next && prev->next->next) {
            ListNode* node1 = prev->next;
            ListNode* node2 = node1->next;
            
            prev->next = node2;
            node1->next = node2->next;
            node2->next = node1;
            
            prev = node1;
        }
        
        return dummy.next;
    }
    
    // 递归解法
    ListNode* swapPairsRecursive(ListNode* head) {
        if (!head || !head->next) return head;
        
        ListNode* next = head->next;
        head->next = swapPairsRecursive(next->next);
        next->next = head;
        
        return next;
    }
};
```

## 复杂度分析
### 迭代解法
- 时间复杂度：O(n),其中n是链表的长度
  - 需要遍历一遍链表,每次处理两个节点
- 空间复杂度：O(1)
  - 只使用了常数额外空间

### 递归解法
- 时间复杂度：O(n)
  - 每个节点都会被访问一次
- 空间复杂度：O(n)
  - 递归调用栈的深度为链表长度的一半
这里可以给出解释：
1 -> 2 -> 3 -> 4 -> null
head = 1, next = 2
等待递归处理[3->4]的结果...
head = 3, next = 4
head->next = null  (因为4后面是null)
4 -> 3 -> null
返回4作为新头
head(1)->next = 4  (上一步返回的头)
next(2)->next = 1
2 -> 1 -> 4 -> 3 -> null

## 关键点
1. 使用哑节点简化头节点的交换处理
2. 正确保存和调整节点的next指针
3. 处理特殊情况(空链表或只有一个节点)
4. 理解递归解法中每次返回交换后的新头节点
5. 注意不能修改节点的值,只能通过调整指针实现交换