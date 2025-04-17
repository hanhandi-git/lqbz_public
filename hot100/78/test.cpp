#include <iostream>
#include <cassert>
#include <set>
#include <vector>
using namespace std;

class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> current;
        backtrack(nums, 0, current, result);
        return result;
    }
    
private:
    void backtrack(const vector<int>& nums, int start, 
                  vector<int>& current, 
                  vector<vector<int>>& result) {
        // 每个状态都是一个有效的子集
        result.push_back(current);
        
        // 从start开始,避免重复
        for (int i = start; i < nums.size(); i++) {
            current.push_back(nums[i]);
            backtrack(nums, i + 1, current, result);
            current.pop_back();
        }
    }
};

class SubsetsTest {
public:
    void runAllTests() {
        testEmptySet();
        testSingleElement();
        testMultipleElements();
        testDuplicateElements();
        std::cout << "所有测试用例通过!" << std::endl;
    }

private:
    Solution solution;

    // 测试空集合
    void testEmptySet() {
        vector<int> nums;
        auto result = solution.subsets(nums);
        assert(result.size() == 1);  // 只包含空集
        assert(result[0].empty());
        std::cout << "空集合测试通过" << std::endl;
    }

    // 测试单个元素
    void testSingleElement() {
        vector<int> nums = {1};
        auto result = solution.subsets(nums);
        assert(result.size() == 2);  // 空集和{1}
        
        // 验证结果包含[]和[1]
        bool hasEmpty = false;
        bool hasSingle = false;
        for (const auto& subset : result) {
            if (subset.empty()) hasEmpty = true;
            if (subset.size() == 1 && subset[0] == 1) hasSingle = true;
        }
        assert(hasEmpty && hasSingle);
        std::cout << "单个元素测试通过" << std::endl;
    }

    // 测试多个元素
    void testMultipleElements() {
        vector<int> nums = {1, 2, 3};
        auto result = solution.subsets(nums);
        assert(result.size() == 8);  // 2^3 = 8个子集
        
        // 验证一些特定子集
        set<vector<int>> resultSet(result.begin(), result.end());
        assert(resultSet.count(vector<int>{}));  // 空集
        assert(resultSet.count(vector<int>{1}));
        assert(resultSet.count(vector<int>{2}));
        assert(resultSet.count(vector<int>{3}));
        assert(resultSet.count(vector<int>{1, 2}));
        assert(resultSet.count(vector<int>{1, 3}));
        assert(resultSet.count(vector<int>{2, 3}));
        assert(resultSet.count(vector<int>{1, 2, 3}));
        
        std::cout << "多个元素测试通过" << std::endl;
    }

    // 测试包含重复元素的情况
    void testDuplicateElements() {
        vector<int> nums = {1, 2, 2};
        auto result = solution.subsets(nums);
        assert(result.size() == 8);  // 仍然是2^3,因为我们不去重
        
        // 打印所有子集用于调试
        std::cout << "重复元素的所有子集:" << std::endl;
        for (const auto& subset : result) {
            std::cout << "[ ";
            for (int num : subset) {
                std::cout << num << " ";
            }
            std::cout << "]" << std::endl;
        }
        
        std::cout << "重复元素测试通过" << std::endl;
    }
};

int main() {
    SubsetsTest test;
    test.runAllTests();
    return 0;
}