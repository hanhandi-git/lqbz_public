#include <iostream>
#include <vector>
#include <functional>
#include <vector>
using namespace std;
class Solution {
public:
    int maxCoins(vector<int>& nums) {
        int n = nums.size();
        // 填充
        vector<int> balloons(n + 2, 1);
        for (int i = 1; i <= n; i++)
        {
            balloons[i] = nums[i-1];
        }
        vector<vector<int>> dp(n+2, vector<int>(n+2, 0));

        for (int len = 1; len <= n; len++) 
        {
            for (int left = 1; left <= n - len + 1; left++) 
            {
                int right = left + len - 1;
                for (int k = left; k <= right; k++)
                {
                    dp[left][right] = std::max(dp[left][right], 
                    dp[left][k-1] + dp[k+1][right] + balloons[left-1] * balloons[k] * balloons[right+1]);
                }
            }
        }
        return dp[1][n];
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
        runTest("测试用例1", [this]() {
            std::vector<int> nums = {3, 1, 5, 8};
            return (solution.maxCoins(nums) == 167); // 预期最大得分
        });

        runTest("测试用例2", [this]() {
            std::vector<int> nums = {1, 5};
            return (solution.maxCoins(nums) == 10); // 预期最大得分
        });

        runTest("测试用例4", [this]() {
            std::vector<int> nums = {1, 2, 3};
            return (solution.maxCoins(nums) == 12); // 预期最大得分
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