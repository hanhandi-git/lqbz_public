#include <unordered_set>
#include <iostream>
#include <functional>
#include <unordered_set>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

// 链表节点定义
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    // 哈希表解法
    ListNode *getIntersectionNode_hash(ListNode *headA, ListNode *headB) {
        std::unordered_set<ListNode*> visited;
        ListNode* temp = headA;
        while (temp != nullptr) {
            visited.insert(temp);
            temp = temp->next;
        }
        
        temp = headB;
        while (temp != nullptr) {
            if (visited.count(temp)) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }
    
    // 双指针解法
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (headA == nullptr || headB == nullptr) return nullptr;
        
        ListNode* pA = headA;
        ListNode* pB = headB;
        
        // 如果链表不相交，pA和pB最终都��变成nullptr
        // 如果链表相交，pA和pB最终会在相交点相遇
        while (pA != pB) {
            // 先移动指针
            pA = pA == nullptr ? headB : pA->next;
            pB = pB == nullptr ? headA : pB->next;
            
            // 打印当前状态（移动后）
            std::cout << "pA: " << (pA ? std::to_string(pA->val) : "nullptr") << std::endl;
            std::cout << "pB: " << (pB ? std::to_string(pB->val) : "nullptr") << std::endl;
            
            // 如果两个指针都变成nullptr，说明链表不相交
            if (pA == nullptr && pB == nullptr) {
                return nullptr;
            }
        }
        
        return pA; // 返回相交点或nullptr
    }
};

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    // 辅助函数：创建测试用的链表
    std::pair<ListNode*, ListNode*> createIntersectingLists(const std::vector<int>& listA, 
                                                           const std::vector<int>& listB, 
                                                           int skipA, int skipB) {
        ListNode* headA = nullptr;
        ListNode* headB = nullptr;
        ListNode* tailA = nullptr;
        ListNode* tailB = nullptr;
        
        // ���建链表A
        for (int i = 0; i < listA.size(); i++) {
            if (headA == nullptr) {
                headA = new ListNode(listA[i]);
                tailA = headA;
            } else {
                tailA->next = new ListNode(listA[i]);
                tailA = tailA->next;
            }
        }
        
        // 创建链表B
        for (int i = 0; i < listB.size(); i++) {
            if (headB == nullptr) {
                headB = new ListNode(listB[i]);
                tailB = headB;
            } else {
                tailB->next = new ListNode(listB[i]);
                tailB = tailB->next;
            }
        }
        
        // 如果需要相交（skipA和skipB都小于各自链表的长度）
        if (skipA < listA.size() && skipB < listB.size()) {
            // 找到相交点
            ListNode* intersectA = headA;
            for (int i = 0; i < skipA; i++) {
                intersectA = intersectA->next;
            }
            
            // 找到B链表的对应位置
            ListNode* prevB = headB;
            for (int i = 1; i < skipB; i++) {
                prevB = prevB->next;
            }
            
            // 释放B链表后续节点
            ListNode* temp = prevB->next;
            while (temp != nullptr) {
                ListNode* next = temp->next;
                delete temp;
                temp = next;
            }
            
            // 连接相交点
            prevB->next = intersectA;
        }
        
        return std::make_pair(headA, headB);
    }

    // 清理链表内存
    void cleanupLists(ListNode* headA, ListNode* headB) {
        std::unordered_set<ListNode*> deleted;  // 记录已删除的节点
        
        // 先删除链表A中的所有节点
        ListNode* currA = headA;
        while (currA != nullptr) {
            ListNode* next = currA->next;
            deleted.insert(currA);
            delete currA;
            currA = next;
        }
        
        // 只删除链表B中未在A中删除的节点
        ListNode* currB = headB;
        while (currB != nullptr) {
            ListNode* next = currB->next;
            if (deleted.count(currB) == 0) {  // 如果节点还未被删除
                delete currB;
            }
            currB = next;
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
        // 测试相交的情况
        runTest("示例1 - 相交链表", [this]() {
            std::pair<ListNode*, ListNode*> lists = createIntersectingLists({4,1,8,4,5}, {5,6,1,8,4,5}, 2, 3);
            ListNode* headA = lists.first;
            ListNode* headB = lists.second;
            ListNode* result = solution.getIntersectionNode(headA, headB);
            bool passed = (result != nullptr && result->val == 8);
            cleanupLists(headA, headB);
            return passed;
        });

        // 测试不相交的情况
        runTest("示例2 - 不相交链表", [this]() {
            std::pair<ListNode*, ListNode*> lists = createIntersectingLists({2,6,4}, {1,5}, 3, 2);
            // 打印链表

            ListNode* headA = lists.first;
            ListNode* headB = lists.second;
            ListNode* result = solution.getIntersectionNode(headA, headB);
            bool passed = (result == nullptr);
            // 打印结果
            cleanupLists(headA, headB);
            return passed;
        });

        // 测试空链表
        runTest("空链表测试", [this]() {
            return solution.getIntersectionNode(nullptr, nullptr) == nullptr;
        });

        // 测试单节点链表
        runTest("单节点相交", [this]() {
            ListNode* common = new ListNode(1);
            bool passed = solution.getIntersectionNode(common, common) == common;
            delete common;
            return passed;
        });

        // 测试哈希表方法
        runTest("哈希表方法测试", [this]() {
            std::pair<ListNode*, ListNode*> lists = createIntersectingLists({4,1,8,4,5}, {5,6,1,8,4,5}, 2, 3);
            ListNode* headA = lists.first;
            ListNode* headB = lists.second;
            ListNode* result = solution.getIntersectionNode_hash(headA, headB);
            bool passed = (result != nullptr && result->val == 8);
            cleanupLists(headA, headB);
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
