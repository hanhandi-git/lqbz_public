#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include <algorithm>
#include <deque>
using namespace std;
// 双端队列解法
class Solution {
private:
    class MyQueue {
        private:
            deque<int> data;
        public:
            void push(int n) {
                // 将小于当前的都释放掉
                while (!data.empty() && data.back() < n) {
                    data.pop_back();
                }
                data.push_back(n);
            }
            int max() {
                return data.front();
            }
            void pop(int n) {
                if (!data.empty() && data.back() == n) {
                    data.pop_back();
                }
            }
    };
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        MyQueue window;
        vector<int> res;
        for (int i = 0; i < nums.size(); i++)
        {
            if (i < k - 1) {
                window.push(nums[i]);
            } else {
                window.push(nums[i]);
                res.push_back(window.max());
                window.pop(nums[i-k+1]);
            }
        }
        return res;
    }
}; 
// class Solution {
// public:
//     std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
//         std::vector<int> result;
//         std::deque<int> dq;
        
//         for (int i = 0; i < nums.size(); ++i) {
//             // 移除队列中不在当前窗口的元素
//             // 当 dq.front() == i - k 队列头部的元素索引 + 窗口大小 = 当前处理的元素索引
//             // 换句话说，队列头部的元素已经不在当前的滑动窗口内了
//             // 假设当前窗口包含索引为 3, 4, 5 的元素 如果队列头部的元素索引是 2，那么 2 == 5 - 3，说明索引为 2 的元素已经不在当前窗口内了
//             if (!dq.empty() && dq.front() + k <= i)
//             {
//                 dq.pop_front();
//             }
//             while (!dq.empty() && nums[dq.back()] < nums[i])
//             {
//                 dq.pop_back();
//             }
//             dq.push_back(i);
//             // 窗口大小达到k时
//             if (i + 1 >= k)
//             {
//                 result.push_back(nums[dq.front()]);
//             }
            
//         }
        
//         return result;
//     }
// };
// 下面的是暴力解法
// class Solution {
// public:
//     std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
//         // 在这里实现你的解决方案
//         if (k > nums.size() || nums.size() == 0) return {};
//         if (k == nums.size()) return {*std::max_element(nums.begin(), nums.end())};
//         std::vector<int> res;
//         for (int i = 0; i <= nums.size() - k; i++)
//         {
//             std::vector<int> new_arrary;
//             std::copy(nums.begin() + i, nums.begin() + i + k, std::back_inserter(new_arrary));
//             int tmp_max = *std::max_element(new_arrary.begin(), new_arrary.end());
//             res.push_back(tmp_max);
//         }
//         return res;
//     }
// };

// class Solution {
// public:
//     std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
//         std::vector<int> result;
//         std::deque<int> dq;
        
//         for (int i = 0; i < nums.size(); ++i) {
//             // 移除队列中不在当前窗口的元素
//             // 当 dq.front() == i - k 队列头部的元素索引 + 窗口大小 = 当前处理的元素索引
//             // 换句话说，队列头部的元素已经不在当前的滑动窗口内了
//             // 假设当前窗口包含索引为 3, 4, 5 的元素 如果队列头部的元素索引是 2，那么 2 == 5 - 3，说明索引为 2 的元素已经不在当前窗口内了
//             if (!dq.empty() && dq.front() == i - k) {
//                 dq.pop_front();
//             }
            
//             // 移除队列中所有小于当前元素的索引
//             // 
//             while (!dq.empty() && nums[dq.back()] < nums[i]) {
//                 dq.pop_back();
//             }
            
//             dq.push_back(i);
            
//             // 当窗口大小达到 k 时，开始添加结果
//             if (i >= k - 1) {
//                 result.push_back(nums[dq.front()]);
//             }
//         }
        
//         return result;
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