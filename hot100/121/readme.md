## 问题链接
https://leetcode.cn/problems/best-time-to-buy-and-sell-stock/

## 问题描述
给定一个数组 `prices` ，其中 `prices[i]` 表示某支股票第 `i` 天的价格。你最多可以进行一次交易（即买入和卖出一支股票），请你计算可以获得的最大利润。如果你不能获得任何利润，返回 0。

### 示例 1：
输入：
```
prices = [7, 1, 5, 3, 6, 4]
```
输出：5
解释：在第 2 天（股票价格 = 1）买入，在第 5 天（股票价格 = 6）卖出，最大利润 = 6 - 1 = 5。

### 示例 2：
输入：
```
prices = [7, 6, 4, 3, 1]
```
输出：0
解释：在这种情况下, 没有交易完成, 所以最大利润为 0。

### 提示：
- `1 <= prices.length <= 10^5`
- `0 <= prices[i] <= 10^4`

## 解题思路

### 方法一：一次遍历
为了找到最大利润，我们可以使用一次遍历的方法。具体步骤如下：

1. 初始化两个变量 `min_price` 和 `max_profit`，分别用于存储当前最低价格和最大利润。
2. 遍历价格数组，对于每一天的价格：
   - 更新 `min_price` 为当前价格和 `min_price` 的较小值。
   - 计算当前利润为当前价格减去 `min_price`。
   - 更新 `max_profit` 为当前利润和 `max_profit` 的较大值。
3. 返回 `max_profit`。

### 代码实现

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int min_price = INT_MAX;  // 初始化为最大值
        int max_profit = 0;  // 初始化为0

        for (int price : prices) {
            min_price = min(min_price, price);  // 更新最低价格
            max_profit = max(max_profit, price - min_price);  // 更新最大利润
        }

        return max_profit;
    }
};
```

## 复杂度分析

### 时间复杂度
- O(n)：每个价格只被访问一次。

### 空间复杂度
- O(1)：只使用了常数级别的额外空间。

## 优化技巧

1. 提前处理特殊情况：
   - 如果价格数组为空，直接返回0。
   
2. 只需一次遍历，避免不必要的重复计算。

3. 使用常数空间存储最低价格和最大利润，提升空间效率。

## 相关题目
- 122. 买卖股票的最佳时机 II
- 123. 买卖股票的最佳时机 III
- 188. 买卖股票的最佳时机 IV

这些题目都涉及到股票交易的策略，值得一起学习和比较。
