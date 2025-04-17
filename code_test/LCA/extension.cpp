#include <iostream>
#include <functional>
#include <string>
#include <unordered_set>
#include <vector>

// 二叉树节点定义
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;  // 用于带父指针的变体
    TreeNode(int x) : val(x), left(nullptr), right(nullptr), parent(nullptr) {}
};

// N叉树节点定义
struct Node {
    int val;
    std::vector<Node*> children;
    Node(int x) : val(x) {}
};

class Solution {
public:
    // 1. 二叉搜索树的最近公共祖先
    TreeNode* lowestCommonAncestorBST(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (root == nullptr) return nullptr;
        
        // 如果p、q都在左子树
        if (p->val < root->val && q->val < root->val) {
            return lowestCommonAncestorBST(root->left, p, q);
        }
        
        // 如果p、q都在右子树
        if (p->val > root->val && q->val > root->val) {
            return lowestCommonAncestorBST(root->right, p, q);
        }
        
        // 如果p、q分别在左右子树，或者其中一个就是root
        return root;
    }
    
    // 2. 带有父指针的二叉树的最近公共祖先
    TreeNode* lowestCommonAncestorWithParent(TreeNode* p, TreeNode* q) {
        std::unordered_set<TreeNode*> ancestors;
        
        // 记录p的所有祖先节点
        while (p != nullptr) {
            ancestors.insert(p);
            p = p->parent;
        }
        
        // 检查q的祖先节点是否在p的祖先集合中
        while (q != nullptr) {
            if (ancestors.count(q)) {
                return q;
            }
            q = q->parent;
        }
        
        return nullptr;
    }
    
    // 3. N叉树的最近公共祖先
    Node* lowestCommonAncestorNary(Node* root, Node* p, Node* q) {
        if (root == nullptr || root == p || root == q) {
            return root;
        }
        
        Node* found = nullptr;
        int count = 0;
        
        // 遍历所有子节点
        for (Node* child : root->children) {
            Node* result = lowestCommonAncestorNary(child, p, q);
            if (result != nullptr) {
                count++;
                found = result;
            }
        }
        
        // 如果找到两个节点，返回当前root
        if (count == 2) return root;
        // 如果只找到一个节点，返回那个节点
        if (count == 1) return found;
        // 如果都没找到，返回null
        return nullptr;
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
        // BST测试
        runTest("BST - 基本测试", [this]() {
            /*
                  6
                /   \
               2     8
              / \   / \
             0   4 7   9
            */
            TreeNode* root = new TreeNode(6);
            TreeNode* node2 = new TreeNode(2);
            TreeNode* node8 = new TreeNode(8);
            TreeNode* node0 = new TreeNode(0);
            TreeNode* node4 = new TreeNode(4);
            TreeNode* node7 = new TreeNode(7);
            TreeNode* node9 = new TreeNode(9);

            root->left = node2;
            root->right = node8;
            node2->left = node0;
            node2->right = node4;
            node8->left = node7;
            node8->right = node9;

            return solution.lowestCommonAncestorBST(root, node2, node8) == root;
        });

        // 带父指针测试
        runTest("父指针 - 基本测试", [this]() {
            TreeNode* root = new TreeNode(3);
            TreeNode* node5 = new TreeNode(5);
            TreeNode* node1 = new TreeNode(1);

            root->left = node5;
            root->right = node1;
            node5->parent = root;
            node1->parent = root;

            return solution.lowestCommonAncestorWithParent(node5, node1) == root;
        });

        // N叉树测试
        runTest("N叉树 - 基本测试", [this]() {
            Node* root = new Node(1);
            Node* node2 = new Node(2);
            Node* node3 = new Node(3);
            Node* node4 = new Node(4);
            
            root->children = {node2, node3, node4};

            return solution.lowestCommonAncestorNary(root, node2, node3) == root;
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
