#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>
#include <sstream>
using namespace std;

// IP地址相关的扩展功能
class IPAddressExtensions {
public:
    // IP地址验证
    bool isValidIP(string IP) {
        vector<string> segments;
        string segment;
        stringstream ss(IP);
        while (getline(ss, segment, '.')) {
            segments.push_back(segment);
        }
        
        if (segments.size() != 4) return false;
        
        for (const string& s : segments) {
            if (!isValidSegment(s)) return false;
        }
        
        return true;
    }

    // IPv6地址验证
    string validIPv6(string IP) {
        vector<string> segments;
        string segment;
        stringstream ss(IP);
        
        // 检查是否以冒号结尾
        if (IP.back() == ':') return "Neither";
        
        while (getline(ss, segment, ':')) {
            segments.push_back(segment);
        }
        
        // 检查段数
        if (segments.size() != 8) return "Neither";
        
        // 验证每个段
        for (const string& s : segments) {
            if (!isValidIPv6Segment(s)) return "Neither";
        }
        
        return "IPv6";
    }

    // CIDR地址块计算
    vector<string> ipRange(string ip, int mask) {
        vector<string> result;
        unsigned int ipInt = ipToInt(ip);
        
        unsigned int netmask = 0xFFFFFFFF << (32 - mask);
        unsigned int network = ipInt & netmask;
        unsigned int broadcast = network | (~netmask);
        
        for (unsigned int i = network + 1; i < broadcast; i++) {
            result.push_back(intToIp(i));
        }
        
        return result;
    }

private:
    bool isValidSegment(const string& s) {
        if (s.empty() || s.length() > 3 || 
            (s.length() > 1 && s[0] == '0')) return false;
        
        try {
            int val = stoi(s);
            return val <= 255;
        } catch (...) {
            return false;
        }
    }

    bool isValidIPv6Segment(const string& s) {
        // 长度检查：每段必须在1到4个字符之间
        if (s.empty() || s.length() > 4) return false;
        
        // 检查每个字符是否是有效的十六进制数字
        for (char c : s) {
            if (!isxdigit(c)) return false;
            
            // 将字符转换为小写进行检查
            c = tolower(c);
            if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
                return false;
            }
        }
        
        return true;
    }

    unsigned int ipToInt(const string& ip) {
        unsigned int result = 0;
        vector<string> segments;
        string segment;
        stringstream ss(ip);
        
        while (getline(ss, segment, '.')) {
            result = (result << 8) + stoi(segment);
        }
        
        return result;
    }
    
    string intToIp(unsigned int ip) {
        return to_string(ip >> 24) + "." +
               to_string((ip >> 16) & 0xFF) + "." +
               to_string((ip >> 8) & 0xFF) + "." +
               to_string(ip & 0xFF);
    }
};

// 原有的LCS扩展功能保持不变
class TestFramework {
private:
    IPAddressExtensions ipExtensions;
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
        // IP地址相关测试
        runTest("IP地址验证 - 有效IP", [this]() {
            return ipExtensions.isValidIP("192.168.1.1");
        });

        runTest("IP地址验证 - 无效IP", [this]() {
            return !ipExtensions.isValidIP("256.1.2.3");
        });

        runTest("IP地址验证 - 前导零", [this]() {
            return !ipExtensions.isValidIP("01.1.1.1");
        });

        runTest("IPv6验证 - 有效IPv6", [this]() {
            return ipExtensions.validIPv6("2001:0db8:85a3:0000:0000:8a2e:0370:7334") == "IPv6";
        });

        runTest("IPv6验证 - 无效IPv6", [this]() {
            return ipExtensions.validIPv6("2001:0db8:85a3:0:0:8A2E:0370:7334:") == "Neither";
        });

        runTest("CIDR计算 - 基本测试", [this]() {
            vector<string> result = ipExtensions.ipRange("192.168.1.0", 24);
            return !result.empty() && result.front() == "192.168.1.1";
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
