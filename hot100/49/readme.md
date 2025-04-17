## 问题链接
https://leetcode.cn/problems/group-anagrams/

## 问题描述
给你一个字符串数组，请你将 字母异位词 组合在一起。可以按任意顺序返回结果列表。

字母异位词 是由重新排列源单词的所有字母得到的一个新单词。

### 示例 1:

输入: strs = ["eat","tea","tan","ate","nat","bat"]
输出: [["bat"],["nat","tan"],["ate","eat","tea"]]

### 示例 2:

输入: strs = [""]
输出: [[""]]

### 示例 3:

输入: strs = ["a"]
输出: [["a"]]

### 提示：

1 <= strs.length <= 104
0 <= strs[i].length <= 100
strs[i] 仅包含小写字母

## 解题思路
本题使用哈希表来对字母异位词进行分组。对于每个字符串，我们将其排序后的结果作为哈希表的键，原字符串作为值存入对应的组中。最后将哈希表转换为vector返回结果。

## 代码实现
```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> mp;
        
        // 遍历每个字符串
        for(string& s : strs) {
            string key = s;
            sort(key.begin(), key.end());  // 排序后的字符串作为key
            mp[key].push_back(s);
        }
        
        // 转换哈希表为vector
        vector<vector<string>> res;
        for(auto& pair : mp) {
            res.push_back(pair.second);
        }
        
        return res;
    }
};
```

## 复杂度分析
- 时间复杂度：O(nklogk)，其中 n 是字符串数组的长度，k 是字符串的最大长度。需要遍历 n 个字符串，每个字符串需要 O(klogk) 的时间排序。
- 空间复杂度：O(nk)，哈希表需要存储所有字符串。

## 问题扩展

1. **优化排序方法**
   问题：如何避免使用排序来判断异位词？
   
   答案：可以使用计数方法，统计每个字符出现的次数作为key：
   ```cpp
   string getKey(string& s) {
       vector<int> count(26, 0);
       for(char c : s) {
           count[c - 'a']++;
       }
       string key;
       for(int i = 0; i < 26; i++) {
           if(count[i] > 0) {
               key += to_string(i) + to_string(count[i]);
           }
       }
       return key;
   }
   ```

2. **大规模数据优化**
   问题：如果输入数据量非常大，如何优化？
   
   答案：可以使用并行处理，将数据分块处理后合并结果。也可以使用质数相乘的方法代替排序。

3. **内存优化**
   问题：如何减少内存使用？
   
   答案：可以使用字符计数的压缩表示，或者使用位运算来表示字符出现情况。

这些扩展问题帮助我们思考如何在不同场景下优化字母异位词分组的解决方案。