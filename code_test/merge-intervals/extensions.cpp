#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>
#include <functional>

class IntervalExtensions {
public:
    // 1. 插入区间 todo
    std::vector<std::vector<int>> insert(std::vector<std::vector<int>>& intervals, 
                                       std::vector<int>& newInterval) {
        std::vector<std::vector<int>> result;
        int i = 0;
        
        // 添加所有在新区间之前的区间
        while (i < intervals.size() && intervals[i][1] < newInterval[0]) {
            result.push_back(intervals[i++]);
        }
        
        // 合并所有与新区间重叠的区间
        while (i < intervals.size() && intervals[i][0] <= newInterval[1]) {
            newInterval[0] = std::min(newInterval[0], intervals[i][0]);
            newInterval[1] = std::max(newInterval[1], intervals[i][1]);
            i++;
        }
        result.push_back(newInterval);
        
        // 添加剩余的区间
        while (i < intervals.size()) {
            result.push_back(intervals[i++]);
        }
        
        return result;
    }

    // 2. 会议室
    bool canAttendMeetings(std::vector<std::vector<int>>& intervals) {
        std::sort(intervals.begin(), intervals.end());
        for (int i = 1; i < intervals.size(); i++)
        {
            if (intervals[i][0] < intervals[i-1][1]) return false;
        }
        return true;
    }

    // 3. 会议室 II
    int minMeetingRooms(std::vector<std::vector<int>>& intervals) {
        if (intervals.empty()) return 0;
        std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
        for (int i = 0; i < intervals.size(); i++)
        {
            while (!pq.empty() && intervals[i][0] >= pq.top())
            {
                pq.pop();
            }
            pq.push(intervals[i][1]);
        }
        return pq.size();
    }

    // 4. 区间列表的交集
    std::vector<std::vector<int>> intervalIntersection(std::vector<std::vector<int>>& A, 
                                                      std::vector<std::vector<int>>& B) {
        std::vector<std::vector<int>> result;
        int i = 0, j = 0;
        while (i < A.size() && j < B.size())
        {
            int start = std::max(A[i][0], B[j][0]);
            int end = std::min(A[i][1], B[j][1]);
            if (start <= end)
            {
                result.push_back({start, end});
            }
            //
            if (A[i][1] < B[j][1])
            {
                i++;
            } else {
                j++;
            }
        }
        return result;
    }

    // 5. 用最少数量的箭引爆气球
    int findMinArrowShots(std::vector<std::vector<int>>& points) {
        if (points.empty()) return 0;
        std::sort(points.begin(), points.end(),[](const std::vector<int>& a, const std::vector<int>& b){
            return a[1] < b[1];
        });
        int arrows = 1;
        int pos = points[0][1];
        for (int i = 1; i < points.size(); i++)
        {
            if (points[i][0] > pos)
            {
                arrows++;
                pos = points[i][1];
            }
        }
        return arrows;
    }
};

class TestFramework {
private:
    IntervalExtensions solution;
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
        // 插入区间测试
        runTest("插入区间 - 基本情况", [this]() {
            std::vector<std::vector<int>> intervals = {{1,3},{6,9}};
            std::vector<int> newInterval = {2,5};
            std::vector<std::vector<int>> expected = {{1,5},{6,9}};
            return compareVectors(solution.insert(intervals, newInterval), expected);
        });

        runTest("插入区间 - 空数组", [this]() {
            std::vector<std::vector<int>> intervals;
            std::vector<int> newInterval = {5,7};
            std::vector<std::vector<int>> expected = {{5,7}};
            return compareVectors(solution.insert(intervals, newInterval), expected);
        });

        runTest("插入区间 - 完全不重叠", [this]() {
            std::vector<std::vector<int>> intervals = {{1,2},{3,4}};
            std::vector<int> newInterval = {6,8};
            std::vector<std::vector<int>> expected = {{1,2},{3,4},{6,8}};
            return compareVectors(solution.insert(intervals, newInterval), expected);
        });

        runTest("插入区间 - 完全覆盖", [this]() {
            std::vector<std::vector<int>> intervals = {{3,5},{6,7},{8,10}};
            std::vector<int> newInterval = {2,11};
            std::vector<std::vector<int>> expected = {{2,11}};
            return compareVectors(solution.insert(intervals, newInterval), expected);
        });

        // 会议室测试
        runTest("会议室 - 无冲突", [this]() {
            std::vector<std::vector<int>> intervals = {{1,4},{5,6},{7,8}};
            return solution.canAttendMeetings(intervals) == true;
        });

        runTest("会议室 - 有冲突", [this]() {
            std::vector<std::vector<int>> intervals = {{0,30},{5,10},{15,20}};
            return solution.canAttendMeetings(intervals) == false;
        });

        runTest("会议室 - 空数组", [this]() {
            std::vector<std::vector<int>> intervals;
            return solution.canAttendMeetings(intervals) == true;
        });

        runTest("会议室 - 单个会议", [this]() {
            std::vector<std::vector<int>> intervals = {{1,2}};
            return solution.canAttendMeetings(intervals) == true;
        });

        runTest("会议室 - 相邻时间", [this]() {
            std::vector<std::vector<int>> intervals = {{1,2},{2,3},{3,4}};
            return solution.canAttendMeetings(intervals) == true;
        });

        // 会议室II测试
        runTest("会议室II - 基本情况", [this]() {
            std::vector<std::vector<int>> intervals = {{0,30},{5,10},{15,20}};
            return solution.minMeetingRooms(intervals) == 2;
        });

        runTest("会议室II - 空数组", [this]() {
            std::vector<std::vector<int>> intervals;
            return solution.minMeetingRooms(intervals) == 0;
        });

        runTest("会议室II - 单个会议", [this]() {
            std::vector<std::vector<int>> intervals = {{1,2}};
            return solution.minMeetingRooms(intervals) == 1;
        });

        runTest("会议室II - 多重重叠", [this]() {
            std::vector<std::vector<int>> intervals = {{1,10},{2,7},{3,19},{8,12},{10,20},{11,30}};
            return solution.minMeetingRooms(intervals) == 4;
        });

        runTest("会议室II - 无重叠", [this]() {
            std::vector<std::vector<int>> intervals = {{1,2},{3,4},{5,6}};
            return solution.minMeetingRooms(intervals) == 1;
        });

        // 区间列表的交集测试
        runTest("区间交集 - 基本情况", [this]() {
            std::vector<std::vector<int>> A = {{0,2},{5,10},{13,23},{24,25}};
            std::vector<std::vector<int>> B = {{1,5},{8,12},{15,24},{25,26}};
            std::vector<std::vector<int>> expected = {{1,2},{5,5},{8,10},{15,23},{24,24},{25,25}};
            return compareVectors(solution.intervalIntersection(A, B), expected);
        });

        runTest("区间交集 - 空数组", [this]() {
            std::vector<std::vector<int>> A;
            std::vector<std::vector<int>> B = {{1,2}};
            std::vector<std::vector<int>> expected;
            return compareVectors(solution.intervalIntersection(A, B), expected);
        });

        runTest("区间交集 - 无交集", [this]() {
            std::vector<std::vector<int>> A = {{1,2},{3,4}};
            std::vector<std::vector<int>> B = {{5,6},{7,8}};
            std::vector<std::vector<int>> expected;
            return compareVectors(solution.intervalIntersection(A, B), expected);
        });

        runTest("区间交集 - 完全重叠", [this]() {
            std::vector<std::vector<int>> A = {{1,4}};
            std::vector<std::vector<int>> B = {{1,4}};
            std::vector<std::vector<int>> expected = {{1,4}};
            return compareVectors(solution.intervalIntersection(A, B), expected);
        });

        // 最少箭数测试
        runTest("最少箭数 - 基本情况", [this]() {
            std::vector<std::vector<int>> points = {{10,16},{2,8},{1,6},{7,12}};
            return solution.findMinArrowShots(points) == 2;
        });

        runTest("最少箭数 - 空数组", [this]() {
            std::vector<std::vector<int>> points;
            return solution.findMinArrowShots(points) == 0;
        });

        runTest("最少箭数 - 单个气球", [this]() {
            std::vector<std::vector<int>> points = {{1,2}};
            return solution.findMinArrowShots(points) == 1;
        });

        runTest("最少箭数 - 完全重叠", [this]() {
            std::vector<std::vector<int>> points = {{1,2},{1,2},{1,2}};
            return solution.findMinArrowShots(points) == 1;
        });

        runTest("最少箭数 - 无重叠", [this]() {
            std::vector<std::vector<int>> points = {{1,2},{3,4},{5,6}};
            return solution.findMinArrowShots(points) == 3;
        });

        runTest("最少箭数 - 部分重叠", [this]() {
            std::vector<std::vector<int>> points = {{1,6},{2,8},{7,12},{10,16}};
            return solution.findMinArrowShots(points) == 2;
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
