#include <iostream>
#include <vector>
#include <functional>
using namespace std;

class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, amount + 1);
        dp[0] = 0; // 凑成金额 0 不需要任何硬币
        
        for (int i = 1; i <= amount; i++) {
            for (int coin : coins) {
                if (i - coin >= 0) {
                    dp[i] = min(dp[i], dp[i - coin] + 1);
                }
            }
        }
        
        return dp[amount] == amount + 1 ? -1 : dp[amount];
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
            std::vector<int> coins = {1, 2, 5};
            int amount = 11;
            return (solution.coinChange(coins, amount) == 3); // 预期最少硬币数
        });

        runTest("测试用例2", [this]() {
            std::vector<int> coins = {2};
            int amount = 3;
            return (solution.coinChange(coins, amount) == -1); // 预期无法凑成
        });

        runTest("测试用例3", [this]() {
            std::vector<int> coins = {1};
            int amount = 0;
            return (solution.coinChange(coins, amount) == 0); // 预期最少硬币数为0
        });

        runTest("测试用例4", [this]() {
            std::vector<int> coins = {1, 5, 10};
            int amount = 27;
            return (solution.coinChange(coins, amount) == 4); // 预期最少硬币数
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