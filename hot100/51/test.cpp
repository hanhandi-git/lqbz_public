#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>

class Solution {
private:
    std::vector<std::vector<std::string>> result;
    
    bool isValid(std::vector<int>& queens, int row, int col) {
        for (int i = 0; i < row; i++) {
            if (queens[i] == col) return false;
            if (abs(row - i) == abs(col - queens[i])) return false;
        }
        return true;
    }
    
    std::vector<std::string> generateBoard(std::vector<int>& queens, int n) {
        std::vector<std::string> board(n, std::string(n, '.'));
        for (int i = 0; i < n; i++) {
            board[i][queens[i]] = 'Q';
        }
        return board;
    }
    
    void backtrack(std::vector<int>& queens, int row, int n) {
        if (row == n) {
            result.push_back(generateBoard(queens, n));
            return;
        }
        
        for (int col = 0; col < n; col++) {
            if (isValid(queens, row, col)) {
                queens[row] = col;
                backtrack(queens, row + 1, n);
                queens[row] = -1;
            }
        }
    }
    
public:
    std::vector<std::vector<std::string>> solveNQueens(int n) {
        result.clear();
        std::vector<int> queens(n, -1);
        backtrack(queens, 0, n);
        return result;
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

    bool expectEqual(const std::vector<std::vector<std::string>>& expected, 
                    const std::vector<std::vector<std::string>>& actual) {
        if (expected.size() != actual.size()) return false;
        
        auto sortedExpected = expected;
        auto sortedActual = actual;
        
        std::sort(sortedExpected.begin(), sortedExpected.end());
        std::sort(sortedActual.begin(), sortedActual.end());
        
        return sortedExpected == sortedActual;
    }

public:
    void runAllTests() {
        runTest("N=4 测试", [this]() {
            std::vector<std::vector<std::string>> expected = {
                {".Q..","...Q","Q...","..Q."},
                {"..Q.","Q...","...Q",".Q.."}
            };
            return expectEqual(expected, solution.solveNQueens(4));
        });

        runTest("N=1 测试", [this]() {
            std::vector<std::vector<std::string>> expected = {{"Q"}};
            return expectEqual(expected, solution.solveNQueens(1));
        });

        runTest("N=2 测试 - 无解", [this]() {
            std::vector<std::vector<std::string>> expected = {};
            return expectEqual(expected, solution.solveNQueens(2));
        });

        runTest("N=3 测试 - 无解", [this]() {
            std::vector<std::vector<std::string>> expected = {};
            return expectEqual(expected, solution.solveNQueens(3));
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