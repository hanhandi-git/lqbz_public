#include <iostream>
#include <stack>
#include <functional>
#include <string>

// MinStack的实现
class MinStack {
private:
   std::stack<int> st;
   std::stack<int> minSt;
public:
    MinStack() {}
    
    void push(int val) {
       st.push(val);
       if (minSt.empty() || minSt.top() >= val) {
            minSt.push(val);
       }
    }
    
    void pop() {
        if (st.top() == minSt.top()) {
            minSt.pop();
        }
        st.pop();
    }
    
    int top() {
        return st.top();
    }
    
    int getMin() {
        return minSt.top();
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
        // 基本功能测试
        runTest("基本功能测试", [this]() {
            MinStack minStack;
            minStack.push(-2);
            minStack.push(0);
            minStack.push(-3);
            bool test1 = minStack.getMin() == -3;  // 返回 -3
            minStack.pop();
            bool test2 = minStack.top() == 0;      // 返回 0
            bool test3 = minStack.getMin() == -2;  // 返回 -2
            return test1 && test2 && test3;
        });

        // 重复元素测试
        runTest("重复元素测试", [this]() {
            MinStack minStack;
            minStack.push(1);
            minStack.push(1);
            minStack.push(1);
            bool test1 = minStack.getMin() == 1;
            minStack.pop();
            bool test2 = minStack.getMin() == 1;
            return test1 && test2;
        });

        // 最小值变化测试
        runTest("最小值变化测试", [this]() {
            MinStack minStack;
            minStack.push(5);
            minStack.push(3);
            minStack.push(4);
            minStack.push(1);
            minStack.push(2);
            bool test1 = minStack.getMin() == 1;
            minStack.pop();
            minStack.pop();
            bool test2 = minStack.getMin() == 3;
            return test1 && test2;
        });

        // 单个元素测试
        runTest("单个元素测试", [this]() {
            MinStack minStack;
            minStack.push(42);
            bool test1 = minStack.top() == 42;
            bool test2 = minStack.getMin() == 42;
            return test1 && test2;
        });

        // 递增序列测试
        runTest("递增序列测试", [this]() {
            MinStack minStack;
            minStack.push(1);
            minStack.push(2);
            minStack.push(3);
            bool test1 = minStack.getMin() == 1;
            minStack.pop();
            bool test2 = minStack.getMin() == 1;
            return test1 && test2;
        });

        // 递减序列测试
        runTest("递减序列测试", [this]() {
            MinStack minStack;
            minStack.push(3);
            minStack.push(2);
            minStack.push(1);
            bool test1 = minStack.getMin() == 1;
            minStack.pop();
            bool test2 = minStack.getMin() == 2;
            return test1 && test2;
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