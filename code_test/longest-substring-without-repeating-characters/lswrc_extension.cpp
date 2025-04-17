#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <unordered_set>
using namespace std;

void printVector(const vector<string>& vec) {
    cout << "Vector contents: ";
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<string>(cout, " "));
    cout << endl;
}

class Solution {
public:
    // 原始解法
    int lengthOfLongestSubstring(string s) {
        if (s.size() <= 1) return s.size();
        unordered_map<char, int> window;
        int left = 0;
        int right = 0;
        int maxlen = 1;
        while (right < s.size()) {
            // 扩大窗口
            char c = s[right];
            window[c]++;
            right++;
            // shrink window
            while(window[c] > 1) {
                window[s[left]]--;
                left++;
            }
            // 计算本窗口长度
            maxlen = max(maxlen, right - left);
        }
        return maxlen;
    }

    // 扩展1：找出最长的不含重复字符的子串
    string longestSubstringWithoutRepeating(string s) {
        if (s.size() <= 1) return s;
        std::unordered_map<char, int> window;
        int left = 0;
        int right = 0;
        int max_len = 1;
        int max_start = 0;
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
            if (right - left >= max_len) {
                max_start = left;
                max_len = right - left;
            }
        }
        return s.substr(max_start, max_len);
    }

    // 扩展2：找出所有最长的不含重复字符的子串
    vector<string> allLongestSubstringWithoutRepeating(string s) {
        vector<string> res;
        if (s.empty()) return res;
        unordered_map<char, int> window;
        unordered_set<string> unique_substrings;  // 用于去重
        int left = 0, right = 0, max_len = 0;

        while (right < s.size()) {
            char c = s[right];
            window[c]++;
            right++;

            while (window[c] > 1) {
                window[s[left]]--;
                left++;
            }

            int current_len = right - left;
            if (current_len > max_len) {
                max_len = current_len;
                unordered_set<string> temp;
                unique_substrings.swap(temp);
                unique_substrings.insert(s.substr(left, max_len));
            } else if (current_len == max_len) {
                unique_substrings.insert(s.substr(left, max_len));
            }
        }
        // 用新元素替换容器中的所有现有元素
        res.assign(unique_substrings.begin(), unique_substrings.end());
        return res;
    }
};

class TestFramework {
private:
    int passed = 0;
    int total = 0;

    void runTest(const string& testName, function<bool()> test) {
        total++;
        if (test()) {
            cout << "PASS: " << testName << endl;
            passed++;
        } else {
            cout << "FAIL: " << testName << endl;
        }
    }

public:
    void runAllTests() {
        Solution solution;

        // 测试 lengthOfLongestSubstring
        runTest("lengthOfLongestSubstring: Empty String", [&]() {
            return solution.lengthOfLongestSubstring("") == 0;
        });
        runTest("lengthOfLongestSubstring: Single Character", [&]() {
            return solution.lengthOfLongestSubstring("a") == 1;
        });
        runTest("lengthOfLongestSubstring: No Repeating Characters", [&]() {
            return solution.lengthOfLongestSubstring("abcde") == 5;
        });
        runTest("lengthOfLongestSubstring: With Repeating Characters", [&]() {
            return solution.lengthOfLongestSubstring("abcabcbb") == 3;
        });
        runTest("lengthOfLongestSubstring: All Repeating Characters", [&]() {
            return solution.lengthOfLongestSubstring("bbbbb") == 1;
        });
        runTest("lengthOfLongestSubstring: Mixed Characters", [&]() {
            return solution.lengthOfLongestSubstring("pwwkew") == 3;
        });

        // 测试 longestSubstringWithoutRepeating
        runTest("longestSubstringWithoutRepeating: Empty String", [&]() {
            return solution.longestSubstringWithoutRepeating("") == "";
        });
        runTest("longestSubstringWithoutRepeating: Single Character", [&]() {
            return solution.longestSubstringWithoutRepeating("a") == "a";
        });
        runTest("longestSubstringWithoutRepeating: No Repeating Characters", [&]() {
            return solution.longestSubstringWithoutRepeating("abcde") == "abcde";
        });
        runTest("longestSubstringWithoutRepeating: With Repeating Characters", [&]() {
            return solution.longestSubstringWithoutRepeating("abcabcbb") == "abc";
        });
        runTest("longestSubstringWithoutRepeating: All Repeating Characters", [&]() {
            return solution.longestSubstringWithoutRepeating("bbbbb") == "b";
        });
        runTest("longestSubstringWithoutRepeating: Mixed Characters", [&]() {
            string result = solution.longestSubstringWithoutRepeating("pwwkew");
            return result == "wke" || result == "kew";
        });

        // 测试 allLongestSubstringWithoutRepeating
        runTest("allLongestSubstringWithoutRepeating: Empty String", [&]() {
            return solution.allLongestSubstringWithoutRepeating("") == vector<string>{};
        });
        runTest("allLongestSubstringWithoutRepeating: Single Character", [&]() {
            vector<string> result = solution.allLongestSubstringWithoutRepeating("a");
            vector<string> expected = {"a"};
            return result.size() == expected.size() && std::is_permutation(result.begin(), result.end(), expected.begin());
        });
        runTest("allLongestSubstringWithoutRepeating: No Repeating Characters", [&]() {
            vector<string> result = solution.allLongestSubstringWithoutRepeating("abcde");
            vector<string> expected = {"abcde"};
            return result.size() == expected.size() && std::is_permutation(result.begin(), result.end(), expected.begin());
        });
        runTest("allLongestSubstringWithoutRepeating: With Repeating Characters", [&]() {
            vector<string> result = solution.allLongestSubstringWithoutRepeating("abcabcbb");
            printVector(result);
            vector<string> expected = {"abc", "bca", "cab"};
            return result.size() == expected.size() && std::is_permutation(result.begin(), result.end(), expected.begin());
        });
        runTest("allLongestSubstringWithoutRepeating: All Repeating Characters", [&]() {
            vector<string> result = solution.allLongestSubstringWithoutRepeating("bbbbb");
            printVector(result);
            vector<string> expected = {"b"};
            return result.size() == expected.size() && std::is_permutation(result.begin(), result.end(), expected.begin());
        });
        runTest("allLongestSubstringWithoutRepeating: Mixed Characters", [&]() {
            vector<string> result = solution.allLongestSubstringWithoutRepeating("pwwkew");
            vector<string> expected = {"wke", "kew"};
            return result.size() == expected.size() && std::is_permutation(result.begin(), result.end(), expected.begin());
        });

        cout << "\n测试结果: " << passed << " 通过, " 
             << (total - passed) << " 失败, " 
             << total << " 总计" << endl;
    }
};

int main() {
    TestFramework testFramework;
    testFramework.runAllTests();
    return 0;
}
