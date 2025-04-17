#include <iostream>
#include <functional>
#include <vector>
#include <climits>
#include <ctype.h>

class Solution {
public:
    int myAtoi(std::string s) {
        int i = 0;
        int n = s.size();
        if (n == 0) return 0;
        while (s[i] == ' ') {
            i++;
        }

        int sign = 1;
        if (i < n ) {
            if (s[i] == '+') {
                sign = 1;
                i++;
            } else if (s[i] == '-') {
                sign = -1;
                i++;
            }
        }

        long ures = 0;
        while (i < n && isdigit(s[i])) {
            ures = ures * 10 + (s[i] - '0');
            if (sign == 1 && ures > INT_MAX) {
                return INT_MAX;
            } else if (sign == -1 && ures > INT_MAX){
                return INT_MIN;
            }
            i++;
        }
        return sign * ures;
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
        // 测试 myAtoi 方法
        runTest("示例1 - '42'", [this]() {
            return solution.myAtoi("42") == 42;
        });

        runTest("示例2 - '   -42'", [this]() {
            return solution.myAtoi("   -42") == -42;
        });

        runTest("示例3 - '4193 with words'", [this]() {
            return solution.myAtoi("4193 with words") == 4193;
        });

        runTest("示例4 - '   +0 123'", [this]() {
            return solution.myAtoi("   +0 123") == 0;
        });

        runTest("示例5 - 'words and 987'", [this]() {
            return solution.myAtoi("words and 987") == 0;
        });

        runTest("示例6 - '-91283472332'", [this]() {
            return solution.myAtoi("-91283472332") == INT_MIN;
        });

        runTest("示例7 - '91283472332'", [this]() {
            return solution.myAtoi("91283472332") == INT_MAX;
        });

        runTest("示例8 - '   -   42'", [this]() {
            return solution.myAtoi("   -   42") == 0;
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