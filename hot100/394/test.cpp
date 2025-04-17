#include <iostream>
#include <stack>
#include <functional>
#include <string>

class Solution {
public:
    bool isdigist(char& c) {
        if (c >= '0' && c <= '9') return true;
        else return false;
    }

    bool isstr(char& c) {
        if (c >= 'a' && c <= 'z') return true;
        else return false;
    }

    std::string decodeString(std::string s) {
        std::stack<std::string> str_st;
        std::stack<int> cnt_st;
        std::string res = "";
        int cnt = 0;
        for (auto c : s)
        {
            if (isdigist(c)) cnt = cnt * 10 + c - '0';
            if (isstr(c)) res += c;
            if (c == '[') {
                cnt_st.push(cnt);
                str_st.push(res);
                cnt = 0;
                res = "";
            } else if (c == ']') {
                auto tmp_str = res;     // 保存当前的结果字符串
                res = str_st.top();     // 从字符串栈中弹出之前保存的字符串
                str_st.pop();
                int tmp_cnt = cnt_st.top();
                cnt_st.pop();
                for (int i = 0; i < tmp_cnt; i++) {
                    res += tmp_str;
                }
            }
        }
        // std::cout << res << std::endl;
        return res;
    }
};

// 测试框架
class TestFramework {
private:
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
        // 解码字符串测试
        runTest("解码字符串测试", [this]() {
            Solution solution;
            std::string test1 = solution.decodeString("3[a2[c]]"); // 应该返回 "accaccacc"
            std::string test2 = solution.decodeString("2[abc]3[cd]ef"); // 应该返回 "abcabccdcdcdef"
            std::string test3 = solution.decodeString(""); // 应该返回 ""
            std::string test4 = solution.decodeString("abc"); // 应该返回 "abc"
            std::string test5 = solution.decodeString("2[3[a]]"); // 应该返回 "aaabaa"
            std::string test6 = solution.decodeString("2[abc3[cd]]"); // 应该返回 "abccdcdabccdcd"
            std::string test7 = solution.decodeString("4[ab]"); // 应该返回 "abababab"
            std::string test8 = solution.decodeString("1[a]"); // 应该返回 "a"
            return test1 == "accaccacc" && test2 == "abcabccdcdcdef" &&
                   test3 == "" && test4 == "abc" &&
                   test5 == "aaaaaa" && test6 == "abccdcdcdabccdcdcd" &&
                   test7 == "abababab" && test8 == "a";
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