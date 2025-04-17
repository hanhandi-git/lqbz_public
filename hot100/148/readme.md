# 排序链表

## 问题链接
[LeetCode 148. Sort List](https://leetcode.com/problems/sort-list/)

## 问题描述
给你链表的头结点 head，请将其按升序排序并返回排序后的链表。

要求:
- 时间复杂度为 O(nlogn)
- 空间复杂度为 O(1)

## 解题思路
1. 根据时间复杂度 O(nlogn) 的要求,可以考虑使用:
   - 归并排序
   - 快速排序
   - 堆排序
   
2. 由于链表不支持随机访问,快速排序的效率会受到影响。而堆排序需要额外空间,因此最适合使用归并排序。

3. 归并排序的主要步骤:
   - 使用快慢指针找到链表中点,将链表分为两半
   - 对两个子链表分别递归排序
   - 将两个排序后的子链表合并为一个有序链表

4. 具体实现细节:
   - 使用快慢指针(fast和slow)找中点:
     * fast每次移动两步
     * slow每次移动一步
     * 当fast到达末尾时,slow指向中点
   - 递归终止条件是链表为空或只有一个节点
   - 合并两个有序链表使用双指针法

## 代码实现
```cpp
class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if (!head || !head->next) return head;
        
        // 找到中点
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        // 断开链表
        ListNode* mid = slow->next;
        slow->next = nullptr;
        
        // 递归排序
        ListNode* left = sortList(head);
        ListNode* right = sortList(mid);
        
        // 合并有序链表
        return merge(left, right);
    }
    
private:
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* curr = &dummy;
        
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                curr->next = l1;
                l1 = l1->next;
            } else {
                curr->next = l2;
                l2 = l2->next;
            }
            curr = curr->next;
        }
        
        curr->next = l1 ? l1 : l2;
        return dummy.next;
    }
};
```

## 复杂度分析
- 时间复杂度：O(nlogn)
  * 递归树的深度是 logn
  * 每一层需要 O(n) 的时间合并
  * 总时间复杂度为 O(nlogn)
- 空间复杂度：O(1)
  * 虽然有递归调用
  * 但是递归调用的空间是函数调用栈,不计入空间复杂度
  * 除此之外只使用了常数个变量

## 示例
```
输入: head = [4,2,1,3]
输出: [1,2,3,4]

输入: head = [-1,5,3,4,0]
输出: [-1,0,3,4,5]

输入: head = []
输出: []
```