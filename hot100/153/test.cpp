#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;

class Solution {
public:
    int findMin(vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        while (left < right)
        {
            int mid = left + (right - left) / 2;
            if (nums[mid] < nums[right]) { // 说明min在左侧 说明右侧递增
                right = mid;
            } else {    // 说明min在右侧 且不为mid
                left = mid + 1;
            }
        }
        return nums[left];
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
        runTest("测试用例 1: nums = [3,4,5,1,2]", [this]() {
            vector<int> nums = {3, 4, 5, 1, 2};
            int expected = 1;
            return solution.findMin(nums) == expected; // 最小值为 1
        });

        runTest("测试用例 2: nums = [4,5,6,7,0,1,2]", [this]() {
            vector<int> nums = {4, 5, 6, 7, 0, 1, 2};
            int expected = 0;
            return solution.findMin(nums) == expected; // 最小值为 0
        });

        runTest("测试用例 3: nums = [1]", [this]() {
            vector<int> nums = {1};
            int expected = 1;
            return solution.findMin(nums) == expected; // 最小值为 1
        });

        runTest("测试用例 4: nums = [1, 3]", [this]() {
            vector<int> nums = {1, 3};
            int expected = 1;
            return solution.findMin(nums) == expected; // 最小值为 1
        });

        runTest("测试用例 5: nums = [3, 1]", [this]() {
            vector<int> nums = {3, 1};
            int expected = 1;
            return solution.findMin(nums) == expected; // 最小值为 1
        });

        runTest("测试用例 6: nums = [2, 2, 2, 0, 1, 2]", [this]() {
            vector<int> nums = {2, 2, 2, 0, 1, 2};
            int expected = 0;
            return solution.findMin(nums) == expected; // 最小值为 0
        });

        runTest("测试用例 7: nums = [1, 2, 3, 4, 5]", [this]() {
            vector<int> nums = {1, 2, 3, 4, 5};
            int expected = 1;
            return solution.findMin(nums) == expected; // 最小值为 1
        });

        runTest("测试用例 8: nums = [5, 1, 2, 3, 4]", [this]() {
            vector<int> nums = {5, 1, 2, 3, 4};
            int expected = 1;
            return solution.findMin(nums) == expected; // 最小值为 1
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