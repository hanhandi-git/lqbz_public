#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;

class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.empty() || matrix[0].empty()) return false;
        int n = matrix.size();
        int m = matrix[0].size();
        int left = 0;
        int right = n * m -1;
        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            int x = mid / m;    // 这里应该是列数
            int y = mid % m;
            if (target == matrix[x][y]) {
                return true;
            } else if (target > matrix[x][y]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return false; // 未找到目标值
    }
};

// 测试框架
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
        runTest("测试用例 1: matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 3", [this]() {
            vector<vector<int>> matrix = {{1, 3, 5, 7}, {10, 11, 16, 20}, {23, 30, 34, 60}};
            return solution.searchMatrix(matrix, 3) == true;
        });

        runTest("测试用例 2: matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 13", [this]() {
            vector<vector<int>> matrix = {{1, 3, 5, 7}, {10, 11, 16, 20}, {23, 30, 34, 60}};
            return solution.searchMatrix(matrix, 13) == false;
        });

        runTest("测试用例 3: matrix = [], target = 1", [this]() {
            vector<vector<int>> matrix = {};
            return solution.searchMatrix(matrix, 1) == false; // 空矩阵
        });

        runTest("测试用例 4: matrix = [[]], target = 1", [this]() {
            vector<vector<int>> matrix = {{}};
            return solution.searchMatrix(matrix, 1) == false; // 空行
        });

        runTest("测试用例 5: matrix = [[1]], target = 1", [this]() {
            vector<vector<int>> matrix = {{1}};
            return solution.searchMatrix(matrix, 1) == true; // 目标值在矩阵中
        });

        runTest("测试用例 6: matrix = [[1]], target = 2", [this]() {
            vector<vector<int>> matrix = {{1}};
            return solution.searchMatrix(matrix, 2) == false; // 目标值不在矩阵中
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