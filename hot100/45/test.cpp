#include <vector>
#include <iostream>
#include <functional>

using namespace std;

class Solution {
public:
    int jump(vector<int>& nums) {
        int steps = 0;
        int start = 0;
        int end = 0;
        while (end < nums.size() - 1)
        {
            int max_deps = 0;
            for (int i = start; i <= end; i++)
            {
                max_deps = std::max(max_deps, i + nums[i]);
            }
            start = end + 1;
            end = max_deps;
            steps++;
        }
        return steps;
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

public:
    void runAllTests() {
        // 测试用例1: 示例1
        runTest("示例1", [this]() {
            vector<int> nums = {2, 3, 1, 1, 4};
            return solution.jump(nums) == 2;  // 预期返回2
        });

        // 测试用例2: 示例2
        runTest("示例2", [this]() {
            vector<int> nums = {2, 1};
            return solution.jump(nums) == 1;  // 预期返回1
        });

        // 测试用例3: 只有一个元素
        runTest("单个元素", [this]() {
            vector<int> nums = {0};
            return solution.jump(nums) == 0;  // 预期返回0
        });

        // 测试用例4: 只有两个元素，能跳到最后
        runTest("两个元素可达", [this]() {
            vector<int> nums = {1, 0};
            return solution.jump(nums) == 1;  // 预期返回1
        });

        // 测试用例5: 只有两个元素，不能跳到最后
        runTest("两个元素不可达", [this]() {
            vector<int> nums = {1,2,1,1,1};
            return solution.jump(nums) == 3;  // 预期返回0
        });

        // 测试用例6: 较长数组，能跳到最后
        runTest("长数组可达", [this]() {
            vector<int> nums = {2, 5, 0, 0};
            return solution.jump(nums) == 2;  // 预期返回1
        });

        // // 测试用例7: 较长数组，不能跳到最后
        // runTest("长数组不可达", [this]() {
        //     vector<int> nums = {1, 1, 0, 1, 0};
        //     return solution.jump(nums) == 0;  // 预期返回0
        // });

        // 测试用例8: 较长数组，边界情况
        runTest("边界情况", [this]() {
            vector<int> nums = {1, 2, 3, 4, 5, 0, 0, 0, 0};
            return solution.jump(nums) == 4;  // 预期返回3
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