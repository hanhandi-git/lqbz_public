#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;

class Solution {
public:
    // 1. 字符串相乘
    string multiply(string num1, string num2) {
        if (num1 == "0" || num2 == "0") return "0";
        int n = num1.length();
        int m = num2.length();
        std::vector<int> res(n + m, 0);
        std::string result;
        for (int i = n - 1; i >= 0; i--)
        {
            for (int j = m - 1; j >= 0; j--)
            {
                int p1 = i + j; // 高位
                int p2 = i + j + 1; // 低位
                int sum = (num1[i] - '0') * (num2[j] - '0') + res[p2];
                res[p1] += sum / 10;
                res[p2] = sum % 10;
            }
        }
        // 去除前导0
        int index = 0;
        while(index < res.size() && res[index] == 0) index++;
        while(index < res.size()) result += res[index++] + '0';
        // 打印result
        std::cout << result << std::endl;
        if (result.empty()) return "0";
        else return result;
    }

    // 2. 二进制求和
    string addBinary(string a, string b) {
        string result;
        int carry = 0;
        int i = a.length() - 1;
        int j = b.length() - 1;
        
        while (i >= 0 || j >= 0 || carry > 0) {
            int sum = carry;
            if (i >= 0) sum += a[i--] - '0';
            if (j >= 0) sum += b[j--] - '0';
            
            carry = sum / 2;
            result += (sum % 2 + '0');
        }
        
        reverse(result.begin(), result.end());
        return result;
    }

    // 3. 两数相加（链表版本）
    struct ListNode {
        int val;
        ListNode *next;
        ListNode() : val(0), next(nullptr) {}
        ListNode(int x) : val(x), next(nullptr) {}
        ListNode(int x, ListNode *next) : val(x), next(next) {}
    };

    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy = ListNode(0);
        ListNode* cur = &dummy;
        int prev = 0;
        while (l1 || l2 || prev)
        {
            int sum = prev;
            if (l1) {
                sum += l1->val;
                l1 = l1->next;
            }
            if (l2) {
                sum += l2->val;
                l2 = l2->next;
            }
            ListNode* node = new ListNode(sum % 10);
            prev = sum / 10;
            cur->next = node;
            cur = cur->next;
        } 
        return dummy.next;
    }
};

// 测试框架
class TestFramework {
private:
    Solution solution;
    int passed = 0;
    int total = 0;

    void runTest(const string& testName, function<bool()> test) {
        total++;
        if (test()) {
            cout << "通过: " << testName << endl;
            passed++;
        } else {
            cout << "失败: " << testName << endl;
        }
    }

public:
    void runAllTests() {
        // 字符串相乘测试
        runTest("字符串相乘 - 基本测试", [this]() {
            return solution.multiply("123", "456") == "56088";
        });

        runTest("字符串相乘 - 零乘法", [this]() {
            return solution.multiply("0", "123") == "0";
        });

        runTest("字符串相乘 - 大数乘法", [this]() {
            return solution.multiply("999", "999") == "998001";
        });

        // 二进制求和测试
        runTest("二进制求和 - 基本测试", [this]() {
            return solution.addBinary("11", "1") == "100";
        });

        runTest("二进制求和 - 复杂测试", [this]() {
            return solution.addBinary("1010", "1011") == "10101";
        });

        runTest("二进制求和 - 进位测试", [this]() {
            return solution.addBinary("111", "111") == "1110";
        });

        // 链表相加测试
        runTest("链表相加 - 基本测试", [this]() {
            // 创建测试链表 342 + 465 = 807
            auto l1 = new Solution::ListNode(2);
            l1->next = new Solution::ListNode(4);
            l1->next->next = new Solution::ListNode(3);

            auto l2 = new Solution::ListNode(5);
            l2->next = new Solution::ListNode(6);
            l2->next->next = new Solution::ListNode(4);

            auto result = solution.addTwoNumbers(l1, l2);
            return result->val == 7 && 
                   result->next->val == 0 && 
                   result->next->next->val == 8;
        });

        runTest("链表相加 - 不同长度", [this]() {
            // 测试 99 + 1 = 100
            auto l1 = new Solution::ListNode(9);
            l1->next = new Solution::ListNode(9);

            auto l2 = new Solution::ListNode(1);

            auto result = solution.addTwoNumbers(l1, l2);
            return result->val == 0 && 
                   result->next->val == 0 && 
                   result->next->next->val == 1;
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
