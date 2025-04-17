#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <climits>

class Solution {
public:
    // 基础版本：单次交易
    int maxProfit(std::vector<int>& prices) {
        if (prices.empty()) return 0;
        
        int minPrice = INT_MAX;
        int maxProfit = 0;
        
        for (int price : prices) {
            minPrice = std::min(minPrice, price);
            maxProfit = std::max(maxProfit, price - minPrice);
        }
        
        return maxProfit;
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
        runTest("基本测试 - 有利润", [this]() {
            std::vector<int> prices = {7,1,5,3,6,4};
            return solution.maxProfit(prices) == 5;
        });

        runTest("基本测试 - 无利润", [this]() {
            std::vector<int> prices = {7,6,4,3,1};
            return solution.maxProfit(prices) == 0;
        });

        // 边界测试
        runTest("边界测试 - 空数组", [this]() {
            std::vector<int> prices;
            return solution.maxProfit(prices) == 0;
        });

        runTest("边界测试 - 单个价格", [this]() {
            std::vector<int> prices = {1};
            return solution.maxProfit(prices) == 0;
        });

        // 特殊情况测试
        runTest("特殊情况 - 相同价格", [this]() {
            std::vector<int> prices = {1,1,1,1};
            return solution.maxProfit(prices) == 0;
        });

        runTest("特殊情况 - 先升后降", [this]() {
            std::vector<int> prices = {1,4,2};
            return solution.maxProfit(prices) == 3;
        });

        runTest("特殊情况 - 连续上升", [this]() {
            std::vector<int> prices = {1,2,3,4,5};
            return solution.maxProfit(prices) == 4;
        });

        runTest("特殊情况 - 连续下降", [this]() {
            std::vector<int> prices = {5,4,3,2,1};
            return solution.maxProfit(prices) == 0;
        });

        runTest("特殊情况 - 波动价格", [this]() {
            std::vector<int> prices = {3,3,5,0,0,3,1,4};
            return solution.maxProfit(prices) == 4;
        });

        runTest("特殊情况 - 大数测试", [this]() {
            std::vector<int> prices = {INT_MAX, 1, INT_MAX};
            return solution.maxProfit(prices) == INT_MAX - 1;
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
