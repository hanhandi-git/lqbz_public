#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>
using namespace std;
class LCSExtensions {
public:
    // 1. 最长公共子串
    int longestCommonSubstring(std::string text1, std::string text2) {
        int maxLen = 0;
        int m = text1.length();
        int n = text2.length();
        std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1, 0));
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (text1[i-1] == text2[j-1])
                {
                    dp[i][j] = dp[i-1][j-1] + 1;
                    maxLen = std::max(maxLen, dp[i][j]);
                }
            }
        }
        return maxLen;
    }

    // 2. 最短公共超序列
    std::string shortestCommonSupersequence(std::string text1, std::string text2) {
        int m = text1.length(), n = text2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (text1[i-1] == text2[j-1]) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = std::max(dp[i-1][j], dp[i][j-1]);
                }
            }
        }
        int i = m;
        int j = n;
        std::string res;
        while (i > 0 && j >0)
        {
            if (text1[i-1] == text2[j-1])
            {
                res += text1[i-1];
                i--;
                j--;
            } else {
                if (dp[i-1][j] > dp[i][j-1]) {
                    res += text1[i-1];
                    i--;
                } else {
                    res += text2[j-1];
                    j--;
                }
            }
        }
        while (i > 0) {
            res += text1[i-1];
            i--;
        }
        while (j > 0) {
            res += text2[j-1];
            j--;
        }
        std::reverse(res.begin(), res.end());
        // std::cout << res << std::endl;
        return res;
    }

    // 3. 打印最长公共子序列
    std::string getLCS(std::string text1, std::string text2) {
        int m = text1.length(), n = text2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (text1[i-1] == text2[j-1]) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = std::max(dp[i-1][j], dp[i][j-1]);
                }
            }
        }
        int i = m;
        int j = n;
        std::string res;
        while (i > 0 && j >0)
        {
            if (text1[i-1] == text2[j-1])
            {
                res += text1[i-1];
                i--;
                j--;
            } else {
                if (dp[i-1][j] > dp[i][j-1]) {
                    i--;
                } else {
                    j--;
                }
            }
        }
        // 打印res
        // std::cout << res << std::endl;
        std::reverse(res.begin(), res.end());

        return res;
    }

    // 4. 最长回文子序列
    int longestPalindromeSubseq(std::string s) {
        std::string p = s;
        std::reverse(p.begin(), p.end());
        return longestCommonSubsequence(s, p);
    }

    // 5. 编辑距离
    int minDistance(std::string word1, std::string word2) {
        int res;
        int m = word1.size();
        int n = word2.size();
        std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1, 0));
        // dp[i][j] word1[0..i-1] -> word2[0..j-1]的最小编辑次数
        for (int i = 0; i <= m; i++) dp[i][0] = i; 
        for (int j = 0; j <= n; j++) dp[0][j] = j;
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (word1[i-1] == word2[j-1])
                {
                    dp[i][j] = dp[i-1][j-1];
                } else {
                    dp[i][j] = std::min(dp[i-1][j], std::min(dp[i-1][j-1], dp[i][j-1])) + 1;
                }
            }
        }
        return dp[m][n];
    }

private:
    int longestCommonSubsequence(std::string text1, std::string text2) {
        int m = text1.length(), n = text2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (text1[i-1] == text2[j-1]) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = std::max(dp[i-1][j], dp[i][j-1]);
                }
            }
        }
        return dp[m][n];
    }
};

class TestFramework {
private:
    LCSExtensions solution;
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
        // 最长公共子串测试
        runTest("最长公共子串 - 基本情况", [this]() {
            return solution.longestCommonSubstring("abcde", "abgde") == 2;
        });
        runTest("最长公共子串 - 完全相同", [this]() {
            return solution.longestCommonSubstring("abc", "abc") == 3;
        });
        runTest("最长公共子串 - 无公共子串", [this]() {
            return solution.longestCommonSubstring("abc", "def") == 0;
        });
        runTest("最长公共子串 - 空字符串", [this]() {
            return solution.longestCommonSubstring("", "abc") == 0;
        });
        runTest("最长公共子串 - 多个相同长度子串", [this]() {
            return solution.longestCommonSubstring("ababc", "babcd") == 4;
        });

        // 最短公共超序列测试
        runTest("最短公共超序列 - 基本情况", [this]() {
            return solution.shortestCommonSupersequence("abac", "cab") == "cabac";
        });
        runTest("最短公共超序列 - 完全相同", [this]() {
            return solution.shortestCommonSupersequence("abc", "abc") == "abc";
        });
        runTest("最短公共超序列 - 无公共字符", [this]() {
            return solution.shortestCommonSupersequence("abc", "def") == "abcdef";
        });
        runTest("最短公共超序列 - 一个空串", [this]() {
            return solution.shortestCommonSupersequence("", "abc") == "abc";
        });
        runTest("最短公共超序列 - 包含关系", [this]() {
            return solution.shortestCommonSupersequence("abc", "ac") == "abc";
        });

        // 打印最长公共子序列测试
        runTest("打印LCS - 基本情况", [this]() {
            return solution.getLCS("abcde", "ace") == "ace";
        });
        runTest("打印LCS - 完全相同", [this]() {
            return solution.getLCS("abc", "abc") == "abc";
        });
        runTest("打印LCS - 无公共子序列", [this]() {
            return solution.getLCS("abc", "def") == "";
        });
        runTest("打印LCS - 空字符串", [this]() {
            return solution.getLCS("", "abc") == "";
        });
        runTest("打印LCS - 多个LCS", [this]() {
            string result = solution.getLCS("baaba", "ababa");
            return result.length() == 4 && (result == "aaba" || result == "abaa");
        });

        // 最长回文子序列测试
        runTest("最长回文子序列 - 基本情况", [this]() {
            return solution.longestPalindromeSubseq("bbbab") == 4;
        });
        runTest("最长回文子序列 - 单个字符", [this]() {
            return solution.longestPalindromeSubseq("a") == 1;
        });
        runTest("最长回文子序列 - 全相同字符", [this]() {
            return solution.longestPalindromeSubseq("aaaa") == 4;
        });
        runTest("最长回文子序列 - 没有回文", [this]() {
            return solution.longestPalindromeSubseq("abc") == 1;
        });
        runTest("最长回文子序列 - 偶数长度回文", [this]() {
            return solution.longestPalindromeSubseq("abba") == 4;
        });

        // 编辑距离测试
        runTest("编辑距离 - 基本情况", [this]() {
            return solution.minDistance("horse", "ros") == 3;
        });
        runTest("编辑距离 - 完全相同", [this]() {
            return solution.minDistance("abc", "abc") == 0;
        });
        runTest("编辑距离 - 完全不同", [this]() {
            return solution.minDistance("abc", "def") == 3;
        });
        runTest("编辑距离 - 空字符串", [this]() {
            return solution.minDistance("", "abc") == 3;
        });
        runTest("编辑距离 - 只需插入", [this]() {
            return solution.minDistance("abc", "abcd") == 1;
        });
        runTest("编辑距离 - 只需删除", [this]() {
            return solution.minDistance("abcd", "abc") == 1;
        });
        runTest("编辑距离 - 只需替换", [this]() {
            return solution.minDistance("abc", "abd") == 1;
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
