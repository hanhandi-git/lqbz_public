#include <iostream>
#include <functional>
#include <string>
#include <algorithm>
using namespace std;

class Solution {
public:
    string addStrings(string num1, string num2) {
        int len1 = num1.length() - 1;
        int len2 = num2.length() - 1;
        std::string res = "";
        int jinwei = 0;
        while (len1 >= 0 && len2 >= 0)
        {
            int val1 = num1[len1] - '0';
            int val2 = num2[len2] - '0';
            int sum = (jinwei + val1 + val2) % 10;
            jinwei = (jinwei + val1 + val2) / 10;
            res += (sum + '0');
            len1--;
            len2--;
        }
        while (len1 >= 0) {
            int sum = (jinwei + num1[len1] - '0') % 10;
            jinwei = (jinwei + num1[len1] - '0') / 10;
            res += (sum + '0');
            len1--;
        }
        while (len2 >= 0) {
            int sum = (jinwei + num2[len2] - '0') % 10;
            jinwei = (jinwei + num2[len2] - '0') / 10;
            res += (sum + '0');
            len2--;
        }
        if (jinwei > 0) {
            res += (jinwei + '0');
        }
        std::reverse(res.begin(), res.end());
        return res;
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
        runTest("基本测试 - 简单相加", [this]() {
            return solution.addStrings("11", "123") == "134";
        });

        runTest("基本测试 - 需要进位", [this]() {
            return solution.addStrings("456", "77") == "533";
        });

        runTest("基本测试 - 零相加", [this]() {
            return solution.addStrings("0", "0") == "0";
        });

        // 边界测试
        runTest("边界测试 - 一个数为零", [this]() {
            return solution.addStrings("123", "0") == "123";
        });

        runTest("边界测试 - 大数相加", [this]() {
            return solution.addStrings("9999", "1") == "10000";
        });

        // 复杂测试
        runTest("复杂测试 - 不同长度", [this]() {
            return solution.addStrings("1", "9999") == "10000";
        });

        runTest("复杂测试 - 多位进位", [this]() {
            return solution.addStrings("999", "999") == "1998";
        });

        runTest("复杂测试 - 长数字", [this]() {
            return solution.addStrings("1234567890", "9876543210") == "11111111100";
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
