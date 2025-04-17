#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

class Solution {
public:
    // 1. 快速排序
    std::vector<int> sortArray(std::vector<int>& nums) {
        quickSort(nums, 0, nums.size() - 1);
        return nums;
    }
    
    // 2. 第K个最大元素
    int findKthLargest(std::vector<int>& nums, int k) {
        return quickSelect(nums, 0, nums.size() - 1, nums.size() - k);
    }

    // 3. 颜色分类
    void sortColors(std::vector<int>& nums) {
        // 在任何时刻，[0, left) 区间内的元素都是 0
        // (right, nums.size()-1] 区间内的元素都是 2
        // [left, i) 区间内的元素都是 1
        // 通过这种方式，算法可以在一次遍历中将所有的 0 移到左边，2 移到右边，而 1 自然就留在了中间，从而完成颜色的排序。这是一个非常高效的原地排序算法，时间复杂度为 O(n)，空间复杂度为 O(1)。
        int left = 0, right = nums.size() - 1;
        int i = 0;
        while (i <= right) {
            if (nums[i] == 0) {
                std::swap(nums[i], nums[left]);
                i++;
                left++;
                // 交换后，i所指的元素必定是0或者1，因为排序是从左往右的
            } else if (nums[i] == 2) {
                std::swap(nums[i], nums[right]);
                right--;
                // 交换后，i 所指的新元素是未知的（可能是 0、1 或 2）
            } else {
                i++;
            }
        }
    }

private:
    void quickSort(std::vector<int>& nums, int left, int right) {
        if (left < right) {
            int pivotIndex = partition(nums, left, right);
            quickSort(nums, left, pivotIndex - 1);  // 递归处理左侧
            quickSort(nums, pivotIndex + 1, right); // 递归处理右侧
        }
    }
    
    int partition(std::vector<int>& nums, int left, int right) {
        int pivot = nums[right];
        int i = left - 1;
        
        for (int j = left; j < right; j++) {
            if (nums[j] <= pivot) {
                i++;
                std::swap(nums[i], nums[j]);
            }
        }
        
        std::swap(nums[i + 1], nums[right]);
        return i + 1;
    }

    int quickSelect(std::vector<int>& nums, int left, int right, int k) {
        // 由于我们在每次递归调用时都确保 k 在当前的搜索范围内，所以当 left == right 时，这个唯一的元素必定是第 k 小的元素。
        if (left == right) {
            return nums[right];
        }
        int pivot = partition(nums, left, right);
        if (k == pivot) {
            return nums[pivot];
        }
        else if (k > pivot) return quickSelect(nums, pivot + 1, right, k);
        else return quickSelect(nums, left, pivot - 1, k);
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
        // 快速排序测试
        runTest("快速排序 - 正常数组", [this]() {
            std::vector<int> nums = {5,2,3,1,4};
            std::vector<int> expected = {1,2,3,4,5};
            std::vector<int> result = solution.sortArray(nums);
            return result == expected;
        });

        runTest("快速排序 - 已排序数组", [this]() {
            std::vector<int> nums = {1,2,3,4,5};
            std::vector<int> expected = {1,2,3,4,5};
            std::vector<int> result = solution.sortArray(nums);
            return result == expected;
        });

        runTest("快速排序 - 逆序数组", [this]() {
            std::vector<int> nums = {5,4,3,2,1};
            std::vector<int> expected = {1,2,3,4,5};
            std::vector<int> result = solution.sortArray(nums);
            return result == expected;
        });

        runTest("快速排序 - 包含重复元素", [this]() {
            std::vector<int> nums = {3,1,4,1,5,9,2,6,5,3,5};
            std::vector<int> expected = {1,1,2,3,3,4,5,5,5,6,9};
            std::vector<int> result = solution.sortArray(nums);
            return result == expected;
        });

        // 第K个最大元素测试
        runTest("第K个最大元素 - 正常情况", [this]() {
            std::vector<int> nums = {3,2,1,5,6,4};
            int k = 2;
            int result = solution.findKthLargest(nums, k);
            return result == 5;
        });

        runTest("第K个最大元素 - k=1", [this]() {
            std::vector<int> nums = {3,2,1,5,6,4};
            int k = 1;
            int result = solution.findKthLargest(nums, k);
            return result == 6;
        });

        runTest("第K个最大元素 - k等于数组长度", [this]() {
            std::vector<int> nums = {3,2,1,5,6,4};
            int k = 6;
            int result = solution.findKthLargest(nums, k);
            return result == 1;
        });

        runTest("第K个最大元素 - 包含重复元素", [this]() {
            std::vector<int> nums = {3,2,3,1,2,4,5,5,6};
            int k = 4;
            int result = solution.findKthLargest(nums, k);
            return result == 4;
        });

        // 颜色分类测试
        runTest("颜色分类 - 正常情况", [this]() {
            std::vector<int> nums = {2,0,2,1,1,0};
            std::vector<int> expected = {0,0,1,1,2,2};
            solution.sortColors(nums);
            return nums == expected;
        });

        runTest("颜色分类 - 已排序", [this]() {
            std::vector<int> nums = {0,0,1,1,2,2};
            std::vector<int> expected = {0,0,1,1,2,2};
            solution.sortColors(nums);
            return nums == expected;
        });

        runTest("颜色分类 - 逆序", [this]() {
            std::vector<int> nums = {2,2,1,1,0,0};
            std::vector<int> expected = {0,0,1,1,2,2};
            solution.sortColors(nums);
            return nums == expected;
        });

        runTest("颜色分类 - 只有两种颜色", [this]() {
            std::vector<int> nums = {1,0,1,0,1,0};
            std::vector<int> expected = {0,0,0,1,1,1};
            solution.sortColors(nums);
            return nums == expected;
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
