## 问题链接
[LeetCode 2 - Add Two Numbers](https://leetcode.cn/problems/add-two-numbers/)

## 问题描述
给你两个非空的链表,表示两个非负的整数。它们每位数字都是按照逆序的方式存储的,并且每个节点只能存储一位数字。

请你将两个数相加,并以相同形式返回一个表示和的链表。

你可以假设除了数字 0 之外,这两个数都不会以 0 开头。

### 示例 1：
输入：l1 = [2,4,3], l2 = [5,6,4]
输出：[7,0,8]
解释：342 + 465 = 807

### 示例 2：
输入：l1 = [0], l2 = [0]
输出：[0]

### 示例 3：
输入：l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]
输出：[8,9,9,9,0,0,0,1]
解释：9999999 + 9999 = 10009998

### 提示：
- 每个链表中的节点数在范围 [1, 100] 内
- 0 <= Node.val <= 9
- 题目数据保证列表表示的数字不含前导零

## 解题思路

### 1. 基本思路
- 同时遍历两个链表,对应位置的数字相加
- 需要考虑进位情况
- 当一个链表较长时,继续遍历较长的链表
- 最后需要检查是否还有进位需要增加新节点

### 2. 关键点分析

#### 1. 链表遍历
- 使用两个指针分别遍历两个链表
- 当其中一个链表遍历完时,将对应位置的值视为0继续计算

#### 2. 进位处理
- 使用一个变量carry记录进位
- 每次计算时考虑当前两个数字和进位的和
- 新节点的值为和对10取余
- 新的进位为和除以10的整数部分

#### 3. 处理最后的进位
- 遍历结束后,如果carry为1,需要新增一个值为1的节点


### 代码实现
```cpp
/
Definition for singly-linked list.
struct ListNode {
int val;
ListNode next;
ListNode() : val(0), next(nullptr) {}
ListNode(int x) : val(x), next(nullptr) {}
ListNode(int x, ListNode next) : val(x), next(next) {}
};
/
class Solution {
public:
ListNode addTwoNumbers(ListNode l1, ListNode l2) {
ListNode dummy = new ListNode(0); // 哑节点
ListNode curr = dummy;
int carry = 0; // 进位
// 只要还有节点或者有进位就继续循环
while (l1 != nullptr || l2 != nullptr || carry > 0) {
int sum = carry;
// 获取l1当前节点的值
if (l1 != nullptr) {
sum += l1->val;
l1 = l1->next;
}
// 获取l2当前节点的值
if (l2 != nullptr) {
sum += l2->val;
l2 = l2->next;
}
// 计算新节点的值和新的进位
carry = sum / 10;
curr->next = new ListNode(sum % 10);
curr = curr->next;
}
ListNode result = dummy->next;
delete dummy;
return result;
}
};
```

## 复杂度分析

### 时间复杂度
- O(max(m,n))：其中m和n分别为两个链表的长度
- 需要遍历两个链表中较长的那个

### 空间复杂度
- O(max(m,n))：需要创建一个新链表来存储结果
- 新链表的长度最多为较长链表的长度加1(考虑最后可能的进位)