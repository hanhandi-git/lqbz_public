#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;

// 定义二叉树节点结构
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    struct Result {
        int rob;     // 抢劫当前节点时的最大金额
        int not_rob; // 不抢劫当前节点时的最大金额
    };
    
    Result dfs(TreeNode* node) {
        if (!node) return {0, 0};
        
        Result left = dfs(node->left);
        Result right = dfs(node->right);
        
        // 抢劫当前节点，则不能抢劫子节点
        int rob = node->val + left.not_rob + right.not_rob;
        
        // 不抢劫当前节点，则可以选择抢或不抢子节点
        int not_rob = max(left.rob, left.not_rob) + max(right.rob, right.not_rob);
        
        return {rob, not_rob};
    }
    
    int rob(TreeNode* root) {
        Result result = dfs(root);
        return max(result.rob, result.not_rob);
    }
};

// 辅助函数：构建树并在不再需要时释放内存
TreeNode* createTree(const vector<int>& values, int index = 0) {
    if (index >= values.size() || values[index] == -1) { // 使用-1表示null节点
        return nullptr;
    }
    
    TreeNode* root = new TreeNode(values[index]);
    root->left = createTree(values, 2 * index + 1);
    root->right = createTree(values, 2 * index + 2);
    
    return root;
}

// 辅助函数：释放树内存
void deleteTree(TreeNode* root) {
    if (root) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

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
        runTest("测试用例1 - 示例树", [this]() {
            // 构建树: [3,2,3,null,3,null,1]
            TreeNode* root = new TreeNode(3);
            root->left = new TreeNode(2);
            root->right = new TreeNode(3);
            root->left->right = new TreeNode(3);
            root->right->right = new TreeNode(1);
            
            int result = solution.rob(root);
            deleteTree(root);
            return result == 7; // 预期结果为7 (3+3+1)
        });

        runTest("测试用例2 - 另一示例树", [this]() {
            // 构建树: [3,4,5,1,3,null,1]
            TreeNode* root = new TreeNode(3);
            root->left = new TreeNode(4);
            root->right = new TreeNode(5);
            root->left->left = new TreeNode(1);
            root->left->right = new TreeNode(3);
            root->right->right = new TreeNode(1);
            
            int result = solution.rob(root);
            deleteTree(root);
            return result == 9; // 预期结果为9 (4+5)
        });

        runTest("测试用例3 - 单节点树", [this]() {
            TreeNode* root = new TreeNode(1);
            int result = solution.rob(root);
            delete root;
            return result == 1; // 预期结果为1
        });

        runTest("测试用例4 - 空树", [this]() {
            TreeNode* root = nullptr;
            int result = solution.rob(root);
            return result == 0; // 预期结果为0
        });

        runTest("测试用例5 - 链状树", [this]() {
            TreeNode* root = new TreeNode(2);
            root->right = new TreeNode(1);
            root->right->right = new TreeNode(4);
            root->right->right->right = new TreeNode(3);
            
            int result = solution.rob(root);
            deleteTree(root);
            return result == 6; // 预期结果为6 (2+4)
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