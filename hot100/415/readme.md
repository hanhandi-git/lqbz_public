# 字符串相加

## 问题链接
[LeetCode 415. 字符串相加](https://leetcode.com/problems/add-strings/)

## 问题描述
给定两个字符串形式的非负整数 num1 和 num2，计算它们的和并同样以字符串形式返回。

你不能使用任何内建的大数类型（如 BigInteger）或直接将输入转换为整数来处理。

## 解题思路
1. 模拟人工加法运算的过程，从右往左逐位相加。
2. 维护一个进位变量，记录每次相加后的进位情况。
3. 将结果逐位添加到结果字符串中，最后反转结果字符串。

### 算法流程
1. 从两个字符串的末尾开始，逐位相加。
2. 计算当前位的和，包括两个数字和进位。
3. 将当前位的结果添加到结果字符串中，并更新进位。
4. 继续处理下一位，直到两个字符串都处理完毕。
5. 如果最后还有进位，将进位添加到结果字符串中。
6. 反转结果字符串并返回。

## 代码实现
```cpp
class Solution {
public:
    string addStrings(string num1, string num2) {
        string result = "";
        int carry = 0;
        int i = num1.length() - 1;
        int j = num2.length() - 1;
        
        while (i >= 0 || j >= 0 || carry > 0) {
            int x = (i >= 0) ? (num1[i] - '0') : 0;
            int y = (j >= 0) ? (num2[j] - '0') : 0;
            int sum = x + y + carry;
            
            result += to_string(sum % 10);
            carry = sum / 10;
            
            i--;
            j--;
        }
        
        reverse(result.begin(), result.end());
        return result;
    }
};
```

## 复杂度分析
- 时间复杂度：O(max(n, m))，其中 n 和 m 分别是两个字符串的长度。
- 空间复杂度：O(max(n, m))，用于存储结果字符串。

## 示例说明
让我们通过一个具体的例子来说明算法的工作过程：

输入：
- num1 = "123"
- num2 = "456"

过程：
1. 从末尾开始，处理 '3' 和 '6'：3 + 6 = 9，进位 = 0，结果 = "9"
2. 处理 '2' 和 '5'：2 + 5 = 7，进位 = 0，结果 = "79"
3. 处理 '1' 和 '4'：1 + 4 = 5，进位 = 0，结果 = "579"
4. 反转结果字符串："579"

输出："579"

这个例子展示了如何通过模拟加法运算来实现字符串相加。

## 优化思路
1. **直接构建结果字符串**：可以预先计算结果字符串的长度，然后直接构建结果字符串，避免反转操作。
2. **使用 StringBuilder**：在某些语言中，可以使用 StringBuilder 来提高字符串拼接的效率。
3. **并行处理**：对于非常长的字符串，可以考虑并行处理不同部分，然后合并结果。

## 注意事项
1. 处理进位时，需要考虑最高位的进位情况。
2. 处理两个字符串长度不同的情况，可以将短字符串视为在前面补零。
3. 确保结果字符串不包含前导零，除非结果本身为零。

## 相关题目
- [LeetCode 2. 两数相加](https://leetcode.com/problems/add-two-numbers/)
- [LeetCode 43. 字符串相乘](https://leetcode.com/problems/multiply-strings/)
- [LeetCode 67. 二进制求和](https://leetcode.com/problems/add-binary/)