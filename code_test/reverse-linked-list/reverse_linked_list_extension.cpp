#include <iostream>
#include <list>
#include <unordered_map>
#include <queue>
#include <random>
#include <functional>

// 定义链表节点结构
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 辅助函数：反转链表的一部分
std::pair<ListNode*, ListNode*> reverseList(ListNode* head, ListNode* tail) {
    ListNode* prev = tail->next;
    ListNode* p = head;
    while (prev != tail) {
        ListNode* next = p->next;
        p->next = prev;
        prev = p;
        p = next;
    }
    return {tail, head};
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
// 2. K 个一组翻转链表
ListNode* reverseKGroup(ListNode* head, int k) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode* prev = &dummy;
    
    while (head) {
        ListNode* tail = prev;
        for (int i = 0; i < k; ++i) {
            tail = tail->next;
            if (!tail) return dummy.next;
        }
        
        ListNode* next = tail->next;
        std::tie(head, tail) = reverseList(head, tail);
        prev->next = head;
        tail->next = next;
        prev = tail;
        head = tail->next;
    }
    
    return dummy.next;
}


// 3. 两两交换链表中的节点
ListNode* swapPairs(ListNode* head) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode* prev = &dummy;
    
    while (head && head->next) {
        ListNode* first = head;
        ListNode* second = head->next;
        
        prev->next = second;
        first->next = second->next;
        second->next = first;
        
        prev = first;
        head = first->next;
    }
    
    return dummy.next;
}

// 辅助函数：打印链表
void printList(ListNode* head) {
    while (head) {
        std::cout << head->val << " -> ";
        head = head->next;
    }
    std::cout << "nullptr" << std::endl;
}

// Test Framework
class TestFramework {
private:
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

public:
    void runAllTests() {
        // 原有的测试代码保持不变...

        // 新增的链表反转测试
        runTest("TestReverseBetween", [this]() {
            ListNode* head = new ListNode(1);
            head->next = new ListNode(2);
            head->next->next = new ListNode(3);
            head->next->next->next = new ListNode(4);
            head->next->next->next->next = new ListNode(5);
            
            std::cout << "Original list: ";
            printList(head);
            
            head = reverseBetween(head, 2, 4);
            
            std::cout << "After reversing from position 2 to 4: ";
            printList(head);
            
            // 验证结果
            return (head->val == 1 && head->next->val == 4 && 
                    head->next->next->val == 3 && head->next->next->next->val == 2 && 
                    head->next->next->next->next->val == 5);
        });

        runTest("TestReverseKGroup", [this]() {
            ListNode* head = new ListNode(1);
            head->next = new ListNode(2);
            head->next->next = new ListNode(3);
            head->next->next->next = new ListNode(4);
            head->next->next->next->next = new ListNode(5);
            
            std::cout << "Original list: ";
            printList(head);
            
            head = reverseKGroup(head, 2);
            
            std::cout << "After reversing in groups of 2: ";
            printList(head);
            
            // 验证结果
            return (head->val == 2 && head->next->val == 1 && 
                    head->next->next->val == 4 && head->next->next->next->val == 3 && 
                    head->next->next->next->next->val == 5);
        });

        runTest("TestSwapPairs", [this]() {
            ListNode* head = new ListNode(1);
            head->next = new ListNode(2);
            head->next->next = new ListNode(3);
            head->next->next->next = new ListNode(4);
            
            std::cout << "Original list: ";
            printList(head);
            
            head = swapPairs(head);
            
            std::cout << "After swapping pairs: ";
            printList(head);
            
            // 验证结果
            return (head->val == 2 && head->next->val == 1 && 
                    head->next->next->val == 4 && head->next->next->next->val == 3);
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
