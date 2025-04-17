#include <iostream>
#include <vector>
#include <string>
#include <queue>
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
private:

    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy(-1);
        ListNode* curr = &dummy;
        while (l1 && l2)
        {
            if (l1->val < l2->val)
            {
                curr->next = l1;
                curr = curr->next;
                l1 = l1->next;
            } else {
                curr->next = l2;
                curr = curr->next;
                l2 = l2->next;
            }
            if (l1) curr->next = l1;
            if (l2) curr->next = l2;
        }
        return dummy.next;
    }

    ListNode* mergeKListsHelper(vector<ListNode*>& lists, int left, int right) {
        if (left == right) return lists[left];
        if (left + 1 == right) return mergeTwoLists(lists[left], lists[right]);
        int mid = left + (right - left) / 2;

        // 解决子问题
        ListNode* leftResult = mergeKListsHelper(lists, left, mid);
        ListNode* rightResult = mergeKListsHelper(lists, mid + 1, right);
        return mergeTwoLists(leftResult, rightResult);
    }

public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) return nullptr;
        return mergeKListsHelper(lists, 0, lists.size() - 1);
    }
};

// class Solution {
// public:
//     ListNode* mergeKLists(vector<ListNode*>& lists)
//     {
//         auto cmp = [](const ListNode* a, const ListNode* b){ return a->val > b->val;};
//         std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(cmp)> que(cmp);
//         for (int i = 0; i < lists.size(); i++)
//         {
//             if (lists[i] != nullptr) {
//                 que.push(lists[i]);
//             }
//         }
//         ListNode dummy(0);
//         ListNode* curr = &dummy;
//         while (!que.empty())
//         {
//             auto node = que.top();
//             que.pop();
//             curr->next = node;
//             curr = curr->next;
//             if (node->next) que.push(node->next);
//         }
//         return dummy.next;
//     }
// };


// class Solution {
// private:

//     ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
//         ListNode dummy = ListNode(-1);
//         ListNode* cur = &dummy;
//         while (l1 && l2)
//         {
//             if (l1->val <= l2->val)
//             {
//                 cur->next = l1;
//                 l1 = l1->next;
//                 cur = cur->next;
//             }
//             else
//             {
//                 cur->next = l2;
//                 l2 = l2->next;
//                 cur = cur->next;
//             }
//         }
//         if (l1) 
//         {
//             cur->next = l1;
//         }
//         if (l2) 
//         {
//             cur->next = l2;
//         }
//         return dummy.next;
//     }

//     ListNode* mergeKListsHelper(vector<ListNode*>& lists, int left, int right) {
//         if (left == right) return lists[left];
//         else if (left + 1 == right) return mergeTwoLists(lists[left], lists[right]);
//         int mid = left + (right - left) / 2;
//         ListNode* leftNode = mergeKListsHelper(lists, left, mid);
//         ListNode* rightNode = mergeKListsHelper(lists, mid+1, right);
//         return mergeTwoLists(leftNode, rightNode);
//     }

// public:
//     ListNode* mergeKLists(vector<ListNode*>& lists) {
//         if (lists.empty()) return nullptr;
//         return mergeKListsHelper(lists, 0, lists.size() - 1);
//     }
// };

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
        // 测试用例1：基本情况
        runTest("基本测试", [this]() {
        vector<ListNode*> lists = {
            createList({1, 4, 5}),
            createList({1, 3, 4}),
            createList({2, 6})
        };
        ListNode* result = solution.mergeKLists(lists);
        ListNode* expected = createList({1, 1, 2, 3, 4, 4, 5, 6});
        bool isEqual = areListsEqual(result, expected);
        return isEqual;
    });

        // 测试用例2：空链表
        runTest("空链表测试", [this]() {
            vector<ListNode*> lists = {};
            ListNode* result = solution.mergeKLists(lists);
            return result == nullptr;
        });

        // 测试用例3：单个空链表
        runTest("单个空链表测试", [this]() {
            vector<ListNode*> lists = {nullptr};
            ListNode* result = solution.mergeKLists(lists);
            return result == nullptr;
        });

        // 输出测试结果统计
        std::cout << "\n测试完成！通过: " << passed << "/" << total << " 测试用例" << std::endl;
    }
};

int main() {
    TestFramework framework;
    framework.runAllTests();
    return 0;
}
