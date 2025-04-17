#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

int main() {
    // 创建一个随机的整数向量
    std::vector<int> data = {5, 3, 8, 4, 2, 7, 1, 6};

    // 打印原始向量
    std::cout << "Original vector: ";
    std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    // 使用 nth_element 找到第 4 个元素（索引为 3）的排序位置
    auto cmp = [](int a, int b){ return a < b; };
    // 注意：这里的 n 是从 0 开始的索引 
    // 调用后，第 4 个元素将位于其排序后应该在的位置
    std::nth_element(data.begin(), data.begin() + 3, data.end(), cmp);

    // 打印经过 nth_element 处理后的向量
    std::cout << "Vector after nth_element: ";
    std::copy(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    return 0;
}