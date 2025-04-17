#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <stack>
using namespace std;
class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
       std::vector<int> res(temperatures.size(), 0);
       std::stack<int> st;
       for (int i = 0; i < temperatures.size(); i++)
       {

            // std::cout << "temperatures[i] = " << temperatures[i] << std::endl;
            // 直到找到一个比当前温度更高的温度为止
            while (!st.empty() && temperatures[i] > temperatures[st.top()])
            {
                int preIndex = st.top();
                st.pop();
                res[preIndex] = i - preIndex;
            }
            st.push(i);
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
        // 基本测试 - readme中的示例
        runTest("基本测试 - readme示例", [this]() {
            vector<int> temperatures = {73,74,75,71,69,72,76,73};
            vector<int> expected = {1,1,4,2,1,1,0,0};
            vector<int> result = solution.dailyTemperatures(temperatures);
            return vectorEqual(result, expected);
        });

        // 边界测试 - 空数组
        runTest("边界测试 - 空数组", [this]() {
            vector<int> temperatures = {};
            vector<int> expected = {};
            vector<int> result = solution.dailyTemperatures(temperatures);
            return vectorEqual(result, expected);
        });

        // 边界测试 - 单个元素
        runTest("边界测试 - 单个元素", [this]() {
            vector<int> temperatures = {30};
            vector<int> expected = {0};
            vector<int> result = solution.dailyTemperatures(temperatures);
            return vectorEqual(result, expected);
        });

        // 特殊情况 - 递增序列
        runTest("特殊情况 - 递增序列", [this]() {
            vector<int> temperatures = {30,40,50,60};
            vector<int> expected = {1,1,1,0};
            vector<int> result = solution.dailyTemperatures(temperatures);
            return vectorEqual(result, expected);
        });

        // 特殊情况 - 递减序列
        runTest("特殊情况 - 递减序列", [this]() {
            vector<int> temperatures = {60,50,40,30};
            vector<int> expected = {0,0,0,0};
            vector<int> result = solution.dailyTemperatures(temperatures);
            return vectorEqual(result, expected);
        });

        // 特殊情况 - 相同温度
        runTest("特殊情况 - 相同温度", [this]() {
            vector<int> temperatures = {30,30,30,30};
            vector<int> expected = {0,0,0,0};
            vector<int> result = solution.dailyTemperatures(temperatures);
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