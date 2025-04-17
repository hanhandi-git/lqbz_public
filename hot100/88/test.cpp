#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>


class Solution {
public:
    void merge(std::vector<int>& nums1, int m, std::vector<int>& nums2, int n) {
        int i = m - 1;
        int j = n - 1;
        int k = n + m -1;
        while (i >= 0 && j >= 0) 
        {
            if (nums1[i] > nums2[j]) {
                nums1[k--] = nums1[i--];
            } else {
                nums1[k--] = nums2[j--];
            }
        }
        while (j >= 0) {
            nums1[k--] = nums2[j--];
        }
    }
};

// class Solution {
// public:
//     void merge(std::vector<int>& nums1, int m, std::vector<int>& nums2, int n) {
//         int i = m - 1;
//         int j = n - 1;
//         int k = m + n - 1;
        
//         while (i >= 0 && j >= 0) {
//             if (nums1[i] > nums2[j]) {
//                 nums1[k--] = nums1[i--];
//             } else {
//                 nums1[k--] = nums2[j--];
//             }
//         }
        
//         while (j >= 0) {
//             nums1[k--] = nums2[j--];
//         }
//     }
// };

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

    bool expectEqual(const std::vector<int>& expected, const std::vector<int>& actual) {
        if (expected.size() != actual.size()) return false;
        for (size_t i = 0; i < expected.size(); i++) {
            if (expected[i] != actual[i]) return false;
        }
        return true;
    }

public:
    void runAllTests() {
        runTest("基本测试", [this]() {
            std::vector<int> nums1 = {1, 2, 3, 0, 0, 0};
            std::vector<int> nums2 = {2, 5, 6};
            std::vector<int> expected = {1, 2, 2, 3, 5, 6};
            solution.merge(nums1, 3, nums2, 3);
            return expectEqual(expected, nums1);
        });

        runTest("nums2为空", [this]() {
            std::vector<int> nums1 = {1, 2, 3};
            std::vector<int> nums2 = {};
            std::vector<int> expected = {1, 2, 3};
            solution.merge(nums1, 3, nums2, 0);
            return expectEqual(expected, nums1);
        });

        runTest("nums1为空", [this]() {
            std::vector<int> nums1 = {0, 0, 0};
            std::vector<int> nums2 = {1, 2, 3};
            std::vector<int> expected = {1, 2, 3};
            solution.merge(nums1, 0, nums2, 3);
            return expectEqual(expected, nums1);
        });

        runTest("包含相同元素", [this]() {
            std::vector<int> nums1 = {1, 2, 2, 0, 0, 0};
            std::vector<int> nums2 = {1, 2, 3};
            std::vector<int> expected = {1, 1, 2, 2, 2, 3};
            solution.merge(nums1, 3, nums2, 3);
            return expectEqual(expected, nums1);
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