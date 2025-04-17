/**
 * @file intersection_of_linked_lists.cpp
 * @brief 实现相交链表的查找算法
 */

#include <iostream>
#include <vector>
#include <functional>

// 链表节点定义
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    /**
     * @brief 查找两个链表的相交节点
     * @param headA 第一个链表的头节点
     * @param headB 第二个链表的头节点
     * @return 相交节点,如果不存在则返回nullptr
     */
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (headA == nullptr || headB == nullptr) return nullptr;
        ListNode* pA = headA;
        ListNode* pB = headB;

        while (pA != pB)
        {
            if (pA == nullptr) pA = headB;
            else pA = pA->next;
            if (pB == nullptr) pB = headA;
            else pB = pB->next;
        }
        return pA;
    }
};

// class Solution {
// public:
//     /**
//      * @brief 查找两个链表的相交节点
//      * @param headA 第一个链表的头节点
//      * @param headB 第二个链表的头节点
//      * @return 相交节点,如果不存在则返回nullptr
//      */
//     ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
//         if (!headA || !headB) return nullptr;
        
//         ListNode *pA = headA;
//         ListNode *pB = headB;
        
//         while (pA != pB) {
//             pA = pA ? pA->next : headB;
//             pB = pB ? pB->next : headA;
//         }
        
//         return pA;
//     }
// };

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    // 创建链表的辅助函数
    ListNode* createList(const std::vector<int>& values) {
        ListNode* dummy = new ListNode(0);
        ListNode* current = dummy;
        for (int val : values) {
            current->next = new ListNode(val);
            current = current->next;
        }
        ListNode* head = dummy->next;
        delete dummy;
        return head;
    }

    // 清理链表的辅助函数 
    void deleteList(ListNode* head, ListNode* stop = nullptr) {
        while (head && head != stop) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        bool result = test();
        if (result) {
            std::cout << "通过: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "失败: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        // 测试相交链表
        runTest("相交链表测试", [this]() {
            // 创建公共部分
            ListNode* common = createList({8, 4, 5});
            
            // 创建链表A的非公共部分
            ListNode* headA = createList({4, 1});
            ListNode* tailA = headA;
            while (tailA->next) tailA = tailA->next;
            tailA->next = common;
            
            // 创建链表B的非公共部分
            ListNode* headB = createList({5, 6, 1});
            ListNode* tailB = headB;
            while (tailB->next) tailB = tailB->next;
            tailB->next = common;
            
            ListNode* result = solution.getIntersectionNode(headA, headB);
            bool passed = (result == common);
            
            // 先删除非公共部分,再删除公共部分
            deleteList(headA, common);
            deleteList(headB, common);
            deleteList(common);
            
            return passed;
        });

        // 测试不相交链表
        runTest("不相交链表测试", [this]() {
            ListNode* headA = createList({1, 2, 3});
            ListNode* headB = createList({4, 5, 6});
            
            ListNode* result = solution.getIntersectionNode(headA, headB);
            bool passed = (result == nullptr);
            
            deleteList(headA);
            deleteList(headB);
            
            return passed;
        });

        // 测试空链表
        runTest("空链表测试", [this]() {
            ListNode* headA = nullptr;
            ListNode* headB = createList({1, 2, 3});
            
            ListNode* result = solution.getIntersectionNode(headA, headB);
            bool passed = (result == nullptr);
            
            deleteList(headB);
            
            return passed;
        });

        // 测试长度不同的相交链表
        runTest("长度不同的相交链表测试", [this]() {
            // 创建公共部分
            ListNode* common = createList({2, 3});
            
            // 创建链表A的非公共部分
            ListNode* headA = createList({1});
            ListNode* tailA = headA;
            while (tailA->next) tailA = tailA->next;
            tailA->next = common;
            
            // 创建链表B的非公共部分
            ListNode* headB = createList({4, 5, 6});
            ListNode* tailB = headB;
            while (tailB->next) tailB = tailB->next;
            tailB->next = common;
            
            ListNode* result = solution.getIntersectionNode(headA, headB);
            bool passed = (result == common);
            
            // 先删除非公共部分,再删除公共部分
            deleteList(headA, common);
            deleteList(headB, common);
            deleteList(common);
            
            return passed;
        });

        // 测试相交点在开头的情况
        runTest("相交点在开头测试", [this]() {
            ListNode* common = createList({1, 2, 3});
            ListNode* headA = common;
            ListNode* headB = common;
            
            ListNode* result = solution.getIntersectionNode(headA, headB);
            bool passed = (result == common);
            
            deleteList(common);
            
            return passed;
        });

        // 测试只有一个节点的相交链表
        runTest("单节点相交链表测试", [this]() {
            ListNode* common = new ListNode(1);
            ListNode* headA = common;
            ListNode* headB = common;
            
            ListNode* result = solution.getIntersectionNode(headA, headB);
            bool passed = (result == common);
            
            delete common;
            
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