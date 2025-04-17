#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>
#include <climits>
class Solution {
public:
    int search(std::vector<int>& nums, int target) {
        int left = 0, right = nums.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2; // 防止溢出

            if (nums[mid] == target) {
                return mid; // 找到目标值
            } else if (nums[mid] < target) {
                left = mid + 1; // 目标值在右半部分
            } else {
                right = mid - 1; // 目标值在左半部分
            }
        }

        return -1; // 未找到目标值
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

        // 新增二分查找测试用例
        runTest("二分查找 - 示例1", [this]() {
            std::vector<int> nums = {-1, 0, 3, 5, 9, 12};
            return solution.search(nums, 9) == 4; // 目标值9的索引
        });

        runTest("二分查找 - 示例2", [this]() {
            std::vector<int> nums = {-1, 0, 3, 5, 9, 12};
            return solution.search(nums, 2) == -1; // 目标值2不存在
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