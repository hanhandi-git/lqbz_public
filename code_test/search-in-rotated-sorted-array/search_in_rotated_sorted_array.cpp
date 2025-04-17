#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>  

class Solution {
public:
    int search(std::vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;
        while (left <= right)
        {
            // int mid = (right + left) / 2;
            int mid = left + ((right - left) >> 1);
            // std::cout << "left:" << left << ",mid:" << mid << ",right" << right << std::endl;
            if (nums[mid] == target) return mid;
            // 根据nums[mid] [right] [left]来确定左半有序还是右半有序
            if (nums[left] <= nums[mid])    // 左半有序
            {
                if (nums[left] <= target && nums[mid] > target) {  // target 在左半
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }   
            else {  // 右半有序
                if (nums[mid] < target && nums[right] >= target)   // target 在右半
                {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        return -1;
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
            std::cout << "PASS: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "FAIL: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        runTest("Example1", [this]() {
            std::vector<int> nums = {4,5,6,7,0,1,2};
            return solution.search(nums, 0) == 4;
        });

        runTest("Example2", [this]() {
            std::vector<int> nums = {4,5,6,7,0,1,2};
            return solution.search(nums, 3) == -1;
        });

        runTest("Example3", [this]() {
            std::vector<int> nums = {1};
            return solution.search(nums, 0) == -1;
        });

        runTest("EmptyInput", [this]() {
            std::vector<int> nums = {};
            return solution.search(nums, 5) == -1;
        });

        runTest("TargetAtStart", [this]() {
            std::vector<int> nums = {5,1,2,3,4};
            return solution.search(nums, 5) == 0;
        });

        runTest("TargetAtEnd", [this]() {
            std::vector<int> nums = {3,4,5,1,2};
            return solution.search(nums, 2) == 4;
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
