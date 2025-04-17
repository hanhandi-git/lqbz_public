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
using namespace std;

class Solution {
public:
    /**
     * @brief 查找字符串中的最长回文子串
     * @param s 输入字符串
     * @return 最长回文子串
     */
    pair<int, int> judge(const string& str, int left, int right) {
       while (left < str.size() && right >= 0) {
            if (str[left] != str[right]) {
                break;
            }
            left--;
            right++;
       }
       return {left+1, right-1};
    }
    string longestPalindrome(const string& s) {
       if (s.size() <= 1) return s;
       std::pair<int, int> res(0, 0);
       for (int i = 0; i < s.size(); i++)
       {
            std::pair<int, int> tmp1 = judge(s,i,i);
            std::pair<int, int> tmp2 = judge(s, i, i+1);
            int len1 = tmp1.second - tmp1.first + 1;
            int len2 = tmp2.second - tmp2.first + 1;
            int res_len = res.second - res.first + 1;
            if (len1 > res_len) res = tmp1;
            if (len2 > res_len) res = tmp2;
       }
       return s.substr(res.first, res.second - res.first + 1);
    }
};

// class Solution {
// public:
//     /**
//      * @brief 查找字符串中的最长回文子串
//      * @param s 输入字符串
//      * @return 最长回文子串
//      */
//     pair<int, int> judge(const string& str, int left, int right) {
//         while (left >= 0 && right < str.size()) {
//             if (str[left] != str[right]) break;
//             left--;
//             right++;
//         }
//         return {left+1, right-1};
//     }
//     string longestPalindrome(const string& s) {
//         if (s.size() <= 1) return s;
//         std::pair<int, int> res(0, 0);
//         for (int i = 0; i < s.size(); i++) {
//             auto l1 = judge(s, i, i);
//             auto l2 = judge(s, i, i+1);
//             int len1 = l1.second - l1.first + 1;
//             int len2 = l2.second - l2.first + 1;
//             int res_len = res.second - res.first + 1;
//             if (len1 > res_len) res = l1;
//             if (len2 > res_len) res = l2;
//         }
//         return s.substr(res.first, res.second - res.first + 1);
//     }
// };

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
