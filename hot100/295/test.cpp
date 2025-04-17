#include <vector>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <functional>
using namespace std;
// MedianFinder 类
class MedianFinder {
public:
    priority_queue<int, std::vector<int>> maxHeap;
    priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    /** initialize your data structure here. */
    MedianFinder() {}

    void addNum(int num) {
       maxHeap.push(num);
       minHeap.push(maxHeap.top());
       maxHeap.pop();

       if (minHeap.size() > maxHeap.size()) 
       {
            maxHeap.push(minHeap.top());
            minHeap.pop();
       }
    }

    double findMedian() {
       if (maxHeap.size() == minHeap.size()) {
            return (maxHeap.top() + minHeap.top()) / 2.0;
       } else {
            return maxHeap.top();
       }
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
        // 中位数测试
        runTest("中位数测试", [this]() {
            MedianFinder medianFinder;
            medianFinder.addNum(1);
            if (medianFinder.findMedian() != 1.0) return false;
            medianFinder.addNum(2);
            if (medianFinder.findMedian() != 1.5) return false;
            medianFinder.addNum(3);
            if (medianFinder.findMedian() != 2.0) return false;
            medianFinder.addNum(4);
            if (medianFinder.findMedian() != 2.5) return false;
            return true;
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