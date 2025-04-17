#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>

// Definition for a Node.
class Node {
public:
    int val;
    Node* next;
    Node* random;
    
    Node(int _val) {
        val = _val;
        next = nullptr;
        random = nullptr;
    }
};

class Solution {
public:
    // 方法一：哈希表法
    Node* copyRandomList1(Node* head) {
        if (!head) return nullptr;
        
        // 创建原节点到新节点的映射
        std::unordered_map<Node*, Node*> nodeMap;
        
        // 第一次遍历：创建所有新节点
        Node* curr = head;
        while (curr) {
            nodeMap[curr] = new Node(curr->val);
            curr = curr->next;
        }
        
        // 第二次遍历：设置指针
        curr = head;
        while (curr) {
            nodeMap[curr]->next = nodeMap[curr->next];
            nodeMap[curr]->random = nodeMap[curr->random];
            curr = curr->next;
        }
        
        return nodeMap[head];
    }
    
    // 方法二：原地修改法
    Node* copyRandomList2(Node* head) {
        if (!head) return nullptr;
        
        // 第一步：在每个原节点后创建新节点
        Node* curr = head;
        while (curr) {
            Node* newNode = new Node(curr->val);
            newNode->next = curr->next;
            curr->next = newNode;
            curr = newNode->next;
        }
        
        // 第二步：设置random指针
        curr = head;
        while (curr) {
            if (curr->random) {
                curr->next->random = curr->random->next;
            }
            curr = curr->next->next;
        }
        
        // 第三步：分离两个链表
        Node* newHead = head->next;
        curr = head;
        Node* currNew = newHead;
        while (curr) {
            curr->next = currNew->next;
            curr = curr->next;
            if (curr) {
                currNew->next = curr->next;
                currNew = currNew->next;
            }
        }
        
        return newHead;
    }

    // 方法三：直接构造法
    Node* copyRandomList3(Node* head) {
        if (!head) return nullptr;
        
        // 创建新的头节点
        Node* newHead = new Node(head->val);
        Node* oldCurr = head->next;
        Node* newCurr = newHead;
        
        // 第一次遍历：构建所有新节点并连接next指针
        while (oldCurr) {
            newCurr->next = new Node(oldCurr->val);
            newCurr = newCurr->next;
            oldCurr = oldCurr->next;
        }
        
        // 第二次遍历：设置random指针
        oldCurr = head;
        newCurr = newHead;
        while (oldCurr) {
            if (oldCurr->random) {
                // 找到random指针在原链表中的位置
                Node* oldRandom = oldCurr->random;
                Node* oldTemp = head;
                Node* newTemp = newHead;
                
                // 同步移动直到找到对应位置
                while (oldTemp != oldRandom) {
                    oldTemp = oldTemp->next;
                    newTemp = newTemp->next;
                }
                newCurr->random = newTemp;
            }
            oldCurr = oldCurr->next;
            newCurr = newCurr->next;
        }
        
        return newHead;
    }
};

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;
    
    // 辅助函数：创建测试链表
    Node* createList(const std::vector<std::pair<int, int>>& nodes) {
        if (nodes.empty()) return nullptr;
        
        // 创建所有节点
        std::vector<Node*> nodeList;
        for (const auto& node : nodes) {
            nodeList.push_back(new Node(node.first));
        }
        
        // 设置next指针
        for (size_t i = 0; i < nodeList.size() - 1; ++i) {
            nodeList[i]->next = nodeList[i + 1];
        }
        
        // 设置random指针
        for (size_t i = 0; i < nodes.size(); ++i) {
            if (nodes[i].second != -1) {
                nodeList[i]->random = nodeList[nodes[i].second];
            }
        }
        
        return nodeList[0];
    }
    
    // 辅助函数：验证两个链表是否相同
    bool compareLists(Node* l1, Node* l2) {
        std::unordered_map<Node*, int> pos1, pos2;
        
        // 记录第一个链表中节点的位置
        Node* curr = l1;
        int index = 0;
        while (curr) {
            pos1[curr] = index++;
            curr = curr->next;
        }
        
        // 记录第二个链表中节点的位置
        curr = l2;
        index = 0;
        while (curr) {
            pos2[curr] = index++;
            curr = curr->next;
        }
        
        // 比较两个链表
        curr = l1;
        Node* curr2 = l2;
        while (curr && curr2) {
            if (curr->val != curr2->val) return false;
            
            // 比较random指针
            if ((!curr->random && curr2->random) || 
                (curr->random && !curr2->random)) return false;
            
            if (curr->random && curr2->random) {
                int pos1_random = pos1[curr->random];
                int pos2_random = pos2[curr2->random];
                if (pos1_random != pos2_random) return false;
            }
            
            curr = curr->next;
            curr2 = curr2->next;
        }
        
        return !curr && !curr2;
    }
    
    // 辅助函数：清理链表内存
    void cleanupList(Node* head) {
        while (head) {
            Node* temp = head;
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
        // 测试用例1：基本示例
        runTest("测试1: 基本示例", [this]() {
            std::vector<std::pair<int, int>> nodes = {
                {7, -1}, {13, 0}, {11, 4}, {10, 2}, {1, 0}
            };
            Node* head = createList(nodes);
            Node* result1 = solution.copyRandomList1(head);
            Node* result2 = solution.copyRandomList2(head);
            Node* result3 = solution.copyRandomList3(head);
            bool isValid = compareLists(head, result1) && compareLists(head, result2) && compareLists(head, result3);
            cleanupList(head);
            cleanupList(result1);
            cleanupList(result2);
            cleanupList(result3);
            return isValid;
        });
        
        // 测试用例2：空链表
        runTest("测试2: 空链表", [this]() {
            return solution.copyRandomList1(nullptr) == nullptr &&
                   solution.copyRandomList2(nullptr) == nullptr &&
                   solution.copyRandomList3(nullptr) == nullptr;
        });
        
        // 测试用例3：单节点
        runTest("测试3: 单节点", [this]() {
            std::vector<std::pair<int, int>> nodes = {{1, 0}};
            Node* head = createList(nodes);
            Node* result1 = solution.copyRandomList1(head);
            Node* result2 = solution.copyRandomList2(head);
            Node* result3 = solution.copyRandomList3(head);
            bool isValid = compareLists(head, result1) && compareLists(head, result2) && compareLists(head, result3);
            cleanupList(head);
            cleanupList(result1);
            cleanupList(result2);
            cleanupList(result3);
            return isValid;
        });
        
        // 测试用例4：循环引用
        runTest("测试4: 循环引用", [this]() {
            std::vector<std::pair<int, int>> nodes = {{1, 1}, {2, 0}};
            Node* head = createList(nodes);
            Node* result1 = solution.copyRandomList1(head);
            Node* result2 = solution.copyRandomList2(head);
            Node* result3 = solution.copyRandomList3(head);
            bool isValid = compareLists(head, result1) && compareLists(head, result2) && compareLists(head, result3);
            cleanupList(head);
            cleanupList(result1);
            cleanupList(result2);
            cleanupList(result3);
            return isValid;
        });
        
        std::cout << "测试完成：共 " << total << " 个测试用例，通过 " << passed << " 个" << std::endl;
    }
};

int main() {
    TestFramework testFramework;
    testFramework.runAllTests();
    return 0;
}
