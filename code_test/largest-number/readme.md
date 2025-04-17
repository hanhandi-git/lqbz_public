# 滑动窗口最大值

## 问题链接
最大数

## 问题描述

给定一组非负整数 `nums`，重新排列每个数的顺序（每个数不可拆分）使之组成一个最大的整数。

注意：输出结果可能非常大，所以你需要返回一个字符串而不是整数。

### 示例 1:

输入：nums = [10,2]
输出："210"

### 示例 2:

输入：nums = [3,30,34,5,9]
输出："9534330"

### 提示：

- 1 <= nums.length <= 100
- 0 <= nums[i] <= 10^9

## 解题思路

1. 将数组中的所有整数转换为字符串。
2. 定义一个比较函数，用于比较两个字符串的组合哪个更大。
3. 使用自定义的比较函数对字符串数组进行排序。
4. 将排序后的字符串数组连接起来，形成最终结果。
5. 特殊情况处理：如果结果的第一个字符是 '0'，则返回 "0"。

## 代码实现

这里可以添加具体的代码实现，可以是 C++、Python 或其他语言的版本。

## 复杂度分析

- 时间复杂度：O(nlogn)，其中 n 是数组 nums 的长度。排序的时间复杂度为 O(nlogn)，比较两个字符串的时间复杂度为 O(k)，其中 k 是字符串的平均长度。
- 空间复杂度：O(n)，用于存储字符串数组。


问题的关键是排序函数的定义
```cpp
std::sort(numStrs.begin(), numStrs.end(), [](const std::string& a, const std::string& b) {
            return a + b > b + a;
        });
```
1. 比较逻辑：这个比较函数比较的是两个字符串 a 和 b 拼接后的结果。它检查 "a+b" 是否大于 "b+a"。
2. 为什么这样比较：
对于最大数问题，我们需要找到一种排序方式，使得拼接后的数字最大。
通过比较 "a+b" 和 "b+a"，我们可以直接判断哪种拼接顺序会产生更大的数字。
3. 举例说明：
假设 a = "3", b = "30"
"3" + "30" = "330"
"30" + "3" = "303"
显然 "330" > "303"，所以在最终结果中，"3" 应该排在 "30" 前面
4. 优势：
这种比较方法可以处理各种情况，包括不同长度的数字字符串。
它确保了最终拼接的结果是最大的可能数字。

```cpp
class Solution {
public:
    std::string largestNumber(std::vector<int>& nums) {
        std::vector<std::string> numStrs;
        for (int num : nums) {
            numStrs.push_back(std::to_string(num));
        }
        
        std::sort(numStrs.begin(), numStrs.end(), [](const std::string& a, const std::string& b) {
            return a + b > b + a;
        });
        
        if (numStrs[0] == "0") {
            return "0";
        }
        
        std::string result;
        for (const std::string& numStr : numStrs) {
            result += numStr;
        }
        
        return result;
    }
};
```