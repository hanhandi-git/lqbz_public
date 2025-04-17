#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;

class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;
        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                return mid;
            } else if (nums[mid] > target) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        return left;
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
        runTest("测试用例 1: nums = [1,3,5,6], target = 5", [this]() {
            vector<int> nums = {1, 3, 5, 6};
            return solution.searchInsert(nums, 5) == 2;
        });

        runTest("测试用例 2: nums = [1,3,5,6], target = 2", [this]() {
            vector<int> nums = {1, 3, 5, 6};
            return solution.searchInsert(nums, 2) == 1;
        });

        runTest("测试用例 3: nums = [1,3,5,6], target = 7", [this]() {
            vector<int> nums = {1, 3, 5, 6};
            return solution.searchInsert(nums, 7) == 4;
        });

        runTest("测试用例 4: nums = [1,3,5,6], target = 0", [this]() {
            vector<int> nums = {1, 3, 5, 6};
            return solution.searchInsert(nums, 0) == 0;
        });

        runTest("测试用例 5: nums = [], target = 1", [this]() {
            vector<int> nums = {};
            return solution.searchInsert(nums, 1) == 0; // 插入位置为0
        });

        runTest("测试用例 6: nums = [1], target = 0", [this]() {
            vector<int> nums = {1};
            return solution.searchInsert(nums, 0) == 0; // 插入位置为0
        });

        runTest("测试用例 7: nums = [1], target = 2", [this]() {
            vector<int> nums = {1};
            return solution.searchInsert(nums, 2) == 1; // 插入位置为1
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