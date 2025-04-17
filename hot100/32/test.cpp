#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <stack>
using namespace std;

// 实现最长有效括号的Solution类
class Solution {
public:
    int longestValidParentheses(string s) {
        int maxLen = 0;
        stack<int> st;
        st.push(-1);
        
        for (int i = 0; i < s.length(); i++) {
            if (s[i] == '(') {
                st.push(i);
            } else {
                st.pop();
                if (st.empty()) {
                    st.push(i);
                } else {
                    maxLen = max(maxLen, i - st.top());
                }
            }
        }
        return maxLen;
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
        runTest("测试用例 1: s = \"(()\"", [this]() {
            string s = "(()";
            int expected = 2;
            return solution.longestValidParentheses(s) == expected;
        });

        runTest("测试用例 2: s = \")()())\"", [this]() {
            string s = ")()())";
            int expected = 4;
            return solution.longestValidParentheses(s) == expected;
        });

        runTest("测试用例 3: s = \"\"", [this]() {
            string s = "";
            int expected = 0;
            return solution.longestValidParentheses(s) == expected;
        });

        runTest("测试用例 4: s = \"()(()\"", [this]() {
            string s = "()(()";
            int expected = 2;
            return solution.longestValidParentheses(s) == expected;
        });

        runTest("测试用例 5: s = \"()(())\"", [this]() {
            string s = "()(())";
            int expected = 6;
            return solution.longestValidParentheses(s) == expected;
        });

        // 添加更多测试用例
        runTest("测试用例 6: s = \"((()))()\"", [this]() {
            string s = "((()))()";
            int expected = 8;
            return solution.longestValidParentheses(s) == expected;
        });

        runTest("测试用例 7: s = \"())\"", [this]() {
            string s = "())";
            int expected = 2;
            return solution.longestValidParentheses(s) == expected;
        });

        runTest("测试用例 8: 边界情况 - 只有左括号", [this]() {
            string s = "(((";
            int expected = 0;
            return solution.longestValidParentheses(s) == expected;
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