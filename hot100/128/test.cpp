#include <vector>
#include <unordered_set>
#include <iostream>
#include <functional>

class Solution {
public:
    int longestConsecutive(std::vector<int>& nums) {
        if (nums.empty()) return 0;
        if (nums.size() == 1) return 1;
        
        std::unordered_set<int> num_set(nums.begin(), nums.end());
        int longest_streak = 1;
        
        for (const int& num : num_set) {
            if (!num_set.count(num - 1)) {
                int current_streak = 1;
                int current_num = num;
                
                while (num_set.count(current_num + 1)) {
                    current_streak++;
                    current_num++;
                }
                
                longest_streak = std::max(longest_streak, current_streak);
            }
        }
        
        return longest_streak;
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
        // 测试用例1: 基本示例
        runTest("示例1", [this]() {
            std::vector<int> nums = {100,4,200,1,3,2};
            return solution.longestConsecutive(nums) == 4;
        });

        // 测试用例2: 更长的序列
        runTest("示例2", [this]() {
            std::vector<int> nums = {0,3,7,2,5,8,4,6,0,1};
            return solution.longestConsecutive(nums) == 9;
        });

        // 测试用例3: 空数组
        runTest("空数组", [this]() {
            std::vector<int> nums = {};
            return solution.longestConsecutive(nums) == 0;
        });

        // 测试用例4: 单个元素
        runTest("单个元素", [this]() {
            std::vector<int> nums = {1};
            return solution.longestConsecutive(nums) == 1;
        });

        // 测试用例5: 重复元素
        runTest("重复元素", [this]() {
            std::vector<int> nums = {1,1,2,2,3,3,4,4};
            return solution.longestConsecutive(nums) == 4;
        });

        // 测试用例6: 不连续的序列
        runTest("不连续序列", [this]() {
            std::vector<int> nums = {1,3,5,7,9};
            return solution.longestConsecutive(nums) == 1;
        });

        // 测试用例7: 负数
        runTest("包含负数", [this]() {
            std::vector<int> nums = {-5,-4,-3,-2,-1,0,1};
            return solution.longestConsecutive(nums) == 7;
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