#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;

class Solution {
public:
    int climbStairs(int n) {
        if (n <= 1 ) return 1;
        std::vector<int> dp(n+1, 0);
        dp[0] = 1;
        dp[1] = 1;
        for (int i = 2; i < dp.size(); i++)
        {
            dp[i] = dp[i-1] + dp[i-2];
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
        runTest("测试用例 1: n = 2", [this]() {
            return solution.climbStairs(2) == 2; // 1 阶 + 1 阶 或 2 阶
        });

        runTest("测试用例 2: n = 3", [this]() {
            return solution.climbStairs(3) == 3; // 1 阶 + 1 阶 + 1 阶 或 1 阶 + 2 阶 或 2 阶 + 1 阶
        });

        runTest("测试用例 3: n = 1", [this]() {
            return solution.climbStairs(1) == 1; // 只有 1 种方法
        });

        runTest("测试用例 4: n = 0", [this]() {
            return solution.climbStairs(0) == 1; // 0 阶的方法只有 1 种，即不动
        });

        runTest("测试用例 5: n = 4", [this]() {
            return solution.climbStairs(4) == 5; // 1+1+1+1, 1+1+2, 1+2+1, 2+1+1, 2+2
        });

        runTest("测试用例 6: n = 5", [this]() {
            return solution.climbStairs(5) == 8; // 1+1+1+1+1, 1+1+1+2, 1+1+2+1, 1+2+1+1, 2+1+1+1, 2+2+1, 2+1+2, 1+2+2
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