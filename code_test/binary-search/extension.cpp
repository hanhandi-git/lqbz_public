// extension.cpp
#include <iostream>
#include <vector>
#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>
#include <climits>


// 扩展题目 1: 寻找旋转排序数组中的最小值
class Solution {
public:
    int findMin(std::vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            if (nums[mid] > nums[right]) {
                left = mid + 1;
            } else if (nums[mid] < nums[right]) {
                right = mid - 1;
            } else {
                right--;
            }
        }
        return nums[left];
    }

    int findPeakElement(std::vector<int>& nums) {
        int left = 0, right = nums.size() - 1;

        while (left < right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] < nums[mid + 1]) {
                left = mid + 1; // 峰值在右半部分
            } else {
                right = mid; // 峰值在左半部分或mid
            }
        }

        return left; // 返回峰值元素的索引
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
        // 新增寻找旋转排序数组中的最小值测试用例
        runTest("寻找最小值 - 示例1", [this]() {
            std::vector<int> nums = {3, 4, 5, 1, 2};
            return solution.findMin(nums) == 1; // 最小值为1
        });

        runTest("寻找最小值 - 示例2", [this]() {
            std::vector<int> nums = {4, 5, 6, 7, 0, 1, 2};
            return solution.findMin(nums) == 0; // 最小值为0
        });

        runTest("寻找最小值 - 示例3", [this]() {
            std::vector<int> nums = {1, 2, 3, 4, 5};
            return solution.findMin(nums) == 1; // 最小值为1
        });

        runTest("寻找最小值 - 示例4", [this]() {
            std::vector<int> nums = {2, 2, 2, 0, 1, 2};
            return solution.findMin(nums) == 0; // 最小值为0
        });

        runTest("寻找最小值 - 示例5", [this]() {
            std::vector<int> nums = {1};
            return solution.findMin(nums) == 1; // 最小值为1
        });

        runTest("寻找最小值 - 示例6", [this]() {
            std::vector<int> nums = {2, 2, 2, 2, 2};
            return solution.findMin(nums) == 2; // 最小值为2
        });

        // 新增寻找峰值元素测试用例
        runTest("寻找峰值元素 - 示例1", [this]() {
            std::vector<int> nums = {1, 2, 3, 1};
            return solution.findPeakElement(nums) == 2; // 峰值元素索引为2
        });

        runTest("寻找峰值元素 - 示例2", [this]() {
            std::vector<int> nums = {1, 2, 1, 3, 5, 6, 4};
            return solution.findPeakElement(nums) == 1 || solution.findPeakElement(nums) == 5; // 峰值元素索引为1或5
        });

        runTest("寻找峰值元素 - 示例3", [this]() {
            std::vector<int> nums = {1, 2, 3, 4, 5};
            return solution.findPeakElement(nums) == 4; // 峰值元素索引为4
        });

        runTest("寻找峰值元素 - 示例4", [this]() {
            std::vector<int> nums = {5, 4, 3, 2, 1};
            return solution.findPeakElement(nums) == 0; // 峰值元素索引为0
        });

        runTest("寻找峰值元素 - 示例5", [this]() {
            std::vector<int> nums = {1};
            return solution.findPeakElement(nums) == 0; // 峰值元素索引为0
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
