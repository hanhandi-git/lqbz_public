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
        int n = height.size();
        if (n <= 0) return 0;
        std::vector<int> left(n, 0);
        std::vector<int> right(n, 0);
        left[0] = height[0];
        for (int i = 1; i < n; i++) {
            left[i] = std::max(left[i-1], height[i]);
        }
        right[n-1] = height[n-1];
        for (int i = n - 2; i >=0; i--) {
            right[i] = std::max(right[i+1], height[i]);
        }
        for (int i = 0; i < n; i++) {
            water += (std::min(left[i], right[i]) - height[i]);
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
