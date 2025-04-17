#include <iostream>
#include <vector>
#include <functional>
#include <numeric>
using namespace std;

// class Solution {
// public:
//     int findTargetSumWays(vector<int>& nums, int target) {
//         int sum = std::accumulate(nums.begin(), nums.end(), 0);
        
//         // 如果target绝对值大于sum，或者(target + sum)为奇数，则无解
//         if (abs(target) > sum || (target + sum) % 2 != 0) {
//             return 0;
//         }
        
//         int positiveSum = (target + sum) / 2;
        
//         // 动态规划求解子集和问题
//         vector<int> dp(positiveSum + 1, 0);
//         dp[0] = 1;  // 空集和为0的方案数为1
        
//         for (int num : nums) {
//             for (int j = positiveSum; j >= num; j--) {
//                 dp[j] += dp[j - num];
//             }
//         }
        
//         return dp[positiveSum];
//     }
// };

class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        return backtrack(nums, target, 0, 0);
    }
    
private:
    int backtrack(vector<int>& nums, int target, int index, int currentSum) {
        // 基本情况：已处理完所有数字
        if (index == nums.size()) {
            // 如果当前总和等于目标值，则找到一种有效方案
            return currentSum == target ? 1 : 0;
        }
        
        // 尝试添加'+'号
        int addPlus = backtrack(nums, target, index + 1, currentSum + nums[index]);
        
        // 尝试添加'-'号
        int addMinus = backtrack(nums, target, index + 1, currentSum - nums[index]);
        
        // 返回两种选择的总方案数
        return addPlus + addMinus;
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
        runTest("测试用例1 - 基本情况", [this]() {
            vector<int> nums = {1, 1, 1, 1, 1};
            int target = 3;
            return (solution.findTargetSumWays(nums, target) == 5);
        });

        runTest("测试用例2 - 无法达成目标", [this]() {
            vector<int> nums = {1, 2, 3};
            int target = 7;
            return (solution.findTargetSumWays(nums, target) == 0);
        });

        runTest("测试用例3 - 单元素数组", [this]() {
            vector<int> nums = {1};
            int target = 1;
            return (solution.findTargetSumWays(nums, target) == 1);
        });

        runTest("测试用例4 - 包含0的情况", [this]() {
            vector<int> nums = {0, 0, 0, 0, 0};
            int target = 0;
            return (solution.findTargetSumWays(nums, target) == 32); // 2^5 = 32种方式
        });

        runTest("测试用例5 - 负目标值", [this]() {
            vector<int> nums = {1, 2, 1};
            int target = -2;
            return (solution.findTargetSumWays(nums, target) == 2); //  +1-2-1=-2 和 -1-2+1=-2
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