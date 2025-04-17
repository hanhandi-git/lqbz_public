#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <functional>
#include <iterator>

class Solution {
public:
    // 1. 最大子数组和
    int maxSubArray(std::vector<int>& nums) {
        int maxSum = nums[0];
        int currentSum = nums[0];
        
        for (int i = 1; i < nums.size(); i++) {
            currentSum = std::max(nums[i], currentSum + nums[i]);
            maxSum = std::max(maxSum, currentSum);
        }
        
        return maxSum;
    }

    // 2. 最大子矩阵和
    int maxSubMatrix(std::vector<std::vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return 0;
        
        int rows = matrix.size();
        int cols = matrix[0].size();
        int maxSum = INT_MIN;
        // 最外层循环 left 遍历所有可能的左边界列。
        // 中间层循环 right 遍历从左边界到右边界的所有可能列组合。
        // 最内层循环 i 遍历每一行，将当前列（right）的值加到 rowSum 中。
        // 固定左右边界（列），这样就将二维问题转化为一维问题。
        // 对于每一对左右边界，我们计算每一行在这个范围内的和，存储在 rowSum 中。
        // 然后对 rowSum 使用一维的最大子数组和算法
        for (int left = 0; left < cols; left++) {
            std::vector<int> rowSum(rows, 0);
            for (int right = left; right < cols; right++) {
                for (int i = 0; i < rows; i++) {
                    rowSum[i] += matrix[i][right];
                }
                maxSum = std::max(maxSum, maxSubArray(rowSum));
            }
        }
        
        return maxSum;
    }

    // 3. 买卖股票的最佳时机
    int maxProfit(std::vector<int>& prices) {
        if (prices.empty()) return 0;
        int minPrice = prices[0];
        int maxProfit = 0;
        for (auto price : prices)
        {
            minPrice = std::min(minPrice, price);
            maxProfit = std::max(maxProfit, price - minPrice);
        }
        return maxProfit;
    }

    // 4. 环形子数组的最大和
    int maxSubarraySumCircular(std::vector<int>& nums) {
        int totalSum = 0;
        int maxSum = nums[0];
        int minSum = nums[0];
        int currentMax = 0;
        int currentMin = 0;
        
        for (int num : nums) {
            totalSum += num;
            currentMax = std::max(num, currentMax + num);
            maxSum = std::max(maxSum, currentMax);
            currentMin = std::min(num, currentMin + num);
            minSum = std::min(minSum, currentMin);
        }
        
        return maxSum > 0 ? std::max(maxSum, totalSum - minSum) : maxSum;
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
        runTest("最大子数组和", [this]() {
            std::vector<int> nums = {-2,1,-3,4,-1,2,1,-5,4};
            return solution.maxSubArray(nums) == 6;
        });

        runTest("最大子矩阵和", [this]() {
            std::vector<std::vector<int>> matrix = {
                {1,0,1},
                {0,-2,3}
            };
            return solution.maxSubMatrix(matrix) == 4;
        });

        runTest("买卖股票的最佳时机", [this]() {
            std::vector<int> prices = {7,1,5,3,6,4};
            return solution.maxProfit(prices) == 5;
        });

        runTest("环形子数组的最大和", [this]() {
            std::vector<int> nums = {1,-2,3,-2};
            return solution.maxSubarraySumCircular(nums) == 3;
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
