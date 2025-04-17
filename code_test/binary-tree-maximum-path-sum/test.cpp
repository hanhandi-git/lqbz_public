#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
#include <climits>
#include <queue>

// 定义二叉树节点结构
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class TreePrinter {
public:
    static void printTreeWithConnections(TreeNode* root) {
        if (!root) {
            std::cout << "空树" << std::endl;
            return;
        }

        // 获取树的高度
        int height = getHeight(root);
        
        // 使用队列进行层序遍历
        std::queue<TreeNode*> q;
        q.push(root);
        
        int level = 0;
        while (!q.empty() && level < height) {
            int levelSize = q.size();
            
            // 打印当前层的空格
            printSpaces(pow(2, height - level - 1) - 1);
            
            // 处理当前层的所有节点
            for (int i = 0; i < levelSize; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                if (node) {
                    std::cout << node->val;
                    q.push(node->left);
                    q.push(node->right);
                } else {
                    std::cout << " ";
                    q.push(nullptr);
                    q.push(nullptr);
                }
                
                // 打印节点之间的空格
                printSpaces(pow(2, height - level) - 1);
            }
            std::cout << std::endl;
            
            // 如果不是最后一层，打印连接线
            if (level < height - 1) {
                printSpaces(pow(2, height - level - 2) - 1);
                for (int i = 0; i < levelSize; i++) {
                    if (q.front()) {
                        std::cout << "/";
                        printSpaces(pow(2, height - level - 1) - 1);
                        std::cout << "\\";
                    } else {
                        printSpaces(pow(2, height - level - 1) + 1);
                    }
                    printSpaces(pow(2, height - level - 1) - 1);
                }
                std::cout << std::endl;
            }
            
            level++;
        }
    }

private:
    static int getHeight(TreeNode* root) {
        if (!root) return 0;
        return 1 + std::max(getHeight(root->left), getHeight(root->right));
    }
    
    static void printSpaces(int count) {
        for (int i = 0; i < count; i++) {
            std::cout << " ";
        }
    }
    
    static int pow(int base, int exp) {
        int result = 1;
        for (int i = 0; i < exp; i++) {
            result *= base;
        }
        return result;
    }
};

/**
 * @class Solution
 * @brief 计算二叉树的最大路径和
 * 
 * 路径被定义为一条从树中任意节点出发,沿父节点-子节点连接,达到任意节点的序列。
 * 同一个节点在路径中最多出现一次。该路径至少包含一个节点,且不一定经过根节点。
 */
class Solution {
private:
    int maxSum = INT_MIN;  // 全局最大路径和
    
    /**
     * @brief 计算节点的最大贡献值
     * @param node 当前节点
     * @return 以当前节点为端点的最大路径和
     * 
     * 对于每个节点,计算以该节点为端点的最大路径和,同时更新全局最大路径和
     */
    int maxGain(TreeNode* node) {
        if (!node) return 0;

        // 计算左右子树的最大贡献值
        int leftGain = std::max(maxGain(node->left), 0);
        int rightGain = std::max(maxGain(node->right), 0);
        
        // 更新全局最大路径和
        int pathSum = node->val + leftGain + rightGain;
        maxSum = std::max(maxSum, pathSum);
        
        // 返回节点的最大贡献值
        return node->val + std::max(leftGain, rightGain);
    }
    
public:
    int maxPathSum(TreeNode* root) {
        maxSum = INT_MIN;  // 重置最大值
        maxGain(root);
        return maxSum;
    }
};

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        bool result = false;
        try {
            result = test();
            if (result) {
                std::cout << "通过: " << testName << std::endl;
                passed++;
            } else {
                std::cout << "失败: " << testName << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << "错误: " << testName << " - " << e.what() << std::endl;
        }
    }

    // 改进的构建二叉树函数，使用层序遍历
    TreeNode* buildTree(const std::vector<int>& values) {
        if (values.empty()) return nullptr;
        
        std::vector<TreeNode*> nodes(values.size(), nullptr);
        
        // 创建所有节点
        for (size_t i = 0; i < values.size(); i++) {
            if (values[i] != INT_MIN) {
                nodes[i] = new TreeNode(values[i]);
            }
        }
        
        // 建立节点之间的连接
        for (size_t i = 0; i < values.size(); i++) {
            if (nodes[i]) {
                size_t leftIndex = 2 * i + 1;
                size_t rightIndex = 2 * i + 2;
                
                if (leftIndex < values.size()) {
                    nodes[i]->left = nodes[leftIndex];
                }
                if (rightIndex < values.size()) {
                    nodes[i]->right = nodes[rightIndex];
                }
            }
        }
        
        return nodes[0];
    }

    // 清理二叉树
    void cleanupTree(TreeNode* root) {
        if (!root) return;
        cleanupTree(root->left);
        cleanupTree(root->right);
        delete root;
    }

public:
    void runAllTests() {
        // 测试示例1
        runTest("示例1", [this]() {
            std::vector<int> values = {1, 2, 3};
            TreeNode* root = buildTree(values);
            int result = solution.maxPathSum(root);
            cleanupTree(root);
            return result == 6;
        });

        // 测试示例2
        runTest("示例2", [this]() {
            std::vector<int> values = {-10, 9, 20, INT_MIN, INT_MIN, 15, 7};
            TreeNode* root = buildTree(values);
            int result = solution.maxPathSum(root);
            cleanupTree(root);
            return result == 42;
        });

        // 测试单节点
        runTest("单节点", [this]() {
            TreeNode* root = new TreeNode(1);
            int result = solution.maxPathSum(root);
            cleanupTree(root);
            return result == 1;
        });

        // 测试负数节点
        runTest("负数节点", [this]() {
            TreeNode* root = new TreeNode(-3);
            int result = solution.maxPathSum(root);
            cleanupTree(root);
            return result == -3;
        });

        // 测试全负数路径
        runTest("全负数路径", [this]() {
            std::vector<int> values = {-2, -1, -3};
            TreeNode* root = buildTree(values);
            int result = solution.maxPathSum(root);
            cleanupTree(root);
            return result == -1;
        });

        // 测试复杂路径
        runTest("复杂路径", [this]() {
            std::vector<int> values = {1, -2, 3, 4, 5, -6, 2};
            TreeNode* root = buildTree(values);
            
            std::cout << "\n树的结构:" << std::endl;
            TreePrinter::printTreeWithConnections(root);
            
            int result = solution.maxPathSum(root);
            std::cout << "计算结果: " << result << std::endl;
            cleanupTree(root);
            return result == 9;
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
