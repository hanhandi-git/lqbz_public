# 电话号码的字母组合

## 问题链接
[LeetCode 17. 电话号码的字母组合](https://leetcode.com/problems/letter-combinations-of-a-phone-number/)

## 问题描述
给定一个仅包含数字2-9的字符串,返回所有它能表示的字母组合。答案可以按任意顺序返回。

给出数字到字母的映射如下(与电话按键相同)。注意1不对应任何字母。
```
2: abc
3: def
4: ghi
5: jkl
6: mno
7: pqrs
8: tuv
9: wxyz
```

示例:
输入: digits = "23"
输出: ["ad","ae","af","bd","be","bf","cd","ce","cf"]

## 解题思路
这是一个典型的回溯问题,可以通过以下步骤解决:

1. 首先建立数字到字母的映射表
2. 使用回溯法生成所有可能的组合:
   - 从输入的第一个数字开始
   - 对于当前数字对应的每个字母
   - 将该字母加入当前组合
   - 递归处理下一个数字
   - 回溯时移除最后添加的字母
3. 特殊情况处理:
   - 输入为空字符串时返回空vector
   - 输入只有一个数字时直接返回该数字对应的所有字母

## 代码实现
```cpp
class Solution {
private:
    vector<string> letterMap = {
        "",     // 0
        "",     // 1
        "abc",  // 2
        "def",  // 3
        "ghi",  // 4
        "jkl",  // 5
        "mno",  // 6
        "pqrs", // 7
        "tuv",  // 8
        "wxyz"  // 9
    };
    
    vector<string> result;
    string current;
    
    void backtrack(const string& digits, int index) {
        if (index == digits.length()) {
            result.push_back(current);
            return;
        }
        
        string letters = letterMap[digits[index] - '0'];
        for (char letter : letters) {
            current.push_back(letter);
            backtrack(digits, index + 1);
            current.pop_back();
        }
    }
    
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return {};
        
        result.clear();
        current.clear();
        backtrack(digits, 0);
        return result;
    }
};
```

## 复杂度分析
- 时间复杂度：O(4^N * N),其中N是输入的数字个数
  - 每个数字最多对应4个字母(7和9对应4个字母)
  - 需要生成所有可能的组合
  - 每个组合需要O(N)的时间来构建字符串
- 空间复杂度：O(N)
  - 递归调用栈的深度为N
  - 不考虑存储最终结果所需的空间

## 关键点
1. 使用回溯法生成所有可能的组合
2. 建立正确的数字到字母的映射关系
3. 处理特殊情况(空字符串输入)
4. 理解递归和回溯的过程
5. 注意字符串的操作效率