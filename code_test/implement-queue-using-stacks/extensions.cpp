#include <iostream>
#include <queue>
#include <functional>

class MyStack {
public:
    std::queue<int> que;
    /** Initialize your data structure here. */
    MyStack() {}

    /** Push element x onto stack. */
    void push(int x) {
        que.push(x);
        int size = que.size();
        for (int i = 0; i < size - 1; i++)
        {
            que.push(que.front());
            que.pop();
        }
        return;
    }

    /** Removes the element on the top of the stack and returns that element. */
    int pop() {
        auto front = que.front();
        que.pop();
        return front;
    }

    /** Get the top element. */
    int top() {
        return que.front();
    }

    /** Returns whether the stack is empty. */
    bool empty() {
        return que.empty();
    }
};

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
        // 测试入栈和出栈
        runTest("测试入栈和出栈", [this]() {
            MyStack stack;
            stack.push(1);
            stack.push(2);
            return stack.pop() == 2 && stack.top() == 1 && !stack.empty();
        });

        // 测试空栈
        runTest("测试空栈", [this]() {
            MyStack stack;
            return stack.empty();
        });

        // 测试多次入栈和出栈
        runTest("测试多次入栈和出栈", [this]() {
            MyStack stack;
            stack.push(3);
            stack.push(4);
            stack.pop(); // 弹出4
            return stack.top() == 3 && !stack.empty();
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
