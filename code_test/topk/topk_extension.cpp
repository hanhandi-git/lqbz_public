#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <functional>

class Solution {
public:
    // 1. 最小的k个数
    std::vector<int> getLeastNumbers(std::vector<int>& arr, int k) {
        if (k == 0 || arr.empty()) return {};
        std::vector<int> result(k, 0);
        std::priority_queue<int, std::vector<int>, std::less<int>> q; // 大顶堆
        for (int i = 0; i < k; ++i) {
            q.push(arr[i]);
        }
        for (int i = k; i < arr.size(); ++i) {
            if (q.top() > arr[i]) {
                q.pop();
                q.push(arr[i]);
            }
        }
        for (int i = k - 1; i >= 0; --i) {
            result[i] = q.top();
            q.pop();
        }
        return result;
    }

    // 2. 数据流中的第K大元素
    class KthLargest {
    private:
        std::priority_queue<int, std::vector<int>, std::greater<int>> pq;
        int k;
    public:
        KthLargest(int k, std::vector<int>& nums) : k(k) {
            for (int num : nums) {
                add(num);
            }
        }
        
        int add(int val) {
            if (pq.size() < k) {
                pq.push(val);
            } else if (val > pq.top()) {
                pq.pop();
                pq.push(val);
            }
            return pq.top();
        }
    };

    // 3. 前K个高频元素
    std::vector<int> topKFrequent(std::vector<int>& nums, int k) {
        if (nums.empty() || k <= 0) return {};

        // 统计频率
        std::unordered_map<int, int> freq_map;
        for (int num : nums) {
            freq_map[num]++;
        }

        // 使用小顶堆，保存频率最高的k个元素
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

        for (const auto& pair : freq_map) {
            if (pq.size() >= k) pq.pop();
            pq.push({pair.second, pair.first}); // 按照second排序（这样就是频率了）            
        }


        // 构建结果
        std::vector<int> result;
        while (!pq.empty()) {
            result.push_back(pq.top().second);
            pq.pop();
        }

        return result;
    }
    // 快速选择法 
    // std::vector<int> topKFrequent(std::vector<int>& nums, int k) {
    //     std::unordered_map<int, int> count;
    //     for (int num : nums) {
    //         count[num]++;
    //     }
        
    //     std::vector<std::pair<int, int>> freqPairs;
    //     for (const auto& pair : count) {
    //         freqPairs.push_back(pair);
    //     }
        
    //     // 降序排列
    //     auto cmp = [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
    //         return a.second > b.second;
    //     };
        
    //     std::nth_element(freqPairs.begin(), freqPairs.begin() + k - 1, freqPairs.end(), cmp);
        
    //     std::vector<int> result;
    //     for (int i = 0; i < k; ++i) {
    //         result.push_back(freqPairs[i].first);
    //     }
    //     return result;
    // }
};

// Test Framework
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        if (test()) {
            std::cout << "通过: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "失败: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        runTest("最小的k个数", [this]() {
            std::vector<int> arr = {4,5,1,6,2,7,3,8};
            int k = 4;
            std::vector<int> result = solution.getLeastNumbers(arr, k);
            std::sort(result.begin(), result.end());
            return result == std::vector<int>{1,2,3,4};
        });

        runTest("数据流中的第K大元素", [this]() {
            std::vector<int> nums = {4,5,8,2};
            Solution::KthLargest kthLargest(3, nums);
            return kthLargest.add(3) == 4 &&
                   kthLargest.add(5) == 5 &&
                   kthLargest.add(10) == 5 &&
                   kthLargest.add(9) == 8 &&
                   kthLargest.add(4) == 8;
        });

        runTest("前K个高频元素", [this]() {
            std::vector<int> nums = {1,1,1,2,2,3};
            int k = 2;
            std::vector<int> result = solution.topKFrequent(nums, k);
            std::sort(result.begin(), result.end());
            return result == std::vector<int>{1,2};
        });

        std::cout << "\n测试结果: " << passed << " 通过, " 
                  << (total - passed) << " 失败, " 
                  << total << " 总计" << std::endl;
    }
};

int main() {
    TestFramework testFramework;
    testFramework.runAllTests();
    return 0;
}
