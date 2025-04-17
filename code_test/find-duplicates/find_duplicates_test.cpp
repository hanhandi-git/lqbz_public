#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>

class Solution {
public:
    std::vector<int> findDuplicates(std::vector<int>& nums) {
        std::vector<int> result;
        for (int num : nums) {
            int index = std::abs(num) - 1;
            if (nums[index] > 0 ) nums[index] = nums[index] * -1;
            else result.emplace_back(abs(num));
        }
        // for (auto num : result) std::cout << num << ",";
        // std::cout << std::endl;
        // std::sort(result.begin(), result.end());
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

    bool expectEqual(const std::vector<int>& expected, const std::vector<int>& actual) {
        return expected == actual;
    }

public:
    void runAllTests() {
        runTest("Example1", [this]() {
            std::vector<int> nums = {4,3,2,7,8,2,3,1};
            std::vector<int> expected = {2,3};
            return expectEqual(expected, solution.findDuplicates(nums));
        });

        runTest("Example2", [this]() {
            std::vector<int> nums = {1,1,2};
            std::vector<int> expected = {1};
            return expectEqual(expected, solution.findDuplicates(nums));
        });

        runTest("NoDuplicates", [this]() {
            std::vector<int> nums = {1,2,3,4,5};
            std::vector<int> expected = {};
            return expectEqual(expected, solution.findDuplicates(nums));
        });

        runTest("AllDuplicates", [this]() {
            std::vector<int> nums = {1,1,2,2,3,3,4,4};
            std::vector<int> expected = {1,2,3,4};
            return expectEqual(expected, solution.findDuplicates(nums));
        });

        runTest("LargeInput", [this]() {
            std::vector<int> nums(100000);
            std::vector<int> expected;
            for (int i = 0; i < 100000; i++) {
                nums[i] = (i % 50000) + 1;
                if (i >= 50000) {
                    expected.push_back(nums[i]);
                }
            }
            return expectEqual(expected, solution.findDuplicates(nums));
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