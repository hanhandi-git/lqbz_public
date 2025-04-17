#include <iostream>
#include <cassert>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> result;
        vector<int> current;
        backtrack(candidates, target, 0, current, result);
        return result;
    }
    
private:
    void backtrack(const vector<int>& candidates, int target,
                  int start, vector<int>& current, 
                  vector<vector<int>>& result) {
        if (target == 0) {
            result.push_back(current);
            return;
        }
        
        for (int i = start; i < candidates.size(); i++) {
            if (target < candidates[i]) continue;
            current.push_back(candidates[i]);
            backtrack(candidates, target - candidates[i], i, current, result);
            current.pop_back();
        }
    }
};

class CombinationSumTest {
public:
    void runAllTests() {
        testExample1();
        testExample2();
        testNoSolution();
        testSingleElement();
        testRepeatedUse();
        std::cout << "所有测试用例通过!" << std::endl;
    }

private:
    Solution solution;

    // 测试示例1: candidates = [2,3,6,7], target = 7
    void testExample1() {
        vector<int> candidates = {2, 3, 6, 7};
        int target = 7;
        auto result = solution.combinationSum(candidates, target);
        
        // 期望结果: [[2,2,3],[7]]
        assert(result.size() == 2);
        
        set<vector<int>> resultSet;
        for (auto& comb : result) {
            sort(comb.begin(), comb.end());
            resultSet.insert(comb);
        }
        
        assert(resultSet.count(vector<int>{2, 2, 3}));
        assert(resultSet.count(vector<int>{7}));
        
        std::cout << "测试示例1通过" << std::endl;
    }
    
    // 测试示例2: candidates = [2,3,5], target = 8
    void testExample2() {
        vector<int> candidates = {2, 3, 5};
        int target = 8;
        auto result = solution.combinationSum(candidates, target);
        
        // 期望结果: [[2,2,2,2],[2,3,3],[3,5]]
        assert(result.size() == 3);
        
        set<vector<int>> resultSet;
        for (auto& comb : result) {
            sort(comb.begin(), comb.end());
            resultSet.insert(comb);
        }
        
        assert(resultSet.count(vector<int>{2, 2, 2, 2}));
        assert(resultSet.count(vector<int>{2, 3, 3}));
        assert(resultSet.count(vector<int>{3, 5}));
        
        std::cout << "测试示例2通过" << std::endl;
    }
    
    // 测试无解情况
    void testNoSolution() {
        vector<int> candidates = {2, 3, 5};
        int target = 1; // 无法组成
        auto result = solution.combinationSum(candidates, target);
        
        assert(result.empty());
        std::cout << "测试无解情况通过" << std::endl;
    }
    
    // 测试单个元素情况
    void testSingleElement() {
        vector<int> candidates = {5};
        int target = 5;
        auto result = solution.combinationSum(candidates, target);
        
        assert(result.size() == 1);
        assert(result[0].size() == 1);
        assert(result[0][0] == 5);
        
        std::cout << "测试单个元素通过" << std::endl;
    }
    
    // 测试同一元素反复使用
    void testRepeatedUse() {
        vector<int> candidates = {3};
        int target = 9;
        auto result = solution.combinationSum(candidates, target);
        
        assert(result.size() == 1);
        assert(result[0].size() == 3); // [3,3,3]
        
        for (int num : result[0]) {
            assert(num == 3);
        }
        
        std::cout << "测试元素重复使用通过" << std::endl;
    }
};

int main() {
    CombinationSumTest test;
    test.runAllTests();
    return 0;
}