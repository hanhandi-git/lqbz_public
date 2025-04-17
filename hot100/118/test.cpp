#include <vector>
#include <iostream>
#include <functional>

using namespace std;

class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> triangle(numRows);
        for (int i = 0; i < numRows; i++)
        {
            triangle[i].resize(i+1);
            triangle[i][0] = triangle[i][i] = 1;
            for (int j = 1; j < i; j++)     // 每行元素个数=i+1
            {
                triangle[i][j] = triangle[i-1][j-1] + triangle[i-1][j];
            }
        }
        return triangle;
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
        // 测试用例1: numRows = 5
        runTest("示例1: numRows = 5", [this]() {
            vector<vector<int>> expected = {
                {1},
                {1, 1},
                {1, 2, 1},
                {1, 3, 3, 1},
                {1, 4, 6, 4, 1}
            };
            return solution.generate(5) == expected;
        });

        // 测试用例2: numRows = 1
        runTest("示例2: numRows = 1", [this]() {
            vector<vector<int>> expected = {
                {1}
            };
            return solution.generate(1) == expected;
        });

        // 测试用例3: numRows = 0
        runTest("边界情况: numRows = 0", [this]() {
            vector<vector<int>> expected = {};
            return solution.generate(0) == expected;
        });

        // 测试用例4: numRows = 2
        runTest("numRows = 2", [this]() {
            vector<vector<int>> expected = {
                {1},
                {1, 1}
            };
            return solution.generate(2) == expected;
        });

        // 测试用例5: numRows = 3
        runTest("numRows = 3", [this]() {
            vector<vector<int>> expected = {
                {1},
                {1, 1},
                {1, 2, 1}
            };
            return solution.generate(3) == expected;
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