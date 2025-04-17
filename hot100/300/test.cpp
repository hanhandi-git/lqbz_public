#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
using namespace std;

// 动态规划解法
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) 
    {
        int n = nums.size();
        if (n == 0) return 0;
        std::vector<int> dp(n, 1);
        int maxLen = 1;
        for (int i = 1; i < n; i++)
        {
            for (int j = 0; j < i; j++)
            {
                if (nums[i] > nums[j])
                {
                    dp[i] = std::max(dp[i], dp[j] + 1);
                    maxLen = std::max(maxLen, dp[i]);
                }
            }
        }
        return maxLen;
    }
};
// // 动态规划解法
// class Solution {
// public:
//     int lengthOfLIS(vector<int>& nums) 
//     {
//         int n = nums.size();
//         if (n == 0) return 0;
        
//         vector<int> dp(n, 1); // 初始化dp数组，每个元素至少是长度为1的子序列
        
//         for (int i = 1; i < n; i++) {
//             for (int j = 0; j < i; j++) {
//                 if (nums[i] > nums[j]) {
//                     dp[i] = max(dp[i], dp[j] + 1);
//                 }
//             }
//         }
        
//         return *max_element(dp.begin(), dp.end());
//     }
// };

// 二分查找优化解法
class OptimizedSolution 
{
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> tails;
        
        for (int num : nums) {
            auto it = lower_bound(tails.begin(), tails.end(), num);
            if (it == tails.end()) {
                tails.push_back(num);
            } else {
                *it = num;
            }
        }
        
        return tails.size();
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
        // 最长上升子序列测试 - 动态规划解法
        runTest("动态规划解法测试1", [this]() {
            Solution solution;
            vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};
            return solution.lengthOfLIS(nums) == 4;
        });

        runTest("动态规划解法测试2", [this]() {
            Solution solution;
            vector<int> nums = {0, 1, 0, 3, 2, 3};
            return solution.lengthOfLIS(nums) == 4;
        });

        runTest("动态规划解法测试3", [this]() {
            Solution solution;
            vector<int> nums = {7, 7, 7, 7, 7, 7, 7};
            return solution.lengthOfLIS(nums) == 1;
        });

        // 最长上升子序列测试 - 二分查找优化解法
        runTest("二分查找优化解法测试1", [this]() {
            OptimizedSolution solution;
            vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};
            return solution.lengthOfLIS(nums) == 4;
        });

        runTest("二分查找优化解法测试2", [this]() {
            OptimizedSolution solution;
            vector<int> nums = {0, 1, 0, 3, 2, 3};
            return solution.lengthOfLIS(nums) == 4;
        });

        runTest("二分查找优化解法测试3", [this]() {
            OptimizedSolution solution;
            vector<int> nums = {7, 7, 7, 7, 7, 7, 7};
            return solution.lengthOfLIS(nums) == 1;
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