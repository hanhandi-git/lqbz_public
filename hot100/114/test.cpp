#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>  
#include <climits>  

class TreeNode {
public:
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* preNode;
    
    Solution() : preNode(nullptr) {}  // 添加构造函数初始化preNode
    
    void flatten(TreeNode* root) {
        preNode = nullptr;  // 每次调用flatten前重置preNode
        flattenTree(root);
    }
    
private:
    void flattenTree(TreeNode* root) {
        if (root == NULL) return;
        flattenTree(root->right);
        flattenTree(root->left);
        root->left = NULL;
        root->right = preNode;
        preNode = root;
    }
};
class TestFramework {
private:
    Solution solution;
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

    bool expectEqual(bool expected, bool actual) {
        return expected == actual;
    }

    // 辅助函数，用于检查链表的顺序
    bool checkFlattenedList(TreeNode* root, const std::vector<int>& expected) {
        for (int val : expected) {
            if (!root || root->val != val) return false;
            root = root->right; // 移动到下一个节点
        }
        return root == nullptr; // 确保没有多余的节点
    }

public:
    void runAllTests() {
        runTest("FlattenEmptyTree", [this]() {
            TreeNode* root = nullptr; // 空树
            solution.flatten(root);
            return root == nullptr; // 应该仍然是空树
        });

        runTest("FlattenSingleNodeTree", [this]() {
            TreeNode* root = new TreeNode(1); // 只有一个节点
            solution.flatten(root);
            std::vector<int> expected = {1};
            bool result = checkFlattenedList(root, expected);
            delete root; // 清理内存
            return result;
        });

        runTest("FlattenCompleteBinaryTree", [this]() {
            TreeNode* root = new TreeNode(1);
            root->left = new TreeNode(2);
            root->right = new TreeNode(3);
            root->left->left = new TreeNode(4);
            root->left->right = new TreeNode(5);
            root->right->left = new TreeNode(6);
            root->right->right = new TreeNode(7);

            solution.flatten(root);

            // 检查展开后的链表顺序
            std::vector<int> expected = {1, 2, 4, 5, 3, 6, 7};
            bool result = checkFlattenedList(root, expected);

            // 清理内存 - 修改这部分
            TreeNode* current = root;
            while (current) {
                TreeNode* next = current->right;
                delete current;
                current = next;
            }

            return result;
        });

        runTest("FlattenUnbalancedTree", [this]() {
            TreeNode* root = new TreeNode(1);
            root->left = new TreeNode(2);
            root->left->left = new TreeNode(3);
            root->left->left->left = new TreeNode(4);

            solution.flatten(root);

            // 检查展开后的链表顺序
            std::vector<int> expected = {1, 2, 3, 4};
            bool result = checkFlattenedList(root, expected);


            return result;
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