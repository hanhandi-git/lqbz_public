# 字符串解码

## 问题链接
[LeetCode 394. 字符串解码](https://leetcode.com/problems/decode-string/)

## 问题描述
给定一个经过编码的字符串，返回它解码后的字符串。

编码规则如下：
- k[encoded_string]，表示字符串 encoded_string 重复 k 次。
- 1 <= k <= 30，encoded_string 由小写字母、数字和方括号组成。

请你实现 `decodeString` 函数：
- `string decodeString(string s)`：解码字符串并返回结果。
## 逻辑推导流程
以下是解码字符串的逻辑推导流程：

1. **初始化**：
   - 创建两个栈：`counts` 用于存储重复次数，`result` 用于存储已构建的字符串。
   - 初始化 `currentString` 为一个空字符串，用于构建当前的字符串。
   - 初始化 `currentNum` 为 0，用于构建当前的数字。

2. **遍历字符串**：
   - 对于字符串中的每个字符 `c`：
     - **如果 `c` 是数字**：
       - 更新 `currentNum`，将其乘以 10 并加上当前数字，以处理多位数情况。
     - **如果 `c` 是 `[`**：
       - 将 `currentNum` 压入 `counts` 栈。
       - 将 `currentString` 压入 `result` 栈。
       - 重置 `currentNum` 和 `currentString` 为 0 和空字符串。
     - **如果 `c` 是 `]`**：
       - 创建一个临时字符串 `temp`，保存当前的 `currentString`。
       - 从 `result` 栈中弹出上一个字符串，并将其赋值给 `currentString`。
       - 从 `counts` 栈中弹出重复次数 `repeatCount`。
       - 将 `temp` 字符串重复 `repeatCount` 次并添加到 `currentString` 中。
     - **如果 `c` 是其他字符**：
       - 将字符 `c` 添加到 `currentString` 中。

3. **返回结果**：
   - 遍历结束后，`currentString` 中即为解码后的字符串，返回该字符串。

## 解题思路
1. 使用栈来处理嵌套的结构。
2. 遍历字符串：
   - 当遇到数字时，构建完整的数字（可能是多位数）。
   - 当遇到 `[` 时，将当前构建的数字和已构建的字符串压入栈中，并重置当前字符串和数字。
   - 当遇到 `]` 时，弹出栈顶的字符串和数字，构建当前字符串的重复部分。
   - 其他字符直接添加到当前字符串中。
3. 最终返回构建的字符串。

## 代码实现
```cpp
class Solution {
public:
    string decodeString(string s) {
        stack<int> counts; // 存储重复次数
        stack<string> result; // 存储已构建的字符串
        string currentString = ""; // 当前构建的字符串
        int currentNum = 0; // 当前数字

        for (char c : s) {
            if (isdigit(c)) {
                currentNum = currentNum * 10 + (c - '0'); // 构建完整的数字
            } else if (c == '[') {
                counts.push(currentNum); // 压入当前数字
                result.push(currentString); // 压入当前字符串
                currentNum = 0; // 重置当前数字
                currentString = ""; // 重置当前字符串
            } else if (c == ']') {
                string temp = currentString;
                currentString = result.top(); // 弹出上一个字符串
                result.pop();
                int repeatCount = counts.top(); // 弹出重复次数
                counts.pop();
                for (int i = 0; i < repeatCount; i++) {
                    currentString += temp; // 构建重复字符串
                }
            } else {
                currentString += c; // 添加字符到当前字符串
            }
        }

        return currentString; // 返回最终构建的字符串
    }
};
```

## 复杂度分析
- 时间复杂度：O(N)，其中 N 是字符串的长度。每个字符都被处理一次。
- 空间复杂度：O(K)，其中 K 是栈中存储的字符串和数字的数量，最坏情况下可能需要存储所有字符。



当然可以！我们可以通过一个具体的例子来解释这个解码字符串的逻辑流程。假设我们有一个编码字符串 `s = "3[a2[c]]"`，我们将按照你提供的逻辑推导流程逐步解析这个字符串。

### 示例字符串
```
s = "3[a2[c]]"
```

### 逻辑推导流程

1. **初始化**：
   - 创建两个栈：`counts` 和 `result`。
   - 初始化 `currentString` 为一个空字符串 `""`。
   - 初始化 `currentNum` 为 `0`。

2. **遍历字符串**：
   - 对于字符串中的每个字符 `c`：

   - **字符 '3'**：
     - `c` 是数字，更新 `currentNum`：`currentNum = 0 * 10 + 3 = 3`。

   - **字符 '['**：
     - `c` 是 `[`，将 `currentNum` 压入 `counts` 栈：`counts.push(3)`。
     - 将 `currentString` 压入 `result` 栈：`result.push("")`。
     - 重置 `currentNum` 和 `currentString` 为 `0` 和 `""`。

   - **字符 'a'**：
     - `c` 是其他字符，将 `c` 添加到 `currentString` 中：`currentString = "a"`。

   - **字符 '2'**：
     - `c` 是数字，更新 `currentNum`：`currentNum = 0 * 10 + 2 = 2`。

   - **字符 '['**：
     - `c` 是 `[`，将 `currentNum` 压入 `counts` 栈：`counts.push(2)`。
     - 将 `currentString` 压入 `result` 栈：`result.push("a")`。
     - 重置 `currentNum` 和 `currentString` 为 `0` 和 `""`。

   - **字符 'c'**：
     - `c` 是其他字符，将 `c` 添加到 `currentString` 中：`currentString = "c"`。

   - **字符 ']'**：
     - `c` 是 `]`，创建一个临时字符串 `temp`，保存当前的 `currentString`：`temp = "c"`。
     - 从 `result` 栈中弹出上一个字符串并赋值给 `currentString`：`currentString = "a"`。
     - 从 `counts` 栈中弹出重复次数 `repeatCount`：`repeatCount = 2`。
     - 将 `temp` 字符串重复 `repeatCount` 次并添加到 `currentString` 中：
       - `currentString += "c" * 2`，结果为 `currentString = "acc"`。

   - **字符 ']'**：
     - `c` 是 `]`，创建一个临时字符串 `temp`，保存当前的 `currentString`：`temp = "acc"`。
     - 从 `result` 栈中弹出上一个字符串并赋值给 `currentString`：`currentString = ""`。
     - 从 `counts` 栈中弹出重复次数 `repeatCount`：`repeatCount = 3`。
     - 将 `temp` 字符串重复 `repeatCount` 次并添加到 `currentString` 中：
       - `currentString += "acc" * 3`，结果为 `currentString = "accaccacc"`。

3. **返回结果**：
   - 遍历结束后，`currentString` 中即为解码后的字符串，返回该字符串：`"accaccacc"`。

### 最终结果
因此，字符串 `s = "3[a2[c]]"` 解码后的结果是 `"accaccacc"`。这个过程展示了如何通过栈来处理嵌套结构，并逐步构建最终的解码字符串。
