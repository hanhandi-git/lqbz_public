# 买卖股票的最佳时机

## 问题链接
[LeetCode 121. 买卖股票的最佳时机](https://leetcode.com/problems/best-time-to-buy-and-sell-stock/)

## 问题描述
给定一个数组 prices ，它的第 i 个元素 prices[i] 表示一支给定股票第 i 天的价格。

你只能选择 某一天 买入这只股票，并选择在 未来的某一个不同的日子 卖出该股票。设计一个算法来计算你所能获取的最大利润。

返回你可以从这笔交易中获取的最大利润。如果你不能获取任何利润，返回 0 。

## 解题思路
1. 使用一次遍历，记录到目前为止的最低价格
2. 对于每一天的价格：
   - 更新历史最低价格
   - 计算当天卖出的利润（当天价格减去历史最低价格）
   - 更新最大利润
3. 最终返回最大利润

## 代码实现
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty()) return 0;
        
        int minPrice = INT_MAX;  // 记录历史最低价格
        int maxProfit = 0;       // 记录最大利润
        
        for (int price : prices) {
            minPrice = min(minPrice, price);  // 更新历史最低价格
            maxProfit = max(maxProfit, price - minPrice);  // 更新最大利润
        }
        
        return maxProfit;
    }
};
```

## 复杂度分析
- 时间复杂度：O(n)，其中 n 是价格数组的长度
- 空间复杂度：O(1)，只使用了常数额外空间

## 扩展：其他股票问题
1. 可以进行多次交易（无限次交易）
2. 最多进行两次交易
3. 最多进行k次交易
4. 含冷冻期的股票交易
5. 含手续费的股票交易

解题思路变体：
* 对于多次交易：贪心法，只要有利润就进行交易
* 对于有限次交易：使用动态规划，状态包含交易次数
* 对于特殊限制：在状态转移时考虑冷冻期或手续费

## 扩展问题详解

### 1. 股票问题 II - 可以进行多次交易
#### 问题链接
[LeetCode 122. 买卖股票的最佳时机 II](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-ii/)

#### 问题描述
给你一个整数数组 prices ，其中 prices[i] 表示某支股票第 i 天的价格。

在每一天，你可以决定是否购买和/或出售股票。你在任何时候 最多 只能持有 一股 股票。你也可以先购买，然后在 同一天 出售。

返回 你能获得的 最大 利润。

#### 解题思路
- 只要后一天的价格高于前一天，就进行买卖
- 等价于计算所有上涨区间的利润之和

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int totalProfit = 0;
        
        for (int i = 1; i < prices.size(); i++) {
            if (prices[i] > prices[i-1]) {
                totalProfit += prices[i] - prices[i-1];
            }
        }
        
        return totalProfit;
    }
};
```

### 2. 股票问题 III - 最多两次交易
#### 问题链接
[LeetCode 123. 买卖股票的最佳时机 III](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iii/)

#### 问题描述
给定一个数组，它的第 i 个元素是一支给定的股票在第 i 天的价格。

设计一个算法来计算你所能获取的最大利润。你最多可以完成 两笔 交易。

注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。

#### 解题思路
- 使用动态规划，状态包含当前天数和交易次数
- 维护四个变量：第一次买入、第一次卖出、第二次买入、第二次卖出

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
    // 初始化四个状态变量
        int firstBuy = INT_MIN;   // 第一次买入后的收益 表示还没有进行过购买
        int firstSell = 0;        // 第一次卖出后的收益 表示还没有进行过交易
        int secondBuy = INT_MIN;  // 第二次买入后的收益 表示还没有进行过第二次购买
        int secondSell = 0;       // 第二次卖出后的收益 表示还没有完成过第二次交易
        
        for (int price : prices) {
            // 第一次买入：当前价格的负值和之前买入状态的最大值
            // 要么保持之前的买入状态 要么用当前价格买入
            firstBuy = std::max(firstBuy, -price);
            
            // 第一次卖出：当前价格加上第一次买入的收益，和之前卖出状态的最大值
            // 要么保持之前的卖出状态 要么在第一次买入的基础上卖出
            firstSell = std::max(firstSell, firstBuy + price);
            
            // 第二次买入：用第一次卖出的收益减去当前价格，和之前第二次买入状态的最大值
            // 要么保持之前的第二次买入状态 要么用第一次卖出的利润买入新的股票
            secondBuy = std::max(secondBuy, firstSell - price);
            
            // 第二次卖出：当前价格加上第二次买入的收益，和之前第二次卖出状态的最大值
            // 要么保持之前的第二次卖出状态 要么在第二次买入的基础上卖出
            secondSell = std::max(secondSell, secondBuy + price);
        }
        
        return secondSell;  // 返回最终的最大收益
    }
};
```

### 3. 股票问题 IV - 最多k次交易
#### 问题链接
[LeetCode 188. 买卖股票的最佳时机 IV](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iv/)

#### 问题描述
给定一个整数数组 prices ，它的第 i 个元素 prices[i] 是一支给定的股票在第 i 天的价格。

设计一个算法来计算你所能获取的最大利润。你最多可以完成 k 笔交易。

注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。

#### 解题思路
- 使用二维动态规划
- dp[i][j] 表示到第i天最多进行j次交易的最大利润

```cpp
class Solution {
public:
    int maxProfit(int k, vector<int>& prices) {
        if (prices.empty() || k == 0) return 0;
        
        // 如果k超过n/2，等价于无限次交易
        if (k >= prices.size() / 2) {
            int profit = 0;
            for (int i = 1; i < prices.size(); i++) {
                if (prices[i] > prices[i-1]) {
                    profit += prices[i] - prices[i-1];
                }
            }
            return profit;
        }
        
        vector<vector<int>> dp(k + 1, vector<int>(2, INT_MIN));
        dp[0][0] = 0;
        
        for (int price : prices) {
            for (int j = k; j >= 1; j--) {
                dp[j][0] = max(dp[j][0], dp[j][1] + price);
                dp[j][1] = max(dp[j][1], dp[j-1][0] - price);
            }
        }
        
        return max(0, dp[k][0]);
    }
};
```

### 4. 股票问题 - 含冷冻期
#### 问题链接
[LeetCode 309. 最佳买卖股票时机含冷冻期](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-cooldown/)

#### 问题描述
给定一个整数数组prices，其中第 i 个元素代表了第 i 天的股票价格。

设计一个算法计算出最大利润。在满足以下约束条件下，你可以尽可能地完成更多的交易（多次买卖一支股票）：
- 卖出股票后，你无法在第二天买入股票 (即冷冻期为 1 天)。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty()) return 0;
        
        int sold = 0;           // 前一天卖出后的最大利润
        int held = -prices[0];  // 持有股票时的最大利润
        int rest = 0;          // 前一天为冷冻期的最大利润
        
        for (int i = 1; i < prices.size(); i++) {
            int prevSold = sold;
            sold = held + prices[i];
            held = max(held, rest - prices[i]);
            rest = max(rest, prevSold);
        }
        
        return max(sold, rest);
    }
};
```

### 5. 股票问题 - 含手续费
#### 问题链接
[LeetCode 714. 买卖股票的最佳时机含手续费](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/)

#### 问题描述
给定一个整数数组 prices ，其中 prices[i]表示第 i 天的股票价格 ；整数 fee 代表了交易股票的手续费用。

你可以无限次地完成交易，但是你每笔交易都需要付手续费。如果你已经购买了一个股票，在卖出它之前你就不能再继续购买股票了。

返回获得利润的最大值。

注意：这里的一笔交易指买入持有并卖出股票的整个过程，每笔交易你只需要为支付一次手续费。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {
        int sold = 0;           // 不持有股票时的最大利润
        int held = -prices[0];  // 持有股票时的最大利润
        
        for (int price : prices) {
            int prevSold = sold;
            sold = max(sold, held + price - fee);
            held = max(held, prevSold - price);
        }
        
        return sold;
    }
};
```

### 总结
股票买卖问题是一个经典的动态规划问题系列，从最基础的单次交易到复杂的多次交易和特殊限制：

1. 基础版本使用贪心思想，记录最低价格并更新最大利润
2. 多次交易版本可以使用贪心法，只要有利润就交易
3. 有限次交易版本需要使用动态规划，状态设计是关键
4. 特殊限制版本（如冷冻期、手续费）在状态转移时需要考虑额外条件

理解这些变体有助于掌握动态规划的思想，同时也展示了如何通过不同的状态设计来处理不同的约束条件。
