#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>  


class Solution {
public:
    std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
        std::vector<std::vector<int>> res;
        if (nums.size() < 3) return {};
        std::sort(nums.begin(), nums.end());
        for (int i = 0; i < nums.size(); i++)
        {
            if (nums[i] > 0) break;
            if (i > 0 && nums[i] == nums[i-1]) continue;
            int left = i + 1;
            int right = nums.size() - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum == 0) {
                    res.emplace_back(std::vector<int>{nums[i], nums[left], nums[right]});
                    // 去重
                    while (left < right && nums[left] == nums[left+1]) left++;
                    while (left < right && nums[right] == nums[right-1]) right--;
                    left++;
                    right--;
                }
                else if (sum > 0) {
                    right--;
                } else {
                    left++;
                }
            }
        }
        return res;
    }
};

// class Solution {
// public:
//     std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
//         std::vector<std::vector<int>> res;
//         if (nums.size() < 3) return {};
//         std::sort(nums.begin(), nums.end());
//         for (int i = 0; i < nums.size(); i++) {
//             if (nums[i] > 0) break;
//             if ( i > 0 && nums[i] == nums[i-1]) continue;
//             int left = i + 1;
//             int right = nums.size() - 1;
//             while (left < right) {
//                 int sum = nums[i] + nums[left] + nums[right];
//                 if (sum < 0) {
//                     left++;
//                 } else if (sum > 0) {
//                     right--;
//                 } else {
//                     res.push_back({nums[i], nums[left], nums[right]});
//                     while (left < right && nums[left] == nums[left+1]) left++;
//                     while (left < right && nums[right] == nums[right-1]) right--;
//                     left++;
//                     right--;
//                 }
//             }
//         }
//         return res;
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
            std::cout << "PASS: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "FAIL: " << testName << std::endl;
        }
    }

    bool expectEqual(const std::vector<std::vector<int>>& expected, const std::vector<std::vector<int>>& actual) {
        if (expected.size() != actual.size()) return false;
        for (const auto& triplet : expected) {
            if (std::find(actual.begin(), actual.end(), triplet) == actual.end()) {
                return false;
            }
        }
        return true;
    }

public:
    void runAllTests() {
        runTest("Example1", [this]() {
            std::vector<int> nums = {-1,0,1,2,-1,-4};
            std::vector<std::vector<int>> expected = {{-1,-1,2},{-1,0,1}};
            return expectEqual(expected, solution.threeSum(nums));
        });

        runTest("Example2", [this]() {
            std::vector<int> nums = {0,1,1};
            std::vector<std::vector<int>> expected = {};
            return expectEqual(expected, solution.threeSum(nums));
        });

        runTest("Example3", [this]() {
            std::vector<int> nums = {0,0,0};
            std::vector<std::vector<int>> expected = {{0,0,0}};
            return expectEqual(expected, solution.threeSum(nums));
        });

        runTest("EmptyInput", [this]() {
            std::vector<int> nums = {};
            std::vector<std::vector<int>> expected = {};
            return expectEqual(expected, solution.threeSum(nums));
        });

        runTest("LargerInput", [this]() {
            std::vector<int> nums = {-2,0,0,2,2};
            std::vector<std::vector<int>> expected = {{-2,0,2}};
            return expectEqual(expected, solution.threeSum(nums));
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
