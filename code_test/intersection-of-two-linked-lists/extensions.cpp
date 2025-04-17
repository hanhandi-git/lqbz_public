#include <unordered_set>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class LinkedListExtensions {
public:
    // 1. 检测链表是否有环
    bool hasCycle(ListNode *head) {
        if (head == nullptr || head->next == nullptr) return false;
        
        ListNode* slow = head;
        ListNode* fast = head->next;
        
        while (slow != fast) {
            if (fast == nullptr || fast->next == nullptr) {
                return false;
            }
            slow = slow->next;
            fast = fast->next->next;
        }
        
        return true;
    }

    // 2. 找到环的入口节点
    ListNode *detectCycle(ListNode *head) {
        if (head == nullptr || head->next == nullptr) return nullptr;
        
        ListNode* slow = head;
        ListNode* fast = head;
        
        // 找到相遇点
        do {
            if (fast == nullptr || fast->next == nullptr) {
                return nullptr;
            }
            slow = slow->next;
            fast = fast->next->next;
        } while (slow != fast);
        
        // 找到环的入口
        slow = head;
        while (slow != fast) {
            slow = slow->next;
            fast = fast->next;
        }
        
        return slow;
    }

    // 3. 处理带环链表的相交问题
    ListNode* getIntersectionNodeWithCycle(ListNode* headA, ListNode* headB) {
        // 获取A的环入口
        ListNode* cycleA = detectCycle(headA);
        // 获取B的环入口
        ListNode* cycleB = detectCycle(headB);
        
        // 情况1：都无环
        if (cycleA == nullptr && cycleB == nullptr) {
            return getIntersectionNode(headA, headB);
        }
        
        // 情况2：一个有环一个无环
        if ((cycleA == nullptr) != (cycleB == nullptr)) {
            return nullptr;
        }
        
        // 情况3：都有环
        // 判断是否是同一个环
        ListNode* temp = cycleA->next;
        while (temp != cycleA) {
            if (temp == cycleB) {
                return cycleA; // 返回任一环入口即可
            }
            temp = temp->next;
        }
        
        return nullptr;
    }

private:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (headA == nullptr || headB == nullptr) return nullptr;
        
        ListNode* pA = headA;
        ListNode* pB = headB;
        
        while (pA != pB) {
            pA = pA == nullptr ? headB : pA->next;
            pB = pB == nullptr ? headA : pB->next;
        }
        
        return pA;
    }
};

class TestFramework {
private:
    LinkedListExtensions solution;
    int passed = 0;
    int total = 0;

    // 创建带环链表
    ListNode* createCyclicList(const std::vector<int>& values, int pos) {
        if (values.empty()) return nullptr;
        
        ListNode* head = new ListNode(values[0]);
        ListNode* current = head;
        ListNode* cycleNode = (pos == 0) ? head : nullptr;
        
        for (int i = 1; i < values.size(); i++) {
            current->next = new ListNode(values[i]);
            current = current->next;
            if (i == pos) cycleNode = current;
        }
        
        if (cycleNode != nullptr) {
            current->next = cycleNode;
        }
        
        return head;
    }

    // 清理带环链表
    void cleanupCyclicList(ListNode* head) {
        std::unordered_set<ListNode*> visited;
        while (head != nullptr && visited.count(head) == 0) {
            visited.insert(head);
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
        // 测试环检测
        runTest("环检测 - 无环", [this]() {
            ListNode* head = createCyclicList({1,2,3,4,5}, -1);
            bool result = !solution.hasCycle(head);
            cleanupCyclicList(head);
            return result;
        });

        runTest("环检测 - 有环", [this]() {
            ListNode* head = createCyclicList({1,2,3,4,5}, 2);
            bool result = solution.hasCycle(head);
            cleanupCyclicList(head);
            return result;
        });

        // 测试环入口检测
        runTest("环入口检测 - 无环", [this]() {
            ListNode* head = createCyclicList({1,2,3,4,5}, -1);
            bool result = solution.detectCycle(head) == nullptr;
            cleanupCyclicList(head);
            return result;
        });

        runTest("环入口检测 - 有环", [this]() {
            ListNode* head = createCyclicList({1,2,3,4,5}, 2);
            ListNode* cycleStart = solution.detectCycle(head);
            bool result = (cycleStart != nullptr && cycleStart->val == 3);
            cleanupCyclicList(head);
            return result;
        });

        // 测试带环链表相交
        runTest("带环链表相交 - 都无环", [this]() {
            ListNode* headA = createCyclicList({1,2,3}, -1);
            ListNode* headB = createCyclicList({4,5,6}, -1);
            bool result = solution.getIntersectionNodeWithCycle(headA, headB) == nullptr;
            cleanupCyclicList(headA);
            cleanupCyclicList(headB);
            return result;
        });

        runTest("带环链表相交 - 一个有环一个无环", [this]() {
            ListNode* headA = createCyclicList({1,2,3,4,5}, 2);
            ListNode* headB = createCyclicList({4,5,6}, -1);
            bool result = solution.getIntersectionNodeWithCycle(headA, headB) == nullptr;
            cleanupCyclicList(headA);
            cleanupCyclicList(headB);
            return result;
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
