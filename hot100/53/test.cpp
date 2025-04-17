#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>

class Solution {
public:
    int maxSubArray(std::vector<int>& nums) {
        if (nums.empty()) return -1;
        int currentSum = nums[0];
        int maxSum = nums[0];
        for (int i = 1; i < nums.size(); i++)
        {
            currentSum = std::max(nums[i], nums[i] + currentSum);
            maxSum = std::max(currentSum, maxSum);
        }
        return maxSum;
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
        // 示例测试
        runTest("示例1测试", [this]() {
            std::vector<int> nums = {-2,1,-3,4,-1,2,1,-5,4};
            return solution.maxSubArray(nums) == 6;
        });

        runTest("示例2测试", [this]() {
            std::vector<int> nums = {1};
            return solution.maxSubArray(nums) == 1;
        });

        runTest("示例3测试", [this]() {
            std::vector<int> nums = {5,4,-1,7,8};
            return solution.maxSubArray(nums) == 23;
        });

        // 边界情况测试
        runTest("全负数测试", [this]() {
            std::vector<int> nums = {-2,-1,-3,-4,-1,-2,-1,-5,-4};
            return solution.maxSubArray(nums) == -1;
        });

        runTest("全正数测试", [this]() {
            std::vector<int> nums = {1,2,3,4,5};
            return solution.maxSubArray(nums) == 15;
        });

        runTest("包含零测试", [this]() {
            std::vector<int> nums = {-2,0,-1,0,2};
            return solution.maxSubArray(nums) == 2;
        });

        // 特殊情况测试
        runTest("单元素负数测试", [this]() {
            std::vector<int> nums = {-1};
            return solution.maxSubArray(nums) == -1;
        });

        runTest("连续负数后更大和测试", [this]() {
            std::vector<int> nums = {1,-2,-3,4,5,-2};
            return solution.maxSubArray(nums) == 9;
        });

        runTest("交替正负数测试", [this]() {
            std::vector<int> nums = {1,-1,1,-1,1,-1};
            return solution.maxSubArray(nums) == 1;
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