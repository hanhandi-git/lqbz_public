## 问题链接
https://leetcode.cn/problems/linked-list-cycle/

## 问题描述
给定一个链表，判断链表中是否有环。为了表示给定链表中的环，我们使用一个整数 pos 来表示链表尾连接到链表中的某个节点。如果 pos 是 -1，则在该链表中没有环。

### 示例 1：

输入：head = [3,2,0,-4], pos = 1  
输出：true  
解释：链表中有一个环，环的起始节点是链表中的第二个节点。

### 示例 2：

输入：head = [1,2], pos = 0  
输出：true  
解释：链表中有一个环，环的起始节点是链表中的第一个节点。

### 示例 3：

输入：head = [1], pos = -1  
输出：false  
解释：链表中没有环。

### 提示：

- 链表中节点的数目范围是 [0, 10^4]。
- -10^5 <= Node.val <= 10^5
- pos 的值为 -1 或者链表中的一个有效索引。

## 解题思路
本题可以使用快慢指针的方法来解决。主要步骤如下：

1. 使用两个指针，快指针（fast）和慢指针（slow）。初始时，两个指针都指向链表的头节点。
2. 快指针每次移动两步，慢指针每次移动一步。
3. 如果链表中存在环，快指针和慢指针最终会相遇；如果不存在环，快指针会到达链表的末尾（nullptr）。

## 代码实现
```cpp
class Solution {
public:
    bool hasCycle(ListNode *head) {
        if (!head) return false; // 如果链表为空，返回 false
        
        ListNode *slow = head;
        ListNode *fast = head;
        
        while (fast && fast->next) {
            slow = slow->next; // 慢指针移动一步
            fast = fast->next->next; // 快指针移动两步
            
            if (slow == fast) { // 如果相遇，说明有环
                return true;
            }
        }
        
        return false; // 如果快指针到达链表末尾，返回 false
    }
};
```

## 复杂度分析
- 时间复杂度：O(n)，其中 n 是链表的长度。快慢指针最多遍历链表两次。
- 空间复杂度：O(1)，只使用了常数级的额外空间。

## 优化思路
1. 可以在遍历过程中加入一些剪枝条件，例如：
   - 如果链表为空，直接返回 false。
   - 如果链表只有一个节点且没有环，直接返回 false。

2. 对于特殊情况的处理：
   - 如果链表长度小于 2，直接返回 false。

通过这些优化，我们可以在某些情况下进一步提高算法的效率。

## 扩展问题及解题思路
1. **链表的中间节点**
   - **问题链接**: https://leetcode.cn/problems/middle-of-the-linked-list/
   - **问题描述**: 给定一个单链表，返回中间节点。如果有两个中间节点，则返回第二个中间节点。
   - **解题思路**: 使用快慢指针，快指针每次移动两步，慢指针每次移动一步。当快指针到达链表末尾时，慢指针正好在中间节点。
   - **代码实现**:   
   ```cpp
   class Solution {
   public:
       ListNode* middleNode(ListNode* head) {
           ListNode* slow = head;
           ListNode* fast = head;
           
           while (fast && fast->next) {
               slow = slow->next; // 慢指针移动一步
               fast = fast->next->next; // 快指针移动两步
           }
           
           return slow; // 返回中间节点
       }
   };   
   ```

2. **链表反转**
   - **问题链接**: https://leetcode.cn/problems/reverse-linked-list/
   - **问题描述**: 反转一个单链表。
   - **解题思路**: 使用三个指针 prev、curr 和 next，逐步反转链表的指向。
   - **代码实现**:   
   ```cpp
   class Solution {
   public:
       ListNode* reverseList(ListNode* head) {
           ListNode* prev = nullptr;
           ListNode* curr = head;
           
           while (curr) {
               ListNode* next = curr->next; // 保存下一个节点
               curr->next = prev; // 反转当前节点的指向
               prev = curr; // 移动 prev 和 curr
               curr = next;
           }
           
           return prev; // 返回新的头节点
       }
   };   
   ```

3. **合并两个有序链表**
   - **问题链接**: https://leetcode.cn/problems/merge-two-sorted-lists/
   - **问题描述**: 将两个有序链表合并为一个新的有序链表。
   - **解题思路**: 使用两个指针分别遍历两个链表，比较节点值并将较小的节点连接到新链表中。
   - **代码实现**:   
   ```cpp
   class Solution {
   public:
       ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
           ListNode* dummy = new ListNode(0); // 创建一个虚拟头节点
           ListNode* tail = dummy;
           
           while (l1 && l2) {
               if (l1->val < l2->val) {
                   tail->next = l1; // 将较小的节点连接到新链表
                   l1 = l1->next; // 移动 l1
               } else {
                   tail->next = l2; // 将较小的节点连接到新链表
                   l2 = l2->next; // 移动 l2
               }
               tail = tail->next; // 移动尾节点
           }
           
           tail->next = l1 ? l1 : l2; // 连接剩余的节点
           return dummy->next; // 返回合并后的链表
       }
   };   
   ```

4. **环形链表 II**
   - **问题链接**: https://leetcode.cn/problems/linked-list-cycle-ii/
   - **问题描述**: 如果链表中存在环，返回环的起始节点。
   - **解题思路**: 使用快慢指针找到相遇点后，再使用一个指针从头节点开始，两个指针相遇时即为环的起始节点。
   - **代码实现**:   
```cpp
   class Solution {
   public:
       ListNode *detectCycle(ListNode *head) {
           if (!head) return nullptr;
           
           ListNode *slow = head;
           ListNode *fast = head;
           
           // 找到相遇点
           while (fast && fast->next) {
               slow = slow->next;
               fast = fast->next->next;
               if (slow == fast) {
                   // 找到环的起始节点
                   ListNode *entry = head;
                   while (entry != slow) {
                       entry = entry->next;
                       slow = slow->next;
                   }
                   return entry; // 返回环的起始节点
               }
           }
           
           return nullptr; // 如果没有环，返回 nullptr
       }
   };   
```

通过理解和实现这些变体，可以加深对链表操作的理解，提高解决复杂问题的能力。


