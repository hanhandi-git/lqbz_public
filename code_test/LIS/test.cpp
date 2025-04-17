#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>  

class Solution {
public:
    // 方法一：动态规划 O(n²)
    int lengthOfLIS(std::vector<int>& nums) {
        int maxLen = 1;
        std::vector<int> dp(nums.size(), 1);
        for (int i = 1; i < nums.size(); i++) {
            for (int j = 0; j < i; j++) {
                if (nums[i] > nums[j]) {
                    dp[i] = std::max(dp[i], dp[j]+1);
                }
            }
            maxLen = std::max(maxLen, dp[i]);
        }
        // // 打印dp
        // for (int i = 0; i < dp.size(); i++)
        // {
        //     std::cout << dp[i] << " ";
        // }
        return maxLen;
    }
    
    // 方法二：贪心 + 二分查找 O(nlogn)
    int lengthOfLIS_binary(std::vector<int>& nums) {
        std::vector<int> tails;
        for (int i = 0; i < nums.size(); i++)
        {
            auto iter = std::lower_bound(tails.begin(), tails.end(), nums[i]);
            if (iter == tails.end())
            {
                tails.push_back(nums[i]);
            }
            else
            {
                *iter = nums[i];
            }
        }
        // 打印tails
        // for (int i = 0; i < tails.size(); i++)
        // {
        //     std::cout << tails[i] << " ";
        // }
        return tails.size();
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
        // 测试动态规划方法
        runTest("示例1 - DP", [this]() {
            std::vector<int> nums = {10,9,2,5,3,7,101,18};
            return solution.lengthOfLIS(nums) == 4;
        });

        runTest("示例2 - DP", [this]() {
            std::vector<int> nums = {0,1,0,3,2,3};
            return solution.lengthOfLIS(nums) == 4;
        });

        runTest("示例3 - DP", [this]() {
            std::vector<int> nums = {7,7,7,7,7,7,7};
            return solution.lengthOfLIS(nums) == 1;
        });

        // 测试二分查找方法
        runTest("示例1 - Binary", [this]() {
            std::vector<int> nums = {10,9,2,5,3,7,101,18};
            return solution.lengthOfLIS_binary(nums) == 4;
        });

        runTest("示例2 - Binary", [this]() {
            std::vector<int> nums = {0,1,0,3,2,3};
            return solution.lengthOfLIS_binary(nums) == 4;
        });

        runTest("示例3 - Binary", [this]() {
            std::vector<int> nums = {7,7,7,7,7,7,7};
            return solution.lengthOfLIS_binary(nums) == 1;
        });

        // 测试边界情况
        runTest("单元素数组", [this]() {
            std::vector<int> nums = {1};
            return solution.lengthOfLIS(nums) == 1 && 
                   solution.lengthOfLIS_binary(nums) == 1;
        });

        runTest("严格递增数组", [this]() {
            std::vector<int> nums = {1,2,3,4,5};
            return solution.lengthOfLIS(nums) == 5 && 
                   solution.lengthOfLIS_binary(nums) == 5;
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
