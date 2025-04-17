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
    // 迭代解法
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode dummy(0);
        dummy.next = head;
        ListNode* prev = &dummy;
        
        while (head) {
            // 检查剩余节点是否足够k个
            ListNode* tail = prev;
            for (int i = 0; i < k; i++) {
                tail = tail->next;
                if (!tail) return dummy.next;
            }
            
            // 保存下一组的起始位置
            ListNode* next = tail->next;
            
            // 反转k个节点
            pair<ListNode*, ListNode*> result = reverseList(head, tail);
            head = result.first;
            tail = result.second;
            
            // 连接前后的节点
            prev->next = head;
            tail->next = next;
            
            // 移动指针
            prev = tail;
            head = next;
        }
        
        return dummy.next;
    }
    
    // 反转链表的辅助函数，返回新的头和尾
    pair<ListNode*, ListNode*> reverseList(ListNode* head, ListNode* tail) {
        ListNode* prev = tail->next;
        ListNode* curr = head;
        while (prev != tail) {
            ListNode* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        return {tail, head};
    }
    
    ListNode* reverseKGroupRecursive(ListNode* head, int k) {
        // 检查是否有k个节点
        int count = 0;
        ListNode* curr = head;
        while(curr && count < k) {
            curr = curr->next;
            count++;
        }
        if (count < k) return head;
        // 翻转前K个节点
        ListNode* pre = nullptr;
        curr = head;
        for (int i = 0; i < k; i++) 
        {
            ListNode* next = curr->next;
            curr->next = pre;
            pre = curr;
            curr = next;
        }
        // 递归
        head->next = reverseKGroupRecursive(curr, k);
        return pre;
    }

    // 递归解法
    // ListNode* reverseKGroupRecursive(ListNode* head, int k) {
    //     // 检查是否有k个节点
    //     ListNode* curr = head;
    //     int count = 0;
    //     while (curr && count < k) {
    //         curr = curr->next;
    //         count++;
    //     }
    //     if (count < k) return head;
        
    //     // 反转前k个节点
    //     curr = head;
    //     ListNode* prev = nullptr;
    //     for (int i = 0; i < k; i++) {
    //         ListNode* next = curr->next;
    //         curr->next = prev;
    //         prev = curr;
    //         curr = next;
    //     }
        
    //     // 递归处理剩余节点
    //     head->next = reverseKGroupRecursive(curr, k);
        
    //     return prev;
    // }
};

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
        // 测试迭代解法
        runTest("迭代解法 - k=2基本测试", [this]() {
            ListNode* head = createList({1,2,3,4});
            ListNode* expected = createList({2,1,4,3});
            ListNode* result = solution.reverseKGroup(head, 2);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        runTest("迭代解法 - k=3基本测试", [this]() {
            ListNode* head = createList({1,2,3,4,5});
            ListNode* expected = createList({3,2,1,4,5});
            ListNode* result = solution.reverseKGroup(head, 3);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        runTest("迭代解法 - 空链表", [this]() {
            return solution.reverseKGroup(nullptr, 2) == nullptr;
        });

        runTest("迭代解法 - 单节点链表", [this]() {
            ListNode* head = createList({1});
            ListNode* expected = createList({1});
            ListNode* result = solution.reverseKGroup(head, 2);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        // 测试递归解法
        runTest("递归解法 - k=2基本测试", [this]() {
            ListNode* head = createList({1,2,3,4});
            ListNode* expected = createList({2,1,4,3});
            ListNode* result = solution.reverseKGroupRecursive(head, 2);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        runTest("递归解法 - k=3基本测试", [this]() {
            ListNode* head = createList({1,2,3,4,5});
            ListNode* expected = createList({3,2,1,4,5});
            ListNode* result = solution.reverseKGroupRecursive(head, 3);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
        });

        runTest("递归解法 - 空链表", [this]() {
            return solution.reverseKGroupRecursive(nullptr, 2) == nullptr;
        });

        runTest("递归解法 - 单节点链表", [this]() {
            ListNode* head = createList({1});
            ListNode* expected = createList({1});
            ListNode* result = solution.reverseKGroupRecursive(head, 2);
            bool isEqual = areListsEqual(result, expected);
            deleteList(result);
            deleteList(expected);
            return isEqual;
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