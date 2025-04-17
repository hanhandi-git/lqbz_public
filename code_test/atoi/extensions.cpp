#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
#include <utility>
#include <stack>
#include <climits> // 添加此行以使用 INT_MAX 和 INT_MIN


class Solution {
public:
    // 实现 myAtoi 函数
    int myAtoi(std::string s) {
        int i = 0;
        int n = s.length();
        
        // 1. 跳过前导空格
        while (i < n && s[i] == ' ') {
            i++;
        }
        
        // 2. 处理符号
        int sign = 1;
        if (i < n && (s[i] == '+' || s[i] == '-')) {
            sign = (s[i] == '+') ? 1 : -1;
            i++;
        }
        
        // 3. 转换数字
        long result = 0;  // 使用long防止计算过程中溢出
        while (i < n && isdigit(s[i])) {
            result = result * 10 + (s[i] - '0');
            
            // 4. 处理溢出
            if (sign == 1 && result > INT_MAX) {
                return INT_MAX;
            }
            if (sign == -1 && -result < INT_MIN) {
                return INT_MIN;
            }
            
            i++;
        }
        
        // 5. 返回结果
        return sign * result;
    }

    // 有效数字的实现
    bool isNumber(std::string s) {
        bool seenDigit = false;
        bool seenExponent = false;
        bool seenDot = false;
        
        for (int i = 0; i < s.length(); i++) {
            if (isdigit(s[i])) {
                seenDigit = true;
            } else if (s[i] == 'e' || s[i] == 'E') {
                if (seenExponent || !seenDigit) return false;
                seenExponent = true;
                seenDigit = false;
            } else if (s[i] == '.') {
                if (seenDot || seenExponent) return false;
                seenDot = true;
            } else if (s[i] == '+' || s[i] == '-') {
                if (i > 0 && s[i-1] != 'e' && s[i-1] != 'E') return false;
            } else {
                return false;
            }
        }
        
        return seenDigit;
    }

    // 字符串计算器的实现
    int calculate(std::string s) {
        std::stack<int> nums;
        std::stack<char> ops;
        int num = 0;
        int result = 0;
        int sign = 1;
        
        for (char c : s) {
            if (isdigit(c)) {
                num = num * 10 + (c - '0');
            } else if (c == '+' || c == '-') {
                result += sign * num;
                num = 0;
                sign = (c == '+') ? 1 : -1;
            } else if (c == '(') {
                nums.push(result);
                ops.push(sign);
                result = 0;
                sign = 1;
            } else if (c == ')') {
                result += sign * num;
                num = 0;
                result = nums.top() + ops.top() * result;
                nums.pop();
                ops.pop();
            }
        }
        
        return result + sign * num;
    }
};

class TestFramework {
private:
    Solution solution; // 修改为使用 Solution 类
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
        // 测试 myAtoi 函数
        runTest("myAtoi - 基本情况", [this]() {
            return solution.myAtoi("42") == 42;
        });

        runTest("myAtoi - 负数", [this]() {
            return solution.myAtoi("   -42") == -42;
        });

        runTest("myAtoi - 带有字母", [this]() {
            return solution.myAtoi("4193 with words") == 4193;
        });

        runTest("myAtoi - 溢出情况", [this]() {
            return solution.myAtoi("2147483648") == INT_MAX; // 超过 32 位整数范围
        });

        runTest("myAtoi - 负溢出情况", [this]() {
            return solution.myAtoi("-2147483649") == INT_MIN; // 超过 32 位整数范围
        });

        runTest("myAtoi - 空字符串", [this]() {
            return solution.myAtoi("") == 0;
        });

        runTest("myAtoi - 只有空格", [this]() {
            return solution.myAtoi("   ") == 0;
        });

        runTest("myAtoi - 只有符号", [this]() {
            return solution.myAtoi("+") == 0;
        });

        // 测试有效数字
        runTest("isNumber - 基本情况", [this]() {
            return solution.isNumber("0.1") == true;
        });

        runTest("isNumber - 科学计数法", [this]() {
            return solution.isNumber("2e10") == true;
        });

        runTest("isNumber - 无效数字", [this]() {
            return solution.isNumber("abc") == false;
        });

        runTest("isNumber - 负号", [this]() {
            return solution.isNumber("-1.5") == true;
        });

        // 测试字符串计算器
        runTest("calculate - 基本情况", [this]() {
            return solution.calculate("1 + 1") == 2;
        });

        runTest("calculate - 带括号", [this]() {
            return solution.calculate("(1+(4+5+2)-3)+(6+8)") == 23;
        });

        runTest("calculate - 负数", [this]() {
            return solution.calculate("-2 + 1") == -1;
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