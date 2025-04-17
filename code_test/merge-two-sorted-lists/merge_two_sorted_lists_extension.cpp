#include <iostream>
#include <vector>
#include <queue>
#include <functional>


// 定义链表节点结构
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    // 1. 合并两个有序链表
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy = ListNode(0);
        ListNode* cur = &dummy;
        while (l1 != nullptr && l2 != nullptr)
        {
            if (l1->val <= l2->val) 
            {
                cur->next = l1;
                l1 = l1->next;
            }
            else 
            {
                cur->next = l2;
                l2 = l2->next;
            }
            cur = cur->next; 
        }
        if (l1 != nullptr)
        {
            cur->next = l1;
        }
        if (l2 != nullptr)
        {
            cur->next = l2;
        }
        return dummy.next;
    }

    // 2. 合并K个升序链表 - 归并实现
    ListNode* mergeKLists(std::vector<ListNode*>& lists) {
        if (lists.empty()) return nullptr;
        return mergeKListsHelper(lists, 0, lists.size() - 1);
    }

    // 2.2 合并K个升序链表 - 小顶堆实现
    ListNode* mergeKListsHeap(std::vector<ListNode*>& lists) {
        // 自定义比较函数，按节点值升序排列
        auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
        std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(cmp)> pq(cmp);
        
        // 将所有链表的头节点加入小顶堆
        for (ListNode* list : lists) {
            if (list != nullptr) {
                pq.push(list);
            }
        }
        
        ListNode dummy(0);
        ListNode* tail = &dummy;
        
        // 不断从堆中取出最小节点
        while (!pq.empty()) {
            ListNode* smallest = pq.top();
            pq.pop();
            tail->next = smallest;
            tail = tail->next;
            
            if (smallest->next != nullptr) {
                pq.push(smallest->next);
            }
        }
        
        return dummy.next;
    }

    // 3. 排序链表
    ListNode* sortList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) {
            return head;
        }
        
        // 找到链表中点
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        ListNode* mid = slow->next;
        slow->next = nullptr;
        
        // 递归排序
        ListNode* left = sortList(head);
        ListNode* right = sortList(mid);
        
        // 合并有序链表
        return mergeTwoLists(left, right);
    }

    /**
     * @brief 将两个链表表示的数字相加
     * @param l1 第一个链表，表示一个数字
     * @param l2 第二个链表，表示一个数字
     * @return 返回相加后的结果链表
     * @note 链表中的每个节点包含一个数字，且数字是反序存储的
     */
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* cur = &dummy;
        int carry = 0;
        // 当两个链表均遍历完，但是进位还有数据的时候也需要进行计算
        while (l1 != nullptr || l2 != nullptr || carry != 0) {
            int x = (l1 != nullptr) ? l1->val : 0;
            int y = (l2 != nullptr) ? l2->val : 0;
            int sum = carry + x + y;
            carry = sum / 10;
            cur->next = new ListNode(sum % 10);
            cur = cur->next;
            
            if (l1 != nullptr) l1 = l1->next;
            if (l2 != nullptr) l2 = l2->next;
        }
        
        return dummy.next;
    }

private:
    // 归并辅助函数
    ListNode* mergeKListsHelper(std::vector<ListNode*>& lists, int left, int right) {
        // 基础情况1：如果left等于right，说明只有一个链表，直接返回
        if (left == right) {
            return lists[left];
        }
        // 基础情况2：如果只有两个链表，直接合并这两个链表
        if (left + 1 == right) {
            return mergeTwoLists(lists[left], lists[right]);
        }
        // 计算中间位置
        int mid = left + ((right - left) >> 1);
        // 递归处理左半部分链表
        ListNode* leftList = mergeKListsHelper(lists, left, mid);
        // 递归处理右半部分链表
        ListNode* rightList = mergeKListsHelper(lists, mid + 1, right);
        return mergeTwoLists(leftList, rightList);
    }
};

// 辅助函数：创建链表
ListNode* createList(std::initializer_list<int> values) {
    ListNode dummy(0);
    ListNode* current = &dummy;
    for (int val : values) {
        current->next = new ListNode(val);
        current = current->next;
    }
    return dummy.next;
}

// 辅助函数：打印链表
void printList(ListNode* head) {
    while (head != nullptr) {
        std::cout << head->val << " ";
        head = head->next;
    }
    std::cout << std::endl;
}

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

    bool listsEqual(ListNode* l1, ListNode* l2) {
        while (l1 != nullptr && l2 != nullptr) {
            if (l1->val != l2->val) return false;
            l1 = l1->next;
            l2 = l2->next;
        }
        return l1 == nullptr && l2 == nullptr;
    }

    // 清理链表内存
    void deleteList(ListNode* head) {
        while (head != nullptr) {
            ListNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

public:
    void runAllTests() {
        // 测试合并两个有序链表
        runTest("合并两个有序链表 - 正常情况", [this]() {
            ListNode* l1 = createList({1,2,4});
            ListNode* l2 = createList({1,3,4});
            ListNode* expected = createList({1,1,2,3,4,4});
            ListNode* result = solution.mergeTwoLists(l1, l2);
            return listsEqual(result, expected);
        });

        // 测试合并K个升序链表
        runTest("合并K个升序链表", [this]() {
            std::vector<ListNode*> lists = {
                createList({1,4,5}),
                createList({1,3,4}),
                createList({2,6})
            };
            ListNode* expected = createList({1,1,2,3,4,4,5,6});
            ListNode* result = solution.mergeKLists(lists);
            return listsEqual(result, expected);
        });

        // 测试排序链表
        runTest("排序链表", [this]() {
            ListNode* head = createList({4,2,1,3});
            ListNode* expected = createList({1,2,3,4});
            ListNode* result = solution.sortList(head);
            return listsEqual(result, expected);
        });

        // 测试两数相加
        runTest("两数相加", [this]() {
            ListNode* l1 = createList({2,4,3});
            ListNode* l2 = createList({5,6,4});
            ListNode* expected = createList({7,0,8});
            ListNode* result = solution.addTwoNumbers(l1, l2);
            bool isEqual = listsEqual(result, expected);
            
            // 清理内存
            deleteList(l1);
            deleteList(l2);
            deleteList(expected);
            deleteList(result);
            
            return isEqual;
        });

        // 测试合并K个升序链表 - 归并实现
        runTest("合并K个升序链表 - 归并实现", [this]() {
            std::vector<ListNode*> lists = {
                createList({1,4,5}),
                createList({1,3,4}),
                createList({2,6})
            };
            ListNode* expected = createList({1,1,2,3,4,4,5,6});
            ListNode* result = solution.mergeKLists(lists);
            return listsEqual(result, expected);
        });

        // 测试合并K个升序链表 - 小顶堆实现
        runTest("合并K个升序链表 - 小顶堆实现", [this]() {
            std::vector<ListNode*> lists = {
                createList({1,4,5}),
                createList({1,3,4}),
                createList({2,6})
            };
            ListNode* expected = createList({1,1,2,3,4,4,5,6});
            ListNode* result = solution.mergeKListsHeap(lists);
            return listsEqual(result, expected);
        });

        // 测试合并K个升序链表 - 空链表情况
        runTest("合并K个升序链表 - 空链表", [this]() {
            std::vector<ListNode*> lists = {};
            ListNode* result1 = solution.mergeKLists(lists);
            ListNode* result2 = solution.mergeKListsHeap(lists);
            return result1 == nullptr && result2 == nullptr;
        });

        // 测试合并K个升序链表 - 单链表情况
        runTest("合并K个升序链表 - 单链表", [this]() {
            std::vector<ListNode*> lists = {createList({1,2,3})};
            ListNode* expected = createList({1,2,3});
            ListNode* result1 = solution.mergeKLists(lists);
            ListNode* result2 = solution.mergeKListsHeap(lists);
            return listsEqual(result1, expected) && listsEqual(result2, expected);
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
