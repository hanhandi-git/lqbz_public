
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

class Solution {
public:
    int majorityElement(vector<int>& nums) {
        if (nums.size() < 0) return 0;
        int candidate = nums[0];
        int cnt = 1;
        for (int i = 1; i < nums.size(); i++) {
            if (cnt == 0) {
                candidate = nums[i];
            }
            if (candidate == nums[i]) {
                cnt++;
            } else {
                cnt--;
            }
        }
        return candidate;
    }
};

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        bool result = test();
        if (result) {
            std::cout << "通过: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "失败: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        // 基本测试 - 奇数长度
        runTest("奇数长度数组测试", [this]() {
            vector<int> nums = {3,2,3};
            return solution.majorityElement(nums) == 3;
        });

        // 基本测试 - 偶数长度
        runTest("偶数长度数组测试", [this]() {
            vector<int> nums = {2,2,1,1,1,2,2};
            return solution.majorityElement(nums) == 2;
        });

        // 单元素测试
        runTest("单元素数组测试", [this]() {
            vector<int> nums = {1};
            return solution.majorityElement(nums) == 1;
        });

        // 所有元素相同
        runTest("所有元素相同测试", [this]() {
            vector<int> nums = {5,5,5,5,5};
            return solution.majorityElement(nums) == 5;
        });

        // 多数元素在开头
        runTest("多数元素在开头测试", [this]() {
            vector<int> nums = {3,3,3,1,2};
            return solution.majorityElement(nums) == 3;
        });

        // 多数元素在结尾
        runTest("多数元素在结尾测试", [this]() {
            vector<int> nums = {1,2,4,4,4};
            return solution.majorityElement(nums) == 4;
        });

        // 多数元素交替出现
        runTest("多数元素交替出现测试", [this]() {
            vector<int> nums = {1,2,1,2,1};
            return solution.majorityElement(nums) == 1;
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