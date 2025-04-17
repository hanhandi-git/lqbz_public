#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>

class Solution {
private:
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> path;
    std::vector<std::vector<bool>> dp;
    
    void backtrack(const std::string& s, int start) {
        if (start >= s.length()) {
            result.push_back(path);
            return;
        }
        
        for (int i = start; i < s.length(); i++) {
            if (dp[start][i]) {
                path.push_back(s.substr(start, i - start + 1));
                backtrack(s, i + 1);
                path.pop_back();
            }
        }
    }
    
public:
    std::vector<std::vector<std::string>> partition(std::string s) {
        int n = s.length();
        dp.assign(n, std::vector<bool>(n, true));
        
        for (int i = n - 1; i >= 0; i--) {
            for (int j = i + 1; j < n; j++) {
                dp[i][j] = (s[i] == s[j] && dp[i + 1][j - 1]);
            }
        }
        
        result.clear();
        path.clear();
        backtrack(s, 0);
        return result;
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
            std::cout << "PASS: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "FAIL: " << testName << std::endl;
        }
    }

    bool expectEqual(const std::vector<std::vector<std::string>>& expected, 
                    const std::vector<std::vector<std::string>>& actual) {
        if (expected.size() != actual.size()) return false;
        
        auto sortedExpected = expected;
        auto sortedActual = actual;
        
        // 对每个子数组进行排序
        for (auto& v : sortedExpected) std::sort(v.begin(), v.end());
        for (auto& v : sortedActual) std::sort(v.begin(), v.end());
        
        // 对整个数组进行排序
        std::sort(sortedExpected.begin(), sortedExpected.end());
        std::sort(sortedActual.begin(), sortedActual.end());
        
        return sortedExpected == sortedActual;
    }

public:
    void runAllTests() {
        runTest("Example1", [this]() {
            std::string s = "aab";
            std::vector<std::vector<std::string>> expected = {{"a","a","b"}, {"aa","b"}};
            return expectEqual(expected, solution.partition(s));
        });

        runTest("Example2", [this]() {
            std::string s = "a";
            std::vector<std::vector<std::string>> expected = {{"a"}};
            return expectEqual(expected, solution.partition(s));
        });

        runTest("AllSameChars", [this]() {
            std::string s = "aaa";
            std::vector<std::vector<std::string>> expected = {
                {"a","a","a"}, 
                {"a","aa"}, 
                {"aa","a"}, 
                {"aaa"}
            };
            return expectEqual(expected, solution.partition(s));
        });

        runTest("NoValidPartition", [this]() {
            std::string s = "ab";
            std::vector<std::vector<std::string>> expected = {{"a","b"}};
            return expectEqual(expected, solution.partition(s));
        });

        runTest("LongerPalindrome", [this]() {
            std::string s = "aba";
            std::vector<std::vector<std::string>> expected = {
                {"a","b","a"},
                {"aba"}
            };
            return expectEqual(expected, solution.partition(s));
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