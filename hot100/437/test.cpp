#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <unordered_map>
using namespace std;

// 定义二叉树节点
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
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
    int pathSumFrom(TreeNode* node, long long targetSum) {
        if (node == nullptr) return 0;
        
        int cnt = 0;
        // 如果当前节点值等于目标值，找到一条有效路径
        if (node->val == targetSum) {
            cnt = 1;
        }
        
        // 继续向下搜索，目标值减去当前节点值
        cnt += pathSumFrom(node->left, targetSum - node->val);   // 左子树中的路径数
        cnt += pathSumFrom(node->right, targetSum - node->val);  // 右子树中的路径数
        
        return cnt;
    }
    int pathSum(TreeNode* root, int targetSum) {
        if (root == nullptr) return 0;
        
        // 路径总数 = 以当前节点为起点的路径数 + 左子树中的路径数 + 右子树中的路径数
        // 为何不能3个pathSumIII，因为会重复 每个节点作为起点只被考虑一次，时间复杂度 O(n²)
        return pathSumFrom(root, targetSum) +      // 以当前节点为起点的路径数
               pathSum(root->left, targetSum) + // 左子树中所有节点为起点的路径数
               pathSum(root->right, targetSum); // 右子树中所有节点为起点的路径数 
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
        // 新增测试用例2 - 只有一个节点
        runTest("测试用例2 - 只有一个节点", [this]() {
            TreeNode* root = new TreeNode(5);
            int sum = 5;
            int expected = 1; // 预期路径数量
            return (solution.pathSum(root, sum) == expected);
        });

        // 新增测试用例3 - 没有路径和等于目标
        runTest("测试用例3 - 没有路径和等于目标", [this]() {
            TreeNode* root = new TreeNode(1);
            root->left = new TreeNode(2);
            root->right = new TreeNode(3);
            int sum = 5;
            int expected = 0; // 预期路径数量
            return (solution.pathSum(root, sum) == expected);
        });

        // 新增测试用例4 - 负数节点
        runTest("测试用例4 - 负数节点", [this]() {
            TreeNode* root = new TreeNode(-2);
            root->right = new TreeNode(-3);
            int sum = -5;
            int expected = 1; // 预期路径数量
            return (solution.pathSum(root, sum) == expected);
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