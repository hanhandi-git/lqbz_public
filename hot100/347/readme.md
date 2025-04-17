# 前 K 个高频元素

## 问题链接
[LeetCode 347. 前 K 个高频元素](https://leetcode.com/problems/top-k-frequent-elements/)

## 问题描述
给定一个非空的整数数组 `nums` 和一个整数 `k`，返回数组中出现频率前 `k` 高的元素。你可以按任意顺序返回答案。

## 逻辑推导流程
以下是获取前 K 个高频元素的逻辑推导流程：

1. **初始化**：
   - 使用一个哈希表（字典）来统计每个元素的出现频率。
   - 创建一个最小堆（优先队列）来存储频率最高的 K 个元素。

2. **统计频率**：
   - 遍历数组 `nums`，将每个元素及其频率存入哈希表。

3. **构建最小堆**：
   - 遍历哈希表，将元素及其频率插入最小堆。
   - 如果堆的大小超过 K，则弹出堆顶元素（频率最低的元素）。

4. **返回结果**：
   - 最小堆中的元素即为频率前 K 高的元素，返回这些元素。

## 解题思路
1. 使用哈希表统计每个元素的频率。
2. 使用最小堆维护频率最高的 K 个元素。
3. 最终从堆中提取出结果。

## 代码实现
```cpp
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> frequencyMap; // 存储元素频率
        for (int num : nums) {
            frequencyMap[num]++; // 统计频率
        }

        // 使用最小堆存储频率最高的 K 个元素
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minHeap;
        for (const auto& entry : frequencyMap) {
            minHeap.push({entry.second, entry.first}); // 将频率和元素插入堆
            if (minHeap.size() > k) {
                minHeap.pop(); // 弹出频率最低的元素
            }
        }

        // 提取结果
        vector<int> result;
        while (!minHeap.empty()) {
            result.push_back(minHeap.top().second); // 获取元素
            minHeap.pop();
        }

        return result; // 返回频率前 K 高的元素
    }
};
```

## 复杂度分析
- 时间复杂度：O(N log K)，其中 N 是数组的长度。我们需要遍历数组并维护一个大小为 K 的最小堆。
- 空间复杂度：O(N)，用于存储频率哈希表和最小堆。

## 示例
假设我们有一个数组 `nums = [1, 1, 1, 2, 2, 3]`，并且 `k = 2`，我们可以通过上述逻辑得到频率前 2 高的元素为 `[1, 2]`。

### 逻辑推导示例
1. 统计频率：`{1: 3, 2: 2, 3: 1}`
2. 构建最小堆，最终得到的堆中元素为 `[2, 1]`（频率为 2 和 3）。
3. 返回结果 `[1, 2]`。
