#include <iostream>
#include <functional>
#include <string>
#include <stack>
#include <unordered_map>
#include <algorithm> 
using namespace std;

class Solution {
public:

    bool isValid(string s) {
        stack<char> st;
        std::unordered_map<char, char> pairs = {
            {'(', ')'},
            {'[', ']'},
            {'{', '}'}
        };
        for (char c : s)
        {
            if (pairs.find(c) != pairs.end()) {
                st.push(pairs[c]);
            }
            else {
                if (st.empty()) return false;
                if (st.top() == c)
                {
                    st.pop();
                }
            }
        }
        // 最后栈应该为空
        return st.empty();
    }
};

// 测试框架
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    void runTest(const string& testName, function<bool()> test) {
        total++;
        if (test()) {
            cout << "通过: " << testName << endl;
            passed++;
        } else {
            cout << "失败: " << testName << endl;
        }
    }

public:
    void runAllTests() {
        // 基本测试
        runTest("基本测试 - 有效括号", [this]() {
            return solution.isValid("()") == true;
        });

        runTest("基本测试 - 嵌套括号", [this]() {
            return solution.isValid("()[]{}") == true;
        });

        runTest("基本测试 - 复杂嵌套", [this]() {
            return solution.isValid("([{}])") == true;
        });

        // 无效情况测试
        runTest("无效测试 - 不匹配", [this]() {
            return solution.isValid("(]") == false;
        });

        runTest("无效测试 - 未闭合", [this]() {
            return solution.isValid("([") == false;
        });

        runTest("无效测试 - 多余的右括号", [this]() {
            return solution.isValid(")(") == false;
        });

        // 边界测试
        runTest("边界测试 - 空字符串", [this]() {
            return solution.isValid("") == true;
        });

        runTest("边界测试 - 单个字符", [this]() {
            return solution.isValid("[") == false;
        });

        // 复杂测试
        runTest("复杂测试 - 多层嵌套", [this]() {
            return solution.isValid("{[()]}") == true;
        });

        runTest("复杂测试 - 多种错误组合", [this]() {
            return solution.isValid("([)]") == false;
        });

        cout << "\n测试结果: " << passed << " 通过, " 
             << (total - passed) << " 失败, " 
             << total << " 总计" << endl;
    }
};

int main() {
    TestFramework testFramework;
    testFramework.runAllTests();
    return 0;
}
