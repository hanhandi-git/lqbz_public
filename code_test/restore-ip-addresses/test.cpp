#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>
#include <sstream>
using namespace std;

class Solution {
public:
    // IP地址复原
    vector<string> restoreIpAddresses(string s) {
        vector<string> result;
        string current;
        backtrack(s, 0, 0, current, result);
        return result;
    }
    
private:
    void backtrack(const string& s, int pos, int segment, string current, vector<string>& result) {
        if (pos == s.size() && segment == 4) {
            result.push_back(current.substr(0, current.size()-1)); // 去掉最后的点号
            return;
        }
        if (pos == s.size() || segment == 4) return;
        int remain = s.size() - pos;
        // 当剩余字符过多或者过少
        if (remain > (4 - segment) * 3 || remain < (4 - segment) * 1) return;
        int num = 0;
        for (int i = 0; i < 3 && pos + i < s.size(); i++)
        {
            // 处理前导0
            if (s[pos] == '0' && i > 0) break;  // 如果以0开头，只允许单个0
            num = num * 10 + (s[pos + i] - '0');
            // 检查是否是有效的IP段 
            if (num <= 255) {
                backtrack(s, pos + i + 1, segment + 1, current + s.substr(pos, i+1) + ".", result);
            }
            if (num > 255) break;
        }
    }
};

// TestFramework类保持不变
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
        // IP地址复原测试
        runTest("IP地址复原 - 示例1", [this]() {
            vector<string> result = solution.restoreIpAddresses("25525511135");
            vector<string> expected = {"255.255.11.135", "255.255.111.35"};
            sort(result.begin(), result.end());
            sort(expected.begin(), expected.end());
            return result == expected;
        });

        runTest("IP地址复原 - 示例2", [this]() {
            vector<string> result = solution.restoreIpAddresses("0000");
            return result == vector<string>{"0.0.0.0"};
        });

        runTest("IP地址复原 - 示例3", [this]() {
            vector<string> result = solution.restoreIpAddresses("101023");
            vector<string> expected = {"1.0.10.23","1.0.102.3","10.1.0.23",
                                     "10.10.2.3","101.0.2.3"};
            sort(result.begin(), result.end());
            sort(expected.begin(), expected.end());
            return result == expected;
        });

        runTest("IP地址复原 - 空字符串", [this]() {
            return solution.restoreIpAddresses("").empty();
        });

        runTest("IP地址复原 - 无效长度", [this]() {
            return solution.restoreIpAddresses("1").empty();
        });

        runTest("IP地址复原 - 前导零", [this]() {
            vector<string> result = solution.restoreIpAddresses("010010");
            vector<string> expected = {"0.10.0.10","0.100.1.0"};
            sort(result.begin(), result.end());
            sort(expected.begin(), expected.end());
            return result == expected;
        });

        runTest("IP地址复原 - 全0测试", [this]() {
            vector<string> result = solution.restoreIpAddresses("00000");
            return result.empty();  // 不应该有有效结果，因为每段不能有前导0
        });

        runTest("IP地址复原 - 最大值测试", [this]() {
            vector<string> result = solution.restoreIpAddresses("255255255255");
            vector<string> expected = {"255.255.255.255"};
            return result == expected;
        });

        runTest("IP地址复原 - 最小值测试", [this]() {
            vector<string> result = solution.restoreIpAddresses("0000");
            vector<string> expected = {"0.0.0.0"};
            return result == expected;
        });

        runTest("IP地址复原 - 边界值测试1", [this]() {
            vector<string> result = solution.restoreIpAddresses("2552552551");
            vector<string> expected = {
                "255.255.255.1",
                "255.255.25.51"
            };
            sort(result.begin(), result.end());
            sort(expected.begin(), expected.end());
            return result == expected;
        });

        runTest("IP地址复原 - 边界值测试2", [this]() {
            vector<string> result = solution.restoreIpAddresses("1921680");
            vector<string> expected = {
                "1.9.216.80",
                "1.92.16.80",
                "1.92.168.0",
                "19.2.16.80",
                "19.2.168.0",
                "19.21.6.80",
                "19.21.68.0",
                "19.216.8.0",
                "192.1.6.80",
                "192.1.68.0",
                "192.16.8.0"
            };
            sort(result.begin(), result.end());
            sort(expected.begin(), expected.end());
            return result == expected;
        });

        runTest("IP地址复原 - 短字符串测试", [this]() {
            vector<string> result = solution.restoreIpAddresses("123");
            return result.empty();  // 太短，无法形成有效IP
        });

        runTest("IP地址复原 - 长字符串测试", [this]() {
            vector<string> result = solution.restoreIpAddresses("12345678901");
            return result.empty();  // 太长，无法形成有效IP
        });

        runTest("IP地址复原 - 前导零组合测试", [this]() {
            vector<string> result = solution.restoreIpAddresses("0279245587303");
            return result.empty();  // 数字太大且有前导零，无法形成有效IP
        });


        runTest("IP地址复原 - 连续数字测试", [this]() {
            vector<string> result = solution.restoreIpAddresses("99999999");
            vector<string> expected = {
                "99.99.99.99"
            };
            sort(result.begin(), result.end());
            sort(expected.begin(), expected.end());
            return result == expected;
        });

        runTest("IP地址复原 - 零开头组合测试", [this]() {
            vector<string> result = solution.restoreIpAddresses("0111");
            vector<string> expected = {"0.1.1.1"};
            return result == expected;
        });

        runTest("IP地址复原 - 特殊边界测试", [this]() {
            vector<string> result = solution.restoreIpAddresses("25525");
            vector<string> expected = {
                "2.5.5.25",
                "2.5.52.5",
                "2.55.2.5",
                "25.5.2.5",
            };
            sort(result.begin(), result.end());
            sort(expected.begin(), expected.end());
            return result == expected;
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