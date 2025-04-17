## 问题链接
https://leetcode.cn/problems/reverse-linked-list-ii/

## 问题描述
给你单链表的头指针 `head` 和两个整数 `left` 和 `right` ，其中 `left <= right` 。请你反转从位置 `left` 到位置 `right` 的链表节点，返回反转后的链表。

### 示例 1：
![示例1](https://assets.leetcode.com/uploads/2021/02/19/rev2ex2.jpg)

输入：head = [1,2,3,4,5], left = 2, right = 4  
输出：[1,4,3,2,5]

### 示例 2：

输入：head = [5], left = 1, right = 1  
输出：[5]

### 提示：
- 链表中节点数目为 n
- 1 <= n <= 500
- -500 <= Node.val <= 500
- 1 <= left <= right <= n

## 解题思路
要反转链表中的一部分，我们需要找到需要反转部分的前一个节点和后一个节点，然后进行局部反转。具体步骤如下：

1. 创建一个哑节点（dummy node）作为链表的新头部，使得即使需要反转从第一个节点开始的部分也能统一处理
2. 找到反转部分的前一个节点 `prev`
3. 定位到反转起始位置的节点 `curr`
4. 进行局部反转，同时保持对反转部分前后节点的引用
5. 拼接反转后的部分与原链表

## 代码实现
```cpp
class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        // 创建哑节点，简化边界情况处理
        ListNode* dummy = new ListNode(0);
        dummy->next = head;
        
        // prev指向要反转部分的前一个节点
        ListNode* prev = dummy;
        for (int i = 1; i < left; i++) {
            prev = prev->next;
        }
        
        // curr指向要反转的起始节点
        ListNode* curr = prev->next;
        
        // 进行局部反转
        for (int i = 0; i < right - left; i++) {
            ListNode* next = curr->next;
            curr->next = next->next;
            next->next = prev->next;
            prev->next = next;
        }
        
        // 释放哑节点并返回结果
        ListNode* result = dummy->next;
        delete dummy;
        return result;
    }
};
```

## 复杂度分析
- 时间复杂度：O(n)，其中 n 是链表的长度。我们至多需要遍历链表一次。
- 空间复杂度：O(1)，我们只使用了常数个变量。

## 图解算法

假设我们有链表 1→2→3→4→5，需要反转位置2到位置4的节点（即 2→3→4 变为 4→3→2）：

1. 创建哑节点 `dummy` → 1 → 2 → 3 → 4 → 5
2. `prev` 指向节点1（即要反转部分的前一个节点）
3. `curr` 指向节点2（反转的起始节点）
4. 执行反转操作：
   - 第一次迭代：`dummy` → 1 → 3 → 2 → 4 → 5
   - 第二次迭代：`dummy` → 1 → 4 → 3 → 2 → 5

## 优化思路
1. 一次遍历方法：上述实现已经是一次遍历的方法，时间复杂度为O(n)。
2. 递归解法：也可以使用递归来解决这个问题，但空间复杂度会增加到O(n)。
3. 实际应用中，可以添加额外的错误检查，例如确保`left`和`right`的值有效。
4. 如果频繁执行此操作，可以考虑使用更适合插入和删除的数据结构，如双向链表。