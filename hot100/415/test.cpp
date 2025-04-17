#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <algorithm>
using namespace std;

class Solution {
public:
    string addStrings(string num1, string num2) {
        string res = "";
        int i = num1.size() - 1;
        int j = num2.size() - 1;
        int carry = 0;
        while (i >= 0 || j >= 0 || carry > 0)
        {
            int x = (i >= 0) ? num1[i] - '0' : 0;
            int y = (j >= 0) ? num2[j] - '0' : 0;
            int sum = x + y + carry;
            carry = sum / 10;
            res = to_string(sum % 10) + res;
            i--;
            j--;
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
        // 测试用例1 - 基本示例
        runTest("测试用例1 - 基本示例", [this]() {
            string num1 = "123";
            string num2 = "456";
            string expected = "579";
            return (solution.addStrings(num1, num2) == expected);
        });

        // 测试用例2 - 有进位
        runTest("测试用例2 - 有进位", [this]() {
            string num1 = "456";
            string num2 = "777";
            string expected = "1233";
            return (solution.addStrings(num1, num2) == expected);
        });

        // 测试用例3 - 不同长度的字符串
        runTest("测试用例3 - 不同长度的字符串", [this]() {
            string num1 = "1";
            string num2 = "9999";
            string expected = "10000";
            return (solution.addStrings(num1, num2) == expected);
        });

        // 测试用例4 - 零的情况
        runTest("测试用例4 - 零的情况", [this]() {
            string num1 = "0";
            string num2 = "0";
            string expected = "0";
            return (solution.addStrings(num1, num2) == expected);
        });

        // 测试用例5 - 大数相加
        runTest("测试用例5 - 大数相加", [this]() {
            string num1 = "9999999999999999999999999999999";
            string num2 = "1";
            string expected = "10000000000000000000000000000000";
            return (solution.addStrings(num1, num2) == expected);
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