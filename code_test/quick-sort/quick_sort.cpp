#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>


class Solution {
public:
    std::vector<int> sortArray(std::vector<int>& nums) {
        quickSort(nums, 0, nums.size() - 1);
        return nums;
    }
    
private:
    void quickSort(std::vector<int>& nums, int left, int right) {
        if (left < right)
        {
            int pivot = partition(nums, left, right);
            quickSort(nums, left, pivot - 1);
            quickSort(nums, pivot + 1, right);
        }
        return;
    }
    
    int partition(std::vector<int>& nums, int left, int right) {
        int pivot = nums[right];
        int i = left - 1;   // i 是小于等于基准的元素的最后位置
        for (int j = left; j < right; j++)
        {
            if (nums[j] < pivot)
            {
                i++;
                std::swap(nums[i], nums[j]);
            }
        }
        std::swap(nums[i + 1], nums[right]);
        return i + 1;
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
        runTest("测试用例1", [this]() {
            std::vector<int> nums = {5,2,3,1};
            std::vector<int> expected = {1,2,3,5};
            std::vector<int> result = solution.sortArray(nums);
            return result == expected;
        });

        runTest("测试用例2", [this]() {
            std::vector<int> nums = {5,1,1,2,0,0};
            std::vector<int> expected = {0,0,1,1,2,5};
            std::vector<int> result = solution.sortArray(nums);
            return result == expected;
        });

        runTest("测试用例3", [this]() {
            std::vector<int> nums = {1};
            std::vector<int> expected = {1};
            std::vector<int> result = solution.sortArray(nums);
            return result == expected;
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
