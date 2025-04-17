## 问题链接
https://leetcode.cn/problems/permutations/

## 问题描述
给定一个不含重复数字的数组 nums ，返回其 所有可能的全排列 。你可以 按任意顺序 返回答案。

### 示例 1:

输入：nums = [1,2,3]
输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]

### 示例 2:

输入：nums = [0,1]
输出：[[0,1],[1,0]]

### 示例 3:

输入：nums = [1]
输出：[[1]]

### 提示：

1 <= nums.length <= 6
-10 <= nums[i] <= 10
nums 中的所有整数 互不相同

## 解题思路
本题使用深度优先搜索（DFS）来生成所有可能的排列。我们通过维护一个 `used` 数组来标记已经使用过的元素，使用 `path` 数组来存储当前的排列，并使用递归来生成所有可能的排列。

## 代码实现
```cpp
class Solution {
public:
    std::vector<std::vector<int>> res;
    std::vector<int> path;
    std::vector<bool> used;

    void dfs(int start, std::vector<int>& nums) {

        if (path.size() == nums.size()) {
            res.emplace_back(path);
            return;
        }
        for (int i = 0; i < nums.size(); i++)
        {
            if (used[i] == true) continue;
            used[i] = true;
            path.emplace_back(nums[i]);
            dfs(i + 1, nums);
            path.pop_back();
            used[i] = false;
        }
        return;
    }
    std::vector<std::vector<int>> permute(std::vector<int>& nums) {
        res.clear();
        path.clear();
        used.clear();
        if (nums.size() <= 0) return {{}};
        // 在这里实现全排列算法
        used = std::vector<bool>(nums.size(), false);
        dfs(0, nums);
        return res;
    }
};
```

## 复杂度分析
- 时间复杂度：O(n!)，其中 n 是数组的长度。总共有 n! 种排列。
- 空间复杂度：O(n)，递归调用栈的深度最大为 n，而 `used` 和 `path` 数组也各占用 O(n) 的空间。

## 问题扩展

1. **包含重复数字的全排列**
   问题：如果输入数组包含重复数字，如何避免生成重复的排列？
   
   答案：可以先对数组排序，然后在 DFS 过程中跳过相同的相邻元素。具体实现时，需要添加一个条件：
   ```cpp
   if (i > 0 && nums[i] == nums[i-1] && !used[i-1]) continue;
   ```

2. **字符串的全排列**
   问题：如何修改代码以处理字符串的全排列？
   
   答案：将输入类型从 `vector<int>` 改为 `string`，并相应修改 `path` 的类型为 `string`。其他逻辑保持不变。

3. **限制条件的全排列**
   问题：如何生成满足特定条件的排列？例如，相邻数字不能相同。
   
   答案：在 DFS 函数中添加条件检查。例如，对于相邻数字不能相同的要求：
   ```cpp
   if (!path.empty() && abs(nums[i] - path.back()) == 1) continue;
   ```

   详细说明：
   在某些情况下，我们可能需要生成满足特定条件的排列，而不是所有可能的排列。这些条件可以根据具体问题的要求而变化。以"相邻数字不能相同"为例：

   问题描述：给定一个包含不重复数字的数组，生成所有可能的排列，但要求任意两个相邻的数字之间的差的绝对值不能等于1。

   示例：
   输入：`nums = [1, 3, 5]`
   输出：`[[1, 5, 3], [3, 1, 5], [3, 5, 1], [5, 1, 3], [5, 3, 1]]`

   注意，[1, 3, 5] 和 [5, 3, 1] 不在输出中，因为它们包含相邻的数字，它们之间的差的绝对值为1。

   实现思路：
   1. 使用深度优先搜索（DFS）来生成排列。
   2. 在添加新数字到当前排列时，检查它是否满足条件。
   3. 如果不满足条件，跳过该数字，继续尝试下一个数字。

   代码实现：
   ```cpp
   class Solution {
   public:
       vector<vector<int>> res;
       vector<int> path;
       vector<bool> used;

       void dfs(vector<int>& nums) {
           if (path.size() == nums.size()) {
               res.push_back(path);
               return;
           }
           for (int i = 0; i < nums.size(); i++) {
               if (used[i]) continue;
               if (!path.empty() && abs(nums[i] - path.back()) == 1) continue;
               
               used[i] = true;
               path.push_back(nums[i]);
               dfs(nums);
               path.pop_back();
               used[i] = false;
           }
       }

       vector<vector<int>> permuteWithConstraint(vector<int>& nums) {
           used.resize(nums.size(), false);
           dfs(nums);
           return res;
       }
   };
   ```

4. **第 k 个排列**
   问题：如何直接找到全排列中的第 k 个排列，而不生成所有排列？
   
   答案：使用数学方法。计算每一位可能的数字，根据 k 的值逐位确定。这需要使用阶乘来计算每一位的可能性数量。

   详细解题思路：
   1. 首先，计算 (n-1)! 的值，这代表了固定第一个数字后的排列数量。
   2. 使用 k / (n-1)! 来确定第一个数字在剩余数字中的索引。
   3. 更新 k 为 k % (n-1)!。
   4. 重复步骤 1-3，每次减少 n 的值，直到所有位置都被填满。

   示例：
   1. n = 3, k = 3
      - 可能的排列：[1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], [3,2,1]
      - 第 3 个排列是 [2,1,3]
      
      计算过程：
      - 首先，(3-1)! = 2，表示固定第一个数字后有 2 种排列
      - k = 3，3 / 2 = 1，所以第一个数字是剩余数字中的第二个（索引从 0 开始），即 2
      - 更新 k = 3 % 2 = 1
      - 对剩下的数字 [1,3] 重复过程，1 / 1 = 1，选择 3
      - 最后剩下 1
      - 因此，结果是 [2,1,3]

   2. n = 4, k = 9
      - 第 9 个排列是 [2,3,1,4]
      
      计算过程：
      - (4-1)! = 6，k = 9
      - 9 / 6 = 1，选择第二个数字 2
      - 更新 k = 9 % 6 = 3
      - 对剩下的 [1,3,4]，3 / 2 = 1，选择 3
      - 更新 k = 3 % 2 = 1
      - 对剩下的 [1,4]，1 / 1 = 1，选择 4
      - 最后剩下 1
      - 结果是 [2,3,1,4]

   3. n = 3, k = 1
      - 第 1 个排列是 [1,2,3]
      
      计算过程：
      - (3-1)! = 2，k = 1
      - 1 / 2 = 0，选择第一个数字 1
      - 更新 k = 1 % 2 = 1
      - 对剩下的 [2,3]，1 / 1 = 1，选择 3
      - 最后剩下 2
      - 结果是 [1,2,3]

   这些例子展示了如何使用数学方法直接计算第 k 个排列，而不需要生成所有可能的排列。这种方法特别适用于 k 较大的情况，因为它避免了生成和存储大量排列的开销。

   代码实现：
   ```cpp
   class Solution {
   public:
       string getPermutation(int n, int k) {
           vector<int> factorial(n);
           vector<char> nums;
           factorial[0] = 1;
           for(int i = 1; i < n; i++) {
               factorial[i] = factorial[i-1] * i;
               nums.push_back(i + '0');
           }
           nums.push_back(n + '0');
           
           string result;
           k--; // 将 k 转换为 0-indexed
           
           for(int i = n - 1; i >= 0; i--) {
               int idx = k / factorial[i];
               k %= factorial[i];
               
               result.push_back(nums[idx]);
               nums.erase(nums.begin() + idx);
           }
           
           return result;
       }
   };
   ```

   这种方法的时间复杂度为 O(n^2)，主要是因为在每一步中我们需要删除已使用的数字。空间复杂度为 O(n)，用于存储阶乘数组和数字数组。

   通过这种方法，我们可以直接计算出第 k 个排列，而不需要生成所有的排列，大大提高了效率。

5. **全排列的下一个排列**
   问题：给定一个排列，如何找到字典序中的下一个排列？
   
   答案：从右向左找到第一个相邻的升序对 (i,j)，然后在 i 右侧找到大于 nums[i] 的最小数，与 nums[i] 交换，最后将 i+1 到末尾的部分反转。

   详细思路：
   1. 从右向左扫描数组，找到第一个 nums[i] < nums[i+1] 的位置，记为 i。
   2. 如果找不到这样的 i，说明整个数组是降序的，已经是最后一个排列，直接将整个数组反转即可。
   3. 在 [i+1, end) 区间从右向左查找第一个大于 nums[i] 的元素，记为 j。
   4. 交换 nums[i] 和 nums[j]。
   5. 将 [i+1, end) 区间的元素反转。

   代码实现：
   ```cpp
   class Solution {
   public:
       void nextPermutation(vector<int>& nums) {
           int n = nums.size();
           int i = n - 2;
           
           // 步骤 1: 找到第一个相邻升序对
           while (i >= 0 && nums[i] >= nums[i + 1]) {
               i--;
           }
           
           if (i >= 0) {
               int j = n - 1;
               // 步骤 3: 找到右侧大于 nums[i] 的最小数
               while (j > i && nums[j] <= nums[i]) {
                   j--;
               }
               // 步骤 4: 交换
               swap(nums[i], nums[j]);
           }
           
           // 步骤 5: 反转 i+1 到末尾的部分
           reverse(nums.begin() + i + 1, nums.end());
       }
   };
   ```

   时间复杂度：O(n)，其中 n 是数组的长度。
   空间复杂度：O(1)，只使用了常数额外空间。

6. **并行化**
   问题：如何并行化全排列的生成过程以提高效率？
   
   答案：可以将初始的几层递归分配给不同的线程处理。每个线程负责生成以特定前缀开始的所有排列。

   详细思路：
   1. 确定并行化的层数，通常是前 1 或 2 层。
   2. 生成这些层的所有可能前缀。
   3. 为每个前缀创建一个线程，每个线程负责生成以该前缀开始的所有排列。
   4. 使用线程安全的数据结构来收集所有线程的结果。

   代码实现（使用 C++11 的线程库）：
   ```cpp
   #include <thread>
   #include <mutex>
   #include <vector>

   class Solution {
   private:
       vector<vector<int>> result;
       mutex resultMutex;

       void dfs(vector<int>& nums, vector<int>& path, vector<bool>& used) {
           if (path.size() == nums.size()) {
               lock_guard<mutex> lock(resultMutex);
               result.push_back(path);
               return;
           }
           for (int i = 0; i < nums.size(); i++) {
               if (!used[i]) {
                   used[i] = true;
                   path.push_back(nums[i]);
                   dfs(nums, path, used);
                   path.pop_back();
                   used[i] = false;
               }
           }
       }

       void generatePermutations(vector<int>& nums, vector<int> prefix) {
           vector<bool> used(nums.size(), false);
           for (int num : prefix) {
               used[num] = true;
           }
           dfs(nums, prefix, used);
       }

   public:
       vector<vector<int>> parallelPermute(vector<int>& nums) {
           result.clear();
           vector<thread> threads;

           // 生成一层前缀
           for (int i = 0; i < nums.size(); i++) {
               threads.emplace_back(&Solution::generatePermutations, this, ref(nums), vector<int>{nums[i]});
           }

           for (auto& t : threads) {
               t.join();
           }

           return result;
       }
   };
   ```

   注意：这种并行化方法在数据量较大时才会显示出明显的性能提升。对于小规模输入，线程创建和管理的开销可能会超过并行化带来的收益。

7. **内存优化**
   问题：对于大规模输入，如何优化内存使用？
   
   答案：使用迭代器或生成器模式，每次只生成一个排列，而不是存储所有排列。在 C++ 中，可以实现一个自定义迭代器。

   详细思路：
   1. 实现一个排列迭代器类，它能够按需生成下一个排列。
   2. 使用 "下一个排列" 的算法来生成排列。
   3. 迭代器只需要存储当前排列状态，而不需要存储所有排列。

   代码实现：
   ```cpp
   class PermutationIterator {
   private:
       vector<int> current;
       bool hasNext;

       void nextPermutation() {
           int n = current.size();
           int i = n - 2;
           while (i >= 0 && current[i] >= current[i + 1]) {
               i--;
           }
           if (i >= 0) {
               int j = n - 1;
               while (j > i && current[j] <= current[i]) {
                   j--;
               }
               swap(current[i], current[j]);
           }
           reverse(current.begin() + i + 1, current.end());
           hasNext = (i != -1);
       }

   public:
       PermutationIterator(vector<int>& nums) : current(nums), hasNext(true) {
           sort(current.begin(), current.end());
       }

       bool hasNextPermutation() {
           return hasNext;
       }

       vector<int> nextPermutation() {
           vector<int> result = current;
           nextPermutation();
           return result;
       }
   };

   class Solution {
   public:
       vector<vector<int>> permute(vector<int>& nums) {
           vector<vector<int>> result;
           PermutationIterator it(nums);
           while (it.hasNextPermutation()) {
               result.push_back(it.nextPermutation());
           }
           return result;
       }
   };
   ```

   这种方法的优点是内存使用更加高效，特别是在只需要逐个处理排列而不需要同时存储所有排列的场景中。它也允许在生成过程中随时停止，而不需要生成所有排列。

   时间复杂度：仍然是 O(n!)，但是空间复杂度降低到了 O(n)。

8. **全排列的应用**
   问题：探讨全排列在实际问题中的应用。
   
   答案：全排列在密码生成、路径规划、组合优化问题等领域有广泛应用。例如，在旅行商问题中，可以使用全排列来枚举所有可能的路径。

9. **不同的遍历顺序**
   问题：如何修改代码以生成按字典序或其他特定顺序的全排列？
   
   答案：对于字典序，可以先对输入数组排序，然后使用 "下一个排列" 的算法反复生成下一个排列，直到回到初始状态。

10. **部分排列**
    问题：如何生成 n 个数中取 k 个数的所有排列？
    
    答案：修改 DFS 函数，增加一个参数 k，当 path 的长度达到 k 时就加入结果集。同时，需要修改终止条件：
    ```cpp
    if (path.size() == k) {
        res.push_back(path);
        return;
    }
    ```

这些扩展问题涵盖了全排列算法的多个方面，包括优化、变体和应用。通过思考和解决这些问题，可以加深对排列算法的理解，并提高算法设计能力。