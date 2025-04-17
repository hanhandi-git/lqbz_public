#include <iostream>
#include <vector>
#include <queue>
#include <functional>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class StreamMerger {
private:
    // 用于小顶堆的比较器
    struct NodeCompare {
        bool operator()(const ListNode* a, const ListNode* b) {
            return a->val > b->val;
        }
    };
    
    // 小顶堆，用于合并排序
    std::priority_queue<ListNode*, std::vector<ListNode*>, NodeCompare> minHeap;
    
    // 结果链表的头尾节点
    ListNode dummy;
    ListNode* tail;

public:
    StreamMerger() : tail(&dummy) {
        dummy.next = nullptr;
    }
    
    // 添加新的链表到合并流程中
    void addList(ListNode* head) {
        if (!head) return;
        
        // 将整个链表加入堆中
        while (head) {
            ListNode* next = head->next;
            head->next = nullptr;  // 断开连接，避免出现环
            minHeap.push(head);
            head = next;
        }
    }
    
    // 完成合并并返回结果
    ListNode* finish() {
        // 从堆中依次取出最小的节点，构建结果链表
        while (!minHeap.empty()) {
            ListNode* node = minHeap.top();
            minHeap.pop();
            tail->next = node;
            tail = node;
        }
        return dummy.next;
    }
    
    // 析构函数，清理未处理的节点
    ~StreamMerger() {
        ListNode* current = dummy.next;
        while (current) {
            ListNode* next = current->next;
            delete current;
            current = next;
        }
        
        while (!minHeap.empty()) {
            ListNode* node = minHeap.top();
            minHeap.pop();
            delete node;
        }
    }
};

// 测试框架
class TestFramework {
private:
    bool listsEqual(ListNode* l1, ListNode* l2) {
        while (l1 && l2) {
            if (l1->val != l2->val) return false;
            l1 = l1->next;
            l2 = l2->next;
        }
        return !l1 && !l2;
    }
    
    ListNode* createList(std::initializer_list<int> values) {
        ListNode dummy(0);
        ListNode* current = &dummy;
        for (int val : values) {
            current->next = new ListNode(val);
            current = current->next;
        }
        return dummy.next;
    }
    
    void printList(ListNode* head) {
        while (head) {
            std::cout << head->val << " ";
            head = head->next;
        }
        std::cout << std::endl;
    }

public:
    void runTests() {
        // 测试1：基本合并功能
        {
            StreamMerger merger;
            merger.addList(createList({1, 4, 5}));
            merger.addList(createList({1, 3, 4}));
            merger.addList(createList({2, 6}));
            
            ListNode* result = merger.finish();
            ListNode* expected = createList({1, 1, 2, 3, 4, 4, 5, 6});
            
            std::cout << "测试1 - 基本合并功能: ";
            if (listsEqual(result, expected)) {
                std::cout << "通过" << std::endl;
            } else {
                std::cout << "失败" << std::endl;
                std::cout << "预期结果: ";
                printList(expected);
                std::cout << "实际结果: ";
                printList(result);
            }
        }
        
        // 测试2：大批次测试
        {
            StreamMerger merger;
            for (int i = 10; i >= 1; --i) {
                merger.addList(createList({i, i+10}));
            }
            
            ListNode* result = merger.finish();
            std::cout << "测试2 - 大批次测试: ";
            std::cout << "结果: ";
            printList(result);
        }
        
        // 测试3：空链表处理
        {
            StreamMerger merger;
            merger.addList(nullptr);
            
            ListNode* result = merger.finish();
            std::cout << "测试3 - 空链表处理: ";
            if (result == nullptr) {
                std::cout << "通过" << std::endl;
            } else {
                std::cout << "失败" << std::endl;
            }
        }
    }
};

int main() {
    TestFramework testFramework;
    testFramework.runTests();
    return 0;
}
