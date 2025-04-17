#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>  
#include <climits>

class ListNode {
public:
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

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

    ListNode *detectCycle(ListNode *head) {
        if (head == nullptr) return nullptr;
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast && fast->next)
        {
            fast = fast->next->next;
            slow = slow->next;
            if (fast == slow) {
                ListNode* cur = head;
                while (cur != slow)
                {
                    cur = cur->next;
                    slow = slow->next;
                }
                return cur;
            }
        }
        return nullptr;
    }

    ListNode* middleNode(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        
        while (fast && fast->next) {
            slow = slow->next; // 慢指针移动一步
            fast = fast->next->next; // 快指针移动两步
        }
        
        return slow; // 返回中间节点
    }

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

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        if (test()) {
            std::cout << "PASS: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "FAIL: " << testName << std::endl;
        }
    }

    bool expectEqual(ListNode* expected, ListNode* actual) {
        return expected == actual;
    }

public:
    void runAllTests() {
        runTest("DetectCycle", [this]() {
            ListNode* head = new ListNode(3);
            ListNode* second = new ListNode(2);
            ListNode* third = new ListNode(0);
            ListNode* fourth = new ListNode(-4);
            head->next = second;
            second->next = third;
            third->next = fourth;
            fourth->next = second; // 创建环

            ListNode* expected = second; // 环的起始节点
            ListNode* result = solution.detectCycle(head);
            delete head; delete second; delete third; delete fourth; // 清理内存
            return expectEqual(expected, result);
        });

        runTest("NoCycle", [this]() {
            ListNode* head = new ListNode(1);
            head->next = nullptr; // 无环

            ListNode* expected = nullptr;
            ListNode* result = solution.detectCycle(head);
            delete head; // 清理内存
            return expectEqual(expected, result);
        });

        runTest("MiddleNode", [this]() {
            ListNode* head = new ListNode(1);
            head->next = new ListNode(2);
            head->next->next = new ListNode(3);
            head->next->next->next = new ListNode(4);
            head->next->next->next->next = new ListNode(5);

            ListNode* expected = head->next->next; // 中间节点是 3
            ListNode* result = solution.middleNode(head);
            delete head->next->next->next->next; // 清理内存
            delete head->next->next; // 清理内存
            delete head->next; // 清理内存
            delete head; // 清理内存
            return expectEqual(expected, result);
        });

        // runTest("ReverseList", [this]() {
        //     ListNode* head = new ListNode(1);
        //     head->next = new ListNode(2);
        //     head->next->next = new ListNode(3);

        //     ListNode* expected = new ListNode(3);
        //     expected->next = new ListNode(2);
        //     expected->next->next = new ListNode(1);

        //     ListNode* result = solution.reverseList(head);
        //     delete head->next->next; // 清理内存
        //     delete head->next; // 清理内存
        //     delete head; // 清理内存
        //     delete expected->next->next; // 清理内存
        //     delete expected->next; // 清理内存
        //     delete expected; // 清理内存
        //     return expectEqual(expected, result);
        // });

        // runTest("MergeTwoLists", [this]() {
        //     ListNode* l1 = new ListNode(1);
        //     l1->next = new ListNode(2);
        //     l1->next->next = new ListNode(4);

        //     ListNode* l2 = new ListNode(1);
        //     l2->next = new ListNode(3);
        //     l2->next->next = new ListNode(4);

        //     ListNode* expected = new ListNode(1);
        //     expected->next = new ListNode(1);
        //     expected->next->next = new ListNode(2);
        //     expected->next->next->next = new ListNode(3);
        //     expected->next->next->next->next = new ListNode(4);
        //     expected->next->next->next->next->next = new ListNode(4);

        //     ListNode* result = solution.mergeTwoLists(l1, l2);
        //     delete l1->next->next; // 清理内存
        //     delete l1->next; // 清理内存
        //     delete l1; // 清理内存
        //     delete l2->next->next; // 清理内存
        //     delete l2->next; // 清理内存
        //     delete l2; // 清理内存
        //     delete expected->next->next->next->next->next; // 清理内存
        //     delete expected->next->next->next->next; // 清理内存
        //     delete expected->next->next; // 清理内存
        //     delete expected->next; // 清理内存
        //     delete expected; // 清理内存
        //     return expectEqual(expected, result);
        // });

        // // 新增测试用例
        // runTest("MiddleNodeEven", [this]() {
        //     ListNode* head = new ListNode(1);
        //     head->next = new ListNode(2);
        //     head->next->next = new ListNode(3);
        //     head->next->next->next = new ListNode(4);

        //     ListNode* expected = head->next->next; // 中间节点是 3
        //     ListNode* result = solution.middleNode(head);
        //     delete head->next->next->next; // 清理内存
        //     delete head->next->next; // 清理内存
        //     delete head->next; // 清理内存
        //     delete head; // 清理内存
        //     return expectEqual(expected, result);
        // });

        // runTest("ReverseListSingleNode", [this]() {
        //     ListNode* head = new ListNode(1);

        //     ListNode* expected = new ListNode(1); // 反转单节点链表仍然是它自己

        //     ListNode* result = solution.reverseList(head);
        //     delete head; // 清理内存
        //     delete expected; // 清理内存
        //     return expectEqual(expected, result);
        // });

        // runTest("MergeTwoListsEmpty", [this]() {
        //     ListNode* l1 = nullptr; // 空链表
        //     ListNode* l2 = new ListNode(1);
        //     l2->next = new ListNode(2);

        //     ListNode* expected = l2; // 合并结果应为 l2

        //     ListNode* result = solution.mergeTwoLists(l1, l2);
        //     delete l2->next; // 清理内存
        //     delete l2; // 清理内存
        //     return expectEqual(expected, result);
        // });

        // runTest("MergeTwoListsBothEmpty", [this]() {
        //     ListNode* l1 = nullptr; // 空链表
        //     ListNode* l2 = nullptr; // 空链表

        //     ListNode* expected = nullptr; // 合并结果应为空

        //     ListNode* result = solution.mergeTwoLists(l1, l2);
        //     return expectEqual(expected, result);
        // });

        std::cout << "\n测试结果: " << passed << " 通过, " 
                  << (total - passed) << " 失败, " 
                  << total << " 总计" << std::endl;
    }
};

int main() {
    TestFramework testFramework;
    testFramework.runAllTests();
    return 0;
}
