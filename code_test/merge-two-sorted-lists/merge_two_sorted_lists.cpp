#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

// 定义链表节点结构
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy = ListNode(0);
        ListNode* cur = &dummy;
        while (l1 != nullptr && l2 != nullptr)
        {
            if (l1->val <= l2->val) 
            {
                cur->next = l1;
                l1 = l1->next;
            }
            else 
            {
                cur->next = l2;
                l2 = l2->next;
            }
            cur = cur->next; 
        }
        if (l1 != nullptr)
        {
            cur->next = l1;
        }
        if (l2 != nullptr)
        {
            cur->next = l2;
        }
        return dummy.next;
    }
};

// 辅助函数：创建链表
ListNode* createList(std::initializer_list<int> values) {
    ListNode dummy(0);
    ListNode* current = &dummy;
    for (int val : values) {
        current->next = new ListNode(val);
        current = current->next;
    }
    return dummy.next;
}

// 辅助函数：打印链表
void printList(ListNode* head) {
    while (head != nullptr) {
        std::cout << head->val << " ";
        head = head->next;
    }
    std::cout << std::endl;
}

// 测试框架
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    void runTest(const std::string& testName, ListNode* l1, ListNode* l2, std::initializer_list<int> expected) {
        total++;
        ListNode* result = solution.mergeTwoLists(l1, l2);
        ListNode* expectedList = createList(expected);
        
        bool isPassed = true;
        ListNode* r = result;
        ListNode* e = expectedList;
        while (r != nullptr && e != nullptr) {
            if (r->val != e->val) {
                isPassed = false;
                break;
            }
            r = r->next;
            e = e->next;
        }
        if (r != nullptr || e != nullptr) {
            isPassed = false;
        }

        if (isPassed) {
            std::cout << "通过: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "失败: " << testName << std::endl;
            std::cout << "预期: ";
            printList(expectedList);
            std::cout << "实际: ";
            printList(result);
        }
    }

public:
    void runAllTests() {
        runTest("测试用例1", createList({1,2,4}), createList({1,3,4}), {1,1,2,3,4,4});
        runTest("测试用例2", createList({}), createList({}), {});
        runTest("测试用例3", createList({}), createList({0}), {0});

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
