#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>

class Solution {
public:
    // 基础动态规划解法
    int longestCommonSubsequence(std::string text1, std::string text2) {
        int m = text1.size();
        int n = text2.size();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (text1[i-1] == text2[j-1]) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = std::max(dp[i-1][j], dp[i][j-1]);
                }
            }
        }
        return dp[m][n];
    }
    
    // 空间优化解法
    int longestCommonSubsequence_optimized(std::string text1, std::string text2) {
        int m = text1.length(), n = text2.length();
        std::vector<int> dp(n + 1, 0);
        
        for (int i = 1; i <= m; i++) {
            int prev = 0;  // dp[i-1][j-1]
            for (int j = 1; j <= n; j++) {
                int temp = dp[j];  // 保存dp[i-1][j]
                if (text1[i-1] == text2[j-1]) {
                    dp[j] = prev + 1;
                } else {
                    dp[j] = std::max(dp[j], dp[j-1]);
                }
                prev = temp;
            }
        }
        return dp[n];
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
        // 测试基础动态规划方法
        runTest("示例1 - DP", [this]() {
            return solution.longestCommonSubsequence("abcde", "ace") == 3;
        });

        runTest("示例2 - DP", [this]() {
            return solution.longestCommonSubsequence("abc", "abc") == 3;
        });

        runTest("示例3 - DP", [this]() {
            return solution.longestCommonSubsequence("abc", "def") == 0;
        });

        // 测试空间优化方法
        runTest("示例1 - 优化", [this]() {
            return solution.longestCommonSubsequence_optimized("abcde", "ace") == 3;
        });

        runTest("示例2 - 优化", [this]() {
            return solution.longestCommonSubsequence_optimized("abc", "abc") == 3;
        });

        runTest("示例3 - 优化", [this]() {
            return solution.longestCommonSubsequence_optimized("abc", "def") == 0;
        });

        // 测试边界情况
        runTest("空字符串", [this]() {
            return solution.longestCommonSubsequence("", "") == 0 && 
                   solution.longestCommonSubsequence_optimized("", "") == 0;
        });

        runTest("单字符", [this]() {
            return solution.longestCommonSubsequence("a", "a") == 1 && 
                   solution.longestCommonSubsequence_optimized("a", "a") == 1;
        });

        runTest("完全不同的字符串", [this]() {
            return solution.longestCommonSubsequence("abc", "def") == 0 && 
                   solution.longestCommonSubsequence_optimized("abc", "def") == 0;
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
