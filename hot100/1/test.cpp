#include <vector>
#include <iostream>
#include <functional>
#include <unordered_map>

class Solution {
public:
    /**
     * @brief 在数组中找出和为目标值的两个整数的下标
     * 
     * @param nums 输入的整数数组
     * @param target 目标值
     * @return vector<int> 返回两个整数的下标
     */
    std::vector<int> twoSum(std::vector<int>& nums, int target) {
        std::unordered_map<int, int> mapping;   // key -> index
        for (int i = 0; i < nums.size(); i++)
        {
            if (mapping.find(target - nums[i]) != mapping.end()) {
                return {mapping[target - nums[i]], i};
            }
            mapping[nums[i]] = i;
        }
        return {}; // 没有找到答案
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
        // 示例1测试
        runTest("示例1 - 基本测试", [this]() {
            std::vector<int> nums = {2,7,11,15};
            int target = 9;
            std::vector<int> expected = {0,1};
            std::vector<int> result = solution.twoSum(nums, target);
            return result == expected;
        });

        // 示例2测试
        runTest("示例2 - 三个数字", [this]() {
            std::vector<int> nums = {3,2,4};
            int target = 6;
            std::vector<int> expected = {1,2};
            std::vector<int> result = solution.twoSum(nums, target);
            return result == expected;
        });

        // 示例3测试
        runTest("示例3 - 重复数字", [this]() {
            std::vector<int> nums = {3,3};
            int target = 6;
            std::vector<int> expected = {0,1};
            std::vector<int> result = solution.twoSum(nums, target);
            return result == expected;
        });

        // 特殊测试1 - 负数
        runTest("特殊1 - 负数", [this]() {
            std::vector<int> nums = {-1,-2,-3,-4,-5};
            int target = -8;
            std::vector<int> expected = {2,4};
            std::vector<int> result = solution.twoSum(nums, target);
            return result == expected;
        });

        // 特殊测试2 - 较大数字
        runTest("特殊2 - 较大数字", [this]() {
            std::vector<int> nums = {1000000000,1000000000};
            int target = 2000000000;
            std::vector<int> expected = {0,1};
            std::vector<int> result = solution.twoSum(nums, target);
            return result == expected;
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