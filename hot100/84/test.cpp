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
    int largestRectangleArea(vector<int>& heights) {
        std::stack<int> st;
        int maxArea = 0;
        heights.push_back(0);   // 防止跑到最后一个元素时，栈还有数据
        for (int i = 0; i < heights.size(); i++)
        {
            while (!st.empty() && heights[i] < heights[st.top()])
            {
                int index = st.top();
                st.pop();
                int wid;
                if (st.empty()) {
                    wid = i;
                } else {
                    // 栈顶元素的索引，表示在当前柱子之前的一个柱子，它的高度大于或等于当前柱子的高度
                    wid = i - st.top() - 1; // 计算宽度为当前索引与栈顶元素的索引之间的距离
                }
                maxArea = std::max(maxArea, wid * heights[index]);
                // std::cout << "maxArea = " << maxArea << ", wid * heights[index] = " << wid * heights[index] << std::endl;
            }
            st.push(i);
        }
        return maxArea;
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
        runTest("测试用例 1: [2, 1, 5, 6, 2, 3]", [&]() {
            vector<int> heights = {2, 1, 5, 6, 2, 3};
            return solution.largestRectangleArea(heights) == 10; // 最大矩形面积为 10
        });

        runTest("测试用例 2: [2, 4]", [&]() {
            vector<int> heights = {2, 4};
            return solution.largestRectangleArea(heights) == 4; // 最大矩形面积为 4
        });

        runTest("测试用例 3: [1, 1, 1, 1]", [&]() {
            vector<int> heights = {1, 1, 1, 1};
            return solution.largestRectangleArea(heights) == 4; // 最大矩形面积为 4
        });

        runTest("测试用例 4: [0, 0, 0, 0]", [&]() {
            vector<int> heights = {0, 0, 0, 0};
            return solution.largestRectangleArea(heights) == 0; // 最大矩形面积为 0
        });

        runTest("测试用例 5: [5, 4, 1, 2]", [&]() {
            vector<int> heights = {5, 4, 1, 2};
            return solution.largestRectangleArea(heights) == 8; // 最大矩形面积为 8
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
