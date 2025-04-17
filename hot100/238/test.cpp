#include <iostream>
#include <vector>
#include <functional>
#include <string>
using namespace std;
class Solution {
public:
    std::vector<int> productExceptSelf(std::vector<int>& nums) {
        int n = nums.size();
        vector<int> left(n);
        vector<int> right(n);
        vector<int> answer(n);
        
        // 计算left数组
        left[0] = 1;
        for(int i = 1; i < n; i++) {
            left[i] = left[i-1] * nums[i-1];
        }
        
        // 计算right数组
        right[n-1] = 1;
        for(int i = n-2; i >= 0; i--) {
            right[i] = right[i+1] * nums[i+1];
        }
        
        // 计算answer数组
        for(int i = 0; i < n; i++) {
            answer[i] = left[i] * right[i];
        }
        
        return answer;
    }
};

// 测试框架
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    // 辅助函数:比较两个vector是否相等
    bool compareVectors(const std::vector<int>& v1, const std::vector<int>& v2) {
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
        // 基本测试
        runTest("基本测试 - 示例1", [this]() {
            std::vector<int> nums = {1,2,3,4};
            std::vector<int> expected = {24,12,8,6};
            return compareVectors(solution.productExceptSelf(nums), expected);
        });

        runTest("基本测试 - 示例2", [this]() {
            std::vector<int> nums = {-1,1,0,-3,3};
            std::vector<int> expected = {0,0,9,0,0};
            return compareVectors(solution.productExceptSelf(nums), expected);
        });

        // 边界测试
        runTest("边界测试 - 两个元素", [this]() {
            std::vector<int> nums = {2,3};
            std::vector<int> expected = {3,2};
            return compareVectors(solution.productExceptSelf(nums), expected);
        });

        // 特殊测试
        runTest("特殊测试 - 包含1", [this]() {
            std::vector<int> nums = {1,2,3,1};
            std::vector<int> expected = {6,3,2,6};
            return compareVectors(solution.productExceptSelf(nums), expected);
        });

        runTest("特殊测试 - 包含单个0", [this]() {
            std::vector<int> nums = {1,0,3,4};
            std::vector<int> expected = {0,12,0,0};
            return compareVectors(solution.productExceptSelf(nums), expected);
        });

        runTest("特殊测试 - 包含多个0", [this]() {
            std::vector<int> nums = {0,2,0,4};
            std::vector<int> expected = {0,0,0,0};
            return compareVectors(solution.productExceptSelf(nums), expected);
        });

        runTest("特殊测试 - 全是1", [this]() {
            std::vector<int> nums = {1,1,1,1};
            std::vector<int> expected = {1,1,1,1};
            return compareVectors(solution.productExceptSelf(nums), expected);
        });

        runTest("特殊测试 - 较大数", [this]() {
            std::vector<int> nums = {2,10,5,20};
            std::vector<int> expected = {1000,200,400,100};
            return compareVectors(solution.productExceptSelf(nums), expected);
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