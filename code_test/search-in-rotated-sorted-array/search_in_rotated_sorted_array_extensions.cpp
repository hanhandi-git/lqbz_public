#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>  
#include <climits>

class Solution {
public:

    // 搜索旋转排序数组 II（允许重复元素）
    bool searchII(std::vector<int>& nums, int target) {
        int left = 0, right = nums.size() - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (nums[mid] == target) {
                return true;
            }
            
            // 处理重复元素的情况
            if (nums[left] == nums[mid]) {
                left++;
                continue;
            }
            
            // 左半部分有序
            if (nums[left] < nums[mid]) {
                if (nums[left] <= target && target < nums[mid]) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
            // 右半部分有序
            else {
                if (nums[mid] < target && target <= nums[right]) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        
        return false;
    }

    // 旋转数组
    void rotate(std::vector<int>& nums, int k) {
        int n = nums.size();
        k = k % n;
        std::vector<int> raw_nums;
        std::copy(nums.begin(), nums.end(), std::back_inserter(raw_nums));
        for (int i = 0; i < n; i++)
        {
            nums[i] = raw_nums[(i - k + n) % n];
        }
        return;
    }

    // 寻找峰值元素
    int findPeakElement(std::vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (nums[mid] > nums[mid + 1]) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        
        return left;
    }

private:

    void reverse(std::vector<int>& nums, int start, int end) {
        while (start < end) {
            std::swap(nums[start], nums[end]);
            start++;
            end--;
        }
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

    bool expectEqual(const std::vector<std::vector<int>>& expected, const std::vector<std::vector<int>>& actual) {
        if (expected.size() != actual.size()) return false;
        for (const auto& combination : expected) {
            if (std::find(actual.begin(), actual.end(), combination) == actual.end()) {
                return false;
            }
        }
        return true;
    }

public:
    void runAllTests() {

        runTest("SearchII_WithDuplicates", [this]() {
            std::vector<int> nums = {2,5,6,0,0,1,2};
            return solution.searchII(nums, 0) == true;
        });

        runTest("Rotate", [this]() {
            std::vector<int> nums = {1,2,3,4,5,6,7};
            std::vector<int> expected = {5,6,7,1,2,3,4};
            solution.rotate(nums, 3);
            return nums == expected;
        });

        runTest("FindPeakElement", [this]() {
            std::vector<int> nums = {1,2,3,1};
            return solution.findPeakElement(nums) == 2;
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
