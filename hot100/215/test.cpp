#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <queue>
class Solution {
public:
    // 快速选择法
    // int findKthLargest(std::vector<int>& nums, int k) {
    //     return quickSelect(nums, 0, nums.size() - 1, nums.size() - k);
    // }

    int findKthLargest(std::vector<int>& nums, int k) {
        if (nums.empty() || k <= 0 || k > nums.size()) return -1;
        std::priority_queue<int, std::vector<int>, std::greater<int>> que;
        for (int i = 0; i < nums.size(); i++) 
        {
            que.push(nums[i]);
            if (que.size() > k) que.pop();
        }
        return que.top();
    }
    
private:
    int quickSelect(std::vector<int>& nums, int left, int right, int k) {
        if (left == right) return nums[left];
        
        int pivotIndex = partition(nums, left, right);
        
        if (k == pivotIndex) {
            return nums[k];
        } else if (k < pivotIndex) {
            return quickSelect(nums, left, pivotIndex - 1, k);
        } else {
            return quickSelect(nums, pivotIndex + 1, right, k);
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
            std::vector<int> nums = {3,2,1,5,6,4};
            int k = 2;
            int result = solution.findKthLargest(nums, k);
            return result == 5;
        });

        runTest("测试用例2", [this]() {
            std::vector<int> nums = {3,2,3,1,2,4,5,5,6};
            int k = 4;
            int result = solution.findKthLargest(nums, k);
            return result == 4;
        });

        runTest("测试用例3 - 单元素数组", [this]() {
            std::vector<int> nums = {1};
            int k = 1;
            int result = solution.findKthLargest(nums, k);
            return result == 1;
        });

        runTest("测试用例4 - 所有元素相同", [this]() {
            std::vector<int> nums = {3,3,3,3,3,3};
            int k = 2;
            int result = solution.findKthLargest(nums, k);
            return result == 3;
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
