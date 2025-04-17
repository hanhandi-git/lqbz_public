#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
#include <utility>

class LISExtensions {
public:
    // 1. 最长递减子序列
    int lengthOfLDS(std::vector<int>& nums) {
        std::vector<int> reversed;
        for (int num : nums) {
            reversed.push_back(-num);
        }
        return lengthOfLIS(reversed);
    }

    // 2. 最长上升子序列的个数
    int numberOfLIS(std::vector<int>& nums) {
        int n = nums.size();
        std::vector<int> dp(n, 1);
        std::vector<int> count(n, 1);   // count[i] 表示以 nums[i] 结尾的、长度为 dp[i] 的最长上升子序列的个数
        int maxLen = 1;
        for (int i = 1; i < n; i++)
        {
            for (int j = 0; j < i; j++)
            {
                if (nums[i] > nums[j]) {
                    if (dp[j] + 1 > dp[i])
                    {
                        dp[i] = dp[j] + 1;
                        count[i] = count[j];
                    }
                    else    // dp[i] == dp[j]+1 找到一个长度相同的
                    {
                        count[i] += count[j];
                    }
                }
            }
            maxLen = std::max(maxLen, dp[i]);
        }
        int result = 0;
        for (int i = 0; i < dp.size(); i++)
        {
            if (maxLen == dp[i])
            {
                result += count[i];
            }
        }
        return result;
    }

    // 3. 最长上升子序列的具体方案 todo 暂时没有做过
    std::vector<int> getLIS(std::vector<int>& nums) {
        int n = nums.size();
        if (n <= 1) return nums;
        
        std::vector<int> dp(n, 1);
        std::vector<int> prev(n, -1);  // 记录前驱节点
        int maxLen = 1;
        int endIndex = 0;
        
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[i] > nums[j] && dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                    prev[i] = j;
                }
            }
            if (dp[i] > maxLen) {
                maxLen = dp[i];
                endIndex = i;
            }
        }
        
        // 重建序列
        std::vector<int> result;
        while (endIndex != -1) {
            result.push_back(nums[endIndex]);
            endIndex = prev[endIndex];
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    // 4. 俄罗斯套娃信封问题
    int maxEnvelopes(std::vector<std::pair<int, int>>& envelopes) {
        std::sort(envelopes.begin(), envelopes.end(),
            [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                return (a.first < b.first) || (a.first == b.first && a.second > b.second);
        });
        std::vector<int> heights;
        for (int i = 0; i < envelopes.size(); i++)
        {
            heights.push_back(envelopes[i].second);
        }
        return lengthOfLIS(heights);
    }

private:
    int lengthOfLIS(std::vector<int>& nums) {
        std::vector<int> tails;
        for (int num : nums) {
            auto it = std::lower_bound(tails.begin(), tails.end(), num);
            if (it == tails.end()) {
                tails.push_back(num);
            } else {
                *it = num;
            }
        }
        // 打印tails
        for (auto num : tails)
        {
            std::cout << num << " ";
        }
        return tails.size();
    }
};

class TestFramework {
private:
    LISExtensions solution;
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
        // 测试最长递减子序列
        runTest("最长递减子序列 - 基本情况", [this]() {
            std::vector<int> nums = {10,9,2,5,3,7,101,18};
            return solution.lengthOfLDS(nums) == 4;  // [10,9,5,3]
        });
        
        runTest("最长递减子序列 - 空数组", [this]() {
            std::vector<int> nums = {};
            return solution.lengthOfLDS(nums) == 0;
        });

        runTest("最长递减子序列 - 单个元素", [this]() {
            std::vector<int> nums = {5};
            return solution.lengthOfLDS(nums) == 1;
        });

        runTest("最长递减子序列 - 全相等元素", [this]() {
            std::vector<int> nums = {1,1,1,1};
            return solution.lengthOfLDS(nums) == 1;
        });

        // 测试最长上升子序列的个数
        runTest("最长上升子序列的个数 - 基本情况", [this]() {
            std::vector<int> nums = {1,3,5,4,7};
            return solution.numberOfLIS(nums) == 2;  // [1,3,4,7] 和 [1,3,5,7]
        });

        runTest("最长上升子序列的个数 - 全相等", [this]() {
            std::vector<int> nums = {1,1,1,1};
            return solution.numberOfLIS(nums) == 4;  // 每个1都可以单独构成LIS
        });

        runTest("最长上升子序列的个数 - 递减序列", [this]() {
            std::vector<int> nums = {4,3,2,1};
            return solution.numberOfLIS(nums) == 4;  // 每个数字单独构成长度为1的LIS
        });

        runTest("最长上升子序列的个数 - 多个相同长度的LIS", [this]() {
            std::vector<int> nums = {2,2,2,2,2};
            return solution.numberOfLIS(nums) == 5;
        });

        // 测试获取具体的最长上升子序列
        runTest("获取LIS - 基本情况", [this]() {
            std::vector<int> nums = {10,9,2,5,3,7,101,18};
            std::vector<int> expected = {2,5,7,101};
            return solution.getLIS(nums) == expected;
        });

        runTest("获取LIS - 空数组", [this]() {
            std::vector<int> nums = {};
            std::vector<int> expected = {};
            return solution.getLIS(nums) == expected;
        });

        runTest("获取LIS - 单调递增", [this]() {
            std::vector<int> nums = {1,2,3,4,5};
            std::vector<int> expected = {1,2,3,4,5};
            return solution.getLIS(nums) == expected;
        });

        runTest("获取LIS - 单调递减", [this]() {
            std::vector<int> nums = {5,4,3,2,1};
            std::vector<int> expected = {5};  // 或任意一个数字
            return solution.getLIS(nums).size() == 1;
        });

        // 测试俄罗斯套娃信封问题
        runTest("俄罗斯套娃信封 - 基本情况", [this]() {
            std::vector<std::pair<int, int>> envelopes = {
                {5,4}, {6,4}, {6,7}, {2,3}
            };
            return solution.maxEnvelopes(envelopes) == 3;
        });

        runTest("俄罗斯套娃信封 - 空集合", [this]() {
            std::vector<std::pair<int, int>> envelopes = {};
            return solution.maxEnvelopes(envelopes) == 0;
        });

        runTest("俄罗斯套娃信封 - 单个信封", [this]() {
            std::vector<std::pair<int, int>> envelopes = {{1,1}};
            return solution.maxEnvelopes(envelopes) == 1;
        });

        runTest("俄罗斯套娃信封 - 无法嵌套", [this]() {
            std::vector<std::pair<int, int>> envelopes = {
                {1,1}, {1,1}, {1,1}
            };
            return solution.maxEnvelopes(envelopes) == 1;
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
