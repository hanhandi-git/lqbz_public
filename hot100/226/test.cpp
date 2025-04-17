#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <queue>

using namespace std;
// 二叉树节点定义
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (root == nullptr) return nullptr;
        std::swap(root->left, root->right);
        invertTree(root->left);
        invertTree(root->right);
        return root;
    }
};

// 测试框架
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    // 辅助函数：从vector创建二叉树
    TreeNode* createTree(const std::vector<int>& values) {
        if (values.empty() || values[0] == -1) return nullptr;
        
        TreeNode* root = new TreeNode(values[0]);
        queue<TreeNode*> q;
        q.push(root);
        
        for (size_t i = 1; i < values.size(); i += 2) {
            TreeNode* current = q.front();
            q.pop();
            
            if (i < values.size() && values[i] != -1) {
                current->left = new TreeNode(values[i]);
                q.push(current->left);
            }
            
            if (i + 1 < values.size() && values[i + 1] != -1) {
                current->right = new TreeNode(values[i + 1]);
                q.push(current->right);
            }
        }
        return root;
    }

    // 辅助函数：比较两棵树是否相同
    bool isSameTree(TreeNode* p, TreeNode* q) {
        if (!p && !q) return true;
        if (!p || !q) return false;
        return (p->val == q->val) && 
               isSameTree(p->left, q->left) && 
               isSameTree(p->right, q->right);
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
        // 基本测试
        runTest("基本测试 - 完全二叉树", [this]() {
            TreeNode* root = createTree({4,2,7,1,3,6,9});
            TreeNode* expected = createTree({4,7,2,9,6,3,1});
            TreeNode* result = solution.invertTree(root);
            return isSameTree(result, expected);
        });

        // 边界测试
        runTest("边界测试 - 空树", [this]() {
            return solution.invertTree(nullptr) == nullptr;
        });

        runTest("边界测试 - 单节点树", [this]() {
            TreeNode* root = new TreeNode(1);
            TreeNode* result = solution.invertTree(root);
            return result->val == 1 && !result->left && !result->right;
        });

        // 特殊测试
        runTest("特殊测试 - 只有左子树", [this]() {
            TreeNode* root = createTree({1,2,-1});
            TreeNode* expected = createTree({1,-1,2});
            TreeNode* result = solution.invertTree(root);
            return isSameTree(result, expected);
        });

        runTest("特殊测试 - 只有右子树", [this]() {
            TreeNode* root = createTree({1,-1,2});
            TreeNode* expected = createTree({1,2,-1});
            TreeNode* result = solution.invertTree(root);
            return isSameTree(result, expected);
        });

        runTest("特殊测试 - 不平衡树", [this]() {
            TreeNode* root = createTree({1,2,3,4,-1,-1,-1});
            TreeNode* expected = createTree({1,3,2,-1,-1,-1,4});
            TreeNode* result = solution.invertTree(root);
            return isSameTree(result, expected);
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