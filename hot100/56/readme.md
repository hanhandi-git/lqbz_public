```thinking
让我分析一下合并区间这道题...

首先理解题目要点：
1. 输入是一组可能重叠的区间
2. 需要合并所有重叠的区间
3. 关键是如何判断区间重叠并合并
4. 需要考虑特殊情况如空输入

解题思路：
- 先按区间起点排序很重要
- 如何判断重叠条件
- 如何合并区间
- 如何处理边界情况

需要包含的内容：
- 问题描述要清晰
- 详细解释思路和算法
- 代码实现
- 复杂度分析
- 相关题目推荐

让我开始编写新的readme...
```

`````markdown:hot100/56/readme.md
# 合并区间

## 问题链接
https://leetcode.cn/problems/merge-intervals/

## 问题描述
给出一个区间的集合，请合并所有重叠的区间。

### 示例 1：
```
输入：intervals = [[1,3],[2,6],[8,10],[15,18]]
输出：[[1,6],[8,10],[15,18]]
解释：区间 [1,3] 和 [2,6] 重叠, 将它们合并为 [1,6]。
```

### 示例 2：
```
输入：intervals = [[1,4],[4,5]]
输出：[[1,5]]
解释：区间 [1,4] 和 [4,5] 可被视为重叠区间。
```

### 提示：
- 1 <= intervals.length <= 104
- intervals[i].length == 2
- 0 <= starti <= endi <= 104

## 解题思路

这道题的关键点在于如何判断和合并重叠的区间。主要步骤如下：

1. 首先将区间按照起始位置排序
2. 遍历排序后的区间：
   - 如果当前区间的起始位置 <= 前一个区间的结束位置，说明有重叠
   - 合并重叠区间，更新结束位置为两个区间结束位置的较大值
   - 如果没有重叠，将当前区间加入结果集

## 代码实现

```cpp
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        if (intervals.empty()) return {};
        
        // 按区间起始位置排序
        sort(intervals.begin(), intervals.end());
        
        vector<vector<int>> result;
        result.push_back(intervals[0]);
        
        for (int i = 1; i < intervals.size(); i++) {
            // 如果当前区间的起始位置小于等于上一个区间的结束位置，则合并
            if (intervals[i][0] <= result.back()[1]) {
                result.back()[1] = max(result.back()[1], intervals[i][1]);
            } else {
                // 否则直接添加到结果中
                result.push_back(intervals[i]);
            }
        }
        
        return result;
    }
};
```

## 复杂度分析

- 时间复杂度：O(nlogn)，其中 n 是区间的数量。排序需要 O(nlogn)，遍历区间需要 O(n)。
- 空间复杂度：O(n)，需要存储合并后的区间。

## 优化思路

1. 特殊情况处理：
   - 空输入直接返回空数组
   - 只有一个区间直接返回

2. 排序优化：
   - 如果已知区间起始位置范围较小，可以使用计数排序
   - 如果数据量很大，可以考虑并行排序

3. 内存优化：
   - 可以考虑原地修改输入数组，减少额外空间使用

## 相关题目
- [57. 插入区间](https://leetcode.cn/problems/insert-interval/)
- [435. 无重叠区间](https://leetcode.cn/problems/non-overlapping-intervals/)
- [452. 用最少数量的箭引爆气球](https://leetcode.cn/problems/minimum-number-of-arrows-to-burst-balloons/)
`````
