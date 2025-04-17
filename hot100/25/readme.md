````thinking
让我思考一下如何改写这个readme来介绍k个一组反转链表的问题...

首先需要理解25题与24题的区别:
- 24题是两两交换,也就是k=2的特殊情况
- 25题需要处理任意k值的情况,复杂度更高
- 25题需要考虑最后不足k个节点时的处理

解题思路上:
1. 迭代法的核心步骤会有变化:
- 需要先判断剩余节点是否足够k个
- 需要实现k个节点的反转,不再是简单的两两交换
- 需要维护更多的指针来完成反转

2. 递归法的思路也需要调整:
- 基本情况需要考虑节点数小于k的情况
- 每次处理k个节点而不是2个
- 递归返回的仍然是反转后的新头节点

代码实现上:
- 需要增加一个反转k个节点的辅助函数
- 需要增加判断剩余节点数量的逻辑
- 其他整体框架可以保持类似

让我组织一下具体的readme内容...
````

# K 个一组翻转链表

## 问题链接
[LeetCode 25. K 个一组翻转链表](https://leetcode.com/problems/reverse-nodes-in-k-group/)

## 问题描述
给你链表的头节点 head ，每 k 个节点一组进行翻转，请你返回修改后的链表。

k 是一个正整数，它的值小于或等于链表的长度。如果节点总数不是 k 的整数倍，那么请将最后剩余的节点保持原有顺序。

示例:
输入：head = [1,2,3,4,5], k = 2
输出：[2,1,4,3,5]

输入：head = [1,2,3,4,5], k = 3
输出：[3,2,1,4,5]

## 解题思路
这道题可以通过迭代或递归两种方式解决:

### 迭代法
1. 创建一个哑节点(dummy node)作为链表的头节点
2. 维护一个指针prev指向待反转的k个节点之前的节点
3. 对于每组k个节点:
   - 首先判断从当前位置开始是否有k个节点
   - 如果不足k个节点，保持原有顺序
   - 如果有k个节点:
     * 保存这k个节点的首尾节点
     * 反转这k个节点
     * 调整前后连接
   - 移动prev指针到下一组的前一个位置
4. 返回dummy.next作为反转后的链表头

### 递归法
1. 递归函数返回反转后的链表头
2. 基本情况:
   - 如果链表为空，直接返回null
   - 首先判断是否有k个节点
   - 如果不足k个节点，直接返回head
3. 递归情况:
   - 反转前k个节点
   - 原来的头节点指向递归处理剩余节点的结果
   - 返回反转后的新头节点

## 代码实现
```cpp
class Solution {
public:
    // 迭代解法
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prev = &dummy;
        
        while (head) {
            // 检查剩余节点是否足够k个
            ListNode* tail = prev;
            for (int i = 0; i < k; i++) {
                tail = tail->next;
                if (!tail) return dummy.next;
            }
            
            // 保存下一组的起始位置
            ListNode* next = tail->next;
            
            // 反转k个节点
            pair<ListNode*, ListNode*> result = reverseList(head, tail);
            head = result.first;
            tail = result.second;
            
            // 连接前后的节点
            prev->next = head;
            tail->next = next;
            
            // 移动指针
            prev = tail;
            head = next;
        }
        
        return dummy.next;
    }
    
    // 反转链表的辅助函数，返回新的头和尾
    pair<ListNode*, ListNode*> reverseList(ListNode* head, ListNode* tail) {
        ListNode* prev = tail->next;
        ListNode* curr = head;
        while (prev != tail) {
            ListNode* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        return {tail, head};
    }
    
    // 递归解法
    ListNode* reverseKGroupRecursive(ListNode* head, int k) {
        // 检查是否有k个节点
        ListNode* curr = head;
        int count = 0;
        while (curr && count < k) {
            curr = curr->next;
            count++;
        }
        if (count < k) return head;
        
        // 反转前k个节点
        curr = head;
        ListNode* prev = nullptr;
        for (int i = 0; i < k; i++) {
            ListNode* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        
        // 递归处理剩余节点
        head->next = reverseKGroupRecursive(curr, k);
        
        return prev;
    }
};
```

## 复杂度分析
### 迭代解法
- 时间复杂度：O(n)
  - 需要遍历整个链表
  - 每个节点会被反转一次
- 空间复杂度：O(1)
  - 只使用了常数额外空间

### 递归解法
- 时间复杂度：O(n)
  - 每个节点都会被访问一次
- 空间复杂度：O(n/k)
  - 递归调用栈的深度为链表长度除以k

## 关键点
1. 使用哑节点简化头节点的处理
2. 在反转前需要先判断剩余节点是否足够k个
3. 正确处理节点之间的连接关系
4. 实现一个反转指定范围内节点的辅助函数
5. 理解递归解法中的基本情况和递归情况
6. 注意保持最后不足k个节点的原有顺序
