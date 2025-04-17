#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>
#include <climits>

class Solution {
public:
    /**
     * @brief 寻找两个正序数组的中位数
     * 
     * @param nums1 第一个有序数组
     * @param nums2 第二个有序数组
     * @return double 返回中位数
     * @note 使用二分查找方法，时间复杂度O(log(min(m,n)))
     */
    double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
        if (nums1.empty() && nums2.empty()) return 0.0;
        if (nums1.size() > nums2.size()) swap(nums1, nums2);
        if (nums1.empty()) {
            if (nums2.size() % 2 == 0) {
                return static_cast<double>(nums2[(nums2.size()-1) / 2] + nums2[(nums2.size()) / 2]) / 2.0;
            } else {
                return nums2[(nums2.size()) / 2];
            }
        }
        int n = nums1.size();
        int m = nums2.size();
        int left = 0, right = n;
        while (left <= right)
        {
            int i = left + (right - left) / 2;
            int j = (m + n + 1) / 2 - i;  // +1 统一奇数和偶数
            int leftMax1 = (i == 0) ? INT_MIN : nums1[i-1];
            int leftMax2 = (j == 0) ? INT_MIN : nums2[j-1];
            int rightMin1 = (i == n) ? INT_MAX : nums1[i];
            int rightMin2 = (j == m) ? INT_MAX : nums2[j];
            if (leftMax1 <= rightMin2 && leftMax2 <= rightMin1)
            {
                if ((m+n) % 2 == 0) {
                    return (std::max(leftMax1, leftMax2) + std::min(rightMin1, rightMin2)) / 2.0;
                } else {
                    return std::max(leftMax1, leftMax2);
                }
            } else if (leftMax1 > rightMin2) {
                right = i - 1;
            } else {
                left = i + 1;
            }
        }
        return 0.0;
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
        // 基本测试用例
        runTest("示例1 - 奇数长度", [this]() {
            std::vector<int> nums1 = {1, 3};
            std::vector<int> nums2 = {2};
            return solution.findMedianSortedArrays(nums1, nums2) == 2.0;
        });

        runTest("示例2 - 偶数长度", [this]() {
            std::vector<int> nums1 = {1, 2};
            std::vector<int> nums2 = {3, 4};
            return solution.findMedianSortedArrays(nums1, nums2) == 2.5;
        });

        // 边界情况测试
        runTest("边界1 - 空数组", [this]() {
            std::vector<int> nums1 = {};
            std::vector<int> nums2 = {1};
            return solution.findMedianSortedArrays(nums1, nums2) == 1.0;
        });

        runTest("边界2 - 两个空数组", [this]() {
            std::vector<int> nums1 = {};
            std::vector<int> nums2 = {};
            return solution.findMedianSortedArrays(nums1, nums2) == 0.0;
        });

        // 特殊情况测试
        runTest("特殊1 - 长度差异大", [this]() {
            std::vector<int> nums1 = {1};
            std::vector<int> nums2 = {2,3,4,5,6};
            return solution.findMedianSortedArrays(nums1, nums2) == 3.5;
        });

        runTest("特殊2 - 重复元素", [this]() {
            std::vector<int> nums1 = {1,2,2};
            std::vector<int> nums2 = {2,3,4};
            return solution.findMedianSortedArrays(nums1, nums2) == 2.0;
        });

        runTest("特殊3 - 相同元素", [this]() {
            std::vector<int> nums1 = {1,1,1};
            std::vector<int> nums2 = {1,1,1};
            return solution.findMedianSortedArrays(nums1, nums2) == 1.0;
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