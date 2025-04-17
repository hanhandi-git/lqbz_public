#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <climits>

class Solution {
public:
    // 1. 多次交易
    int maxProfitII(std::vector<int>& prices) {
        int totalProfit = 0;
        for (int i = 1; i < prices.size(); i++) {
            if (prices[i] > prices[i-1]) {
                totalProfit += prices[i] - prices[i-1];
            }
        }
        return totalProfit;
    }
    
    // 2. 最多两次交易
    int maxProfitIII(std::vector<int>& prices) {
        int firstBuy = INT_MIN, firstSell = 0;
        int secondBuy = INT_MIN, secondSell = 0;
        
        for (int price : prices) {
            firstBuy = std::max(firstBuy, -price);
            firstSell = std::max(firstSell, firstBuy + price);
            secondBuy = std::max(secondBuy, firstSell - price);
            secondSell = std::max(secondSell, secondBuy + price);
        }
        
        return secondSell;
    }

    // 3. 最多k次交易
    // int maxProfitIV(int k, std::vector<int>& prices) {
    //     if (prices.empty() || k == 0) return 0;
        
    //     // 如果k超过n/2，等价于无限次交易
    //     if (k >= prices.size() / 2) {
    //         return maxProfitII(prices);
    //     }
        
    //     // 修改初始化方式
    //     std::vector<std::vector<int>> dp(k + 1, std::vector<int>(2));
    //     // 初始化第0天的状态
    //     for (int i = 0; i <= k; i++) {
    //         dp[i][0] = 0;          // 不持有股票
    //         dp[i][1] = -prices[0]; // 持有股票
    //     }
        
    //     // 遍历每一天
    //     for (int i = 1; i < prices.size(); i++) {
    //         for (int j = k; j >= 1; j--) {
    //             // 第j次交易，不持有股票的最大利润
    //             dp[j][0] = std::max(dp[j][0], dp[j][1] + prices[i]);
    //             // 第j次交易，持有股票的最大利润
    //             dp[j][1] = std::max(dp[j][1], dp[j-1][0] - prices[i]);
    //         }
    //     }
        
    //     return dp[k][0];
    // }
    
    // 4. 含冷冻期
    // int maxProfitWithCooldown(std::vector<int>& prices) {
    //     if (prices.empty()) return 0;
        
    //     int sold = 0;
    //     int held = -prices[0];
    //     int rest = 0;
        
    //     for (int i = 1; i < prices.size(); i++) {
    //         int prevSold = sold;
    //         sold = held + prices[i];
    //         held = std::max(held, rest - prices[i]);
    //         rest = std::max(rest, prevSold);
    //     }
        
    //     return std::max(sold, rest);
    // }
    
    // 5. 含手续费
    // int maxProfitWithFee(std::vector<int>& prices, int fee) {
    //     if (prices.empty()) return 0;  // 添加空数组检查
        
    //     int sold = 0;
    //     int held = -prices[0];
        
    //     for (int price : prices) {
    //         int prevSold = sold;
    //         sold = std::max(sold, held + price - fee);
    //         held = std::max(held, prevSold - price);
    //     }
        
    //     return sold;
    // }
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
        // 多次交易测试
        runTest("多次交易 - 基本测试", [this]() {
            std::vector<int> prices = {7,1,5,3,6,4};
            return solution.maxProfitII(prices) == 7;
        });
        
        runTest("多次交易 - 连续上升", [this]() {
            std::vector<int> prices = {1,2,3,4,5};
            return solution.maxProfitII(prices) == 4;
        });
        
        runTest("多次交易 - 连续下降", [this]() {
            std::vector<int> prices = {5,4,3,2,1};
            return solution.maxProfitII(prices) == 0;
        });

        // 最多两次交易测试
        runTest("两次交易 - 基本测试", [this]() {
            std::vector<int> prices = {3,3,5,0,0,3,1,4};
            return solution.maxProfitIII(prices) == 6;
        });
        
        runTest("两次交易 - 一次最优", [this]() {
            std::vector<int> prices = {1,2,3,4,5};
            return solution.maxProfitIII(prices) == 4;
        });
        
        runTest("两次交易 - 复杂场景", [this]() {
            std::vector<int> prices = {1,2,4,2,5,7,2,4,9,0};
            return solution.maxProfitIII(prices) == 13;
        });

        // k次交易测试 存在bug
        // runTest("k次交易 - k=2测试", [this]() {
        //     std::vector<int> prices = {2,4,1,7,6,8};
        //     return solution.maxProfitIV(2, prices) == 10;
        // });
        
        // runTest("k次交易 - k=3测试", [this]() {
        //     std::vector<int> prices = {3,2,6,5,0,3,1,4};
        //     return solution.maxProfitIV(3, prices) == 8;
        // });
        
        // runTest("k次交易 - k大于天数", [this]() {
        //     std::vector<int> prices = {1,2,3,4,5};
        //     return solution.maxProfitIV(10, prices) == 4;
        // });

        // 含冷冻期测试
        // runTest("冷冻期 - 基本测试", [this]() {
        //     std::vector<int> prices = {1,2,3,0,2};
        //     return solution.maxProfitWithCooldown(prices) == 3;
        // });
        
        // runTest("冷冻期 - 连续交易", [this]() {
        //     std::vector<int> prices = {1,2,3,4,5,6};
        //     return solution.maxProfitWithCooldown(prices) == 4;
        // });
        
        // runTest("冷冻期 - 复杂场景", [this]() {
        //     std::vector<int> prices = {1,4,2,7,6,9,2,11};
        //     return solution.maxProfitWithCooldown(prices) == 11;
        // });

        // // 含手续费测试
        // runTest("手续费 - 基本测试", [this]() {
        //     std::vector<int> prices = {1,3,2,8,4,9};
        //     return solution.maxProfitWithFee(prices, 2) == 8;
        // });
        
        // runTest("手续费 - 高手续费", [this]() {
        //     std::vector<int> prices = {1,3,7,5,10,3};
        //     return solution.maxProfitWithFee(prices, 3) == 6;
        // });
        
        // runTest("手续费 - 不值得交易", [this]() {
        //     std::vector<int> prices = {1,3,2,4};
        //     return solution.maxProfitWithFee(prices, 2) == 0;
        // });

        // 边界测试
        runTest("边界测试 - 空数组", [this]() {
            std::vector<int> prices;
            bool test1 = solution.maxProfitII(prices) == 0;
            bool test2 = solution.maxProfitIII(prices) == 0;
            // bool test3 = solution.maxProfitIV(2, prices) == 0;
            // bool test4 = solution.maxProfitWithCooldown(prices) == 0;
            // bool test5 = solution.maxProfitWithFee(prices, 1) == 0;
            return test1 && test2 ;
        });
        
        runTest("边界测试 - 单个元素", [this]() {
            std::vector<int> prices = {1};
            bool test1 = solution.maxProfitII(prices) == 0;
            bool test2 = solution.maxProfitIII(prices) == 0;
            // bool test3 = solution.maxProfitIV(2, prices) == 0;
            // bool test4 = solution.maxProfitWithCooldown(prices) == 0;
            // bool test5 = solution.maxProfitWithFee(prices, 1) == 0;
            return test1 && test2 ;
        });
        
        runTest("边界测试 - 相同价格", [this]() {
            std::vector<int> prices = {5,5,5,5,5};
            bool test1 = solution.maxProfitII(prices) == 0;
            bool test2 = solution.maxProfitIII(prices) == 0;
            // bool test3 = solution.maxProfitIV(2, prices) == 0;
            // bool test4 = solution.maxProfitWithCooldown(prices) == 0;
            // bool test5 = solution.maxProfitWithFee(prices, 1) == 0;
            return test1 && test2 ;
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
