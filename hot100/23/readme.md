# 合并K个升序链表

## 问题链接
[LeetCode 23. 合并K个升序链表](https://leetcode.com/problems/merge-k-sorted-lists/)

## 问题描述
给你一个链表数组，每个链表都已经按升序排列。请你将所有链表合并到一个升序链表中，返回合并后的链表。

示例:
输入: lists = [[1,4,5],[1,3,4],[2,6]]
输出: [1,1,2,3,4,4,5,6]

## 解题思路
这道题可以通过优先队列（最小堆）来解决：

### 优先队列法
1. 创建一个最小堆，用于存储所有链表的当前节点
2. 初始化：
   - 将每个链表的头节点加入最小堆
3. 合并过程：
   - 每次从堆中取出最小的节点加入结果链表
   - 如果被取出的节点有下一个节点，将其加入堆中
4. 重复步骤3直到堆为空

### 分治法（可选）
1. 将k个链表配对并将同一对中的链表合并
2. 第一轮合并以后，k个链表被合并成了k/2个链表
3. 重复这个过程，直到最终得到了一个链表

## 代码实现
```cpp
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // 使用优先队列（最小堆）来存储节点
        auto comp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
        priority_queue<ListNode*, vector<ListNode*>, decltype(comp)> pq(comp);
        
        // 将所有链表的头节点加入堆
        for (ListNode* list : lists) {
            if (list) pq.push(list);
        }
        
        ListNode dummy(0);
        ListNode* tail = &dummy;
        
        // 不断从堆中取出最小节点
        while (!pq.empty()) {
            ListNode* node = pq.top();
            pq.pop();
            
            tail->next = node;
            tail = tail->next;
            
            if (node->next) {
                pq.push(node->next);
            }
        }
        
        return dummy.next;
    }
};
```

## 复杂度分析
- 时间复杂度：O(N * log k)，其中 N 是所有节点的总数，k 是链表个数
- 空间复杂度：O(k)，优先队列中最多存储 k 个节点
