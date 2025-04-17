#include <iostream>
#include <vector>
#include <functional>
#include <string>

class Solution {
public:
    int longestCommonSubsequence(std::string text1, std::string text2) {
        int m = text1.length();
        int n = text2.length();
        
        // 创建DP表格，初始化为0
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
        
        // 填充DP表格
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (text1[i - 1] == text2[j - 1]) {
                    // 当前字符匹配，LCS长度加1
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    // 当前字符不匹配，取两种可能的最大值
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        
        // 返回最终结果
        return dp[m][n];
    }
};

// 测试框架
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
        // 基本测试
        runTest("基本测试 - 示例1", [this]() {
            std::string text1 = "abcde";
            std::string text2 = "ace";
            return solution.longestCommonSubsequence(text1, text2) == 3;
        });

        runTest("基本测试 - 示例2", [this]() {
            std::string text1 = "abc";
            std::string text2 = "abc";
            return solution.longestCommonSubsequence(text1, text2) == 3;
        });

        runTest("基本测试 - 示例3", [this]() {
            std::string text1 = "abc";
            std::string text2 = "def";
            return solution.longestCommonSubsequence(text1, text2) == 0;
        });

        runTest("基本测试 - 长字符串", [this]() {
            std::string text1 = "bsbininm";
            std::string text2 = "jmjkbkjkv";
            return solution.longestCommonSubsequence(text1, text2) == 1;
        });

        runTest("边界测试 - 空字符串", [this]() {
            std::string text1 = "";
            std::string text2 = "abc";
            return solution.longestCommonSubsequence(text1, text2) == 0;
        });

        runTest("边界测试 - 两个空字符串", [this]() {
            std::string text1 = "";
            std::string text2 = "";
            return solution.longestCommonSubsequence(text1, text2) == 0;
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