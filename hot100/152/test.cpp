#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

// 实现Solution类
class Solution {
public:
    int maxProduct(const std::vector<int>& nums) {
        if (nums.empty()) return 0;
        int mp = nums[0];
        std::vector<int> max_dp(nums.size(), 0);
        std::vector<int> min_dp(nums.size(), 0);
        max_dp[0] = min_dp[0] = nums[0];
        for (int i = 1; i < nums.size(); i++) {
            max_dp[i] = std::max({nums[i], max_dp[i-1] * nums[i], min_dp[i-1] * nums[i]});
            min_dp[i] = std::min({nums[i], max_dp[i-1] * nums[i], min_dp[i-1] * nums[i]});
            mp = std::max(mp, max_dp[i]);
        }
        return mp;
    }
};

// 测试框架
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;
    
    void runTest(const std::string& testName, 
                const std::vector<int>& nums, 
                int expected) {
        total++;
        int result = solution.maxProduct(nums);
        if (result == expected) {
            std::cout << "通过: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "失败: " << testName 
                      << "\n\t预期: " << expected 
                      << ", 实际: " << result << std::endl;
        }
    }

public:
    void runAllTests() {
        // 测试示例1
        runTest("示例1: [2,3,-2,4]", 
                {2,3,-2,4}, 
                6);
        
        // 测试示例2
        runTest("示例2: [-2,0,-1]", 
                {-2,0,-1}, 
                0);
        
        // 测试单个元素
        runTest("单个元素: [5]", 
                {5}, 
                5);
        
        // 测试全负数
        runTest("全负数: [-2,-3,-4]", 
                {-2,-3,-4}, 
                12);
        
        // 测试正负交替
        runTest("正负交替: [1,-2,3,-4,5]", 
                {1,-2,3,-4,5}, 
                120);
        
        
        // 测试边界情况
        runTest("最小整数", 
                {-2147483648}, 
                -2147483648);
        
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