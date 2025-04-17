#include <vector>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <functional>
using namespace std;

// 自定义比较器 比较频率
struct Compare {
    bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b)
    {
        return a.second > b.second;
    }
};

class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        std::vector<int> result;
        std::unordered_map<int, int> mapping;   //  val->freq
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, Compare> pque;
        // 构建hash map
        for (auto num : nums) mapping[num]++;
        // 构建mapping
        for (const auto& entry : mapping)
        {
            pque.push(entry);
            if (pque.size() > k) pque.pop();
        }
        while (!pque.empty())
        {
            result.push_back(pque.top().first);
            pque.pop();
        }
        return result; // 返回频率前 K 高的元素
    }
};

// 测试框架
class TestFramework {
private:
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
        // 前 K 个高频元素测试
        runTest("前 K 个高频元素测试", [this]() {
            Solution solution;
            std::vector<int> test1 = {1, 1, 1, 2, 2, 3};
            std::vector<int> result1 = solution.topKFrequent(test1, 2); // 应该返回 [1, 2] 或 [2, 1]
            std::vector<int> test2 = {1};
            std::vector<int> result2 = solution.topKFrequent(test2, 1); // 应该返回 [1]
            std::vector<int> test3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            std::vector<int> result3 = solution.topKFrequent(test3, 3); // 应该返回任意三个元素
            std::vector<int> test4 = {5, 5, 4, 4, 3, 3, 2, 2, 1};
            std::vector<int> result4 = solution.topKFrequent(test4, 2); // 应该返回 [5, 4] 或 [4, 5]

            return (result1 == std::vector<int>{1, 2} || result1 == std::vector<int>{2, 1}) &&
                   result2 == std::vector<int>{1} &&
                   (result3.size() == 3) &&
                   (result4 == std::vector<int>{5, 4} || result4 == std::vector<int>{4, 5});
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