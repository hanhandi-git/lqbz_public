#include <iostream>
#include <functional>
#include <vector>

// 定义链表节点结构
// struct ListNode {
//     int val;
//     ListNode *next;
//     ListNode(int x) : val(x), next(nullptr) {}
// };

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int val) : val(val), next(nullptr) {}
};

class Solution {
public:
    // 迭代方法
    ListNode* reverseList(ListNode* head) {
        ListNode* pre = nullptr;
        ListNode* cur = head;
        while (cur != nullptr)
        {
            ListNode* next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        return pre;
    }

    // 递归方法
    ListNode* reverseListRecursive(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode* p = reverseList(head->next);
        head->next->next = head;
        head->next = nullptr;
        return p;
    }

    // 新实现的反转链表一部分的函数
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        if (head == nullptr || left == right) {
            return head;
        }

        ListNode dummy(0);
        dummy.next = head;
        ListNode* prev = &dummy;

        // 移动到反转起始位置的前一个节点
        for (int i = 0; i < left - 1; i++) {
            prev = prev->next;
        }

        // start是反转区间的第一个节点
        ListNode* start = prev->next;
        // end是反转区间的最后一个节点
        ListNode* end = start;
        for (int i = 0; i < right - left; i++) {
            end = end->next;
        }

        // 保存反转区间后的第一个节点
        ListNode* next = end->next;

        // 断开反转区间与后面的连接
        end->next = nullptr;

        // 反转区间内的节点
        prev->next = reverseList(start);

        // 连接反转后的部分与原链表
        start->next = next;

        return dummy.next;
    }
};

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        if (test()) {
            std::cout << "PASS: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "FAIL: " << testName << std::endl;
        }
    }

    // 辅助函数：创建链表
    ListNode* createList(const std::vector<int>& values) {
        ListNode dummy(0);
        ListNode* current = &dummy;
        for (int val : values) {
            current->next = new ListNode(val);
            current = current->next;
        }
        return dummy.next;
    }

    // 辅助函数：比较两个链表
    bool compareLists(ListNode* l1, ListNode* l2) {
        while (l1 && l2) {
            if (l1->val != l2->val) return false;
            l1 = l1->next;
            l2 = l2->next;
        }
        return l1 == l2; // 都为nullptr时相等
    }

    // 辅助函数：释放链表内存
    void deleteList(ListNode* head) {
        while (head) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

public:
    void runAllTests() {
        runTest("TestEmptyList", [this]() {
            ListNode* head = nullptr;
            ListNode* reversed = solution.reverseList(head);
            return reversed == nullptr;
        });

        runTest("TestSingleNodeList", [this]() {
            ListNode* head = new ListNode(1);
            ListNode* reversed = solution.reverseList(head);
            bool result = (reversed->val == 1 && reversed->next == nullptr);
            delete reversed;
            return result;
        });

        runTest("TestMultipleNodeList", [this]() {
            ListNode* head = createList({1, 2, 3, 4, 5});
            ListNode* reversed = solution.reverseList(head);
            ListNode* expected = createList({5, 4, 3, 2, 1});
            bool result = compareLists(reversed, expected);
            deleteList(reversed);
            deleteList(expected);
            return result;
        });

        runTest("TestRecursiveReversal", [this]() {
            ListNode* head = createList({1, 2, 3, 4, 5});
            ListNode* reversed = solution.reverseListRecursive(head);
            ListNode* expected = createList({5, 4, 3, 2, 1});
            bool result = compareLists(reversed, expected);
            deleteList(reversed);
            deleteList(expected);
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
