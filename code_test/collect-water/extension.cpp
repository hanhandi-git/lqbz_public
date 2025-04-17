#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <stack>
#include <queue>
#include <utility>

using namespace std;

class Solution {
public:
    // 1. 一维接雨水 - 动态规划解法
    int trap_dp(vector<int>& height) {
        if (height.empty()) return 0;
        
        int n = height.size();
        vector<int> leftMax(n), rightMax(n);
        
        leftMax[0] = height[0];
        for (int i = 1; i < n; i++) {
            leftMax[i] = max(leftMax[i-1], height[i]);
        }
        
        rightMax[n-1] = height[n-1];
        for (int i = n-2; i >= 0; i--) {
            rightMax[i] = max(rightMax[i+1], height[i]);
        }
        
        int water = 0;
        for (int i = 0; i < n; i++) {
            water += min(leftMax[i], rightMax[i]) - height[i];
        }
        
        return water;
    }

    // 2. 一维接雨水 - 双指针解法
    int trap_two_pointers(vector<int>& height) {
        int left = 0, right = height.size() - 1;
        int leftMax = 0, rightMax = 0;
        int water = 0;
        
        while (left < right) {
            if (height[left] < height[right]) {
                height[left] >= leftMax ? leftMax = height[left] : water += leftMax - height[left];
                left++;
            } else {
                height[right] >= rightMax ? rightMax = height[right] : water += rightMax - height[right];
                right--;
            }
        }
        
        return water;
    }

    // 3. 一维接雨水 - 单调栈解法
    int trap_stack(vector<int>& height) {
        stack<int> st;
        int water = 0;
        
        for (int i = 0; i < height.size(); i++) {
            while (!st.empty() && height[i] > height[st.top()]) {
                int top = st.top();
                st.pop();
                
                if (st.empty()) break;
                
                int distance = i - st.top() - 1;
                int bounded_height = min(height[i], height[st.top()]) - height[top];
                water += distance * bounded_height;
            }
            st.push(i);
        }
        
        return water;
    }

    // 4. 二维接雨水（太平洋大西洋水流问题）
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        if (heights.empty()) return {};
        
        int m = heights.size(), n = heights[0].size();
        vector<vector<bool>> pacific(m, vector<bool>(n, false));
        vector<vector<bool>> atlantic(m, vector<bool>(n, false));
        
        for (int i = 0; i < m; i++) {
            dfs(heights, pacific, i, 0, m, n);
            dfs(heights, atlantic, i, n-1, m, n);
        }
        for (int j = 0; j < n; j++) {
            dfs(heights, pacific, 0, j, m, n);
            dfs(heights, atlantic, m-1, j, m, n);
        }
        
        vector<vector<int>> result;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (pacific[i][j] && atlantic[i][j]) {
                    vector<int> point;
                    point.push_back(i);
                    point.push_back(j);
                    result.push_back(point);
                }
            }
        }
        return result;
    }

    // 5. 三维接雨水
    int trapRainWater(vector<vector<int>>& heightMap) {
        if (heightMap.empty()) return 0;
        
        int m = heightMap.size(), n = heightMap[0].size();
        typedef pair<int, pair<int, int>> PairType;
        priority_queue<PairType, vector<PairType>, greater<PairType>> pq;
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        
        // Add border cells
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (i == 0 || i == m-1 || j == 0 || j == n-1) {
                    pq.push(make_pair(heightMap[i][j], make_pair(i, j)));
                    visited[i][j] = true;
                }
            }
        }
        
        vector<pair<int, int>> dirs;
        dirs.push_back(make_pair(0, 1));
        dirs.push_back(make_pair(1, 0));
        dirs.push_back(make_pair(0, -1));
        dirs.push_back(make_pair(-1, 0));
        
        int water = 0;
        int maxHeight = 0;
        
        while (!pq.empty()) {
            PairType current = pq.top();
            pq.pop();
            int height = current.first;
            pair<int, int> pos = current.second;
            maxHeight = max(maxHeight, height);
            
            for (size_t i = 0; i < dirs.size(); i++) {
                int ni = pos.first + dirs[i].first;
                int nj = pos.second + dirs[i].second;
                if (ni >= 0 && ni < m && nj >= 0 && nj < n && !visited[ni][nj]) {
                    visited[ni][nj] = true;
                    if (heightMap[ni][nj] < maxHeight) {
                        water += maxHeight - heightMap[ni][nj];
                    }
                    pq.push(make_pair(heightMap[ni][nj], make_pair(ni, nj)));
                }
            }
        }
        
        return water;
    }

private:
    void dfs(vector<vector<int>>& heights, vector<vector<bool>>& visited, 
             int i, int j, int m, int n) {
        visited[i][j] = true;
        vector<pair<int, int>> dirs;
        dirs.push_back(make_pair(0, 1));
        dirs.push_back(make_pair(1, 0));
        dirs.push_back(make_pair(0, -1));
        dirs.push_back(make_pair(-1, 0));
        
        for (size_t k = 0; k < dirs.size(); k++) {
            int ni = i + dirs[k].first;
            int nj = j + dirs[k].second;
            if (ni >= 0 && ni < m && nj >= 0 && nj < n && 
                !visited[ni][nj] && heights[ni][nj] >= heights[i][j]) {
                dfs(heights, visited, ni, nj, m, n);
            }
        }
    }
};

// 测试框架
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
        // 一维接雨水测试
        runTest("一维接雨水 - 动态规划解法", [this]() -> bool {
            vector<int> height;
            height.push_back(0);
            height.push_back(1);
            height.push_back(0);
            height.push_back(2);
            height.push_back(1);
            height.push_back(0);
            height.push_back(1);
            height.push_back(3);
            height.push_back(2);
            height.push_back(1);
            height.push_back(2);
            height.push_back(1);
            return solution.trap_dp(height) == 6;
        });

        runTest("一维接雨水 - 双指针解法", [this]() -> bool {
            vector<int> height;
            height.push_back(0);
            height.push_back(1);
            height.push_back(0);
            height.push_back(2);
            height.push_back(1);
            height.push_back(0);
            height.push_back(1);
            height.push_back(3);
            height.push_back(2);
            height.push_back(1);
            height.push_back(2);
            height.push_back(1);
            return solution.trap_two_pointers(height) == 6;
        });

        runTest("一维接雨水 - 单调栈解法", [this]() -> bool {
            vector<int> height;
            height.push_back(0);
            height.push_back(1);
            height.push_back(0);
            height.push_back(2);
            height.push_back(1);
            height.push_back(0);
            height.push_back(1);
            height.push_back(3);
            height.push_back(2);
            height.push_back(1);
            height.push_back(2);
            height.push_back(1);
            return solution.trap_stack(height) == 6;
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
