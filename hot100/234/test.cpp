#include <iostream>
#include <functional>
#include <string>
#include <vector>

// 链表节点定义
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 辅助函数：打印链表
void printList(ListNode* head) {
    while (head != nullptr) {
        std::cout << head->val << " ";
        head = head->next;
    }
    std::cout << std::endl;
}

class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if (head == nullptr) return true;
        ListNode* fast = head;
        ListNode* slow = head;
        while (fast && fast->next) {
            fast = fast->next->next;
            slow = slow->next;
        }
        ListNode* new_mid = reverseList(slow->next);
        while (head && new_mid) {
            if (head->val != new_mid->val) {
                return false;
            }
            head = head->next;
            new_mid = new_mid->next;
        }
        return true;
    } 

private:
    ListNode* reverseList(ListNode* head) {
        ListNode* cur = head;
        ListNode* pre = nullptr;
        while (cur) {
            auto next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        return pre;
    }
};

// 测试框架
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    // 辅助函数：从vector创建链表
    ListNode* createList(const std::vector<int>& values) {
        if (values.empty()) return nullptr;
        
        ListNode* head = new ListNode(values[0]);
        ListNode* current = head;
        for (size_t i = 1; i < values.size(); i++) {
            current->next = new ListNode(values[i]);
            current = current->next;
        }
        return head;
    }

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        if (test()) {
            std::cout << "通过: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "失败: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        // 基本测试
        runTest("基本测试 - 回文链表 [1,2,2,1]", [this]() {
            ListNode* head = createList({1, 2, 2, 1});
            return solution.isPalindrome(head);
        });

        runTest("基本测试 - 非回文链表 [1,2,3]", [this]() {
            ListNode* head = createList({1, 2, 3});
            return !solution.isPalindrome(head);
        });

        // 边界测试
        runTest("边界测试 - 空链表", [this]() {
            return solution.isPalindrome(nullptr);
        });

        runTest("边界测试 - 单节点链表", [this]() {
            ListNode* head = new ListNode(1);
            return solution.isPalindrome(head);
        });

        // 特殊测试
        runTest("特殊测试 - 奇数长度回文 [1,2,3,2,1]", [this]() {
            ListNode* head = createList({1, 2, 3, 2, 1});
            return solution.isPalindrome(head);
        });

        runTest("特殊测试 - 偶数长度非回文 [1,2,2,3]", [this]() {
            ListNode* head = createList({1, 2, 2, 3});
            return !solution.isPalindrome(head);
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