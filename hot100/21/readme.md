# 合并两个有序链表

## 问题链接
[LeetCode 21. 合并两个有序链表](https://leetcode.com/problems/merge-two-sorted-lists/)

## 问题描述
将两个升序链表合并为一个新的升序链表并返回。新链表是通过拼接给定的两个链表的所有节点组成的。

示例:
输入: l1 = [1,2,4], l2 = [1,3,4]
输出: [1,1,2,3,4,4]

## 解题思路
这道题可以通过迭代或递归两种方式解决:

### 迭代法
1. 创建一个哑节点(dummy node)作为合并后链表的头节点
2. 维护一个指针cur指向当前处理到的位置
3. 比较两个链表当前节点的值:
   - 取较小值的节点接到cur后面
   - 移动较小值所在链表的指针
   - 移动cur指针
4. 当其中一个链表处理完后,将另一个链表剩余部分接到cur后面
5. 返回dummy.next作为合并后的链表头

### 递归法
1. 递归函数返回合并后的链表头
2. 基本情况:
   - 如果l1为空,返回l2
   - 如果l2为空,返回l1
3. 递归情况:
   - 比较l1和l2当前节点的值
   - 较小值的节点的next指向递归合并剩余节点的结果
   - 返回较小值的节点

## 代码实现
```cpp
class Solution {
public:
    // 迭代解法
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* cur = &dummy;
        
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                cur->next = l1;
                l1 = l1->next;
            } else {
                cur->next = l2;
                l2 = l2->next;
            }
            cur = cur->next;
        }
        
        cur->next = l1 ? l1 : l2;
        return dummy.next;
    }
    
    // 递归解法
    ListNode* mergeTwoListsRecursive(ListNode* l1, ListNode* l2) {
        if (!l1) return l2;
        if (!l2) return l1;
        
        if (l1->val <= l2->val) {
            l1->next = mergeTwoListsRecursive(l1->next, l2);
            return l1;
        } else {
            l2->next = mergeTwoListsRecursive(l1, l2->next);
            return l2;
        }
    }
};
```

## 复杂度分析
### 迭代解法
- 时间复杂度：O(n + m),其中n和m是两个链表的长度
  - 需要遍历两个链表各一次
- 空间复杂度：O(1)
  - 只使用了常数额外空间

### 递归解法
- 时间复杂度：O(n + m)
  - 每个节点都会被访问一次
- 空间复杂度：O(n + m)
  - 递归调用栈的深度为两个链表的总长度

## 关键点
1. 使用哑节点简化头节点的处理
2. 正确处理链表遍历和节点连接
3. 处理特殊情况(空链表)
4. 理解递归解法中递归函数的返回值含义
5. 注意递归解法的空间开销