<think>
Let me analyze this request. The user has shared a README.md file for problem LeetCode 146 (LRU Cache), but is asking me to modify it to create a solution report for "重排链表" (Reorder List), which should be LeetCode 143.

First, I need to understand what LeetCode 143 (Reorder List) is about:

LeetCode 143 asks to reorder a linked list from L0 → L1 → L2 → ... → Ln-1 → Ln to L0 → Ln → L1 → Ln-1 → L2 → Ln-2 → ...

The solution typically involves:
1. Finding the middle of the linked list
2. Reversing the second half
3. Merging the first half with the reversed second half

I'll create a README.md for problem 143 "重排链表" (Reorder List) following the same structure as the provided template:
1. Problem link
2. Problem description
3. Solution approach
4. Code implementation
5. Complexity analysis
6. Extensions (if any)

The language appears to be Chinese, so I'll maintain that.
</think>

# 重排链表

## 问题链接
[LeetCode 143. Reorder List](https://leetcode.com/problems/reorder-list/)

## 问题描述
给定一个单链表 L 的头节点 head ，请将其重新排列为：L0→Ln→L1→Ln-1→L2→Ln-2→…

不能只是单纯的改变节点内部的值，而是需要实际的进行节点交换。

## 解题思路
这个问题可以分为三个步骤来解决：
1. 找到链表的中间节点，将链表分为两部分
2. 将后半部分链表反转
3. 将前半部分和反转后的后半部分合并（交错合并）

## 代码实现
```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next || !head->next->next) return;
        
        // 步骤1：找到中间节点
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        // 分割链表，slow指向前半部分的尾节点
        ListNode* mid = slow->next;
        slow->next = nullptr;
        
        // 步骤2：反转后半部分链表
        ListNode* prev = nullptr;
        ListNode* curr = mid;
        ListNode* next = nullptr;
        while (curr) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        
        // 步骤3：合并两个链表
        ListNode* first = head;
        ListNode* second = prev; // prev现在是反转后链表的头节点
        ListNode* temp1, *temp2;
        
        while (second) {
            // 保存下一个节点
            temp1 = first->next;
            temp2 = second->next;
            
            // 交错连接
            first->next = second;
            second->next = temp1;
            
            // 移动指针
            first = temp1;
            second = temp2;
        }
    }
};
```

## 复杂度分析
- 时间复杂度：O(n)，其中n是链表的长度。
  - 找到中间节点需要O(n/2)
  - 反转链表需要O(n/2)
  - 合并链表需要O(n/2)
  - 总体为O(n)
- 空间复杂度：O(1)，只使用了常数个额外变量，没有使用额外的数据结构。

## 解题关键点
1. 使用快慢指针找到链表中点
2. 链表反转是一个常见操作，需要熟练掌握
3. 在合并过程中需要注意保存下一个节点的引用，避免链接丢失

## 相关题目
1. [206. 反转链表](https://leetcode.com/problems/reverse-linked-list/)
2. [876. 链表的中间结点](https://leetcode.com/problems/middle-of-the-linked-list/)
3. [21. 合并两个有序链表](https://leetcode.com/problems/merge-two-sorted-lists/)

这些链表操作的基本技巧（寻找中点、反转、合并）在许多链表类问题中都非常有用。
