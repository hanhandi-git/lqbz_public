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
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode* dummy = new ListNode(0);
        ListNode* curr = dummy;
        
        while (list1 && list2) {
            if (list1->val <= list2->val) {
                curr->next = list1;
                list1 = list1->next;
            } else {
                curr->next = list2;
                list2 = list2->next;
            }
            curr = curr->next;
        }
        
        curr->next = list1 ? list1 : list2;
        ListNode* result = dummy->next;
        delete dummy;
        return result;
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
        return l1 == l2; // 都为nullptr则相等
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
        // 基本测试 - 两个非空有序链表
        runTest("基本测试 - 两个非空有序链表", [this]() {
            ListNode* l1 = createList({1,2,4});
            ListNode* l2 = createList({1,3,4});
            ListNode* expected = createList({1,1,2,3,4,4});
            ListNode* result = solution.mergeTwoLists(l1, l2);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        // 边界测试 - 一个空链表
        runTest("边界测试 - list1为空", [this]() {
            ListNode* l1 = nullptr;
            ListNode* l2 = createList({1,2,3});
            ListNode* expected = createList({1,2,3});
            ListNode* result = solution.mergeTwoLists(l1, l2);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        // 边界测试 - 两个空链表
        runTest("边界测试 - 两个空链表", [this]() {
            ListNode* result = solution.mergeTwoLists(nullptr, nullptr);
            return result == nullptr;
        });

        // 特殊测试 - 长度不等的链表
        runTest("特殊测试 - 长度不等的链表", [this]() {
            ListNode* l1 = createList({1,2,3,4,5});
            ListNode* l2 = createList({1,2});
            ListNode* expected = createList({1,1,2,2,3,4,5});
            ListNode* result = solution.mergeTwoLists(l1, l2);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        // 特殊测试 - 有重复元素的链表
        runTest("特殊测试 - 有重复元素的链表", [this]() {
            ListNode* l1 = createList({1,1,1});
            ListNode* l2 = createList({1,1});
            ListNode* expected = createList({1,1,1,1,1});
            ListNode* result = solution.mergeTwoLists(l1, l2);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        // 特殊测试 - 只有一个元素的链表
        runTest("特殊测试 - 只有一个元素的链表", [this]() {
            ListNode* l1 = createList({1});
            ListNode* l2 = createList({2});
            ListNode* expected = createList({1,2});
            ListNode* result = solution.mergeTwoLists(l1, l2);
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