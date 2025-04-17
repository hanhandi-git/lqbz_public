#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>
using namespace std;
class Solution {
private:
    bool dfs(vector<vector<char>>& board, string& word, int i, int j, int k) {
        if (k == word.length()) return true;
        if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size() || board[i][j] != word[k]) return false;
        
        char temp = board[i][j];
        board[i][j] = '#';  // 标记已访问
        
        bool result = dfs(board, word, i+1, j, k+1) || 
                     dfs(board, word, i-1, j, k+1) ||
                     dfs(board, word, i, j+1, k+1) || 
                     dfs(board, word, i, j-1, k+1);
                     
        board[i][j] = temp;  // 恢复现场
        return result;
    }
    
public:
    bool exist(vector<vector<char>>& board, string word) {
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[0].size(); j++) {
                if (dfs(board, word, i, j, 0)) return true;
            }
        }
        return false;
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
        runTest("示例1测试", [this]() {
            vector<vector<char>> board = {
                {'A','B','C','E'},
                {'S','F','C','S'},
                {'A','D','E','E'}
            };
            string word = "ABCCED";
            return solution.exist(board, word) == true;
        });

        runTest("示例2测试", [this]() {
            vector<vector<char>> board = {
                {'A','B','C','E'},
                {'S','F','C','S'},
                {'A','D','E','E'}
            };
            string word = "SEE";
            return solution.exist(board, word) == true;
        });

        runTest("示例3测试", [this]() {
            vector<vector<char>> board = {
                {'A','B','C','E'},
                {'S','F','C','S'},
                {'A','D','E','E'}
            };
            string word = "ABCB";
            return solution.exist(board, word) == false;
        });

        runTest("空字符串测试", [this]() {
            vector<vector<char>> board = {{'A'}};
            string word = "";
            return solution.exist(board, word) == true;
        });

        runTest("单个字符测试", [this]() {
            vector<vector<char>> board = {{'A'}};
            string word = "A";
            return solution.exist(board, word) == true;
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