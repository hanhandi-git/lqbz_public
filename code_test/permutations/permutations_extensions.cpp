#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <mutex>
#include <iostream>

using namespace std;

class Solution {
public:
    // 1. 基本的全排列
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> path;
        vector<bool> used(nums.size(), false);
        dfs(nums, path, used, res);
        return res;
    }

    // 2. 包含重复数字的全排列
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> res;
        vector<int> path;
        vector<bool> used(nums.size(), false);
        dfsUnique(nums, path, used, res);
        return res;
    }

    // 3. 限制条件的全排列
    vector<vector<int>> permuteWithConstraint(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> path;
        vector<bool> used(nums.size(), false);
        dfsWithConstraint(nums, path, used, res);
        return res;
    }

    // 4. 第 k 个排列
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

    // 5. 全排列的下一个排列
    void nextPermutation(vector<int>& nums) {
        int n = nums.size();
        int i = n - 2;
        
        while (i >= 0 && nums[i] >= nums[i + 1]) {
            i--;
        }
        
        if (i >= 0) {
            int j = n - 1;
            while (j > i && nums[j] <= nums[i]) {
                j--;
            }
            swap(nums[i], nums[j]);
        }
        
        reverse(nums.begin() + i + 1, nums.end());
    }

    // 6. 并行化全排列
    vector<vector<int>> parallelPermute(vector<int>& nums) {
        result.clear();
        vector<thread> threads;

        for (int i = 0; i < nums.size(); i++) {
            threads.emplace_back(&Solution::generatePermutations, this, ref(nums), vector<int>{nums[i]});
        }

        for (auto& t : threads) {
            t.join();
        }

        return result;
    }

    // 7. 内存优化的全排列迭代器
    class PermutationIterator {
    private:
        vector<int> current;
        bool hasNext;

        void generateNextPermutation() {
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
            generateNextPermutation();
            return result;
        }
    };

private:
    void dfs(vector<int>& nums, vector<int>& path, vector<bool>& used, vector<vector<int>>& res) {
        if (path.size() == nums.size()) {
            res.push_back(path);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (used[i]) continue;
            used[i] = true;
            path.push_back(nums[i]);
            dfs(nums, path, used, res);
            path.pop_back();
            used[i] = false;
        }
    }

    void dfsUnique(vector<int>& nums, vector<int>& path, vector<bool>& used, vector<vector<int>>& res) {
        if (path.size() == nums.size()) {
            res.push_back(path);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (used[i] || (i > 0 && nums[i] == nums[i-1] && !used[i-1])) continue;
            used[i] = true;
            path.push_back(nums[i]);
            dfsUnique(nums, path, used, res);
            path.pop_back();
            used[i] = false;
        }
    }

    void dfsWithConstraint(vector<int>& nums, vector<int>& path, vector<bool>& used, vector<vector<int>>& res) {
        if (path.size() == nums.size()) {
            res.push_back(path);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (used[i]) continue;
            if (!path.empty() && abs(nums[i] - path.back()) == 1) continue;
            used[i] = true;
            path.push_back(nums[i]);
            dfsWithConstraint(nums, path, used, res);
            path.pop_back();
            used[i] = false;
        }
    }

    // 用于并行化的成员变量和函数
    vector<vector<int>> result;
    mutex resultMutex;

    void generatePermutations(vector<int>& nums, vector<int> prefix) {
        vector<bool> used(nums.size(), false);
        for (int num : prefix) {
            used[find(nums.begin(), nums.end(), num) - nums.begin()] = true;
        }
        vector<int> path = prefix;
        dfs(nums, path, used, result);
    }
};

int main()
{
    Solution sol;

    // 1. 基本的全排列
    vector<int> nums1 = {1, 2, 3};
    auto result1 = sol.permute(nums1);
    cout << "1. 基本的全排列：" << endl;
    for (const auto& perm : result1) {
        for (int num : perm) {
            cout << num << " ";
        }
        cout << endl;
    }
    cout << endl;

    // 2. 包含重复数字的全排列
    vector<int> nums2 = {1, 1, 2};
    auto result2 = sol.permuteUnique(nums2);
    cout << "2. 包含重复数字的全排列：" << endl;
    for (const auto& perm : result2) {
        for (int num : perm) {
            cout << num << " ";
        }
        cout << endl;
    }
    cout << endl;

    // 3. 限制条件的全排列
    vector<int> nums3 = {1, 2, 3, 4, 5};
    auto result3 = sol.permuteWithConstraint(nums3);
    cout << "3. 限制条件的全排列：" << endl;
    for (const auto& perm : result3) {
        for (int num : perm) {
            cout << num << " ";
        }
        cout << endl;
    }
    cout << endl;

    // 4. 第 k 个排列
    int n = 4, k = 9;
    string result4 = sol.getPermutation(n, k);
    cout << "4. 第 " << k << " 个排列 (n=" << n << "): " << result4 << endl << endl;

    // 5. 全排列的下一个排列
    vector<int> nums5 = {1, 2, 3};
    cout << "5. 全排列的下一个排列：" << endl;
    cout << "原始排列：";
    for (int num : nums5) {
        cout << num << " ";
    }
    cout << endl;
    sol.nextPermutation(nums5);
    cout << "下一个排列：";
    for (int num : nums5) {
        cout << num << " ";
    }
    cout << endl << endl;

    // 6. 并行化全排列
    vector<int> nums6 = {1, 2, 3, 4};
    auto result6 = sol.parallelPermute(nums6);
    cout << "6. 并行化全排列：" << endl;
    for (const auto& perm : result6) {
        for (int num : perm) {
            cout << num << " ";
        }
        cout << endl;
    }
    cout << endl;

    // 7. 内存优化的全排列迭代器
    vector<int> nums7 = {1, 2, 3};
    Solution::PermutationIterator it(nums7);
    cout << "7. 内存优化的全排列迭代器：" << endl;
    while (it.hasNextPermutation()) {
        auto perm = it.nextPermutation();
        for (int num : perm) {
            cout << num << " ";
        }
        cout << endl;
    }

    return 0;
}