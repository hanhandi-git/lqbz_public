#include <iostream>
#include <vector>
#include <string>

// 链表节点定义
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

// 实现Solution类
class Solution {
public:
    ListNode* sortList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast && fast->next) {
            fast = fast->next->next;
            slow = slow->next;
        }
        ListNode* mid = slow->next;
        slow->next = nullptr;

        ListNode* left = sortList(head);
        ListNode* right = sortList(mid);

        return merge(left, right);
    }
    
private:
    ListNode* merge(ListNode* l1, ListNode* l2) {
       ListNode dummy = ListNode(0);
       ListNode* cur = &dummy;
       while (l1 && l2) {
            if (l1->val < l2->val) {
                cur->next = l1;
                l1 = l1->next;
            } else {
                cur->next = l2;
                l2 = l2->next;
            }
            cur = cur->next;
       }
       if (l1) cur->next = l1;
       if (l2) cur->next = l2;
       return dummy.next;
    }
};

// 测试框架
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;
    
    // 创建链表的辅助函数
    ListNode* createList(const std::vector<int>& nums) {
        ListNode dummy(0);
        ListNode* curr = &dummy;
        for (int num : nums) {
            curr->next = new ListNode(num);
            curr = curr->next;
        }
        return dummy.next;
    }
    
    // 将链表转换为vector的辅助函数
    std::vector<int> listToVector(ListNode* head) {
        std::vector<int> result;
        while (head) {
            result.push_back(head->val);
            head = head->next;
        }
        return result;
    }
    
    // 释放链表内存的辅助函数
    void freeList(ListNode* head) {
        while (head) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    void runTest(const std::string& testName, 
                const std::vector<int>& input, 
                const std::vector<int>& expected) {
        total++;
        ListNode* head = createList(input);
        ListNode* result = solution.sortList(head);
        std::vector<int> resultVec = listToVector(result);
        
        if (resultVec == expected) {
            std::cout << "通过: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "失败: " << testName << std::endl;
            std::cout << "预期: ";
            for (int num : expected) std::cout << num << " ";
            std::cout << "\n实际: ";
            for (int num : resultVec) std::cout << num << " ";
            std::cout << std::endl;
        }
        
        freeList(result);
    }

public:
    void runAllTests() {
        // 测试示例1
        runTest("示例1: [4,2,1,3]", 
                {4,2,1,3}, 
                {1,2,3,4});
        
        // 测试示例2
        runTest("示例2: [-1,5,3,4,0]", 
                {-1,5,3,4,0}, 
                {-1,0,3,4,5});
        
        // 测试示例3
        runTest("示例3: []", 
                {}, 
                {});
        
        // 测试单个元素
        runTest("单个元素: [1]", 
                {1}, 
                {1});
        
        // 测试已排序
        runTest("已排序: [1,2,3,4]", 
                {1,2,3,4}, 
                {1,2,3,4});
        
        // 测试逆序
        runTest("逆序: [4,3,2,1]", 
                {4,3,2,1}, 
                {1,2,3,4});
        
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