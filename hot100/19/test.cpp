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
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy = ListNode(-1);
        dummy.next = head;
        ListNode* slow = &dummy;
        ListNode* fast = &dummy;
        // fast先走n步
        for (int i = 0; i < n; i++) {
            if (fast == nullptr) return nullptr;
            fast = fast->next;
        }
        while (fast->next)
        {
            fast = fast->next;
            slow = slow->next;
        }
        ListNode* del = slow->next;
        slow->next = slow->next->next;
        delete del;
        return dummy.next;
    }
};

// 测试框架
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    // 辅助函数：从vector构建链表
    ListNode* buildList(const vector<int>& nums) {
        if (nums.empty()) return nullptr;
        ListNode* dummy = new ListNode(0);
        ListNode* curr = dummy;
        for (int num : nums) {
            curr->next = new ListNode(num);
            curr = curr->next;
        }
        ListNode* head = dummy->next;
        delete dummy;
        return head;
    }

    // 辅助函数：将链表转换为vector
    vector<int> listToVector(ListNode* head) {
        vector<int> result;
        while (head) {
            result.push_back(head->val);
            head = head->next;
        }
        return result;
    }

    // 辅助函数：释放链表内存
    void deleteList(ListNode* head) {
        while (head) {
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
        // 测试用例1: [1,2,3,4,5], n = 2
        runTest("基本测试 - 5个节点删除倒数第2个", [this]() {
            ListNode* head = buildList({1,2,3,4,5});
            ListNode* result = solution.removeNthFromEnd(head, 2);
            vector<int> resultVec = listToVector(result);
            vector<int> expected = {1,2,3,5};
            deleteList(result);
            return resultVec == expected;
        });

        // 测试用例2: [1], n = 1
        runTest("边界测试 - 单节点删除", [this]() {
            ListNode* head = buildList({1});
            ListNode* result = solution.removeNthFromEnd(head, 1);
            vector<int> resultVec = listToVector(result);
            return resultVec.empty();
        });

        // 测试用例3: [1,2], n = 1
        runTest("基本测试 - 双节点删除倒数第1个", [this]() {
            ListNode* head = buildList({1,2});
            ListNode* result = solution.removeNthFromEnd(head, 1);
            vector<int> resultVec = listToVector(result);
            vector<int> expected = {1};
            deleteList(result);
            return resultVec == expected;
        });

        // 测试用例4: [1,2], n = 2
        runTest("特殊测试 - 删除头节点", [this]() {
            ListNode* head = buildList({1,2});
            ListNode* result = solution.removeNthFromEnd(head, 2);
            vector<int> resultVec = listToVector(result);
            vector<int> expected = {2};
            deleteList(result);
            return resultVec == expected;
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