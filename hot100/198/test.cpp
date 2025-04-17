#include <iostream>
#include <vector>
#include <functional>
#include <string>

class Solution {
public:
    int rob(std::vector<int>& nums) {
       if (nums.empty()) return 0;
       if (nums.size() == 1) return nums[0];
       if (nums.size() == 2) return std::max(nums[0], nums[1]);
       std::vector<int> dp(nums.size());
       dp[0] = nums[0];
       dp[1] = std::max(nums[0], nums[1]);
       for (int i = 2; i < nums.size(); i++)
       {
            dp[i] = std::max(dp[i-1], dp[i-2] + nums[i]);
       }
       return dp[nums.size()-1];
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
        runTest("基本测试 - 示例1", [this]() {
            std::vector<int> nums = {1, 2, 3, 1};
            return solution.rob(nums) == 4;  // 选择1和3号房屋
        });

        runTest("基本测试 - 示例2", [this]() {
            std::vector<int> nums = {2, 7, 9, 3, 1};
            return solution.rob(nums) == 12;  // 选择2和4号房屋
        });

        // 边界测试
        runTest("边界测试 - 空数组", [this]() {
            std::vector<int> nums;
            return solution.rob(nums) == 0;
        });

        runTest("边界测试 - 单个元素", [this]() {
            std::vector<int> nums = {5};
            return solution.rob(nums) == 5;
        });

        runTest("边界测试 - 两个元素", [this]() {
            std::vector<int> nums = {3, 5};
            return solution.rob(nums) == 5;
        });

        // 特殊测试
        runTest("特殊测试 - 所有元素相同", [this]() {
            std::vector<int> nums = {5, 5, 5, 5, 5};
            return solution.rob(nums) == 15;  // 选择0,2,4号房屋
        });

        runTest("特殊测试 - 交替值", [this]() {
            std::vector<int> nums = {2, 10, 2, 10, 2};
            return solution.rob(nums) == 20;  // 选择1,3号房屋
        });

        runTest("特殊测试 - 递增序列", [this]() {
            std::vector<int> nums = {1, 2, 3, 4, 5};
            return solution.rob(nums) == 9;  // 选择1,3,5号房屋
        });

        runTest("特殊测试 - 递减序列", [this]() {
            std::vector<int> nums = {5, 4, 3, 2, 1};
            return solution.rob(nums) == 9;  // 选择0,2,4号房屋
        });

        runTest("特殊测试 - 较大间隔", [this]() {
            std::vector<int> nums = {1, 1, 1, 100, 1};
            return solution.rob(nums) == 101;  // 选择0,3号房屋
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