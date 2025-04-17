# 回文链表

## 问题链接
[LeetCode 234. 回文链表](https://leetcode.com/problems/palindrome-linked-list/)

## 问题描述
给你一个单链表的头节点 head,请你判断该链表是否为回文链表。如果是,返回 true;否则,返回 false。

## 解题思路
1. 使用快慢指针找到链表的中点
2. 反转后半部分链表
3. 将前半部分与反转后的后半部分进行比较
4. 恢复链表结构(可选)

具体步骤:
1. 使用快慢指针,快指针每次走两步,慢指针每次走一步,当快指针到达末尾时,慢指针刚好在中点
2. 从慢指针开始反转后半部分链表
3. 从头节点和反转后的后半部分开始比较
4. 如果所有节点值都相同,则是回文链表

## 代码实现
```cpp
class Solution {
public:
bool isPalindrome(ListNode head) {
if (head == nullptr || head->next == nullptr) {
return true;
}
// 找到中点
ListNode slow = head;
ListNode fast = head;
while (fast->next && fast->next->next) {
slow = slow->next;
fast = fast->next->next;
}
// 反转后半部分
ListNode secondHalf = reverseList(slow->next);
// 比较前半部分和反转后的后半部分
ListNode firstHalf = head;
ListNode temp = secondHalf; // 保存起点以便之后恢复
bool result = true;
while (secondHalf != nullptr) {
if (firstHalf->val != secondHalf->val) {
result = false;
break;
}
firstHalf = firstHalf->next;
secondHalf = secondHalf->next;
}
// 恢复链表结构(可选)
slow->next = reverseList(temp);
return result;
}
private:
ListNode reverseList(ListNode head) {
ListNode prev = nullptr;
ListNode curr = head;
while (curr != nullptr) {
ListNode next = curr->next;
curr->next = prev;
prev = curr;
curr = next;
}
return prev;
}
};
```

## 复杂度分析
- 时间复杂度：O(n),其中 n 是链表的长度
- 空间复杂度：O(1),只使用了常数级别的额外空间

## 关键点
1. 使用快慢指针找中点的技巧
2. 链表反转的实现
3. 注意考虑奇数和偶数长度的情况
4. 可以选择是否恢复原链表结构
