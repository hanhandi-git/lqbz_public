#include <vector>
#include <iostream>
#include <functional>

/**
 * @brief 单向链表节点的定义
 */
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    /**
     * @brief 将两个表示数字的链表相加
     * 
     * @param l1 第一个链表
     * @param l2 第二个链表
     * @return ListNode* 返回相加后的新链表
     */
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy = ListNode(0);
        ListNode* cur = &dummy;
        int carry = 0;
        while (l1 || l2 || carry) {
            int sum = carry;
            if (l1) {
                sum += l1->val;
                l1 = l1->next;
            }
            if (l2) {
                sum += l2->val;
                l2 = l2->next;
            }
            carry = sum / 10;
            ListNode* tmp = new ListNode(sum % 10);
            cur->next = tmp;
            cur = cur->next;
        }
        return dummy.next;
    }
};

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;
    
    /**
     * @brief 从vector创建链表
     */
    ListNode* createList(const std::vector<int>& nums) {
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
    
    /**
     * @brief 比较两个链表是否相等
     */
    bool compareLists(ListNode* l1, ListNode* l2) {
        while (l1 != nullptr && l2 != nullptr) {
            if (l1->val != l2->val) return false;
            l1 = l1->next;
            l2 = l2->next;
        }
        return l1 == nullptr && l2 == nullptr;
    }
    
    /**
     * @brief 释放链表内存
     */
    void deleteList(ListNode* head) {
        while (head != nullptr) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
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
        // 示例1测试
        runTest("示例1 - 基本加法", [this]() {
            ListNode* l1 = createList({2,4,3});
            ListNode* l2 = createList({5,6,4});
            ListNode* expected = createList({7,0,8});
            ListNode* result = solution.addTwoNumbers(l1, l2);
            bool isCorrect = compareLists(result, expected);
            deleteList(l1);
            deleteList(l2);
            deleteList(expected);
            deleteList(result);
            return isCorrect;
        });

        // 示例2测试
        runTest("示例2 - 零的相加", [this]() {
            ListNode* l1 = createList({0});
            ListNode* l2 = createList({0});
            ListNode* expected = createList({0});
            ListNode* result = solution.addTwoNumbers(l1, l2);
            bool isCorrect = compareLists(result, expected);
            deleteList(l1);
            deleteList(l2);
            deleteList(expected);
            deleteList(result);
            return isCorrect;
        });

        // 示例3测试
        runTest("示例3 - 复杂进位", [this]() {
            ListNode* l1 = createList({9,9,9,9,9,9,9});
            ListNode* l2 = createList({9,9,9,9});
            ListNode* expected = createList({8,9,9,9,0,0,0,1});
            ListNode* result = solution.addTwoNumbers(l1, l2);
            bool isCorrect = compareLists(result, expected);
            deleteList(l1);
            deleteList(l2);
            deleteList(expected);
            deleteList(result);
            return isCorrect;
        });

        // 不同长度测试
        runTest("特殊1 - 不同长度", [this]() {
            ListNode* l1 = createList({1,2,3});
            ListNode* l2 = createList({1});
            ListNode* expected = createList({2,2,3});
            ListNode* result = solution.addTwoNumbers(l1, l2);
            bool isCorrect = compareLists(result, expected);
            deleteList(l1);
            deleteList(l2);
            deleteList(expected);
            deleteList(result);
            return isCorrect;
        });

        // 有进位测试
        runTest("特殊2 - 有进位", [this]() {
            ListNode* l1 = createList({5});
            ListNode* l2 = createList({5});
            ListNode* expected = createList({0,1});
            ListNode* result = solution.addTwoNumbers(l1, l2);
            bool isCorrect = compareLists(result, expected);
            deleteList(l1);
            deleteList(l2);
            deleteList(expected);
            deleteList(result);
            return isCorrect;
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