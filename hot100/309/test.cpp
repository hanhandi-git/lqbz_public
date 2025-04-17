#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
using namespace std;

// 股票交易含冷冻期解法
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        if (n <= 1) return 0;
        
        // 初始化三种状态
        int hold = -prices[0]; // 持有股票
        int sold = 0;          // 卖出股票（冷冻期）
        int reset = 0;         // 不持有股票且不在冷冻期
        
        for (int i = 1; i < n; i++) {
            int prevHold = hold;
            int prevSold = sold;
            int prevReset = reset;
            
            // 状态转移
            hold = max(prevHold, prevReset - prices[i]);
            sold = prevHold + prices[i];
            reset = max(prevReset, prevSold);
        }
        
        // 最大利润是最后一天不持有股票的最大值
        return max(sold, reset);
    }
};

// 测试框架
class TestFramework {
private:
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
        // 股票交易含冷冻期测试
        runTest("测试用例1 - 示例", [this]() {
            Solution solution;
            vector<int> prices = {1, 2, 3, 0, 2};
            return solution.maxProfit(prices) == 3;
        });

        runTest("测试用例2 - 持续上涨", [this]() {
            Solution solution;
            vector<int> prices = {1, 2, 3, 4, 5};
            return solution.maxProfit(prices) == 4; // 买1卖3，买4卖5（考虑冷冻期）
        });

        runTest("测试用例3 - 持续下跌", [this]() {
            Solution solution;
            vector<int> prices = {5, 4, 3, 2, 1};
            return solution.maxProfit(prices) == 0; // 不进行交易
        });

        runTest("测试用例4 - 波动价格", [this]() {
            Solution solution;
            vector<int> prices = {1, 4, 2, 7, 6, 8};
            return solution.maxProfit(prices) == 7; // 买1卖8
        });

        runTest("测试用例5 - 单日价格", [this]() {
            Solution solution;
            vector<int> prices = {5};
            return solution.maxProfit(prices) == 0; // 单日无法获利
        });

        runTest("测试用例6 - 空价格列表", [this]() {
            Solution solution;
            vector<int> prices = {};
            return solution.maxProfit(prices) == 0; // 空列表无法获利
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