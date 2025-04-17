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
    TreeNode* sortedArrayToBST(std::vector<int>& nums) {
        return buildBST(nums, 0, nums.size() - 1);
    }
    
    TreeNode* buildBST(std::vector<int>& nums, int left, int right) {
        if (left > right) return nullptr;
        
        // 选择中间位置的元素作为根节点
        int mid = left + (right - left) / 2;
        TreeNode* root = new TreeNode(nums[mid]);
        
        // 递归构建左子树和右子树
        root->left = buildBST(nums, left, mid - 1);
        root->right = buildBST(nums, mid + 1, right);
        
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

    // 辅助函数，用于检查树是否是高度平衡的二叉搜索树
    bool isBalancedBST(TreeNode* root) {
        if (!root) return true;
        
        // 检查是否是二叉搜索树
        if (!isBST(root, INT_MIN, INT_MAX)) return false;
        
        // 检查是否是高度平衡的
        if (abs(height(root->left) - height(root->right)) > 1) return false;
        
        return isBalancedBST(root->left) && isBalancedBST(root->right);
    }
    
    bool isBST(TreeNode* root, int min, int max) {
        if (!root) return true;
        
        if (root->val <= min || root->val >= max) return false;
        
        return isBST(root->left, min, root->val) && isBST(root->right, root->val, max);
    }
    
    int height(TreeNode* root) {
        if (!root) return 0;
        return 1 + std::max(height(root->left), height(root->right));
    }
    
    // 辅助函数，用于检查树的中序遍历是否与原数组相同
    bool checkInorder(TreeNode* root, const std::vector<int>& expected, int& index) {
        if (!root) return true;
        
        if (!checkInorder(root->left, expected, index)) return false;
        
        if (root->val != expected[index++]) return false;
        
        return checkInorder(root->right, expected, index);
    }
    
    // 辅助函数，用于清理树的内存
    void deleteTree(TreeNode* root) {
        if (!root) return;
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }

public:
    void runAllTests() {
        // 测试用例 1: 空数组
        runTest("SortedArrayToBST_Empty", [this]() {
            std::vector<int> nums = {};
            TreeNode* root = solution.sortedArrayToBST(nums);
            bool result = (root == nullptr);
            return result;
        });

        // 测试用例 2: 单个元素
        runTest("SortedArrayToBST_SingleElement", [this]() {
            std::vector<int> nums = {5};
            TreeNode* root = solution.sortedArrayToBST(nums);
            bool result = (root != nullptr && root->val == 5 && root->left == nullptr && root->right == nullptr);
            deleteTree(root);
            return result;
        });

        // 测试用例 3: 示例数组 [-10,-3,0,5,9]
        runTest("SortedArrayToBST_Example", [this]() {
            std::vector<int> nums = {-10,-3,0,5,9};
            TreeNode* root = solution.sortedArrayToBST(nums);
            
            // 检查是否是高度平衡的二叉搜索树
            bool isBalanced = isBalancedBST(root);
            
            // 检查中序遍历是否与原数组相同
            int index = 0;
            bool inorderCorrect = checkInorder(root, nums, index);
            
            deleteTree(root);
            return isBalanced && inorderCorrect;
        });

        // 测试用例 4: 较大的有序数组
        runTest("SortedArrayToBST_LargeArray", [this]() {
            std::vector<int> nums;
            for (int i = 0; i < 100; i++) {
                nums.push_back(i);
            }
            
            TreeNode* root = solution.sortedArrayToBST(nums);
            
            // 检查是否是高度平衡的二叉搜索树
            bool isBalanced = isBalancedBST(root);
            
            // 检查中序遍历是否与原数组相同
            int index = 0;
            bool inorderCorrect = checkInorder(root, nums, index);
            
            deleteTree(root);
            return isBalanced && inorderCorrect;
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