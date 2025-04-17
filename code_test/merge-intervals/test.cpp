#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>  

class Solution {
public:
    // 合并区间的主要实现
    std::vector<std::vector<int>> merge(std::vector<std::vector<int>>& intervals) {
        std::vector<std::vector<int>> result;
        if (intervals.empty()) return result;
        std::sort(intervals.begin(), intervals.end());
        result.push_back(intervals[0]);
        for (int i = 1; i < intervals.size(); i++)
        {
            if (intervals[i][0] <= intervals[i-1][1])
            {
                result.back()[1] = std::max(intervals[i][1], result.back()[1]);
            }
            else
            {
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

    bool compareVectors(const std::vector<std::vector<int>>& a, 
                       const std::vector<std::vector<int>>& b) {
        if (a.size() != b.size()) return false;
        for (size_t i = 0; i < a.size(); i++) {
            if (a[i] != b[i]) return false;
        }
        return true;
    }

public:
    void runAllTests() {
        // 测试示例1
        runTest("示例1", [this]() {
            std::vector<std::vector<int>> intervals = {{1,3},{2,6},{8,10},{15,18}};
            std::vector<std::vector<int>> expected = {{1,6},{8,10},{15,18}};
            return compareVectors(solution.merge(intervals), expected);
        });

        // 测试示例2
        runTest("示例2", [this]() {
            std::vector<std::vector<int>> intervals = {{1,4},{4,5}};
            std::vector<std::vector<int>> expected = {{1,5}};
            return compareVectors(solution.merge(intervals), expected);
        });

        // 测试空数组
        runTest("空数组", [this]() {
            std::vector<std::vector<int>> intervals;
            std::vector<std::vector<int>> expected;
            return compareVectors(solution.merge(intervals), expected);
        });

        // 测试单个区间
        runTest("单个区间", [this]() {
            std::vector<std::vector<int>> intervals = {{1,1}};
            std::vector<std::vector<int>> expected = {{1,1}};
            return compareVectors(solution.merge(intervals), expected);
        });

        // 测试完全重叠的区间
        runTest("完全重叠", [this]() {
            std::vector<std::vector<int>> intervals = {{1,4},{1,4}};
            std::vector<std::vector<int>> expected = {{1,4}};
            return compareVectors(solution.merge(intervals), expected);
        });

        // 测试无重叠区间
        runTest("无重叠", [this]() {
            std::vector<std::vector<int>> intervals = {{1,2},{3,4},{5,6}};
            std::vector<std::vector<int>> expected = {{1,2},{3,4},{5,6}};
            return compareVectors(solution.merge(intervals), expected);
        });

        // 测试多重重叠
        runTest("多重重叠", [this]() {
            std::vector<std::vector<int>> intervals = {{1,4},{2,3},{3,6}};
            std::vector<std::vector<int>> expected = {{1,6}};
            return compareVectors(solution.merge(intervals), expected);
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
