#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <algorithm> 

using namespace std;


class Solution {
public:
        // 添加一个辅助函数来打印栈的内容
    void printStack(stack<char> st) 
        {
            cout << "栈内容: ";
            if (st.empty()) {
                cout << "空";
            } else {
                // 由于stack不能直接遍历，需要创建一个临时栈
                vector<char> temp;
                while (!st.empty()) {
                    temp.push_back(st.top());
                    st.pop();
                }
                // 反向打印以显示栈底到栈顶的顺序
                for (int i = temp.size() - 1; i >= 0; i--) {
                    cout << temp[i] << " ";
                }
            }
            cout << endl;
        }

    int longestValidParentheses(string s) {
        stack<int> st;
        if (s.length() <= 1) return 0;
        st.push(-1);
        int maxLen = 0;
        for (int i = 0; i < s.length(); i++)
        {
           if (s[i] == '(') 
           {
                st.push(i);
           }
           else // 如果是')'，更新
           {
                st.pop();
                if (st.empty())
                {
                    // 说明当前)没有对应的(
                    // 如果栈空了，说明这个右括号没有匹配的左括号
                    // 将其作为新的参考点
                    st.push(i);
                }
                else
                {
                    maxLen = std::max(maxLen, i - st.top());
                }
           }
        }
        return maxLen;
    }

    // 生成所有可能的有效括号组合
    // 参数 n: 需要生成的括号对数
    // 返回值: 包含所有有效括号组合的字符串数组
    vector<string> generateParenthesis(int n) {
        vector<string> result;  // 存储所有有效的括号组合
        backtrack(result, "", 0, 0, n);  // 从空字符串开始回溯
        return result;
    }
    
    // 3. 删除无效的括号
    vector<string> removeInvalidParentheses(string s) {
        vector<string> result;
        int left = 0, right = 0;
        
        // 计算需要删除的左右括号数量
        for (char c : s) {
            if (c == '(') {
                left++;
            } else if (c == ')') {
                if (left > 0) {
                    left--;
                } else {
                    right++;
                }
            }
        }
        
        dfs(s, 0, left, right, result);
        return result;
    }

private:
    // 回溯函数，用于生成有效的括号组合
    // 参数说明:
    // result: 存储所有有效组合的结果数组
    // current: 当前正在构建的括号字符串
    // open: 当前已使用的左括号数量
    // close: 当前已使用的右括号数量
    // max: 需要生成的括号对数（最大数量）
    void backtrack(vector<string>& result, string current, int open, int close, int max) {
        // 终止条件：当前字符串长度等于括号对数的两倍（每对括号需要两个字符）
        if (current.length() == max * 2) {
            result.push_back(current);  // 将完整的有效组合添加到结果中
            return;
        }
        
        // 可以添加左括号的条件：已使用的左括号数量小于最大值
        if (open < max) {
            // 添加左括号，并继续递归
            backtrack(result, current + "(", open + 1, close, max);
        }
        
        // 可以添加右括号的条件：已使用的右括号数量小于已使用的左括号数量
        // 这确保了括号组合的有效性
        if (close < open) {
            // 添加右括号，并继续递归
            backtrack(result, current + ")", open, close + 1, max);
        }
    }
    
    void dfs(string s, int start, int left, int right, vector<string>& result) {
        if (left == 0 && right == 0) {
            if (isValid(s)) {
                result.push_back(s);
            }
            return;
        }
        
        for (int i = start; i < s.length(); i++) {
            if (i > start && s[i] == s[i-1]) continue;
            
            if (right > 0 && s[i] == ')') {
                string next = s.substr(0, i) + s.substr(i + 1);
                dfs(next, i, left, right - 1, result);
            } else if (left > 0 && s[i] == '(') {
                string next = s.substr(0, i) + s.substr(i + 1);
                dfs(next, i, left - 1, right, result);
            }
        }
    }
    
    bool isValid(string s) {
        int count = 0;
        for (char c : s) {
            if (c == '(') count++;
            if (c == ')') count--;
            if (count < 0) return false;
        }
        return count == 0;
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
        // 最长有效括号测试
        runTest("最长有效括号 - 基本测试", [this]() {
            return solution.longestValidParentheses("(()") == 2;
        });

        runTest("最长有效括号 - 复杂测试", [this]() {
            return solution.longestValidParentheses(")()())") == 4;
        });

        // 添加新的测试用例
        runTest("最长有效括号 - 空字符串", [this]() {
            return solution.longestValidParentheses("") == 0;
        });

        runTest("最长有效括号 - 全无效", [this]() {
            return solution.longestValidParentheses(")(") == 0;
        });

        runTest("最长有效括号 - 长序列", [this]() {
            return solution.longestValidParentheses("((()))()") == 8;
        });

        // 括号生成测试
        runTest("括号生成 - n=1", [this]() {
            vector<string> result = solution.generateParenthesis(1);
            return result.size() == 1 && result[0] == "()";
        });

        runTest("括号生成 - n=2", [this]() {
            vector<string> result = solution.generateParenthesis(2);
            return result.size() == 2 && 
                   find(result.begin(), result.end(), "()()") != result.end() &&
                   find(result.begin(), result.end(), "(())") != result.end();
        });

        // 添加新的测试用例
        runTest("括号生成 - n=3", [this]() {
            vector<string> result = solution.generateParenthesis(3);
            vector<string> expected = {"((()))", "(()())", "(())()", "()(())", "()()()"};
            return result.size() == 5 && 
                   all_of(expected.begin(), expected.end(), [&](const string& s) {
                       return find(result.begin(), result.end(), s) != result.end();
                   });
        });

        runTest("括号生成 - n=0", [this]() {
            vector<string> result = solution.generateParenthesis(0);
            return result.size() == 1 && result[0] == "";
        });

        // 删除无效的括号测试
        runTest("删除无效的括号 - 基本测试", [this]() {
            vector<string> result = solution.removeInvalidParentheses("()())()");
            return result.size() == 2 &&
                   find(result.begin(), result.end(), "(())()") != result.end() &&
                   find(result.begin(), result.end(), "()()()") != result.end();
        });

        // 添加新的测试用例
        runTest("删除无效的括号 - 全部无效", [this]() {
            vector<string> result = solution.removeInvalidParentheses(")(");
            return result.size() == 1 && result[0] == "";
        });

        runTest("删除无效的括号 - 已经有效", [this]() {
            vector<string> result = solution.removeInvalidParentheses("()");
            return result.size() == 1 && result[0] == "()";
        });

        runTest("删除无效的括号 - 复杂情况", [this]() {
            vector<string> result = solution.removeInvalidParentheses("(())(((");
            return result.size() == 1 && result[0] == "(())";
        });

        runTest("删除无效的括号 - 空字符串", [this]() {
            vector<string> result = solution.removeInvalidParentheses("");
            return result.size() == 1 && result[0] == "";
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
