#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
using namespace std;

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        if (s.size() <= 1) return s.size();
        std::unordered_map<char, int> window;
        int left = 0;
        int right = 0;
        int max_len = 1;
        while(right < s.size())
        {
            char c = s[right];
            window[c]++;
            right++;
            while (window[c] > 1)   // 注意这里left需要一直移动，直到窗口内没有重复
            {
                window[s[left]]--;
                left++;
            }
            max_len = max(max_len, right - left);
        }
        return max_len;
    }
};

class TestFramework {
private:
    int passed = 0;
    int total = 0;

    void runTest(const std::string& testName, std::function<bool()> test) {
        total++;
        if (test()) {
            std::cout << "PASS: " << testName << std::endl;
            passed++;
        } else {
            std::cout << "FAIL: " << testName << std::endl;
        }
    }

public:
    void runAllTests() {
        Solution solution;

        runTest("Test Empty String", [&]() {
            return solution.lengthOfLongestSubstring("") == 0;
        });

        runTest("Test Single Character", [&]() {
            return solution.lengthOfLongestSubstring("a") == 1;
        });

        runTest("Test No Repeating Characters", [&]() {
            return solution.lengthOfLongestSubstring("abcde") == 5;
        });

        runTest("Test With Repeating Characters", [&]() {
            return solution.lengthOfLongestSubstring("abcabcbb") == 3;
        });

        runTest("Test All Repeating Characters", [&]() {
            return solution.lengthOfLongestSubstring("bbbbb") == 1;
        });

        runTest("Test Mixed Characters", [&]() {
            return solution.lengthOfLongestSubstring("pwwkew") == 3;
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
