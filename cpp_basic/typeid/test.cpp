#include "proto_coding.h"
#include <cassert>
#include <string>
#include <iostream>

// 返回int64_t的测试类
class IntIdTest {
public:
    int64_t id() const { return 12345; }
};

// 返回string的测试类 
class StringIdTest {
public:
    const ::std::string id() const { return "test_id"; }
};

// 返回string引用的测试类
class StringRefIdTest {
public:
    StringRefIdTest() {}
    const std::string& id() const {
        static const std::string test_id = "test_id"; // 静态变量
        return test_id;
    }
};

// 简单的断言函数
template<typename T>
bool assertEqual(const T& expected, const T& actual, const char* testName) {
    if (expected != actual) {
        std::cout << "Test failed: " << testName << std::endl;
        std::cout << "Expected: " << expected << ", but got: " << actual << std::endl;
        return false;
    }
    return true;
}

// 测试函数
bool testIntId() {
    IntIdTest test_obj;
    ProtoCoding<IntIdTest> coding;
    coding.Init(false);
    
    std::string result;
    if (!assertEqual(FICUS_SUCC, coding.GetID(test_obj, result), "IntIdTest GetID"))
        return false;
    if (!assertEqual(std::string("12345"), result, "IntIdTest result"))
        return false;
    return true;
}

bool testStringId() {
    StringIdTest test_obj;
    ProtoCoding<StringIdTest> coding;
    coding.Init(false);
    
    std::string result;
    if (!assertEqual(FICUS_SUCC, coding.GetID(test_obj, result), "StringIdTest GetID"))
        return false;
    if (!assertEqual(std::string("test_id"), result, "StringIdTest result"))
        return false;
    return true;
}

bool testStringRefId() {
    StringRefIdTest test_obj;
    ProtoCoding<StringRefIdTest> coding;
    coding.Init(false);
    
    std::string result;
    if (!assertEqual(FICUS_SUCC, coding.GetID(test_obj, result), "StringRefIdTest GetID"))
        return false;
    if (!assertEqual(std::string("ref_test_id"), result, "StringRefIdTest result"))
        return false;
    return true;
}

int main() {
    int failed = 0;
    
    std::cout << "Running tests..." << std::endl;
    
    if (!testIntId()) failed++;
    if (!testStringId()) failed++;
    if (!testStringRefId()) failed++;
    
    std::cout << "\nTest summary:" << std::endl;
    std::cout << "Total tests: 3" << std::endl;
    std::cout << "Failed: " << failed << std::endl;
    std::cout << "Passed: " << (3 - failed) << std::endl;
    
    return failed ? 1 : 0;
}