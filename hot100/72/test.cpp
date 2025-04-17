#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;

class Solution {
public:
    int minDistance(string word1, string word2) {
        int m = word1.length();
        int n = word2.length();
        // 确保 word1 是较短的字符串，优化空间
        if (m > n) {
            return minDistance(word2, word1);
        }
        vector<int> dp(m + 1);
        // 初始化第一行
        for (int i = 0; i <= m; i++) {
            dp[i] = i;
        }
        // 填充 DP 表格
        for (int j = 1; j <= n; j++) {
            int prev = dp[0];
            dp[0] = j;
            for (int i = 1; i <= m; i++) {
                int temp = dp[i];
                if (word1[i-1] == word2[j-1]) {
                    dp[i] = prev;
                } else {
                    dp[i] = min(min(dp[i-1], dp[i]), prev) + 1;
                }
                prev = temp;
            }
        }
        return dp[m];
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
        // 编辑距离测试
        runTest("编辑距离测试用例 1: horse -> ros", [this]() {
            return solution.minDistance("horse", "ros") == 3;
        });
        
        runTest("编辑距离测试用例 2: intention -> execution", [this]() {
            return solution.minDistance("intention", "execution") == 5;
        });
        
        runTest("编辑距离测试用例 3: 空字符串", [this]() {
            return solution.minDistance("", "") == 0;
        });
        
        runTest("编辑距离测试用例 4: 一个空字符串", [this]() {
            return solution.minDistance("abc", "") == 3;
        });
        
        runTest("编辑距离测试用例 5: 相同字符串", [this]() {
            return solution.minDistance("abcd", "abcd") == 0;
        });
        
        runTest("编辑距离测试用例 6: 只需替换", [this]() {
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