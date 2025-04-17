#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>
#include <functional>
#include <unordered_map>
#include <climits>

// 定义二叉树节点结构
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class PathExtensions {
public:
    int maxDiameter = 0;
    // 1. 二叉树的直径
    int diameterOfBinaryTree(TreeNode* root) {
        maxDepth(root);
        return maxDiameter;
    }

    // 2. 最小路径和
    int minPathSum(std::vector<std::vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;
        int n = grid.size();
        int m = grid[0].size();
        std::vector<std::vector<int>> dp(n, std::vector<int>(m, 0));
        dp[0][0] = grid[0][0];
        for (int i = 1; i < n; i++)
        {
            dp[i][0] = dp[i-1][0] + grid[i][0];
        }
        for (int i = 1; i < m; i++)
        {
            dp[0][i] = dp[0][i-1] + grid[0][i];
        }
        for (int i = 1; i < n; i++)
        {
            for (int j = 1; j < m; j++)
            {
                dp[i][j] = std::min(dp[i-1][j], dp[i][j-1]) + grid[i][j];
            }
        }
        return dp[n-1][m-1];
    }

    /**
     * 路径总和 III：找出路径和等于给定数值的路径总数
     * @param root 二叉树根节点
     * @param targetSum 目标和
     * @return 满足条件的路径数量
     * 
     * 解题思路：使用双重递归
     * 1. 外层递归：遍历每个节点，将其作为路径的起点
     * 2. 内层递归：从选定的起点开始，向下寻找路径
     */
    int pathSumIII(TreeNode* root, int targetSum) {
        if (root == nullptr) return 0;
        
        // 路径总数 = 以当前节点为起点的路径数 + 左子树中的路径数 + 右子树中的路径数
        // 为何不能3个pathSumIII，因为会重复 每个节点作为起点只被考虑一次，时间复杂度 O(n²)
        return pathSumFrom(root, targetSum) +      // 以当前节点为起点的路径数
               pathSumIII(root->left, targetSum) + // 左子树中所有节点为起点的路径数
               pathSumIII(root->right, targetSum); // 右子树中所有节点为起点的路径数
    }

    /**
     * 计算以指定节点为起点的满足条件的路径数量
     * @param node 当前节点
     * @param targetSum 剩余需要的路径和
     * @return 满足条件的路径数量
     * 
     * 实现细节：
     * 1. 使用 long long 避免整数溢出
     * 2. 每次递归都减去当前节点值，检查剩余值
     * 3. 当前节点值等于目标值时，找到一条有效路径
     */
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

    /**
     * 路径总和 IV：计算所有从根到叶子节点路径的和
     * 节点编码规则：三位数字abc表示
     * - a：层级信息(1-4)
     * - b,c：在当前层从左到右的位置(1-8)
     * 例如：113表示第1层第1个节点，值为3
     * 
     * @param nums 包含三位数字的数组，每个数字代表树中的一个节点
     * @return 所有从根到叶子节点路径的和
     */

    // 输入：nums = [113, 215, 221]
    // 表示的树结构：
    //     3    (113: 第1层，位置1，值3)
    //    / \
    //   5   1  (215: 第2层，位置1，值5)
    //        (221: 第2层，位置2，值1)

    // 处理过程：
    // 1. 构建哈希表：
    //    tree[11] = 3  // 第1层位置1的节点值为3
    //    tree[21] = 5  // 第2层位置1的节点值为5
    //    tree[22] = 1  // 第2层位置2的节点值为1

    // 2. 找叶子节点：
    //    - 215(5)：检查31,32不存在，是叶子节点
    //      路径：5->3 = 8
    //    - 221(1)：检查33,34不存在，是叶子节点
    //      路径：1->3 = 4

    // 3. 返回路径和：8 + 4 = 12
    int pathSumIV(std::vector<int>& nums) {
        if (nums.empty()) return 0;
        
        // 使用哈希表存储节点信息，键为位置编码(深度*10+位置)，值为节点值
        // 例如：位置编码11代表第1层第1个节点
        std::unordered_map<int, int> tree;  
        
        // 第一次遍历：构建树的存储结构
        for (int num : nums) {
            int pos = num / 10;   // 获取位置编码（两位）
            int val = num % 10;   // 获取节点值（最后一位）
            tree[pos] = val;      // 存储节点信息
        }
        
        int sum = 0;  // 存储所有路径和
        
        // 第二次遍历：找出所有叶子节点并计算路径和
        for (int num : nums) {
            // 解析节点信息
            int d = num / 100;                // 获取深度（第一位）
            int p = (num % 100) / 10;         // 获取在当前层的位置（第二位）
            int pos = d * 10 + p;             // 计算位置编码
            
            // 计算左右子节点的位置编码
            // 在完全二叉树中：
            // - 左子节点位置 = 父节点位置*2 - 1
            // - 右子节点位置 = 父节点位置*2
            int leftChild = (d + 1) * 10 + p * 2 - 1;   // 左子节点编码
            int rightChild = (d + 1) * 10 + p * 2;      // 右子节点编码
            
            // 判断是否为叶子节点：没有左右子节点的节点就是叶子节点
            if (tree.find(leftChild) == tree.end() && 
                tree.find(rightChild) == tree.end()) {
                
                // 从叶子节点向上回溯到根节点，计算路径和
                int pathSum = 0;
                while (d >= 0) {  // pos > 0 表示还未到达根节点之上
                    pathSum += tree[pos];     // 累加当前节点值
                    p = (p + 1) / 2;         // 计算父节点在其层的位置
                    d--;                      // 层数减1
                    pos = d * 10 + p;         // 计算父节点的位置编码
                }
                sum += pathSum;  // 将这条路径的和加入总和
            }
        }
        
        return sum;
    }

private:
    int maxDepth(TreeNode* root) {
        if (root == nullptr) return 0;
        int left = maxDepth(root->left);
        int right = maxDepth(root->right);
        // 本节点直径与历史最大直径比较
        maxDiameter = std::max(maxDiameter, (left + right));
        int res = std::max(left, right) + 1;
        return res;
    }
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

class TestFramework {
private:
    PathExtensions solution;
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
        
        TreeNode* root = new TreeNode(values[0]);
        std::queue<TreeNode*> q;
        q.push(root);
        
        for (size_t i = 1; i < values.size(); i += 2) {
            TreeNode* current = q.front();
            q.pop();
            
            if (i < values.size() && values[i] != INT_MIN) {
                current->left = new TreeNode(values[i]);
                q.push(current->left);
            }
            
            if (i + 1 < values.size() && values[i + 1] != INT_MIN) {
                current->right = new TreeNode(values[i + 1]);
                q.push(current->right);
            }
        }
        
        return root;
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
        // 二叉树直径测试
        runTest("二叉树直径 - 基本情况", [this]() {
            TreeNode* root = new TreeNode(1);
            root->left = new TreeNode(2);
            root->right = new TreeNode(3);
            root->left->left = new TreeNode(4);
            root->left->right = new TreeNode(5);
            std::cout << "\n树的结构:" << std::endl;
            TreePrinter::printTreeWithConnections(root);
            int result = solution.diameterOfBinaryTree(root);
            std::cout << "直径长度: " << result << std::endl;
            cleanupTree(root);
            return result == 3;  // 4->2->1->3 或 5->2->1->3
        });

        runTest("二叉树直径 - 单边树", [this]() {
            TreeNode* root = new TreeNode(1);
            root->left = new TreeNode(2);
            root->left->left = new TreeNode(3);
            root->left->left->left = new TreeNode(4);
            std::cout << "\n树的结构:" << std::endl;
            TreePrinter::printTreeWithConnections(root);
            int result = solution.diameterOfBinaryTree(root);
            std::cout << "直径长度: " << result << std::endl;
            cleanupTree(root);
            return result == 3;  // 4->3->2->1
        });

        runTest("二叉树直径 - 空树", [this]() {
            int result = solution.diameterOfBinaryTree(nullptr);
            std::cout << "空树直径: " << result << std::endl;
            return result == 0;
        });

        runTest("二叉树直径 - 单节点", [this]() {
            TreeNode* root = new TreeNode(1);
            std::cout << "\n树的结构:" << std::endl;
            TreePrinter::printTreeWithConnections(root);
            int result = solution.diameterOfBinaryTree(root);
            std::cout << "直径长度: " << result << std::endl;
            cleanupTree(root);
            return result == 0;
        });

        // 最小路径和测试
        runTest("最小路径和 - 基本情况", [this]() {
            std::vector<std::vector<int>> grid = {
                {1,3,1},
                {1,5,1},
                {4,2,1}
            };
            return solution.minPathSum(grid) == 7;
        });

        runTest("最小路径和 - 单行", [this]() {
            std::vector<std::vector<int>> grid = {{1,2,3,4}};
            return solution.minPathSum(grid) == 10;
        });

        runTest("最小路径和 - 单列", [this]() {
            std::vector<std::vector<int>> grid = {{1},{2},{3},{4}};
            return solution.minPathSum(grid) == 10;
        });

        runTest("最小路径和 - 全零", [this]() {
            std::vector<std::vector<int>> grid = {
                {0,0,0},
                {0,0,0},
                {0,0,0}
            };
            return solution.minPathSum(grid) == 0;
        });

        runTest("最小路径和 - 大数值", [this]() {
            std::vector<std::vector<int>> grid = {
                {9,9,9},
                {9,1,9},
                {9,9,9}
            };
            return solution.minPathSum(grid) == 37;
        });

        // 路径总和III测试
        runTest("路径总和III - 基本情况", [this]() {
            std::vector<int> values = {10, 5, -3, 3, 2, INT_MIN, 11, 3, -2, INT_MIN, 1};
            TreeNode* root = buildTree(values);
            std::cout << "\n树的结构:" << std::endl;
            TreePrinter::printTreeWithConnections(root);
            int result = solution.pathSumIII(root, 8);
            cleanupTree(root);
            return result == 3;
        });

        runTest("路径总和III - 单节点路径", [this]() {
            std::vector<int> values = {1, 2, 3, 1, INT_MIN, 5, 6};
            TreeNode* root = buildTree(values);
            std::cout << "\n树的结构:" << std::endl;
            TreePrinter::printTreeWithConnections(root);
            int result = solution.pathSumIII(root, 1);
            cleanupTree(root);
            return result == 2;  // 两个值为1的节点
        });

        runTest("路径总和III - 负数路径", [this]() {
            TreeNode* root = new TreeNode(1);
            root->left = new TreeNode(-2);
            root->right = new TreeNode(-3);
            root->left->left = new TreeNode(1);
            root->left->right = new TreeNode(3);
            root->right->left = new TreeNode(-2);
            root->right->right = new TreeNode(2);
            std::cout << "\n树的结构:" << std::endl;
            TreePrinter::printTreeWithConnections(root);
            int result = solution.pathSumIII(root, -4);
            std::cout << "路径数量: " << result << std::endl;
            cleanupTree(root);
            return result == 3;  // 1->-2->-3, -2->-2, 1->-5
        });

        runTest("路径总和III - 零和路径", [this]() {
            TreeNode* root = new TreeNode(0);
            root->left = new TreeNode(1);
            root->right = new TreeNode(-1);
            root->left->left = new TreeNode(-1);
            root->left->right = new TreeNode(1);
            root->right->left = new TreeNode(0);
            root->right->right = new TreeNode(0);
            std::cout << "\n树的结构:" << std::endl;
            TreePrinter::printTreeWithConnections(root);
            int result = solution.pathSumIII(root, 0);
            std::cout << "零和路径数量: " << result << std::endl;
            cleanupTree(root);
            return result == 6;  // 包括单个0节点和所有和为0的路径
        });

        // 路径总和IV测试
        runTest("路径总和IV - 基本情况", [this]() {
            std::vector<int> nums = {113, 215, 221};
            return solution.pathSumIV(nums) == 12;
        });

        runTest("路径总和IV - 单路径", [this]() {
            std::vector<int> nums = {111, 217, 322};
            return solution.pathSumIV(nums) == 10;  // 1->7->2
        });

        runTest("路径总和IV - 完整二叉树", [this]() {
            std::vector<int> nums = {115, 215, 224, 325, 326, 327, 328};
            int result = solution.pathSumIV(nums);
            std::cout << "完整二叉树路径和: " << result << std::endl;
            return result == 44;  // (5+5+5) + (5+5+6) + (5+4+7) + (5+4+8)
        });

        runTest("路径总和IV - 单节点", [this]() {
            std::vector<int> nums = {115};
            return solution.pathSumIV(nums) == 5;
        });

        runTest("路径总和IV - 不平衡树", [this]() {
            std::vector<int> nums = {113, 221, 333, 334};
            int result = solution.pathSumIV(nums);
            std::cout << "不平衡树路径和: " << result << std::endl;
            return result == 14;  // (3+1+3) + (3+1+4)
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
