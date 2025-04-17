#include <iostream>
#include <vector>
#include <functional>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


class Solution {
public:

    ListNode* reverseList(ListNode* head) {
        ListNode dummy(-1);
        dummy.next = head;
        ListNode* prev = &dummy;
        ListNode* cur = head;
        while (cur)
        {
            ListNode* next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }
        return dummy.next;
    }

    void reorderList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return;

        // 找到中点
        ListNode* fast = head;
        ListNode* slow = head;
        while (fast && fast->next)
        {
            fast = fast->next->next;
            slow = slow->next;
        }
        
    }
};

// class Solution {
// public:
//     void reorderList(ListNode* head) {
//         if (!head || !head->next || !head->next->next) return;
        
//         // 步骤1：找到中间节点
//         ListNode* slow = head;
//         ListNode* fast = head;
//         while (fast->next && fast->next->next) {
//             slow = slow->next;
//             fast = fast->next->next;
//         }
        
//         // 分割链表，slow指向前半部分的尾节点
//         ListNode* mid = slow->next;
//         slow->next = nullptr;
        
//         // 步骤2：反转后半部分链表
//         ListNode* prev = nullptr;
//         ListNode* curr = mid;
//         ListNode* next = nullptr;
//         while (curr) {
//             next = curr->next;
//             curr->next = prev;
//             prev = curr;
//             curr = next;
//         }
        
//         // 步骤3：合并两个链表
//         ListNode* first = head;
//         ListNode* second = prev; // prev现在是反转后链表的头节点
//         ListNode* temp1, *temp2;
        
//         while (second) {
//             // 保存下一个节点
//             temp1 = first->next;
//             temp2 = second->next;
            
//             // 交错连接
//             first->next = second;
//             second->next = temp1;
            
//             // 移动指针
//             first = temp1;
//             second = temp2;
//         }
//     }
// };

// 辅助函数，用于创建链表
ListNode* createLinkedList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;
    
    ListNode* head = new ListNode(values[0]);
    ListNode* current = head;
    
    for (size_t i = 1; i < values.size(); ++i) {
        current->next = new ListNode(values[i]);
        current = current->next;
    }
    
    return head;
}

// 辅助函数，用于获取链表的值
std::vector<int> getLinkedListValues(ListNode* head) {
    std::vector<int> result;
    ListNode* current = head;
    
    while (current) {
        result.push_back(current->val);
        current = current->next;
    }
    
    return result;
}

// 辅助函数，用于释放链表内存
void freeLinkedList(ListNode* head) {
    while (head) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

class TestFramework {
private:
    int passed = 0;
    int total = 0;
    Solution solution;

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        if (test()) {
            std::cout << "PASS: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "FAIL: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        // 测试基本情况（偶数个节点）
        runTest("TestEvenNodes", [this]() {
            ListNode* head = createLinkedList({1, 2, 3, 4});
            solution.reorderList(head);
            std::vector<int> result = getLinkedListValues(head);
            std::vector<int> expected = {1, 4, 2, 3};
            bool passed = (result == expected);
            freeLinkedList(head);
            return passed;
        });

        // 测试奇数个节点
        runTest("TestOddNodes", [this]() {
            ListNode* head = createLinkedList({1, 2, 3, 4, 5});
            solution.reorderList(head);
            std::vector<int> result = getLinkedListValues(head);
            std::vector<int> expected = {1, 5, 2, 4, 3};
            bool passed = (result == expected);
            freeLinkedList(head);
            return passed;
        });

        // 测试边界情况：空链表
        runTest("TestEmptyList", [this]() {
            ListNode* head = nullptr;
            solution.reorderList(head);
            return head == nullptr; // 空链表应当保持不变
        });

        // 测试边界情况：单个节点
        runTest("TestSingleNode", [this]() {
            ListNode* head = createLinkedList({1});
            solution.reorderList(head);
            std::vector<int> result = getLinkedListValues(head);
            std::vector<int> expected = {1};
            bool passed = (result == expected);
            freeLinkedList(head);
            return passed;
        });

        // 测试边界情况：两个节点
        runTest("TestTwoNodes", [this]() {
            ListNode* head = createLinkedList({1, 2});
            solution.reorderList(head);
            std::vector<int> result = getLinkedListValues(head);
            std::vector<int> expected = {1, 2};
            bool passed = (result == expected);
            freeLinkedList(head);
            return passed;
        });

        // 测试较长的链表
        runTest("TestLongerList", [this]() {
            ListNode* head = createLinkedList({1, 2, 3, 4, 5, 6, 7, 8});
            solution.reorderList(head);
            std::vector<int> result = getLinkedListValues(head);
            std::vector<int> expected = {1, 8, 2, 7, 3, 6, 4, 5};
            bool passed = (result == expected);
            freeLinkedList(head);
            return passed;
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