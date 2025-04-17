/**
 * @file container_with_most_water.cpp
 * @brief 盛最多水的容器问题的测试文件
 */

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;

class Solution {
public:
    /**
     * @brief 计算能够盛放的最大水量
     * @param height 高度数组
     * @return 最大水量
     */
    int maxArea(vector<int>& height) {
        int maxArea = 0;
        int left = 0;
        int right = height.size() - 1;
        
        while (left < right) {
            int width = right - left;
            int area = min(height[left], height[right]) * width;
            maxArea = max(maxArea, area);
            
            if (height[left] < height[right]) {
                left++;
            } else {
                right--;
            }
        }
        
        return maxArea;
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
        // 基本测试
        runTest("基本测试", [this]() {
            vector<int> height = {1, 1};
            return solution.maxArea(height) == 1;
        });

        // 相同高度测试
        runTest("相同高度测试", [this]() {
            vector<int> height = {4, 4, 4, 4};
            return solution.maxArea(height) == 12;
        });

        // 单调递增序列测试
        runTest("单调递增序列测试", [this]() {
            vector<int> height = {1, 2, 3, 4, 5};
            return solution.maxArea(height) == 6;
        });

        // 单调递减序列测试
        runTest("单调递减序列测试", [this]() {
            vector<int> height = {5, 4, 3, 2, 1};
            return solution.maxArea(height) == 6;
        });

        // 对称容器测试
        runTest("对称容器测试", [this]() {
            vector<int> height = {1, 8, 6, 2, 5, 4, 8, 3, 7};
            return solution.maxArea(height) == 49;
        });

        // 最大容积在两端测试
        runTest("最大容积在两端测试", [this]() {
            vector<int> height = {4, 3, 2, 1, 4};
            return solution.maxArea(height) == 16;
        });

        // 多个相同容积测试
        runTest("多个相同容积测试", [this]() {
            vector<int> height = {3, 3, 3, 3};
            return solution.maxArea(height) == 9;
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