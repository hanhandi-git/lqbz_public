#include <stack>
#include <iostream>
#include <functional>

class MyQueue {
public:
    std::stack<int> stack1;
    std::stack<int> stack2;

    /** Initialize your data structure here. */
    MyQueue() {}

    /** Push element x to the back of queue. */
    void push(int x) {
        stack1.push(x);
    }

    /** Removes the element from in front of queue and returns that element. */
    int pop() {
        if (stack2.empty()) {
            while (!stack1.empty()) {
                stack2.push(stack1.top());
                stack1.pop();
            }
        }
        int front = stack2.top();
        stack2.pop();
        return front;
    }

    /** Get the front element. */
    int peek() {
        if (stack2.empty()) {
            while (!stack1.empty()) {
                stack2.push(stack1.top());
                stack1.pop();
            }
        }
        return stack2.top();
    }

    /** Returns whether the queue is empty. */
    bool empty() {
        return stack1.empty() && stack2.empty();
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
        // 测试入队和出队
        runTest("测试入队和出队", [this]() {
            MyQueue queue;
            queue.push(1);
            queue.push(2);
            return queue.pop() == 1 && queue.peek() == 2 && !queue.empty();
        });

        // 测试空队列
        runTest("测试空队列", [this]() {
            MyQueue queue;
            return queue.empty();
        });

        // 测试多次入队和出队
        runTest("测试多次入队和出队", [this]() {
            MyQueue queue;
            queue.push(3);
            queue.push(4);
            queue.pop(); // 弹出3
            return queue.peek() == 4 && !queue.empty();
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