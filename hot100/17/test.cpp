#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
using namespace std;
class Solution {
private:
    vector<string> res;
    std::string path;
    std::vector<string> mapping = {
        "",     // 0
        "",     // 1
        "abc",  // 2
        "def",  // 3
        "ghi",  // 4
        "jkl",  // 5
        "mno",  // 6
        "pqrs", // 7
        "tuv",  // 8
        "wxyz"  // 9
    };

    void backtrack(const string& digits, int index) {
        if (index == digits.size()) {
            res.push_back(path);
            return;
        }
        std::string letter = mapping[digits[index] - '0'];
        for (int i = 0; i < letter.size(); i++) {
            path.push_back(letter[i]);
            backtrack(digits, index + 1);
            path.pop_back();
        }
        return;  
    }
    
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return {};
        res.clear();
        path = "";
        backtrack(digits, 0);
        // 打印res具体内容
        // for (const auto& s : res) {
        //     std::cout << s << " ";
        // }
        // std::cout << std::endl;
        return res;
    }
};

// 测试框架
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    // 辅助函数：检查两个vector是否包含相同的元素(忽略顺序)
    bool areVectorsEqual(vector<string>& v1, vector<string>& v2) {
        if (v1.size() != v2.size()) return false;
        sort(v1.begin(), v1.end());
        sort(v2.begin(), v2.end());
        return v1 == v2;
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
        // 基本测试
        runTest("基本测试 - '23'", [this]() {
            vector<string> expected = {"ad","ae","af","bd","be","bf","cd","ce","cf"};
            vector<string> result = solution.letterCombinations("23");
            return areVectorsEqual(result, expected);
        });

        // 空字符串测试
        runTest("边界测试 - 空字符串", [this]() {
            vector<string> result = solution.letterCombinations("");
            return result.empty();
        });

        // 单个数字测试
        runTest("基本测试 - 单个数字 '2'", [this]() {
            vector<string> expected = {"a","b","c"};
            vector<string> result = solution.letterCombinations("2");
            return areVectorsEqual(result, expected);
        });

        // 包含4个字母的按键测试
        runTest("特殊测试 - 4字母按键 '7'", [this]() {
            vector<string> expected = {"p","q","r","s"};
            vector<string> result = solution.letterCombinations("7");
            return areVectorsEqual(result, expected);
        });

        // 长字符串测试
        runTest("特殊测试 - 长字符串 '234'", [this]() {
            vector<string> result = solution.letterCombinations("234");
            bool correctSize = result.size() == 3 * 3 * 3; // 应该有27个组合
            bool allLengthThree = all_of(result.begin(), result.end(),
                [](const string& s) { return s.length() == 3; });
            return correctSize && allLengthThree;
        });

        // 重复数字测试
        runTest("特殊测试 - 重复数字 '22'", [this]() {
            vector<string> expected = {"aa","ab","ac","ba","bb","bc","ca","cb","cc"};
            vector<string> result = solution.letterCombinations("22");
            return areVectorsEqual(result, expected);
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