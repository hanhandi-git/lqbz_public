#include <iostream>
#include <vector>
#include <string>
#include <functional>
using namespace std;
// Solution类实现课程表问题的解决方案
class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        // 构造邻接表
        std::vector<std::vector<int>> graph(numCourses);
        // 填充
        for (auto pre : prerequisites)
        {
            graph[pre[0]].push_back(pre[1]);
        }
        std::vector<int> visited(numCourses, 0);
        for (int i = 0; i < visited.size(); i++)
        {
            if (visited[i] == 0 && !dfs(graph, visited, i)) return false;
        }
        return true;
    }
    
private:
    bool dfs(vector<vector<int>>& graph, vector<int>& visited, int curr) {
        if (visited[curr] == 1) return false;
        if (visited[curr] == 2) return true;
        visited[curr] = 1;
        // 递归访问所有邻近
        for (auto next : graph[curr])
        {
            if (!dfs(graph, visited, next)) return false;
        }
        visited[curr] = 2;
        return true;
    }
};

class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    void runTest(const string& testName, function<bool()> test) {
        total++;
        if (test()) {
            cout << "通过: " << testName << endl;
            passed++;
        } else {
            cout << "失败: " << testName << endl;
        }
    }

public:
    void runAllTests() {
        // 测试用例1: 无环的情况
        runTest("测试用例1 - 无环图", [this]() {
            int numCourses = 2;
            vector<vector<int>> prerequisites = {{1,0}};
            return solution.canFinish(numCourses, prerequisites);
        });

        // 测试用例2: 有环的情况
        runTest("测试用例2 - 有环图", [this]() {
            int numCourses = 2;
            vector<vector<int>> prerequisites = {{1,0}, {0,1}};
            return !solution.canFinish(numCourses, prerequisites);
        });

        // 测试用例3: 空输入
        runTest("测试用例3 - 空依赖关系", [this]() {
            int numCourses = 1;
            vector<vector<int>> prerequisites;
            return solution.canFinish(numCourses, prerequisites);
        });

        // 测试用例4: 复杂依赖关系
        runTest("测试用例4 - 复杂依赖关系", [this]() {
            int numCourses = 4;
            vector<vector<int>> prerequisites = {{1,0}, {2,1}, {3,2}};
            return solution.canFinish(numCourses, prerequisites);
        });

        // 测试用例5: 多个独立的环
        runTest("测试用例5 - 多个环", [this]() {
            int numCourses = 4;
            vector<vector<int>> prerequisites = {{1,0}, {0,1}, {3,2}, {2,3}};
            return !solution.canFinish(numCourses, prerequisites);
        });

        // 测试用例6: 单个课程
        runTest("测试用例6 - 单个课程", [this]() {
            int numCourses = 1;
            vector<vector<int>> prerequisites;
            return solution.canFinish(numCourses, prerequisites);
        });

        cout << "\n测试结果: " << passed << " 通过, " 
             << (total - passed) << " 失败, " 
             << total << " 总计" << endl;
    }
};

int main() {
    TestFramework testFramework;
    testFramework.runAllTests();
    return 0;
}