#include <vector>
#include <iostream>
#include <functional>
#include <string>

class Solution {
public:
    void moveZeroes(std::vector<int>& nums) {
        int slow = 0;
        // 将非零元素移动到数组前面
        for(int fast = 0; fast < nums.size(); fast++) {
            if(nums[fast] != 0) {
                nums[slow++] = nums[fast];
            }
        }
        // 将剩余位置填充0
        while(slow < nums.size()) {
            nums[slow++] = 0;
        }
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

    bool expectEqual(const std::vector<int>& expected, const std::vector<int>& actual) {
        return expected == actual;
    }

public:
    void runAllTests() {
        runTest("示例1", [this]() {
            std::vector<int> nums = {0,1,0,3,12};
            std::vector<int> expected = {1,3,12,0,0};
            solution.moveZeroes(nums);
            return expectEqual(expected, nums);
        });

        runTest("示例2", [this]() {
            std::vector<int> nums = {0};
            std::vector<int> expected = {0};
            solution.moveZeroes(nums);
            return expectEqual(expected, nums);
        });

        runTest("没有零的情况", [this]() {
            std::vector<int> nums = {1,2,3};
            std::vector<int> expected = {1,2,3};
            solution.moveZeroes(nums);
            return expectEqual(expected, nums);
        });

        runTest("全是零的情况", [this]() {
            std::vector<int> nums = {0,0,0};
            std::vector<int> expected = {0,0,0};
            solution.moveZeroes(nums);
            return expectEqual(expected, nums);
        });

        runTest("零在开头", [this]() {
            std::vector<int> nums = {0,1,2};
            std::vector<int> expected = {1,2,0};
            solution.moveZeroes(nums);
            return expectEqual(expected, nums);
        });

        runTest("零在结尾", [this]() {
            std::vector<int> nums = {1,2,0};
            std::vector<int> expected = {1,2,0};
            solution.moveZeroes(nums);
            return expectEqual(expected, nums);
        });

        runTest("多个零散布", [this]() {
            std::vector<int> nums = {1,0,2,0,3,0};
            std::vector<int> expected = {1,2,3,0,0,0};
            solution.moveZeroes(nums);
            return expectEqual(expected, nums);
        });

        std::cout << "\n测试结果: " << passed << " 个通过, " 
                  << (total - passed) << " 个失败, " 
                  << total << " 个总数" << std::endl;
    }
};

int main() {
    TestFramework testFramework;
    testFramework.runAllTests();
    return 0;
}