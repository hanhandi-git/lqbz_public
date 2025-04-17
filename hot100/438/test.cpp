#include <iostream>
#include <vector>
#include <functional>
#include <string>
using namespace std;

class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        if (s.empty() || p.empty() || s.size() < p.size()) return {};
        std::vector<int> need(26, 0);
        std::vector<int> window(26, 0);
        std::vector<int> res;
        int n = p.size();
        // 填充need
        for (char c : p)
        {
            need[c - 'a']++;
        }
        // 填充window
        for (int i = 0; i < n; i++)
        {
            window[s[i] - 'a']++;
        }
        if (window == need) {
            res.push_back(0);
        }
        // 移动窗口
        for (int i = n, j = 1; i < s.size(); i++, j++)
        {
            // out
            window[s[j-1] - 'a']--;
            // in
            window[s[i] - 'a']++;
            if (window == need) {
                res.push_back(j);
            }
        }
        return res;
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
        runTest("测试用例1 - 基本情况", [this]() {
            string s = "cbaebabacd";
            string p = "abc";
            vector<int> expected = {0, 6};
            return (solution.findAnagrams(s, p) == expected);
        });

        runTest("测试用例2 - s比p短", [this]() {
            string s = "ab";
            string p = "abc";
            vector<int> expected = {};
            return (solution.findAnagrams(s, p) == expected);
        });

        runTest("测试用例3 - s和p相等", [this]() {
            string s = "abc";
            string p = "abc";
            vector<int> expected = {0};
            return (solution.findAnagrams(s, p) == expected);
        });

        runTest("测试用例4 - p只有一个字符", [this]() {
            string s = "abab";
            string p = "a";
            vector<int> expected = {0, 2};
            return (solution.findAnagrams(s, p) == expected);
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