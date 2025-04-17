````thinking
让我思考一下如何修改这个readme来介绍单词搜索问题...

首先需要理解79题的关键点：
1. 这是一个典型的DFS + 回溯问题
2. 需要从矩阵中的每个点开始尝试搜索
3. 需要避免重复访问已经使用过的字符
4. 需要考虑边界条件和终止条件

解题思路的重点：
- 如何在矩阵中移动(四个方向)
- 如何标记已访问的位置
- 什么时候可以提前返回
- 回溯时如何恢复状态

代码实现需要注意：
- 如何高效判断边界
- 如何优雅处理四个方向的移动
- 如何避免重复计算
- 如何处理特殊情况(空字符串等)

让我基于这些思考来修改readme...
````

以下是修改后的readme.md内容：

# 单词搜索

## 问题链接
https://leetcode.cn/problems/word-search/

## 问题描述
给定一个 `m x n` 二维字符网格 `board` 和一个字符串单词 `word` 。如果 `word` 存在于网格中，返回 `true` ；否则，返回 `false` 。

单词必须按照字母顺序，通过相邻的单元格内的字母构成，其中"相邻"单元格是那些水平相邻或垂直相邻的单元格。同一个单元格内的字母不允许被重复使用。

### 示例 1：
```
输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCCED"
输出：true
```

### 示例 2：
```
输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "SEE"
输出：true
```

### 示例 3：
```
输入：board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCB"
输出：false
```

### 提示：
- m == board.length
- n = board[i].length
- 1 <= m, n <= 6
- 1 <= word.length <= 15
- board 和 word 仅由大小写英文字母组成

## 解题思路

这是一个典型的深度优先搜索(DFS)加回溯的问题。主要思路是：
1. 遍历矩阵中的每个位置，尝试从该位置开始匹配单词
2. 对于每个位置，向四个方向（上、下、左、右）继续搜索下一个字符
3. 使用回溯法，当发现当前路径不通时，回退到上一步尝试其他方向
4. 为了避免重复使用字符，需要标记已访问的位置

### 搜索过程
1. 从矩阵中找到和单词第一个字符相同的位置
2. 从该位置开始，依次向四个方向搜索下一个字符
3. 如果找到一个完整的路径，返回true
4. 如果所有可能的路径都搜索完毕还没找到，返回false

## 代码实现

```cpp
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
```

## 复杂度分析

- 时间复杂度：O(M×N×3^L)
  - M,N 是矩阵的行数和列数
  - L 是单词的长度
  - 对于每个位置，除了来源方向，还需要搜索3个方向
  - 每个位置最多需要搜索3^L次

- 空间复杂度：O(L)
  - L 是单词的长度
  - 主要是递归调用栈的深度

## 优化思路

1. 提前剪枝：
   - 可以先统计矩阵中每个字符的出现次数
   - 如果某个字符在word中出现的次数多于矩阵中的次数，直接返回false

2. 方向数组优化：
   - 使用方向数组来简化四个方向的移动
   - `int dx[4] = {-1, 1, 0, 0}`
   - `int dy[4] = {0, 0, -1, 1}`

3. 内存优化：
   - 可以直接修改原矩阵标记访问过的位置
   - 回溯时恢复原值

4. 搜索顺序优化：
   - 可以根据word的第一个和最后一个字符选择搜索方向
   - 从较少的起点开始搜索

## 相关题目
- [212. 单词搜索 II](https://leetcode.cn/problems/word-search-ii/) - 需要同时搜索多个单词
- [130. 被围绕的区域](https://leetcode.cn/problems/surrounded-regions/) - 类似的矩阵DFS问题
- [200. 岛屿数量](https://leetcode.cn/problems/number-of-islands/) - 经典的矩阵DFS问题
