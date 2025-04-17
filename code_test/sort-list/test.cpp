#include <iostream>
#include <vector>
#include <functional>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode* mid = getMid(head);
        ListNode* left = sortList(head);
        ListNode* right = sortList(mid);
        return merge(left, right);
    }
    
private:
    ListNode* getMid(ListNode* head) {
       ListNode* slow = head;
       ListNode* fast = head->next;
       while (fast && fast->next)
       {
            slow = slow->next;
            fast = fast->next->next;
       }
       ListNode* mid = slow->next;
       slow->next = nullptr;
       return mid;
    }
    
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy = ListNode(0);
        ListNode* head = &dummy;
        while (l1 && l2)
        {
            if (l1->val < l2->val) {
                head->next = l1;
                l1 = l1->next;
                head = head->next;
            } else {
                head->next = l2;
                l2 = l2->next;
                head = head->next;
            }
        }
        if (l1) {
            head->next = l1;
        } else {
            head->next = l2;
        }
        return dummy.next;
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
            std::cout << "通过: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "失败: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        runTest("测试用例1", [this]() {
            ListNode* head = new ListNode(4);
            head->next = new ListNode(2);
            head->next->next = new ListNode(1);
            head->next->next->next = new ListNode(3);
            ListNode* sorted = solution.sortList(head);
            return (sorted->val == 1 && sorted->next->val == 2 && sorted->next->next->val == 3 && sorted->next->next->next->val == 4);
        });

        runTest("测试用例2", [this]() {
            ListNode* head = new ListNode(1);
            head->next = new ListNode(2);
            ListNode* sorted = solution.sortList(head);
            return (sorted->val == 1 && sorted->next->val == 2);
        });

        runTest("测试用例3", [this]() {
            ListNode* head = nullptr; // 空链表
            ListNode* sorted = solution.sortList(head);
            return sorted == nullptr; // 应该返回空
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