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
    TreeNode* buildTree(std::vector<int>& preorder, std::vector<int>& inorder) {
        if (preorder.empty() || inorder.empty()) return nullptr;
        return build(preorder, 0, preorder.size() - 1, inorder, 0, inorder.size() - 1);
    }

    TreeNode* build(std::vector<int>& preorder, int preStart, int preEnd, std::vector<int>& inorder, int inStart, int inEnd)
    {
        if (preStart > preEnd || inStart > inEnd) return nullptr;
        int rootVal = preorder[preStart];
        TreeNode* root = new TreeNode(rootVal);
        int rootIndex = find(inorder.begin() + inStart, inorder.begin() + inEnd + 1, rootVal) - inorder.begin();
        int leftSize = rootIndex - inStart;
        root->left = build(preorder, preStart+1, preStart + leftSize, inorder, inStart, rootIndex - 1);
        root->right = build(preorder, preStart + leftSize+1, preEnd, inorder, rootIndex+1, inEnd);
        return root;
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

    // 辅助函数，用于检查树的结构
    bool checkTreeStructure(TreeNode* root, const std::vector<int>& expected) {
        if (!root && expected.empty()) return true;
        if (!root || expected.empty() || root->val != expected[0]) return false;

        std::vector<int> leftExpected(expected.begin() + 1, expected.begin() + 1 + (expected.size() - 1) / 2);
        std::vector<int> rightExpected(expected.begin() + 1 + (expected.size() - 1) / 2, expected.end());

        return checkTreeStructure(root->left, leftExpected) && checkTreeStructure(root->right, rightExpected);
    }

public:
    void runAllTests() {

        // 新增的测试用例 1
        runTest("BuildTreeTest_Empty", [this]() {
            std::vector<int> preorder = {};
            std::vector<int> inorder = {};
            TreeNode* root = solution.buildTree(preorder, inorder);
            bool result = (root == nullptr); // 预期结果是空树
            delete root; // 不需要清理，因为 root 是 nullptr
            return result;
        });

        // 新增的测试用例 2
        runTest("BuildTreeTest_SingleNode", [this]() {
            std::vector<int> preorder = {1};
            std::vector<int> inorder = {1};
            TreeNode* root = solution.buildTree(preorder, inorder);
            std::vector<int> expected = {1}; // 预期的树结构
            bool result = checkTreeStructure(root, expected);
            delete root; // 清理内存
            return result;
        });

        // 新增的测试用例 5
        runTest("BuildTreeTest_LargeTree", [this]() {
            std::vector<int> preorder = {1, 2, 4, 5, 3, 6, 7};
            std::vector<int> inorder = {4, 2, 5, 1, 6, 3, 7};
            TreeNode* root = solution.buildTree(preorder, inorder);
            std::vector<int> expected = {1, 2, 4, 5, 3, 6, 7}; // 预期的树结构
            bool result = checkTreeStructure(root, expected);
            // 清理内存
            delete root->left->left; // 4
            delete root->left->right; // 5
            delete root->left; // 2
            delete root->right->left; // 6
            delete root->right->right; // 7
            delete root->right; // 3
            delete root; // 1
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