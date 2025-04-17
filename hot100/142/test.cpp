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
        ListNode* fast = head;
        ListNode* slow = head;
        while (fast && fast->next) {
            fast = fast->next->next;
            slow = slow->next;
            if (fast == slow) {
                ListNode* cur = head;
                while (cur != slow) {
                    cur = cur->next;
                    slow = slow->next;
                }
                return slow;
            }
        }
        return nullptr;
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
