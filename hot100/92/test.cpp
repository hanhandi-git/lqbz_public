#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>  

class ListNode {
public:
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};


class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode dummy = ListNode(-1);
        dummy.next = head;
        // 找到left的前一个节点
        ListNode* pre = &dummy;
        for (int i = 0; i < left - 1; i++)
        {
            pre = pre->next;
        }
        // 找到right节点
        ListNode* rightNode = pre;
        for (int i = 0; i < right - left + 1; i++)
        {
            rightNode = rightNode->next;
        }

        // 切断
        ListNode* leftNode = pre->next;
        ListNode* curr = rightNode->next;
        pre->next = nullptr;
        rightNode->next = nullptr;

        // 翻转
        reverseLinkedList(leftNode);

        // 接回
        pre->next = rightNode;
        leftNode->next = curr;

        return dummy.next;
        
    }

    ListNode* reverseLinkedList(ListNode* head)
    {
        ListNode* pre = nullptr;
        ListNode* curr = head;
        while (curr != nullptr)
        {
            ListNode* next = curr->next;
            curr->next = pre;
            pre = curr;
            curr = next;
        }
        return pre;
    }
};

// class Solution {
// public:
//     ListNode* reverseBetween(ListNode* head, int left, int right) {
//         // 创建哑节点，简化边界情况处理
//         ListNode* dummy = new ListNode(0);
//         dummy->next = head;
        
//         // prev指向要反转部分的前一个节点
//         ListNode* prev = dummy;
//         for (int i = 1; i < left; i++) {
//             prev = prev->next;
//         }
        
//         // curr指向要反转的起始节点
//         ListNode* curr = prev->next;
        
//         // 进行局部反转
//         for (int i = 0; i < right - left; i++) {
//             ListNode* next = curr->next;
//             curr->next = next->next;
//             next->next = prev->next;
//             prev->next = next;
//         }
        
//         // 释放哑节点并返回结果
//         ListNode* result = dummy->next;
//         delete dummy;
//         return result;
//     }
// };

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    // 创建链表
    ListNode* createLinkedList(const std::vector<int>& values) {
        if (values.empty()) return nullptr;
        
        ListNode* head = new ListNode(values[0]);
        ListNode* current = head;
        
        for (size_t i = 1; i < values.size(); i++) {
            current->next = new ListNode(values[i]);
            current = current->next;
        }
        
        return head;
    }
    
    // 将链表转换为vector以便比较
    std::vector<int> linkedListToVector(ListNode* head) {
        std::vector<int> result;
        ListNode* current = head;
        
        while (current) {
            result.push_back(current->val);
            current = current->next;
        }
        
        return result;
    }
    
    // 释放链表内存
    void deleteLinkedList(ListNode* head) {
        while (head) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        if (test()) {
            std::cout << "PASS: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "FAIL: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        // 测试用例1：反转链表中间部分
        runTest("反转链表中间部分", [this]() {
            ListNode* head = createLinkedList({1, 2, 3, 4, 5});
            ListNode* result = solution.reverseBetween(head, 2, 4);
            
            std::vector<int> expected = {1, 4, 3, 2, 5};
            std::vector<int> actual = linkedListToVector(result);
            
            deleteLinkedList(result);
            return expected == actual;
        });

        // 测试用例2：反转单节点链表
        runTest("反转单节点链表", [this]() {
            ListNode* head = createLinkedList({5});
            ListNode* result = solution.reverseBetween(head, 1, 1);
            
            std::vector<int> expected = {5};
            std::vector<int> actual = linkedListToVector(result);
            
            deleteLinkedList(result);
            return expected == actual;
        });

        // 测试用例3：反转整个链表
        runTest("反转整个链表", [this]() {
            ListNode* head = createLinkedList({1, 2, 3, 4, 5});
            ListNode* result = solution.reverseBetween(head, 1, 5);
            
            std::vector<int> expected = {5, 4, 3, 2, 1};
            std::vector<int> actual = linkedListToVector(result);
            
            deleteLinkedList(result);
            return expected == actual;
        });

        // 测试用例4：反转链表开头部分
        runTest("反转链表开头部分", [this]() {
            ListNode* head = createLinkedList({1, 2, 3, 4, 5});
            ListNode* result = solution.reverseBetween(head, 1, 3);
            
            std::vector<int> expected = {3, 2, 1, 4, 5};
            std::vector<int> actual = linkedListToVector(result);
            
            deleteLinkedList(result);
            return expected == actual;
        });

        // 测试用例5：反转链表尾部
        runTest("反转链表尾部", [this]() {
            ListNode* head = createLinkedList({1, 2, 3, 4, 5});
            ListNode* result = solution.reverseBetween(head, 3, 5);
            
            std::vector<int> expected = {1, 2, 5, 4, 3};
            std::vector<int> actual = linkedListToVector(result);
            
            deleteLinkedList(result);
            return expected == actual;
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