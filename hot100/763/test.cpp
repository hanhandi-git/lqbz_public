#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <stack>
using namespace std;

class Solution {
public:
    vector<int> partitionLabels(string S) {
        std::vector<int> mapping(26, 0);
        std::vector<int> res;
        for (int i = 0; i < S.size(); i++)
        {
            mapping[S[i] - 'a'] = i;
        }
        int start = 0;
        int end = 0;
        for (int i = 0; i < S.size(); i++)
        {
            end = std::max(end, mapping[S[i] - 'a']);
            if (end == i) {
                res.push_back(end - start + 1);
                start = i + 1;
            }
        }

        return res;
    }
};

// 测试框架
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    // 辅助函数:检查两个vector是否相等
    bool vectorEqual(const vector<int>& v1, const vector<int>& v2) {
        if (v1.size() != v2.size()) return false;
        for (size_t i = 0; i < v1.size(); i++) {
            if (v1[i] != v2[i]) return false;
        }
        return true;
    }

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
        // 新增测试 - partitionLabels
        runTest("基本测试 - partitionLabels示例", [this]() {
            string S = "ababcbacadefegdehijhklij";
            vector<int> expected = {9, 7, 8};
            vector<int> result = solution.partitionLabels(S);
            return vectorEqual(result, expected);
        });

        // 新增测试用例
        runTest("测试用例 - 单个字符", [this]() {
            string S = "a";
            vector<int> expected = {1};
            vector<int> result = solution.partitionLabels(S);
            return vectorEqual(result, expected);
        });

        runTest("测试用例 - 全部相同字符", [this]() {
            string S = "aaaaaa";
            vector<int> expected = {6};
            vector<int> result = solution.partitionLabels(S);
            return vectorEqual(result, expected);
        });

        runTest("测试用例 - 不同字符", [this]() {
            string S = "abcde";
            vector<int> expected = {1, 1, 1, 1, 1};
            vector<int> result = solution.partitionLabels(S);
            return vectorEqual(result, expected);
        });

        runTest("测试用例 - 复杂情况", [this]() {
            string S = "eccbbbbdec";
            vector<int> expected = {10};
            vector<int> result = solution.partitionLabels(S);
            return vectorEqual(result, expected);
        });

        runTest("测试用例 - 边界情况", [this]() {
            string S = "abacabadabacabae";
            vector<int> expected = {15, 1};
            vector<int> result = solution.partitionLabels(S);
            return vectorEqual(result, expected);
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