#include <iostream>
#include <vector>
#include <functional>
#include <queue>
#include <algorithm>
using namespace std;

class Solution {
public:
    int minMeetingRooms(vector<vector<int>>& intervals) {
        std::sort(intervals.begin(), intervals.end());
        std::priority_queue<int, std::vector<int>, std::greater<int>> que;
        for (int i = 0; i < intervals.size(); i++)
        {
            if (!que.empty() && que.top() <= intervals[i][0])
            {
                que.pop();
            }
            que.push(intervals[i][1]);
        }
        return que.size();
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
        runTest("测试用例1", [this]() {
            std::vector<std::vector<int>> intervals = {{0, 30}, {5, 10}, {15, 20}};
            return (solution.minMeetingRooms(intervals) == 2); // 预期最小会议室数量
        });

        runTest("测试用例2", [this]() {
            std::vector<std::vector<int>> intervals = {{7, 10}, {2, 4}};
            return (solution.minMeetingRooms(intervals) == 1); // 预期最小会议室数量
        });

        runTest("测试用例3", [this]() {
            std::vector<std::vector<int>> intervals = {{1, 5}, {2, 3}, {4, 6}};
            return (solution.minMeetingRooms(intervals) == 2); // 预期最小会议室数量
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