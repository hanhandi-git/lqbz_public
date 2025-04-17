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

class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return isValidBSTHelper(root, LONG_MIN, LONG_MAX);
    }
    
    bool isValidBSTHelper(TreeNode* node, long minVal, long maxVal) {
        if (!node) return true; // 空节点是有效的
        
        if (node->val <= minVal || node->val >= maxVal) {
            return false; // 当前节点不在有效范围内
        }
        
        // 递归检查左子树和右子树
        return isValidBSTHelper(node->left, minVal, node->val) &&
               isValidBSTHelper(node->right, node->val, maxVal);
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

public:
    void runAllTests() {
        runTest("ValidBST", [this]() {
            TreeNode* root = new TreeNode(2);
            root->left = new TreeNode(1);
            root->right = new TreeNode(3);

            bool expected = true;
            bool result = solution.isValidBST(root);
            delete root->left; delete root->right; delete root; // 清理内存
            return expectEqual(expected, result);
        });

        runTest("InvalidBST", [this]() {
            TreeNode* root = new TreeNode(5);
            root->left = new TreeNode(1);
            root->right = new TreeNode(4);
            root->right->left = new TreeNode(3);
            root->right->right = new TreeNode(6);

            bool expected = false;
            bool result = solution.isValidBST(root);
            delete root->left; delete root->right->left; delete root->right->right; delete root->right; delete root; // 清理内存
            return expectEqual(expected, result);
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