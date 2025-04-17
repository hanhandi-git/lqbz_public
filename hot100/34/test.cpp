#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;

class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        std::vector<int> result = {-1, -1};
        int left = 0, right = nums.size() - 1;
        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                result[0] = mid;
                right = mid - 1;
            } else if (nums[mid] > target) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        } 
        left = 0;
        right = nums.size()-1;
        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) {
                result[1] = mid;
                left = mid + 1;
            } else if (nums[mid] > target) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }

        return result; // 返回第一个和最后一个位置
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
        runTest("测试用例 1: nums = [5,7,7,8,8,10], target = 8", [this]() {
            vector<int> nums = {5, 7, 7, 8, 8, 10};
            vector<int> expected = {3, 4};
            return solution.searchRange(nums, 8) == expected;
        });

        runTest("测试用例 2: nums = [5,7,7,8,8,10], target = 6", [this]() {
            vector<int> nums = {5, 7, 7, 8, 8, 10};
            vector<int> expected = {-1, -1};
            return solution.searchRange(nums, 6) == expected;
        });

        runTest("测试用例 3: nums = [], target = 0", [this]() {
            vector<int> nums = {};
            vector<int> expected = {-1, -1};
            return solution.searchRange(nums, 0) == expected; // 插入位置为[-1, -1]
        });

        runTest("测试用例 4: nums = [1, 2, 3, 4, 5], target = 3", [this]() {
            vector<int> nums = {1, 2, 3, 4, 5};
            vector<int> expected = {2, 2};
            return solution.searchRange(nums, 3) == expected; // 3的第一个和最后一个位置都是2
        });

        runTest("测试用例 5: nums = [1, 1, 1, 1, 1], target = 1", [this]() {
            vector<int> nums = {1, 1, 1, 1, 1};
            vector<int> expected = {0, 4};
            return solution.searchRange(nums, 1) == expected; // 1的第一个位置是0，最后一个位置是4
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