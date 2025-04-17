#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>  
#include <climits>

class Solution {
public:
    std::vector<std::vector<int>> nSum(std::vector<int>& nums, int n, int target) {
        std::vector<std::vector<int>> result;
        if (nums.size() < n) return result;
        std::sort(nums.begin(), nums.end());
        return nSumRecursive(nums, n, target, 0);
    }

    int threeSumClosest(std::vector<int>& nums, int target) {
        if (nums.size() < 3) return 0;
        std::sort(nums.begin(), nums.end());
        int closest = nums[0] + nums[1] + nums[2];
        
        for (int i = 0; i < nums.size() - 2; i++) {
            int left = i + 1, right = nums.size() - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum == target) return sum;
                if (std::abs(sum - target) < std::abs(closest - target)) {
                    closest = sum;
                }
                if (sum < target) left++;
                else right--;
            }
        }
        return closest;
    }

    int threeSumSmaller(std::vector<int>& nums, int target) {
        if (nums.size() < 3) return 0;
        std::sort(nums.begin(), nums.end());
        int count = 0;
        
        for (int i = 0; i < nums.size() - 2; i++) {
            int left = i + 1, right = nums.size() - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum < target) {
                    count += right - left;
                    left++;
                } else {
                    right--;
                }
            }
        }
        return count;
    }

private:
    std::vector<std::vector<int>> nSumRecursive(std::vector<int>& nums, int n, int target, int start) {
        std::vector<std::vector<int>> result;
        int size = nums.size();

        // 基本情况：两数之和
        if (n == 2) {
            int left = start, right = size - 1;
            while (left < right) {
                int sum = nums[left] + nums[right];
                if (sum == target) {
                    result.push_back({nums[left], nums[right]});
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    left++;
                    right--;
                } else if (sum < target) {
                    left++;
                } else {
                    right--;
                }
            }
        } else {
            // 递归情况：n > 2
            for (int i = start; i < size - n + 1; i++) {
                if (i > start && nums[i] == nums[i - 1]) continue; // 去重
                auto subResults = nSumRecursive(nums, n - 1, target - nums[i], i + 1);
                for (auto& subResult : subResults) {
                    subResult.insert(subResult.begin(), nums[i]);
                    result.push_back(subResult);
                }
            }
        }
        return result;
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
        runTest("TwoSum", [this]() {
            std::vector<int> nums = {2,7,11,15};
            std::vector<std::vector<int>> expected = {{2,7}};
            return expectEqual(expected, solution.nSum(nums, 2, 9));
        });

        runTest("ThreeSum", [this]() {
            std::vector<int> nums = {-1,0,1,2,-1,-4};
            std::vector<std::vector<int>> expected = {{-1,-1,2},{-1,0,1}};
            return expectEqual(expected, solution.nSum(nums, 3, 0));
        });

        runTest("FourSum", [this]() {
            std::vector<int> nums = {1,0,-1,0,-2,2};
            std::vector<std::vector<int>> expected = {{-2,-1,1,2},{-2,0,0,2},{-1,0,0,1}};
            return expectEqual(expected, solution.nSum(nums, 4, 0));
        });

        runTest("EmptyInput", [this]() {
            std::vector<int> nums = {};
            std::vector<std::vector<int>> expected = {};
            return expectEqual(expected, solution.nSum(nums, 2, 0));
        });

        runTest("ThreeSumClosest", [this]() {
            std::vector<int> nums = {-1,2,1,-4};
            int target = 1;
            return solution.threeSumClosest(nums, target) == 2;
        });

        runTest("ThreeSumSmaller", [this]() {
            std::vector<int> nums = {-2,0,1,3};
            int target = 2;
            return solution.threeSumSmaller(nums, target) == 2;
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
