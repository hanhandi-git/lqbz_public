#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;


class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;
        while (left <= right) 
        {
            int mid = left + (right - left) / 2;
            if (nums[mid] == target) return mid;
            if (nums[left] <= nums[mid]) {
                // 左半有序
                if (target >= nums[left] && target < nums[mid]) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            } else {
                // 右半有序
                if (target > nums[mid] && target <= nums[right]) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        return -1; // 未找到目标值
    }
};


// class Solution {
// public:
//     int search(vector<int>& nums, int target) {
//         int left = 0, right = nums.size() - 1;
//         while (left <= right)
//         {
//             int mid = left + (right - left) / 2;
//             if (nums[mid] == target) {
//                 return mid; // 找到目标值，返回索引
//             }
//             if (nums[mid] >= nums[left]) {  // 左边有序
//                 if (nums[left] <= target && target < nums[mid]) {// 在左侧
//                     right = mid - 1;
//                 } else {
//                     left = mid + 1;
//                 }
//             } else {   // 右边有序
//                 if (nums[mid] < target && target <= nums[right]) {// 在右侧
//                     left = mid + 1;
//                 } else {
//                     right = mid - 1;
//                 }
//             }
//         }
//         return -1; // 未找到目标值
//     }
// };

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
        runTest("测试用例 1: nums = [4,5,6,7,0,1,2], target = 0", [this]() {
            vector<int> nums = {4, 5, 6, 7, 0, 1, 2};
            int expected = 4;
            return solution.search(nums, 0) == expected;
        });

        runTest("测试用例 2: nums = [4,5,6,7,0,1,2], target = 3", [this]() {
            vector<int> nums = {4, 5, 6, 7, 0, 1, 2};
            int expected = -1;
            return solution.search(nums, 3) == expected;
        });

        runTest("测试用例 3: nums = [1], target = 0", [this]() {
            vector<int> nums = {1};
            int expected = -1;
            return solution.search(nums, 0) == expected; // 目标值不在数组中
        });

        runTest("测试用例 4: nums = [1, 3], target = 3", [this]() {
            vector<int> nums = {1, 3};
            int expected = 1;
            return solution.search(nums, 3) == expected; // 目标值在数组中
        });

        runTest("测试用例 5: nums = [3, 1], target = 1", [this]() {
            vector<int> nums = {3, 1};
            int expected = 1;
            return solution.search(nums, 1) == expected; // 目标值在数组中
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