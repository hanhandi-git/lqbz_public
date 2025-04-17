#include <vector>
#include <iostream>
#include <functional>
#include <climits>

using namespace std;

// 定义二叉树节点结构
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
public:
    int maxSum = 0;
    int maxPathSum(TreeNode* root) {
        maxSum = INT_MIN;
        dfs(root);
        return maxSum;
    }

    // 返回值是当前节点到其子树的最大路径和的一条边的和
    int dfs(TreeNode* root)
    {
        if (!root) return 0;
        int tmpSum = root->val;
        int leftSum = std::max(dfs(root->left), 0);
        int rightSum = std::max(dfs(root->right), 0);
        tmpSum = tmpSum + leftSum + rightSum;
        maxSum = std::max(maxSum, tmpSum);
        // 父节点只能选择一条边（即要么是左子树，要么是右子树），而不能同时选择两条边。
        return root->val + std::max(leftSum, rightSum);
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
            std::cout << "通过: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "失败: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        // 测试用例1: 示例1
        runTest("示例1", [this]() {
            TreeNode* root = new TreeNode(1);
            root->left = new TreeNode(2);
            root->right = new TreeNode(3);
            return solution.maxPathSum(root) == 6;
        });

        // 测试用例2: 示例2
        runTest("示例2", [this]() {
            TreeNode* root = new TreeNode(-10);
            root->left = new TreeNode(9);
            root->right = new TreeNode(20);
            root->right->left = new TreeNode(15);
            root->right->right = new TreeNode(7);
            return solution.maxPathSum(root) == 42;
        });

        // 测试用例3: 只有一个节点
        runTest("单个节点", [this]() {
            TreeNode* root = new TreeNode(5);
            return solution.maxPathSum(root) == 5;
        });

        // 测试用例4: 所有节点为负数
        runTest("所有节点为负数", [this]() {
            TreeNode* root = new TreeNode(-1);
            root->left = new TreeNode(-2);
            root->right = new TreeNode(-3);
            return solution.maxPathSum(root) == -1;
        });

        // 测试用例5: 复杂树结构
        runTest("复杂树结构", [this]() {
            TreeNode* root = new TreeNode(1);
            root->left = new TreeNode(2);
            root->right = new TreeNode(3);
            root->left->left = new TreeNode(4);
            root->left->right = new TreeNode(5);
            return solution.maxPathSum(root) == 11; // 4 -> 2 -> 1 -> 3
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