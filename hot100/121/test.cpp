#include <vector>
#include <iostream>
#include <functional>
#include <climits>

using namespace std;


class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.size() <= 1) return 0;
        int max_profit = INT_MIN;
        int min_price = INT_MAX;
        for (int i = 0; i < prices.size(); i++)
        {
            min_price = std::min(min_price, prices[i]);
            max_profit = std::max(max_profit, prices[i] - min_price);
        }
        return max_profit;
    }
};

// class Solution {
// public:
//     int maxProfit(vector<int>& prices) {
//         if (prices.empty()) return 0;
//         int max_profit = INT_MIN;
//         int min_price = INT_MAX;
//         for (int i = 0; i < prices.size(); i++)
//         {
//             min_price = std::min(min_price, prices[i]);
//             max_profit = std::max(max_profit, prices[i] - min_price);
//         }
//         return max_profit;
//     }
// };

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
        // 测试用例1: 示例1
        runTest("示例1", [this]() {
            vector<int> prices = {7, 1, 5, 3, 6, 4};
            return solution.maxProfit(prices) == 5;
        });

        // 测试用例2: 示例2
        runTest("示例2", [this]() {
            vector<int> prices = {7, 6, 4, 3, 1};
            return solution.maxProfit(prices) == 0;
        });

        // 测试用例3: 价格数组为空
        runTest("空数组", [this]() {
            vector<int> prices = {};
            return solution.maxProfit(prices) == 0;  // 预期返回0
        });

        // 测试用例4: 只有一个价格
        runTest("单个价格", [this]() {
            vector<int> prices = {5};
            return solution.maxProfit(prices) == 0;  // 预期返回0
        });

        // 测试用例5: 所有价格相同
        runTest("所有价格相同", [this]() {
            vector<int> prices = {3, 3, 3, 3};
            return solution.maxProfit(prices) == 0;  // 预期返回0
        });

        // 测试用例6: 价格逐渐上升
        runTest("价格逐渐上升", [this]() {
            vector<int> prices = {1, 2, 3, 4, 5};
            return solution.maxProfit(prices) == 4;  // 预期返回5 - 1 = 4
        });

        // 测试用例7: 价格逐渐下降
        runTest("价格逐渐下降", [this]() {
            vector<int> prices = {5, 4, 3, 2, 1};
            return solution.maxProfit(prices) == 0;  // 预期返回0
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