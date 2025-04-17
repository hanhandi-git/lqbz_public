# 划分字母区间

## 问题链接
[LeetCode 763. 划分字母区间](https://leetcode.com/problems/partition-labels/)

## 问题描述
给定一个字符串 S，划分它的字母区间，使得每个字母只出现在一个区间中，返回一个区间的大小列表。

示例:
输入: S = "ababcbacadefegdehijhklij"
输出: [9,7,8]

## 解题思路
1. 首先，记录每个字母最后出现的位置。
2. 然后，遍历字符串，维护一个当前区间的结束位置。
3. 当遍历到某个字母时，更新当前区间的结束位置为该字母最后出现的位置。
4. 如果当前索引等于结束位置，说明找到了一个完整的区间，记录区间的大小并更新开始位置。

## 代码实现

```cpp
class Solution {
public:
    vector<int> partitionLabels(string S) {
        vector<int> last(26, 0);
        for (int i = 0; i < S.size(); i++) {
            last[S[i] - 'a'] = i; // 记录每个字母最后出现的位置
        }
        
        vector<int> result;
        int start = 0, end = 0;
        for (int i = 0; i < S.size(); i++) {
            end = max(end, last[S[i] - 'a']); // 更新当前区间的结束位置
            if (i == end) { // 找到一个完整的区间
                result.push_back(end - start + 1);
                start = i + 1; // 更新开始位置
            }
        }
        return result;
    }
};
```

## 复杂度分析
- 时间复杂度：O(n)，其中 n 是字符串 S 的长度。我们只遍历字符串两次。
- 空间复杂度：O(1)，因为我们只使用了固定大小的数组来存储字母的最后位置。