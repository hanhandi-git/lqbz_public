# 滑动窗口最大值

## 问题链接
https://leetcode.cn/problems/sliding-window-maximum/description/

## 问题描述
给你一个整数数组 `nums`，有一个大小为 `k` 的滑动窗口从数组的最左侧移动到数组的最右侧。你只可以看到在滑动窗口内的 `k` 个数字。滑动窗口每次只向右移动一位。

返回滑动窗口中的最大值。

### 示例 1：
输入：nums = [1,3,-1,-3,5,3,6,7], k = 3
输出：[3,3,5,5,6,7]
解释：
滑动窗口的位置                最大值
---------------               -----
[1  3  -1] -3  5  3  6  7       3
 1 [3  -1  -3] 5  3  6  7       3
 1  3 [-1  -3  5] 3  6  7       5
 1  3  -1 [-3  5  3] 6  7       5
 1  3  -1  -3 [5  3  6] 7       6
 1  3  -1  -3  5 [3  6  7]      7

### 示例 2：
输入：nums = [1], k = 1
输出：[1]

### 提示：
- 1 <= nums.length <= 10^5
- -10^4 <= nums[i] <= 10^4
- 1 <= k <= nums.length



### 解法1 暴力

对于一般情况，遍历数组，每次取 k 个元素作为一个窗口：
使用 std::copy 将当前窗口的 k 个元素复制到一个新的数组 new_array 中。
使用 std::max_element 找出 new_array 中的最大值。
将这个最大值添加到结果数组 res 中
这个暴力解法的时间复杂度是 O(nk)，其中 n 是数组长度。对于每个窗口，我们都需要 O(k) 的时间来找到最大值。
```cpp
class Solution {
public:
    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
        // 在这里实现你的解决方案
        if (k > nums.size() || nums.size() == 0) return {};
        if (k == nums.size()) return {*std::max_element(nums.begin(), nums.end())};
        std::vector<int> res;
        for (int i = 0; i <= nums.size() - k; i++)
        {
            std::vector<int> new_arrary;
            std::copy(nums.begin() + i, nums.begin() + i + k, std::back_inserter(new_arrary));
            int tmp_max = *std::max_element(new_arrary.begin(), new_arrary.end());
            res.push_back(tmp_max);
        }
        return res;
    }
};
```


### 解法2 单调队列
一种常用的优化方法是使用双端队列（deque）来维护一个单调递减的队列，这样可以将时间复杂度降低到 O(n)。这种方法通常被称为"单调队列"解法。
1. 使用一个双端队列来存储元素的索引
2. 遍历数组，对于每个元素
* 如果队列头部的索引已经不在当前窗口，将其移除
* 从队列尾部开始，移除所有对应元素小于当前元素的索引
* 将当前的元素的索引加入队列尾部
* 如果已经形成了一个完整的窗口(i >= k-1),则队列头部的元素就是当前窗口的最大值
每个元素最多只会被处理两次（一次入队，一次出队），因此时间复杂度降到了 O(n)。

使用单调队列来解决滑动窗口最大值问题是一种常见的优化方法
这个想法来源于对问题特性的分析：
1. 我们需要在一个滑动的窗口中快速找到最大值。
2. 当窗口滑动时，我们需要能够快速删除离开窗口的元素，并添加新进入窗口的元素。
3. 在任何时刻，我们只关心窗口内的最大值，而不关心其他较小的值
单调队列恰好满足这些需求：
1. 它维护一个递减序列，队首始终是当前窗口的最大值
2. 我们可以方便地从队首删除元素（当元素离开窗口时），也可以从队尾添加元素
3. 通过保持队列的单调性，我们可以在添加新元素时移除所有不可能成为最大值的元素。
至于为什么不用单调栈，主要有以下几个原因：
1. 窗口的滑动特性：
滑动窗口问题需要我们能够从两端操作数据结构（添加新元素和删除旧元素）。单调队列（双端队列）允许我们从两端进行操作，而单调栈通常只允许从一端操作。
2. 最大值的位置：
在这个问题中，最大值可能出现在窗口的任何位置，而不仅仅是窗口的一端。单调队列可以很好地处理这种情况，而单调栈更适合处理那些最大（或最小）值总是出现在序列一端的问题。
3. 元素的删除：
当元素离开窗口时，我们需要能够快速删除它。单调队列允许我们直接从队首删除元素，而单调栈则不容易实现这一点。
4. 问题的特性：
**滑动窗口最大值问题更关注的是在一个范围内快速找到最大值**
而单调栈通常用于**解决"下一个更大元素"或"直方图中最大矩形"**这类问题。

这里为什么存索引而非存值？
1. 方便判断元素是否在当前窗口内：
我们可以很容易地判断一个元素是否还在当前的滑动窗口内。例如，当我们检查 dq.front() + k <= i 时，我们可以直接比较索引，而不需要额外的数据结构来跟踪元素的位置。
2. 避免重复元素的问题：
如果数组中有重复的元素，存储值可能会导致我们无法区分它们。而存储索引可以唯一地标识每个元素，即使它们的值相同
3. 快速访问原始数组中的值：
虽然我们在队列中存储的是索引，但我们可以随时通过 nums[dq.front()] 快速获取到对应的值。这样既保留了索引信息，又能方便地访问实际的值
4. 索引不仅告诉我们元素的值，还隐含了元素在原数组中的位置信息。这对于维护滑动窗口非常有用。
5. 空间效率：
在大多数情况下，存储索引（通常是整数）比存储实际的值更加空间效率，特别是当值的类型比整数更大时（例如，如果值是浮点数或者更复杂的数据类型）
```cpp
class Solution {
public:
    std::vector<int> maxSlidingWindow(std::vector<int>& nums, int k) {
        std::vector<int> result;
        std::deque<int> dq;
        
        for (int i = 0; i < nums.size(); ++i) {
            // 移除队列中不在当前窗口的元素
            // 当 dq.front() == i - k 队列头部的元素索引 + 窗口大小 = 当前处理的元素索引
            // 换句话说，队列头部的元素已经不在当前的滑动窗口内了
            // 假设当前窗口包含索引为 3, 4, 5 的元素 如果队列头部的元素索引是 2，那么 2 == 5 - 3，说明索引为 2 的元素已经不在当前窗口内了
            if (!dq.empty() && dq.front() + k <= i)
            {
                dq.pop_front();
            }
            while (!dq.empty() && nums[dq.back()] < nums[i])
            {
                dq.pop_back();
            }
            dq.push_back(i);
            // 窗口大小达到k时
            if (i + 1 >= k)
            {
                result.push_back(nums[dq.front()]);
            }
            
        }
        
        return result;
    }
};

```

这里可以这样实现，更容易理解
```cpp
class Solution {
private:
    class MyQueue {
        private:
            deque<int> data;
        public:
            void push(int n) {
                while (!data.empty() && data.back() < n) {
                    data.pop_back();
                }
                data.push_back(n);
            }
            int max() {
                return data.front();
            }
            void pop(int n) {
                if (!data.empty() && data.front() == n) {
                    data.pop_front();
                }
            }
    };
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        MyQueue window;
        vector<int> res;
        for (int i = 0; i < nums.size(); i++) {
            if (i < k - 1) {
                window.push(nums[i]);
            } else { // 窗口向前滑动
                window.push(nums[i]);
                res.push_back(window.max());
                window.pop(nums[i - k + 1]);    // 删除窗口最后的元素
            }
        }
        return res;
    }
}; 
```     