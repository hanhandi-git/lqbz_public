#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include <algorithm>

class Solution {
public:
    std::string largestNumber(std::vector<int>& nums) {
        if (nums.size() <= 0) return {};
        std::vector<std::string> numStr;
        for (auto val : nums)
        {
            numStr.push_back(std::to_string(val));
        }
        std::sort(numStr.begin(), numStr.end(),[](const std::string a, const std::string b) {
            return a + b > b + a;
        });
        std::string result = "";
        if (numStr[0] == "0") return "0";
        for (auto& str : numStr)
        {
            result += str;
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

    bool expectEqual(const std::string& expected, const std::string& actual) {
        return expected == actual;
    }

public:
    void runAllTests() {
        runTest("Example1", [this]() {
            std::vector<int> nums = {10, 2};
            std::string expected = "210";
            return expectEqual(expected, solution.largestNumber(nums));
        });

        runTest("Example2", [this]() {
            std::vector<int> nums = {3, 30, 34, 5, 9};
            std::string expected = "9534330";
            return expectEqual(expected, solution.largestNumber(nums));
        });

        runTest("AllZeros", [this]() {
            std::vector<int> nums = {0, 0};
            std::string expected = "0";
            return expectEqual(expected, solution.largestNumber(nums));
        });

        runTest("SingleNumber", [this]() {
            std::vector<int> nums = {1};
            std::string expected = "1";
            return expectEqual(expected, solution.largestNumber(nums));
        });

        runTest("LargeNumbers", [this]() {
            std::vector<int> nums = {999999998, 999999997, 999999999};
            std::string expected = "999999999999999998999999997";
            return expectEqual(expected, solution.largestNumber(nums));
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