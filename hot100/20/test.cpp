#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <stack>
using namespace std;


class Solution {
public:
    bool isValid(string s) {
        std::stack<char> st;
        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == '[') {
                st.push(']');
            } else if (s[i] == '{') {
                st.push('}');
            } else if (s[i] == '(') {
                st.push(')');
            } else {
                // 开始pop
                if (st.empty()) return false;
                if (st.top() == s[i]) {
                    st.pop();
                } else {
                    return false;
                }
            }
        }
        return st.empty();
    }
};

// class Solution {
// public:
//     bool isValid(string s) {
//         std::stack<char> stk;
//         for (auto c : s)
//         {
//             if (c == '[') {
//                 stk.push(']');
//             } else if (c == '(') {
//                 stk.push(')');
//             } else if (c == '{') {
//                 stk.push('}');
//             } else {
//                 if (stk.empty()) return false;
//                 if (stk.top() != c) return false;
//                 else stk.pop();
//             }
//         }
//         return stk.empty();
//     }
// };

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
        // 基本测试 - 有效的括号
        runTest("基本测试 - 有效的括号", [this]() {
            return solution.isValid("([]{}") == false; // 示例输入
        });

        // 基本测试 - 无效的括号
        runTest("基本测试 - 无效的括号", [this]() {
            return solution.isValid("([)]") == false; // 示例输入
        });

        // 边界测试 - 空字符串
        runTest("边界测试 - 空字符串", [this]() {
            return solution.isValid("") == true; // 空字符串是有效的
        });

        // 边界测试 - 单个左括号
        runTest("边界测试 - 单个左括号", [this]() {
            return solution.isValid("(") == false; // 单个左括号无效
        });

        // 边界测试 - 单个右括号
        runTest("边界测试 - 单个右括号", [this]() {
            return solution.isValid(")") == false; // 单个右括号无效
        });

        // 特殊测试 - 嵌套括号
        runTest("特殊测试 - 嵌套括号", [this]() {
            return solution.isValid("{[()]}") == true; // 嵌套有效
        });

        // 特殊测试 - 复杂有效括号
        runTest("特殊测试 - 复杂有效括号", [this]() {
            return solution.isValid("((())){}[{}]") == true; // 复杂有效
        });

        // 特殊测试 - 复杂无效括号
        runTest("特殊测试 - 复杂无效括号", [this]() {
            return solution.isValid("{[(])}") == false; // 复杂无效
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