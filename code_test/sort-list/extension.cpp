#include <iostream>
#include <vector>
#include <algorithm>
#include <functional> // 添加这一行以包含 std::function

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    // 1. 合并两个有序链表
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy(0); // 虚拟头节点
        ListNode* tail = &dummy;

        while (l1 && l2) {
            if (l1->val < l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next; // 移动尾指针
        }

        tail->next = l1 ? l1 : l2; // 连接剩余部分
        return dummy.next; // 返回合并后的链表
    }

    // 2. K个一组翻转链表
        ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prevGroupEnd = &dummy;

        while (true) {
            ListNode* kEnd = prevGroupEnd;
            for (int i = 0; i < k; i++) {
                kEnd = kEnd->next;
                if (kEnd == nullptr) return dummy.next; // 如果 kEnd 为 nullptr，返回原链表
            }

            ListNode* start = prevGroupEnd->next;
            ListNode* nextStart = kEnd->next;
            kEnd->next = nullptr; // 截断

            // 翻转
            prevGroupEnd->next = reverse(start);
            start->next = nextStart;

            // 更新 prevGroupEnd
            prevGroupEnd = start; // 更新为当前组的开始节点
        }
    }

    // 3. 删除链表的倒数第N个节点
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0); // 虚拟头节点
        dummy.next = head;
        ListNode* fast = &dummy;
        ListNode* slow = &dummy;

        // 快指针先走n+1步
        for (int i = 0; i <= n; ++i) {
            fast = fast->next;
        }

        // 快慢指针一起走
        while (fast) {
            slow = slow->next;
            fast = fast->next;
        }

        // 删除倒数第N个节点
        slow->next = slow->next->next;
        return dummy.next; // 返回新的头节点
    }

private:
    // 反转链表
    ListNode* reverse(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr) {
            ListNode* nextTemp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextTemp;
        }
        return prev; // 返回新的头节点
    }
};

// 测试框架
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

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
        runTest("测试合并两个有序链表", [this]() {
            ListNode* l1 = new ListNode(1);
            l1->next = new ListNode(2);
            l1->next->next = new ListNode(4);
            ListNode* l2 = new ListNode(1);
            l2->next = new ListNode(3);
            l2->next->next = new ListNode(4);
            ListNode* merged = solution.mergeTwoLists(l1, l2);
            return (merged->val == 1 && merged->next->val == 1 && merged->next->next->val == 2 && 
                    merged->next->next->next->val == 3 && merged->next->next->next->next->val == 4 && 
                    merged->next->next->next->next->next->val == 4);
        });

        runTest("测试K个一组翻转链表", [this]() {
            ListNode* head = new ListNode(1);
            head->next = new ListNode(2);
            head->next->next = new ListNode(3);
            head->next->next->next = new ListNode(4);
            head->next->next->next->next = new ListNode(5);
            ListNode* reversed = solution.reverseKGroup(head, 2);
            return (reversed->val == 2 && reversed->next->val == 1 && reversed->next->next->val == 4 &&
                    reversed->next->next->next->val == 3 && reversed->next->next->next->next->val == 5);
        });
                runTest("测试K个一组翻转链表 - 边界情况: 空链表", [this]() {
            ListNode* head = nullptr; // 空链表
            ListNode* reversed = solution.reverseKGroup(head, 2);
            return reversed == nullptr; // 应该返回空
        });

        runTest("测试K个一组翻转链表 - 边界情况: 单个节点", [this]() {
            ListNode* head = new ListNode(1); // 只有一个节点
            ListNode* reversed = solution.reverseKGroup(head, 1);
            return (reversed->val == 1); // 应该返回原链表
        });

        runTest("测试K个一组翻转链表 - K大于链表长度", [this]() {
            ListNode* head = new ListNode(1);
            head->next = new ListNode(2);
            head->next->next = new ListNode(3); // 只有三个节点
            ListNode* reversed = solution.reverseKGroup(head, 5); // K=5
            return (reversed->val == 1 && reversed->next->val == 2 && reversed->next->next->val == 3); // 应该返回原链表
        });

        runTest("测试K个一组翻转链表 - K等于链表长度", [this]() {
            ListNode* head = new ListNode(1);
            head->next = new ListNode(2);
            head->next->next = new ListNode(3); // 只有三个节点
            ListNode* reversed = solution.reverseKGroup(head, 3); // K=3
            return (reversed->val == 3 && reversed->next->val == 2 && reversed->next->next->val == 1); // 应该返回翻转后的链表
        });

        runTest("测试K个一组翻转链表 - K为偶数", [this]() {
            ListNode* head = new ListNode(1);
            head->next = new ListNode(2);
            head->next->next = new ListNode(3);
            head->next->next->next = new ListNode(4);
            head->next->next->next->next = new ListNode(5);
            ListNode* reversed = solution.reverseKGroup(head, 2); // K=2
            return (reversed->val == 2 && reversed->next->val == 1 && reversed->next->next->val == 4 &&
                    reversed->next->next->next->val == 3 && reversed->next->next->next->next->val == 5); // 应该返回翻转后的链表
        });

        runTest("测试K个一组翻转链表 - K为奇数", [this]() {
            ListNode* head = new ListNode(1);
            head->next = new ListNode(2);
            head->next->next = new ListNode(3);
            head->next->next->next = new ListNode(4);
            head->next->next->next->next = new ListNode(5);
            ListNode* reversed = solution.reverseKGroup(head, 3); // K=3
            return (reversed->val == 3 && reversed->next->val == 2 && reversed->next->next->val == 1 &&
                    reversed->next->next->next->val == 4 && reversed->next->next->next->next->val == 5); // 应该返回翻转后的链表
        });

        runTest("测试删除链表的倒数第N个节点", [this]() {
            ListNode* head = new ListNode(1);
            head->next = new ListNode(2);
            head->next->next = new ListNode(3);
            head->next->next->next = new ListNode(4);
            head->next->next->next->next = new ListNode(5);
            ListNode* modified = solution.removeNthFromEnd(head, 2);
            return (modified->val == 1 && modified->next->val == 2 && modified->next->next->val == 3 &&
                    modified->next->next->next->val == 5);
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