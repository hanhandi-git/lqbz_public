#include <iostream>
#include <functional>
#include <string>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
#include <utility>
using namespace std;

class Solution {
public:
    // 1. 一维接雨水 - 动态规划解法
    int trap_dp(vector<int>& height) {
        int water = 0;
        if (height.empty()) return 0;
        int n = height.size();
        std::vector<int> maxLeft(n, 0);
        std::vector<int> maxRight(n, 0);
        maxLeft[0] = height[0];
        maxRight[n-1] = height[n-1];
        for (int i = 1; i < n; i++)
        {
            maxLeft[i] = std::max(maxLeft[i-1], height[i]);
        }
        for (int i = n-2; i >= 0; i--)
        {
            maxRight[i] = std::max(maxRight[i+1], height[i]);
        }
        for (int i = 0; i < n; i++)
        {
            water += (std::min(maxLeft[i], maxRight[i]) - height[i]);
        }
        return water;
    }

    // 2. 一维接雨水 - 双指针解法
    int trap_two_pointers(vector<int>& height) {
        int left = 0, right = height.size() - 1;
        int maxLeft = 0, maxRight = 0;
        int water = 0;
        while (left < right)
        {
            if (height[left] < height[right])
            {
                if (height[left] >= maxLeft)
                {
                    maxLeft = height[left];
                }
                water += maxLeft - height[left];
                left++;
            }
            else
            {
                if (height[right] >= maxRight)
                {
                    maxRight = height[right];
                }
                water += maxRight - height[right];
                right--;
            }
        }
        return water;
    }

    /**
     * @brief 使用单调栈解法解决接雨水问题
     * @details 通过维护一个单调递减栈，当遇到比栈顶元素高的柱子时，就可以计算积水
     * @param height 柱子高度数组
     * @return 可以接的雨水总量
     */
        int trap_stack(vector<int>& height) {
        // 单调栈存储的是下标，栈中下标对应的高度是单调递减的
        stack<int> st;
        int water = 0;
        
        // 遍历每个柱子
        for (int i = 0; i < height.size(); i++) {
            // 当栈不为空且当前柱子高度大于栈顶柱子高度时
            // 说明找到了一个可以接雨水的凹槽
            while (!st.empty() && height[i] > height[st.top()]) {
                // 取出凹槽底部的高度
                int top = st.top();
                st.pop();
                
                // 如果栈为空，说明左边没有柱子了，无法接水
                if (st.empty()) break;
                
                // 计算凹槽的宽度：当前柱子下标 - 左边柱子下标 - 1
                int distance = i - st.top() - 1;
                // 计算凹槽的高度：两边柱子的最小高度 - 凹槽底部高度
                int bounded_height = min(height[i], height[st.top()]) - height[top];
                // 累加这个凹槽可以接的水量
                water += distance * bounded_height;
            }
            // 将当前柱子入栈，继续维护单调递减特性
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
                    result.push_back(vector<int>{i, j});
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
        // 1. 测试一维接雨水 - 动态规划解法
        runTest("一维接雨水(DP) - 示例1", [this]() -> bool {
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

        runTest("一维接雨水(DP) - 示例2", [this]() -> bool {
            vector<int> height;
            height.push_back(4);
            height.push_back(2);
            height.push_back(0);
            height.push_back(3);
            height.push_back(2);
            height.push_back(5);
            return solution.trap_dp(height) == 9;
        });

        // 边界情况测试
        runTest("一维接雨水(DP) - 空数组", [this]() -> bool {
            vector<int> height;
            return solution.trap_dp(height) == 0;
        });

        runTest("一维接雨水(DP) - 单个元素", [this]() -> bool {
            vector<int> height;
            height.push_back(1);
            return solution.trap_dp(height) == 0;
        });

        runTest("一维接雨水(DP) - 两个元素", [this]() -> bool {
            vector<int> height;
            height.push_back(1);
            height.push_back(2);
            return solution.trap_dp(height) == 0;
        });

        // 2. 测试一维接雨水 - 双指针解法
        runTest("一维接雨水(双指针) - 示例1", [this]() -> bool {
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

        runTest("一维接雨水(双指针) - 全相等高度", [this]() -> bool {
            vector<int> height;
            height.push_back(5);
            height.push_back(5);
            height.push_back(5);
            height.push_back(5);
            return solution.trap_two_pointers(height) == 0;
        });

        // 3. 测试一维接雨水 - 单调栈解法
        runTest("一维接雨水(单调栈) - 示例1", [this]() -> bool {
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

        // 4. 测试二维接雨水（太平洋大西洋水流）
        runTest("二维接雨水 - 示例1", [this]() -> bool {
            vector<vector<int>> heights;
            vector<int> row1; row1.push_back(1); row1.push_back(2); row1.push_back(2); row1.push_back(3); row1.push_back(5);
            vector<int> row2; row2.push_back(3); row2.push_back(2); row2.push_back(3); row2.push_back(4); row2.push_back(4);
            vector<int> row3; row3.push_back(2); row3.push_back(4); row3.push_back(5); row3.push_back(3); row3.push_back(1);
            vector<int> row4; row4.push_back(6); row4.push_back(7); row4.push_back(1); row4.push_back(4); row4.push_back(5);
            vector<int> row5; row5.push_back(5); row5.push_back(1); row5.push_back(1); row5.push_back(2); row5.push_back(4);
            heights.push_back(row1);
            heights.push_back(row2);
            heights.push_back(row3);
            heights.push_back(row4);
            heights.push_back(row5);
            
            vector<vector<int>> expected;
            vector<int> point1; point1.push_back(0); point1.push_back(4); expected.push_back(point1);
            vector<int> point2; point2.push_back(1); point2.push_back(3); expected.push_back(point2);
            vector<int> point3; point3.push_back(1); point3.push_back(4); expected.push_back(point3);
            vector<int> point4; point4.push_back(2); point4.push_back(2); expected.push_back(point4);
            vector<int> point5; point5.push_back(3); point5.push_back(0); expected.push_back(point5);
            vector<int> point6; point6.push_back(3); point6.push_back(1); expected.push_back(point6);
            vector<int> point7; point7.push_back(4); point7.push_back(0); expected.push_back(point7);
            
            return solution.pacificAtlantic(heights) == expected;
        });

        // 5. 测试三维接雨水
        runTest("三维接雨水 - 示例1", [this]() -> bool {
            vector<vector<int>> heightMap;
            vector<int> row1; row1.push_back(1); row1.push_back(4); row1.push_back(3); row1.push_back(1); row1.push_back(3); row1.push_back(2);
            vector<int> row2; row2.push_back(3); row2.push_back(2); row2.push_back(1); row2.push_back(3); row2.push_back(2); row2.push_back(4);
            vector<int> row3; row3.push_back(2); row3.push_back(3); row3.push_back(3); row3.push_back(2); row3.push_back(3); row3.push_back(1);
            heightMap.push_back(row1);
            heightMap.push_back(row2);
            heightMap.push_back(row3);
            return solution.trapRainWater(heightMap) == 4;
        });

        runTest("三维接雨水 - 边界情况", [this]() -> bool {
            vector<vector<int>> heightMap;
            vector<int> row1; row1.push_back(1); row1.push_back(1); row1.push_back(1);
            vector<int> row2; row2.push_back(1); row2.push_back(1); row2.push_back(1);
            heightMap.push_back(row1);
            heightMap.push_back(row2);
            return solution.trapRainWater(heightMap) == 0;
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
