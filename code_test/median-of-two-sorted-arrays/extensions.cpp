#include <vector>
#include <queue>
#include <iostream>
#include <functional>

/**
 * @brief 扩展问题的实现类
 */
class Extensions {
public:
    /**
     * @brief 寻找两个有序数组中第k小的元素
     * 
     * @param nums1 第一个有序数组
     * @param nums2 第二个有序数组
     * @param k 要找第k小的数
     * @return int 第k小的数
     */
    int findKthElement(std::vector<int>& nums1, std::vector<int>& nums2, int k) {
        int m = nums1.size(), n = nums2.size();
        int index1 = 0, index2 = 0;
        
        while (true) {
            // 边界情况处理
            if (index1 == m) return nums2[index2 + k - 1];
            if (index2 == n) return nums1[index1 + k - 1];
            if (k == 1) return std::min(nums1[index1], nums2[index2]);
            
            // 正常情况处理
            int newIndex1 = std::min(index1 + k/2 - 1, m - 1);
            int newIndex2 = std::min(index2 + k/2 - 1, n - 1);
            
            if (nums1[newIndex1] <= nums2[newIndex2]) {
                k -= (newIndex1 - index1 + 1);
                index1 = newIndex1 + 1;
            } else {
                k -= (newIndex2 - index2 + 1);
                index2 = newIndex2 + 1;
            }
        }
    }

    /**
     * @brief 合并两个有序数组
     * 
     * @param nums1 第一个数组
     * @param nums2 第二个数组
     * @return std::vector<int> 合并后的有序数组
     */
    std::vector<int> mergeSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
        std::vector<int> result;
        int i = 0, j = 0;
        
        while (i < nums1.size() && j < nums2.size()) {
            if (nums1[i] <= nums2[j]) {
                result.push_back(nums1[i++]);
            } else {
                result.push_back(nums2[j++]);
            }
        }
        
        while (i < nums1.size()) result.push_back(nums1[i++]);
        while (j < nums2.size()) result.push_back(nums2[j++]);
        
        return result;
    }
};

/**
 * @brief 动态中位数查找器类
 */
class MedianFinder {
private:
    std::priority_queue<int> maxHeap;   // 大根堆 存储小的数值
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;  // 小根堆 存储大的数值

public:
    /** 
     * @brief 添加一个数到数据流
     * @param num 要添加的数
     */
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
    
    /** 
     * @brief 获取当前的中位数
     * @return double 中位数
     */
    double findMedian() {
        if (maxHeap.empty()) return -1;
        if (maxHeap.size() == minHeap.size()) {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        } else {
            return maxHeap.top();
        }
    }
};

class TestFramework {
private:
    Extensions extensions;
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
        // 测试findKthElement
        runTest("findKthElement - 基本测试", [this]() {
            std::vector<int> nums1 = {1, 3, 5};
            std::vector<int> nums2 = {2, 4, 6};
            return extensions.findKthElement(nums1, nums2, 4) == 4;
        });

        runTest("findKthElement - 空数组", [this]() {
            std::vector<int> nums1 = {};
            std::vector<int> nums2 = {1, 2, 3};
            return extensions.findKthElement(nums1, nums2, 2) == 2;
        });

        // 测试mergeSortedArrays
        runTest("mergeSortedArrays - 基本测试", [this]() {
            std::vector<int> nums1 = {1, 3, 5};
            std::vector<int> nums2 = {2, 4, 6};
            std::vector<int> expected = {1, 2, 3, 4, 5, 6};
            return extensions.mergeSortedArrays(nums1, nums2) == expected;
        });

        runTest("mergeSortedArrays - 空数组", [this]() {
            std::vector<int> nums1 = {};
            std::vector<int> nums2 = {1, 2, 3};
            std::vector<int> expected = {1, 2, 3};
            return extensions.mergeSortedArrays(nums1, nums2) == expected;
        });

        // 测试MedianFinder
        runTest("MedianFinder - 基本测试", [this]() {
            MedianFinder mf;
            mf.addNum(1);
            mf.addNum(2);
            bool test1 = mf.findMedian() == 1.5;
            mf.addNum(3);
            bool test2 = mf.findMedian() == 2.0;
            return test1 && test2;
        });

        runTest("MedianFinder - 重复元素", [this]() {
            MedianFinder mf;
            mf.addNum(1);
            mf.addNum(1);
            bool test1 = mf.findMedian() == 1.0;
            mf.addNum(2);
            bool test2 = mf.findMedian() == 1.0;
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
