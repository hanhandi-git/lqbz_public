#include <iostream>
#include <vector>
#include <functional>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        unordered_map<int, int> mp;
        mp[0] = 1;
        int count = 0;
        int preSum = 0;
        
        for (int i = 0; i < nums.size(); i++)
        {
            preSum += nums[i];
            if (mp.find(preSum - k) != mp.end()) {
                count += mp[preSum - k];
            } 
            mp[preSum]++;
        }
        
        return count;
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
            vector<int> nums = {1,1,1};
            int k = 2;
            return (solution.subarraySum(nums, k) == 2);
        });

        runTest("测试用例2 - 空数组", [this]() {
            vector<int> nums = {};
            int k = 1;
            return (solution.subarraySum(nums, k) == 0);
        });

        runTest("测试用例3 - 单个元素", [this]() {
            vector<int> nums = {1};
            int k = 1;
            return (solution.subarraySum(nums, k) == 1);
        });

        runTest("测试用例5 - 较大数组", [this]() {
            vector<int> nums = {1,2,3,4,5,6,7,8,9,10};
            int k = 15;
            return (solution.subarraySum(nums, k) == 3);
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