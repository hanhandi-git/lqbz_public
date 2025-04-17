# 每日温度

## 问题链接
[LeetCode 739. 每日温度](https://leetcode.com/problems/daily-temperatures/)

## 问题描述
给定一个整数数组 temperatures,表示每天的温度,返回一个数组 answer,其中 answer[i] 是指对于第 i 天,需要等待多少天才会有更高的温度。如果气温在这之后都不会升高,请在该位置用 0 来代替。

示例:
输入: temperatures = [73,74,75,71,69,72,76,73]
输出: [1,1,4,2,1,1,0,0]

## 解题思路
1. 使用单调栈来解决这个问题:
   - 维护一个存储下标的栈,栈中温度值单调递减
   - 当遇到更高温度时,可以得到栈中元素需要等待的天数
2. 具体步骤:
   - 遍历温度数组,对于每个温度:
     - 如果栈不为空且当前温度大于栈顶温度
     - 弹出栈顶元素,计算等待天数
     - 重复上述过程直到栈为空或当前温度不大于栈顶温度
   - 将当前温度的下标入栈
3. 最终栈中剩余的元素,都是遇不到更高温度的日子,对应的结果为0

## 代码实现

```cpp
class Solution {
public:
vector<int> dailyTemperatures(vector<int>& temperatures) {
int n = temperatures.size();
vector<int> answer(n, 0);
stack<int> st; // 存储下标的单调栈
for (int i = 0; i < n; i++) {
// 当前温度大于栈顶温度时,可以更新栈顶元素对应的等待天数
while (!st.empty() && temperatures[i] > temperatures[st.top()]) {
int prevDay = st.top();
st.pop();
answer[prevDay] = i - prevDay;
}
st.push(i);
}
return answer;
}
};

```

## 复杂度分析
- 时间复杂度：O(n)，其中 n 是温度数组的长度。每个元素最多被压入和弹出栈一次。
- 空间复杂度：O(n)，需要一个栈存储元素下标，最坏情况下（温度数组单调递减）需要存储所有元素。