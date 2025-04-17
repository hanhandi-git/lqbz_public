/**
 * @file regular_expression_matching.cpp
 * @brief 实现正则表达式匹配算法
 */

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <queue>
using namespace std;

class Solution {
public:
    /**
     * @brief 判断字符串是否匹配正则表达式模式
     * @param s 输入字符串
     * @param p 正则表达式模式
     * @return 是否完全匹配
     */
    bool isMatch(string s, string p) {
        int m = s.length(), n = p.length();
        vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
        dp[0][0] = true;
        
        // 处理p以'*'开头的情况
        for (int j = 1; j <= n; j++) {
            if (p[j-1] == '*') {
                dp[0][j] = dp[0][j-2];
            }
        }
        
        // 动态规划填表
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (p[j-1] == '*') {
                    dp[i][j] = dp[i][j-2];  // 不使用当前的'*'
                    if (s[i-1] == p[j-2] || p[j-2] == '.') {
                        dp[i][j] = dp[i][j] || dp[i-1][j];  // 使用'*'匹配一次或多次
                    }
                }
                else if (s[i-1] == p[j-1] || p[j-1] == '.') {
                    dp[i][j] = dp[i-1][j-1];
                }
            }
        }
        return dp[m][n];
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
        // 基本字符匹配测试
        runTest("基本字符匹配测试", [this]() {
            return solution.isMatch("aa", "aa") == true;
        });

        // 点号匹配测试
        runTest("点号匹配测试", [this]() {
            return solution.isMatch("ab", "a.") == true;
        });

        // 星号匹配零次测试
        runTest("星号匹配零次测试", [this]() {
            return solution.isMatch("ab", "ab*") == true;
        });

        // 星号匹配多次测试
        runTest("星号匹配多次测试", [this]() {
            return solution.isMatch("aaa", "a*") == true;
        });

        // 复杂模式测试
        runTest("复杂模式测试", [this]() {
            return solution.isMatch("aab", "c*a*b") == true;
        });

        // 空字符串测试
        runTest("空字符串测试", [this]() {
            return solution.isMatch("", "") == true;
        });

        // 点号星号组合测试
        runTest("点号星号组合测试", [this]() {
            return solution.isMatch("aab", ".*b") == true;
        });

        // 不匹配测试
        runTest("不匹配测试", [this]() {
            return solution.isMatch("mississippi", "mis*is*p*.") == false;
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