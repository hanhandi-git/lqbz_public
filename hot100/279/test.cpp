#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <climits>
#include <vector>
using namespace std;


class Solution {
public:
    int numSquares(int n) {
        std::vector<int> dp(n+1, INT_MAX);
        dp[0] = 0;
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j*j <= i; j++)
            {
                dp[i] = std::min(dp[i], dp[i-j*j] + 1);
            }
        }
        return dp[n];
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
        // 基本测试
        runTest("基本测试 - 完全平方数", [this]() {
            return solution.numSquares(1) == 1;  // 1 = 1^2
        });

        runTest("基本测试 - 示例1", [this]() {
            return solution.numSquares(12) == 3;  // 12 = 4 + 4 + 4
        });

        runTest("基本测试 - 示例2", [this]() {
            return solution.numSquares(13) == 2;  // 13 = 4 + 9
        });

        // 边界测试
        runTest("边界测试 - 0", [this]() {
            return solution.numSquares(0) == 0;
        });

        runTest("边界测试 - 1", [this]() {
            return solution.numSquares(1) == 1;
        });

        runTest("边界测试 - 2", [this]() {
            return solution.numSquares(2) == 2;  // 2 = 1 + 1
        });

        // 特殊测试
        runTest("特殊测试 - 完全平方数", [this]() {
            return solution.numSquares(16) == 1;  // 16 = 4^2
        });

        runTest("特殊测试 - 需要多个相同平方数", [this]() {
            return solution.numSquares(8) == 2;  // 8 = 4 + 4
        });

        runTest("特殊测试 - 需要不同平方数", [this]() {
            return solution.numSquares(10) == 2;  // 10 = 1 + 9
        });

        runTest("特殊测试 - 较大数字", [this]() {
            return solution.numSquares(43) == 3;  // 43 = 25 + 9 + 9
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