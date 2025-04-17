#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <functional>

class Solution {
public:
    std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs) {
        std::unordered_map<std::string, std::vector<std::string>> mp;
        
        // 遍历每个字符串
        for(std::string& s : strs) {
            std::string key = s;
            std::sort(key.begin(), key.end());  // 排序后的字符串作为key
            mp[key].push_back(s);
        }
        
        // 转换哈希表为vector
        std::vector<std::vector<std::string>> res;
        for(auto& pair : mp) {
            res.push_back(pair.second);
        }
        
        return res;
    }
};

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

    bool expectEqual(const std::vector<std::vector<std::string>>& expected, 
                    const std::vector<std::vector<std::string>>& actual) {
        if (expected.size() != actual.size()) return false;
        
        // 创建两个集合用于比较
        std::vector<std::vector<std::string>> sortedExpected = expected;
        std::vector<std::vector<std::string>> sortedActual = actual;
        
        // 对每个组内的字符串排序
        for (auto& group : sortedExpected) {
            std::sort(group.begin(), group.end());
        }
        for (auto& group : sortedActual) {
            std::sort(group.begin(), group.end());
        }
        
        // 对组进行排序
        std::sort(sortedExpected.begin(), sortedExpected.end());
        std::sort(sortedActual.begin(), sortedActual.end());
        
        return sortedExpected == sortedActual;
    }

public:
    void runAllTests() {
        runTest("示例1", [this]() {
            std::vector<std::string> strs = {"eat","tea","tan","ate","nat","bat"};
            std::vector<std::vector<std::string>> expected = {
                {"bat"},
                {"nat","tan"},
                {"ate","eat","tea"}
            };
            return expectEqual(expected, solution.groupAnagrams(strs));
        });

        runTest("示例2", [this]() {
            std::vector<std::string> strs = {""};
            std::vector<std::vector<std::string>> expected = {{""}};
            return expectEqual(expected, solution.groupAnagrams(strs));
        });

        runTest("示例3", [this]() {
            std::vector<std::string> strs = {"a"};
            std::vector<std::vector<std::string>> expected = {{"a"}};
            return expectEqual(expected, solution.groupAnagrams(strs));
        });

        runTest("空输入", [this]() {
            std::vector<std::string> strs = {};
            std::vector<std::vector<std::string>> expected = {};
            return expectEqual(expected, solution.groupAnagrams(strs));
        });

        runTest("多组异位词", [this]() {
            std::vector<std::string> strs = {"abc","cba","bac","foo","oof","bar"};
            std::vector<std::vector<std::string>> expected = {
                {"abc","cba","bac"},
                {"foo","oof"},
                {"bar"}
            };
            return expectEqual(expected, solution.groupAnagrams(strs));
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