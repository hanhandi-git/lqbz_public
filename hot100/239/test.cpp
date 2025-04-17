#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include <algorithm>
#include <deque>

// 双端队列解法
class Solution {
public:
    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
        std::deque<int> dq;
        std::vector<int> res;
        for (int i = 0; i < nums.size(); i++) {
            // 判断是否需要出队列
            if (!dq.empty() && dq.front() + k == i) {
                dq.pop_front();
            }
            // 判断是否需要出队
            while (!dq.empty() && nums[i] > nums[dq.back()]) {
                dq.pop_back();
            }
            dq.push_back(i);
            if (i >= k - 1) {
                res.push_back(nums[dq.front()]);
            }
        }
        return res;
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

    template<typename T>
    bool expectEqual(const T& expected, const T& actual) {
        return expected == actual;
    }

public:
    void runAllTests() {
        runTest("Example1", [this]() {
            std::vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
            int k = 3;
            std::vector<int> expected = {3, 3, 5, 5, 6, 7};
            return expectEqual(expected, solution.maxSlidingWindow(nums, k));
        });

        runTest("Example2", [this]() {
            std::vector<int> nums = {1};
            int k = 1;
            std::vector<int> expected = {1};
            return expectEqual(expected, solution.maxSlidingWindow(nums, k));
        });

        runTest("EmptyInput", [this]() {
            std::vector<int> nums = {};
            int k = 0;
            std::vector<int> expected = {};
            return expectEqual(expected, solution.maxSlidingWindow(nums, k));
        });

        runTest("LargeWindow", [this]() {
            std::vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
            int k = 8;
            std::vector<int> expected = {7};
            return expectEqual(expected, solution.maxSlidingWindow(nums, k));
        });

        runTest("NegativeNumbers", [this]() {
            std::vector<int> nums = {-7, -8, 7, 5, 7, 1, 6, 0};
            int k = 4;
            std::vector<int> expected = {7, 7, 7, 7, 7};
            return expectEqual(expected, solution.maxSlidingWindow(nums, k));
        });

        std::cout << "\nTest Results: " << passed << " passed, " 
                  << (total - passed) << " failed, " 
                  << total << " total" << std::endl;
    }
};

int main() {
    TestFramework testFramework;
    testFramework.runAllTests();
    return 0;
}