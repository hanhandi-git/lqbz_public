/**
 * @file longest_palindromic_substr.cpp
 * @brief 实现最长回文子串的基础解法
 */

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <queue>

class Solution {
public:
    /**
     * @brief 查找字符串中的最长回文子串
     * @param s 输入字符串
     * @return 最长回文子串
     */
    std::string longestPalindrome(std::string s) {
        if (s.length() < 2) return s;
        
        int start = 0, maxLen = 1;
        
        // 动态规划数组，dp[i][j]表示s[i..j]是否为回文串
        std::vector<std::vector<bool>> dp(s.length(), std::vector<bool>(s.length(), false));
        
        // 初始化单字符情况
        for (int i = 0; i < s.length(); ++i) {
            dp[i][i] = true;
        }
        
        // 检查长度为2及以上的子串
        for (int len = 2; len <= s.length(); ++len) {
            for (int i = 0; i < s.length() - len + 1; ++i) {
                int j = i + len - 1;
                
                if (len == 2) {
                    dp[i][j] = (s[i] == s[j]);
                } else {
                    dp[i][j] = (s[i] == s[j] && dp[i+1][j-1]);
                }
                
                if (dp[i][j] && len > maxLen) {
                    start = i;
                    maxLen = len;
                }
            }
        }
        
        return s.substr(start, maxLen);
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
        // 添加最长回文子串的测试用例
        runTest("最长回文子串测试1", [this]() {
            std::string s = "babad";
            std::string result = solution.longestPalindrome(s);
            return result == "bab" || result == "aba";
        });

        runTest("最长回文子串测试2", [this]() {
            std::string s = "cbbd";
            std::string result = solution.longestPalindrome(s);
            return result == "bb";
        });

        runTest("最长回文子串测试3 - 单字符", [this]() {
            std::string s = "a";
            std::string result = solution.longestPalindrome(s);
            return result == "a";
        });

        runTest("最长回文子串测试4 - 全相同字符", [this]() {
            std::string s = "aaaa";
            std::string result = solution.longestPalindrome(s);
            return result == "aaaa";
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
