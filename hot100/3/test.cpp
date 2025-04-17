#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
using namespace std;

class Solution {
public:
    int lengthOfLongestSubstring(const string& s) {
        if (s.size()<=1) return s.size();
        unordered_map<char, int> window;   // val -> freq
        int max_len = 0;
        int left = 0;
        int right = 0;
        while (right < s.size()) {
            // 进入
            char c = s[right];
            window[c]++;
            right++;

            // 收缩
            while (window[c] > 1) {
                char d = s[left];
                window[d]--;
                left++;
            }
            max_len = std::max(max_len, right - left);
        }
        return max_len;
    }
};


// class Solution {
// public:
//     int lengthOfLongestSubstring(const string& s) {
//         if (s.size() <= 1) return s.size();
//         std::unordered_map<char, int> window;   // val -> frq
//         int left = 0;
//         int right = 0;
//         int max_len = 0;
//         while (right < s.size()) {
//             // 扩大窗口
//             char c = s[right];
//             window[c]++;
//             right++;
//             // 收缩窗口
//             while (window[c] > 1) {
//                 char b = s[left];
//                 window[b]--;
//                 left++;
//             }
//             max_len = std::max(max_len, right - left);
//         }
//         return max_len;
//     }
// };

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
