/**
 * @file longest_palindromic_substr_extension.cpp
 * @brief 实现最长回文子串的扩展解法和优化方案
 */

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <functional>

class Solution {
public:
    /**
     * @brief 使用中心扩展法查找最长回文子串
     * @param s 输入字符串
     * @return 最长回文子串
     */
    std::string longestPalindromeExpand(std::string s) {
        if (s.length() < 2) return s;
        
        int start = 0, maxLen = 1;
        
        for (int i = 0; i < s.length(); ++i) {
            // 以单个字符为中心
            int len1 = expandAroundCenter(s, i, i);
            // 以两个字符之间的空隙为中心
            int len2 = expandAroundCenter(s, i, i + 1);
            
            int len = std::max(len1, len2);
            if (len > maxLen) {
                start = i - (len - 1) / 2;
                maxLen = len;
            }
        }
        
        return s.substr(start, maxLen);
    }

    /**
     * @brief Manacher算法实现最长回文子串查找
     * @param s 输入字符串
     * @return 最长回文子串
     */
    std::string longestPalindromeManacher(std::string s) {
        if (s.empty()) return "";
        
        // 预处理字符串
        std::string t = "$#";
        for (char c : s) {
            t += c;
            t += '#';
        }
        
        std::vector<int> p(t.length(), 0);
        int center = 0, maxRight = 0;
        int maxLen = 0, start = 0;
        
        for (int i = 1; i < t.length(); ++i) {
            if (i < maxRight) {
                p[i] = std::min(maxRight - i, p[2 * center - i]);
            }
            
            // 中心扩展
            while (i + p[i] + 1 < t.length() && i - p[i] - 1 >= 0 && 
                   t[i + p[i] + 1] == t[i - p[i] - 1]) {
                ++p[i];
            }
            
            // 更新中心和右边界
            if (i + p[i] > maxRight) {
                center = i;
                maxRight = i + p[i];
            }
            
            // 更新最长回文子串
            if (p[i] > maxLen) {
                maxLen = p[i];
                start = (i - maxLen) / 2;
            }
        }
        
        return s.substr(start, maxLen);
    }

private:
    int expandAroundCenter(const std::string& s, int left, int right) {
        while (left >= 0 && right < s.length() && s[left] == s[right]) {
            --left;
            ++right;
        }
        return right - left - 1;
    }
};

// Test Framework
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
        // 添加最长回文子串扩展解法的测试用例
        runTest("中心扩展法测试1", [this]() {
            std::string s = "babad";
            std::string result = solution.longestPalindromeExpand(s);
            return result == "bab" || result == "aba";
        });

        runTest("中心扩展法测试2", [this]() {
            std::string s = "cbbd";
            std::string result = solution.longestPalindromeExpand(s);
            return result == "bb";
        });

        runTest("Manacher算法测试1", [this]() {
            std::string s = "babad";
            std::string result = solution.longestPalindromeManacher(s);
            return result == "bab" || result == "aba";
        });

        runTest("Manacher算法测试2", [this]() {
            std::string s = "cbbd";
            std::string result = solution.longestPalindromeManacher(s);
            return result == "bb";
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
