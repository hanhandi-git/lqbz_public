#include <iostream>
#include <set>
#include <vector>
#include <iterator>

// 辅助函数：打印容器内容
template<typename Container>
void printContainer(const Container& container, const std::string& name) {
    std::cout << "Contents of " << name << ":" << std::endl;
    for (const auto& elem : container) {
        std::cout << elem << " ";
    }
    std::cout << std::endl << std::endl;
}

int main() {
    std::set<int> src = {1, 2, 3, 4, 5};
    std::vector<int> dst;

    // 使用 std::move 和迭代器将元素从 set 移动到 vector
    dst.insert(dst.end(), std::make_move_iterator(src.begin()), std::make_move_iterator(src.end()));
    

    // 打印 vector 的元素
    printContainer(src, "source");  // 这里发现source和dest中都有元素
    printContainer(dst, "dst");
    // 此时 src 处于有效但未定义的状态，可以重新填充或销毁
    return 0;
}