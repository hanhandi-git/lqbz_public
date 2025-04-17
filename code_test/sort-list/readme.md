# 排序链表

## 问题链接
[LeetCode 148. 排序链表](https://leetcode.com/problems/sort-list/)

## 问题描述
给定一个链表的头节点 `head`，请将该链表按升序排列，并返回排序后的链表。

## 解题思路
1. 使用归并排序算法来解决这个问题。
2. 归并排序的基本思想是：
   - 将链表分成两半，递归地对每一半进行排序，然后将两个已排序的链表合并。
3. 分割链表的过程可以使用快慢指针来实现：
   - 使用快慢指针找到链表的中间节点。
   - 将链表分成两部分，递归地对这两部分进行排序。
4. 合并两个已排序的链表的过程：
   - 创建一个虚拟头节点，逐步比较两个链表的节点，将较小的节点添加到结果链表中。

## 代码实现
```cpp
class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) return head; // 基本情况：链表为空或只有一个节点
        
        // 使用快慢指针找到中间节点
        ListNode* mid = getMid(head);
        ListNode* left = sortList(head); // 递归排序左半部分
        ListNode* right = sortList(mid);  // 递归排序右半部分
        
        return merge(left, right); // 合并已排序的链表
    }
    
private:
    ListNode* getMid(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head->next;
        
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        ListNode* mid = slow->next; // 中间节点
        slow->next = nullptr; // 切断链表
        return mid;
    }
    
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0); // 虚拟头节点
        ListNode* tail = &dummy;
        
        while (l1 && l2) {
            if (l1->val < l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next; // 移动尾指针
        }
        
        tail->next = l1 ? l1 : l2; // 连接剩余部分
        return dummy.next; // 返回合并后的链表
    }
};
```

## 复杂度分析
- 时间复杂度：O(n log n)，其中 n 是链表的长度。归并排序的时间复杂度为 O(n log n)。
- 空间复杂度：O(log n)，递归调用栈的深度。

## 扩展：其他相关问题

### 1. 合并两个有序链表

问题描述：将两个有序链表合并为一个新的有序链表。

例子：
```
输入：l1 = [1,2,4], l2 = [1,3,4]
输出：[1,1,2,3,4,4]
```

解题思路：
* 可以使用归并的思想，逐步比较两个链表的节点，将较小的节点添加到结果链表中。

### 2. K个一组翻转链表

问题描述：给定一个链表，每 k 个节点一组进行翻转。

例子：
```
输入：head = [1,2,3,4,5], k = 2
输出：[2,1,4,3,5]
```

解题思路：
* 可以使用递归或迭代的方法，每次翻转 k 个节点，然后递归处理剩余部分。
```cpp
ListNode* reverseKGroup(ListNode* head, int k) {
    ListNode dummy(0); // 虚拟头节点
    dummy.next = head;
    ListNode* prevGroupEnd = &dummy;

    while (true) {
        ListNode* kEnd = prevGroupEnd;
        for (int i = 0; i < k; ++i) {
            kEnd = kEnd->next;
            if (!kEnd) return dummy.next; // 不足k个节点，返回结果
        }

        ListNode* groupStart = prevGroupEnd->next;
        ListNode* nextGroupStart = kEnd->next;
        kEnd->next = nullptr; // 切断当前组

        // 反转当前组
        prevGroupEnd->next = reverse(groupStart);
        groupStart->next = nextGroupStart; // 连接下一个组
        prevGroupEnd = groupStart; // 更新前一组的结束节点
    }
}
```

### 3. 删除链表的倒数第N个节点

问题描述：给定一个链表，删除链表的倒数第 N 个节点。

例子：
```
输入：head = [1,2,3,4,5], n = 2
输出：[1,2,3,5]
```

解题思路：
* 使用快慢指针，快指针先走 n 步，然后快慢指针一起走，直到快指针到达链表末尾。

这些变体问题都是基于链表的基本操作，但可能需要使用不同的技巧或算法来解决。它们展示了如何将链表的操作应用到各种问题中。