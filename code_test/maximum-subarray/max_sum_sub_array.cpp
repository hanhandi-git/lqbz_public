#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>


class Solution {
public:
    int maxSubArray(std::vector<int>& nums) {
        if (nums.empty()) return 0;
        int maxSum = nums[0];
        int res = maxSum;
        for (int i = 1; i < nums.size(); i++)
        {
            maxSum = std::max(maxSum + nums[i], nums[i]);
            res = std::max(res, maxSum);
        }
        return res;
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
        runTest("测试用例1", [this]() {
            std::vector<int> nums = {-2,1,-3,4,-1,2,1,-5,4};
            int result = solution.maxSubArray(nums);
            return result == 6;
        });

        runTest("测试用例2", [this]() {
            std::vector<int> nums = {1};
            int result = solution.maxSubArray(nums);
            return result == 1;
        });

        runTest("测试用例3", [this]() {
            std::vector<int> nums = {5,4,-1,7,8};
            int result = solution.maxSubArray(nums);
            return result == 23;
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
