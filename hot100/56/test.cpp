#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>

class Solution {
public:
    std::vector<std::vector<int>> merge(std::vector<std::vector<int>>& intervals) {
        if (intervals.empty()) return {};
        
        sort(intervals.begin(), intervals.end());
        
        std::vector<std::vector<int>> result;
        result.push_back(intervals[0]);
        
        for (int i = 1; i < intervals.size(); i++) {
            if (intervals[i][0] <= result.back()[1]) {
                result.back()[1] = std::max(result.back()[1], intervals[i][1]);
            } else {
                result.push_back(intervals[i]);
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
            std::cout << "通过: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "失败: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        // 示例测试
        runTest("示例1测试", [this]() {
            std::vector<std::vector<int>> intervals = {{1,3},{2,6},{8,10},{15,18}};
            std::vector<std::vector<int>> expected = {{1,6},{8,10},{15,18}};
            return solution.merge(intervals) == expected;
        });

        runTest("示例2测试", [this]() {
            std::vector<std::vector<int>> intervals = {{1,4},{4,5}};
            std::vector<std::vector<int>> expected = {{1,5}};
            return solution.merge(intervals) == expected;
        });

        // 边界情况测试
        runTest("空数组测试", [this]() {
            std::vector<std::vector<int>> intervals;
            std::vector<std::vector<int>> expected;
            return solution.merge(intervals) == expected;
        });

        runTest("单区间测试", [this]() {
            std::vector<std::vector<int>> intervals = {{1,1}};
            std::vector<std::vector<int>> expected = {{1,1}};
            return solution.merge(intervals) == expected;
        });

        // 特殊情况测试
        runTest("完全重叠区间测试", [this]() {
            std::vector<std::vector<int>> intervals = {{1,4},{1,4}};
            std::vector<std::vector<int>> expected = {{1,4}};
            return solution.merge(intervals) == expected;
        });

        runTest("多重叠区间测试", [this]() {
            std::vector<std::vector<int>> intervals = {{1,4},{2,3},{3,5},{6,7},{8,9},{7,10}};
            std::vector<std::vector<int>> expected = {{1,5},{6,10}};
            return solution.merge(intervals) == expected;
        });

        runTest("相邻区间测试", [this]() {
            std::vector<std::vector<int>> intervals = {{1,2},{2,3},{3,4},{4,5}};
            std::vector<std::vector<int>> expected = {{1,5}};
            return solution.merge(intervals) == expected;
        });

        runTest("无重叠区间测试", [this]() {
            std::vector<std::vector<int>> intervals = {{1,2},{4,5},{7,8}};
            std::vector<std::vector<int>> expected = {{1,2},{4,5},{7,8}};
            return solution.merge(intervals) == expected;
        });

        runTest("包含关系测试", [this]() {
            std::vector<std::vector<int>> intervals = {{1,6},{2,4},{3,5}};
            std::vector<std::vector<int>> expected = {{1,6}};
            return solution.merge(intervals) == expected;
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