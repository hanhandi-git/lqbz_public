#include <iostream>
#include <cassert>
#include <string>
#include <unordered_map>
#include <climits>
using namespace std;

class Solution {
public:
    string minWindow(string s, string t) {
        if (s.empty() || t.empty()) return "";
        unordered_map<char, int> need;
        unordered_map<char, int> window;
        for (auto c : t) need[c]++;
        int valid = 0;
        int len = INT_MAX;
        int start = 0;
        int left = 0;
        int right = 0;
        while (right < s.size())
        {
            // 扩张
            char in = s[right];
            right++;
            if (need.count(in) > 0)
            {
                window[in]++;
                if (window[in] == need[in]) valid++;
            }

            // 收缩
            while (valid == need.size())
            {
                // 更新len和start
                if (right - left < len)
                {
                    len = right - left;
                    start = left;
                }
                char out = s[left];
                left++;
                if (need.count(out) > 0)
                {
                    if (window[out] == need[out]) valid--;
                    window[out]--;
                }
            }
        }
        if (len == INT_MAX) return "";
        else return s.substr(start, len);
    }
};

class MinWindowTest {
public:
    void runAllTests() {
        testBasicCase();
        testRepeatedChars();
        testSingleChar();
        testNoSolution();
        testEmptyStrings();
        testSameString();
        cout << "所有测试用例通过!" << endl;
    }

private:
    Solution solution;

    // 基本测试用例
    void testBasicCase() {
        string s = "ADOBECODEBANC";
        string t = "ABC";
        string result = solution.minWindow(s, t);
        assert(result == "BANC");
        cout << "基本测试用例通过" << endl;
    }

    // 测试包含重复字符的情况
    void testRepeatedChars() {
        string s = "aa";
        string t = "aa";
        string result = solution.minWindow(s, t);
        assert(result == "aa");
        cout << "重复字符测试通过" << endl;
    }

    // 测试目标字符串长度为1的情况
    void testSingleChar() {
        string s = "ADOBECODEBANC";
        string t = "A";
        string result = solution.minWindow(s, t);
        assert(result == "A");
        cout << "单字符测试通过" << endl;
    }

    // 测试无解的情况
    void testNoSolution() {
        string s = "ADOBECODEBANC";
        string t = "X";
        string result = solution.minWindow(s, t);
        assert(result == "");
        cout << "无解情况测试通过" << endl;
    }

    // 测试空字符串情况
    void testEmptyStrings() {
        string s = "";
        string t = "";
        string result = solution.minWindow(s, t);
        assert(result == "");
        
        s = "ABC";
        t = "";
        result = solution.minWindow(s, t);
        assert(result == "");
        cout << "空字符串测试通过" << endl;
    }

    // 测试s和t相同的情况
    void testSameString() {
        string s = "ABC";
        string t = "ABC";
        string result = solution.minWindow(s, t);
        assert(result == "ABC");
        cout << "相同字符串测试通过" << endl;
    }
};

int main() {
    MinWindowTest test;
    test.runAllTests();
    return 0;
}