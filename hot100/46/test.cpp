#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>


class Solution {
public:
   std::vector<std::vector<int>> res;
   std::vector<int> path;
   std::vector<bool> used;

    void printPath(int start)
    {
        // 打印当前的path
        std::cout << "当前path: [";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i];
            if (i < path.size() - 1) std::cout << ",";
        }
        std::cout << "], start: " << start << std::endl;
    }

    void printRes()
    {
        // 打印res的内容
        std::cout << "全排列结果:" << std::endl;
        for (const auto& perm : res) {
            std::cout << "[";
            for (size_t i = 0; i < perm.size(); ++i) {
                std::cout << perm[i];
                if (i < perm.size() - 1) std::cout << ",";
            }
            std::cout << "]" << std::endl;
        }
    }

    void dfs(std::vector<int>& nums) {
        if (path.size() == nums.size()) {
            res.emplace_back(path);
            return;
        }
        // 遍历本层
        for (int i = 0; i < nums.size(); i++)
        {
            if (used[i] == true) continue;
            used[i] = true;
            path.push_back(nums[i]);
            dfs(nums);
            path.pop_back();
            used[i] = false;
        }
        return;
    }
    std::vector<std::vector<int>> permute(std::vector<int>& nums) {
        path.clear();
        res.clear();
        used = std::vector<bool>(nums.size(), false);
        dfs(nums);
        return res;
    }
};

// class Solution {
// public:
//    std::vector<std::vector<int>> res;
//    std::vector<int> path;
//    std::vector<bool> used;

//     void printPath(int start)
//     {
//         // 打印当前的path
//         std::cout << "当前path: [";
//         for (size_t i = 0; i < path.size(); ++i) {
//             std::cout << path[i];
//             if (i < path.size() - 1) std::cout << ",";
//         }
//         std::cout << "], start: " << start << std::endl;
//     }

//     void printRes()
//     {
//         // 打印res的内容
//         std::cout << "全排列结果:" << std::endl;
//         for (const auto& perm : res) {
//             std::cout << "[";
//             for (size_t i = 0; i < perm.size(); ++i) {
//                 std::cout << perm[i];
//                 if (i < perm.size() - 1) std::cout << ",";
//             }
//             std::cout << "]" << std::endl;
//         }
//     }

//     void dfs(std::vector<int>& nums) {
//         if (path.size() == nums.size()) {
//             res.push_back(path);
//             return;
//         }

//         for (int i = 0; i < nums.size(); i++) {
//             if (used[i] == true) continue;
//             path.emplace_back(nums[i]);
//             used[i] = true;
//             dfs(nums);
//             used[i] = false;
//             path.pop_back();
//         }
//         return;
//     }
//     std::vector<std::vector<int>> permute(std::vector<int>& nums) {
//        res.clear();
//        path.clear();
//        used = std::vector<bool>(nums.size(), false);
//        dfs(nums);
//        return res;
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
        for (const auto& perm : expected) {
            if (std::find(actual.begin(), actual.end(), perm) == actual.end()) {
                return false;
            }
        }
        return true;
    }

public:
    void runAllTests() {
        runTest("Example1", [this]() {
            std::vector<int> nums = {1,2,3};
            std::vector<std::vector<int>> expected = {{1,2,3},{1,3,2},{2,1,3},{2,3,1},{3,1,2},{3,2,1}};
            return expectEqual(expected, solution.permute(nums));
        });

        runTest("Example2", [this]() {
            std::vector<int> nums = {0,1};
            std::vector<std::vector<int>> expected = {{0,1},{1,0}};
            return expectEqual(expected, solution.permute(nums));
        });

        runTest("Example3", [this]() {
            std::vector<int> nums = {1};
            std::vector<std::vector<int>> expected = {{1}};
            return expectEqual(expected, solution.permute(nums));
        });

        runTest("EmptyInput", [this]() {
            std::vector<int> nums = {};
            std::vector<std::vector<int>> expected = {{}};
            return expectEqual(expected, solution.permute(nums));
        });

        runTest("LargerInput", [this]() {
            std::vector<int> nums = {1,2,3,4};
            std::vector<std::vector<int>> result = solution.permute(nums);
            // 检查结果的大小是否正确（4! = 24）
            if (result.size() != 24) return false;
            // 检查每个排列是否唯一
            std::sort(result.begin(), result.end());
            return std::unique(result.begin(), result.end()) == result.end();
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