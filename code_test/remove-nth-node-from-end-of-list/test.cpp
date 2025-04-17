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
    
    // 删除倒数第N个节点
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy = ListNode(0);
        dummy.next = head;
        ListNode* slow = &dummy;
        ListNode* fast = &dummy;
        // fast 先走n+1
        for (int i = 0; i <=n; i++)
        {
            fast = fast->next;
        }
        while (fast && slow)
        {
            fast = fast->next;
            slow = slow->next;
        }
        // 删除第n个节点
        ListNode* need_del = slow->next;
        slow->next = slow->next->next;
        delete need_del;
        return dummy.next;
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
        
        // 创建链表A
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
        
        // 提前检查是否需要相交
        // 如果skipA或skipB超出或等于各自链表的长度，或skipB小于1，则返回不相交的链表
        if (skipA >= listA.size() || skipB > listB.size() || skipB < 1) {
            return std::make_pair(headA, headB);
        }
        
        // 找到相交点
        ListNode* intersectA = headA;
        for (int i = 0; i < skipA; i++) {
            if (intersectA == nullptr) {
                return std::make_pair(headA, headB); // 安全检查
            }
            intersectA = intersectA->next;
        }
        
        if (intersectA == nullptr) {
            return std::make_pair(headA, headB);
        }
        
        // 找到B链表的连接点
        ListNode* prevB = headB;
        for (int i = 0; i < skipB - 1; i++) {
            if (prevB == nullptr) {
                return std::make_pair(headA, headB); // 安全检查
            }
            prevB = prevB->next;
        }
        
        if (prevB == nullptr) {
            return std::make_pair(headA, headB);
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
        
        return std::make_pair(headA, headB);
    }

    // 清理链表内存
    void cleanupLists(ListNode* headA, ListNode* headB) {
        if (headA == nullptr && headB == nullptr) {
            return;
        }
        
        std::unordered_set<ListNode*> visited;
        
        // 先遍历链表A，记录所有节点
        ListNode* curr = headA;
        while (curr != nullptr) {
            visited.insert(curr);
            curr = curr->next;
        }
        
        // 遍历链表B，只记录未访问过的节点
        curr = headB;
        while (curr != nullptr) {
            if (visited.find(curr) == visited.end()) {
                visited.insert(curr);
            }
            curr = curr->next;
        }
        
        // 删除所有记录的节点
        for (ListNode* node : visited) {
            delete node;
        }
    }

    // 新增：创建单个链表
    ListNode* createList(const std::vector<int>& values) {
        ListNode* head = nullptr;
        ListNode* tail = nullptr;
        
        for (int val : values) {
            if (head == nullptr) {
                head = new ListNode(val);
                tail = head;
            } else {
                tail->next = new ListNode(val);
                tail = tail->next;
            }
        }
        
        return head;
    }

    // 新增：将链表转换为vector用于比较结果
    std::vector<int> listToVector(ListNode* head) {
        std::vector<int> result;
        while (head != nullptr) {
            result.push_back(head->val);
            head = head->next;
        }
        return result;
    }

    // 新增：清理单个链表
    void cleanupList(ListNode* head) {
        while (head != nullptr) {
            ListNode* next = head->next;
            delete head;
            head = next;
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
        // 删除倒数第N个节点的测试
        runTest("示例1 - 删除倒数第2个节点", [this]() {
            ListNode* head = createList({1,2,3,4,5});
            ListNode* result = solution.removeNthFromEnd(head, 2);
            std::vector<int> expected = {1,2,3,5};
            std::vector<int> actual = listToVector(result);
            cleanupList(result);
            return actual == expected;
        });

        runTest("示例2 - 单节点删除", [this]() {
            ListNode* head = createList({1});
            ListNode* result = solution.removeNthFromEnd(head, 1);
            std::vector<int> actual = listToVector(result);
            return actual.empty();
        });

        runTest("示例3 - 删除倒数第1个节点", [this]() {
            ListNode* head = createList({1,2});
            ListNode* result = solution.removeNthFromEnd(head, 1);
            std::vector<int> expected = {1};
            std::vector<int> actual = listToVector(result);
            cleanupList(result);
            return actual == expected;
        });

        runTest("删除头节点", [this]() {
            ListNode* head = createList({1,2,3});
            ListNode* result = solution.removeNthFromEnd(head, 3);
            std::vector<int> expected = {2,3};
            std::vector<int> actual = listToVector(result);
            cleanupList(result);
            return actual == expected;
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
