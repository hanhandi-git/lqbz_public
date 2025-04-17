#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>  

class ListNode {
public:
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    bool hasCycle(ListNode *head) {
        if (head == nullptr) return false;
        ListNode* fast = head;
        ListNode* slow = head;
        while (fast && fast->next)
        {
            fast = fast->next->next;
            slow = slow->next;
            if (slow == fast) return true;
        }
        return false;
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

    bool expectEqual(bool expected, bool actual) {
        return expected == actual;
    }

public:
    void runAllTests() {
        runTest("CycleExists", [this]() {
            ListNode* head = new ListNode(3);
            ListNode* second = new ListNode(2);
            ListNode* third = new ListNode(0);
            ListNode* fourth = new ListNode(-4);
            head->next = second;
            second->next = third;
            third->next = fourth;
            fourth->next = second; // 创建环

            bool expected = true;
            bool result = solution.hasCycle(head);
            delete head; delete second; delete third; delete fourth; // 清理内存
            return expectEqual(expected, result);
        });

        runTest("NoCycle", [this]() {
            ListNode* head = new ListNode(1);
            head->next = nullptr; // 无环

            bool expected = false;
            bool result = solution.hasCycle(head);
            delete head; // 清理内存
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
