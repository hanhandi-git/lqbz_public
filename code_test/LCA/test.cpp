#include <iostream>
#include <functional>
#include <string>

// 二叉树节点定义
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // 基本情况：如果root为空或者root是p或q中的一个，直接返回root
        if (root == nullptr || root == p || root == q) {
            return root;
        }
        
        // 递归搜索左右子树
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        
        // 如果左右子树都找到了结果，说明当前节点就是LCA
        if (left && right) {
            return root;
        }
        
        // 如果只有一边找到了结果，返回那一边的结果
        return left ? left : right;
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
        // 基本测试
        runTest("基本测试 - 简单树", [this]() {
            /*
                  3
                /   \
               5     1
              / \   / \
             6   2 0   8
                / \
               7   4
            */
            TreeNode* root = new TreeNode(3);
            TreeNode* node5 = new TreeNode(5);
            TreeNode* node1 = new TreeNode(1);
            TreeNode* node6 = new TreeNode(6);
            TreeNode* node2 = new TreeNode(2);
            TreeNode* node0 = new TreeNode(0);
            TreeNode* node8 = new TreeNode(8);
            TreeNode* node7 = new TreeNode(7);
            TreeNode* node4 = new TreeNode(4);

            root->left = node5;
            root->right = node1;
            node5->left = node6;
            node5->right = node2;
            node1->left = node0;
            node1->right = node8;
            node2->left = node7;
            node2->right = node4;

            return solution.lowestCommonAncestor(root, node5, node1) == root;
        });

        runTest("基本测试 - 节点是自身的祖先", [this]() {
            TreeNode* root = new TreeNode(1);
            TreeNode* node2 = new TreeNode(2);
            root->left = node2;
            
            return solution.lowestCommonAncestor(root, root, node2) == root;
        });

        // 边界测试
        runTest("边界测试 - 空树", [this]() {
            return solution.lowestCommonAncestor(nullptr, nullptr, nullptr) == nullptr;
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
