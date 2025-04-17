#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;

// 链表节点定义
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    // 迭代解法
    ListNode* swapPairs(ListNode* head) {
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prev = &dummy;
        
        while (prev->next && prev->next->next) {
            ListNode* node1 = prev->next;
            ListNode* node2 = node1->next;
            
            prev->next = node2;
            node1->next = node2->next;
            node2->next = node1;
            
            prev = node1;
        }
        
        return dummy.next;
    }
    
    // 递归解法
    ListNode* swapPairsRecursive(ListNode* head) {
        if (!head || !head->next) return head;
        ListNode* next = head->next;
        head->next = swapPairsRecursive(next->next);
        next->next = head;
        return next;
    }
};

// 测试框架
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    // 辅助函数：从vector创建链表
    ListNode* createList(const vector<int>& nums) {
        ListNode* dummy = new ListNode(0);
        ListNode* curr = dummy;
        for (int num : nums) {
            curr->next = new ListNode(num);
            curr = curr->next;
        }
        ListNode* result = dummy->next;
        delete dummy;
        return result;
    }

    // 辅助函数：比较两个链表是否相等
    bool areListsEqual(ListNode* l1, ListNode* l2) {
        while (l1 && l2) {
            if (l1->val != l2->val) return false;
            l1 = l1->next;
            l2 = l2->next;
        }
        return l1 == l2;
    }

    // 辅助函数：释放链表内存
    void deleteList(ListNode* head) {
        while (head) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // 辅助函数：打印链表(用于调试)
    void printList(ListNode* head) {
        while (head) {
            cout << head->val << " -> ";
            head = head->next;
        }
        cout << "nullptr" << endl;
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
        // 测试迭代解法
        runTest("迭代解法 - 基本测试", [this]() {
            ListNode* head = createList({1,2,3,4});
            ListNode* expected = createList({2,1,4,3});
            ListNode* result = solution.swapPairs(head);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        runTest("迭代解法 - 奇数长度链表", [this]() {
            ListNode* head = createList({1,2,3});
            ListNode* expected = createList({2,1,3});
            ListNode* result = solution.swapPairs(head);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        runTest("迭代解法 - 空链表", [this]() {
            return solution.swapPairs(nullptr) == nullptr;
        });

        runTest("迭代解法 - 单节点链表", [this]() {
            ListNode* head = createList({1});
            ListNode* expected = createList({1});
            ListNode* result = solution.swapPairs(head);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        // 测试递归解法
        runTest("递归解法 - 基本测试", [this]() {
            ListNode* head = createList({1,2,3,4});
            ListNode* expected = createList({2,1,4,3});
            ListNode* result = solution.swapPairsRecursive(head);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        runTest("递归解法 - 奇数长度链表", [this]() {
            ListNode* head = createList({1,2,3});
            ListNode* expected = createList({2,1,3});
            ListNode* result = solution.swapPairsRecursive(head);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        runTest("递归解法 - 空链表", [this]() {
            return solution.swapPairsRecursive(nullptr) == nullptr;
        });

        runTest("递归解法 - 单节点链表", [this]() {
            ListNode* head = createList({1});
            ListNode* expected = createList({1});
            ListNode* result = solution.swapPairsRecursive(head);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
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