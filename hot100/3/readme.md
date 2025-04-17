# 无重复字符的最长子串

## 问题链接
[LeetCode 3. 无重复字符的最长子串](https://leetcode.com/problems/longest-substring-without-repeating-characters/)

## 问题描述
给定一个字符串 s ，请你找出其中不含有重复字符的 最长子串 的长度。

## 解题思路
1. 使用滑动窗口的方法来解决这个问题。
2. 维护一个窗口，窗口内的字符都是不重复的。
3. 使用两个指针，右指针不断向右移动，左指针在发现重复字符时向右移动。
4. 使用一个哈希表（或者数组）来记录每个字符最后出现的位置。
5. 在移动过程中，不断更新最长子串的长度。

## 代码实现
```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        if (s.size() <= 1) return s.size();
        unordered_map<char, int> window;            // key -> freq
        int left = 0;
        int right = 0;
        int maxlen = 1;
        while (right < s.size()) {
            // 扩大窗口
            char c = s[right];
            window[c]++;
            right++;
            // shrink window
            while(window[c] > 1) {
                window[s[left]]--;
                left++;
            }
            // 计算本窗口长度
            maxlen = max(maxlen, right - left);
        }
        return maxlen;
    }
};
```

## 复杂度分析
- 时间复杂度：O(n)，其中n是字符串的长度。我们只需要遍历一次字符串。
- 空间复杂度：O(min(m, n))，其中m是字符集的大小，n是字符串的长度。在最坏的情况下，我们需要O(min(m, n))的空间来存储哈希表。

## 扩展：其他相关问题
基于上面的实现实现下面的
找出最长的不含重复字符的子串
找出所有最长的不含重复字符的子串


其他的名字类似，但是方法不同的题目,这里不记录题解
1. 最长回文子串：寻找字符串中最长的回文序列。
2. 最长重复子串：寻找字符串中最长的重复子串。
3. 最长公共子序列：寻找两个字符串中最长的公共子序列。

这些问题都涉及到子串或子序列的处理，但每个问题都有其特定的解法和技巧。无重复字符的最长子串问题是一个经典的滑动窗口问题，掌握这种技巧对解决类似的问题很有帮助。
