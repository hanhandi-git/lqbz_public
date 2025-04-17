#include <iostream>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <string>
#include <vector>
#include <deque>
#include <array>
#include <forward_list>
#include <iterator>
#include <type_traits>  // 用于类型萃取工具

// 判断容器是否为关联容器
template <typename T>
struct is_associative_container : std::false_type {};

// 特化各种关联容器
template <typename Key, typename Compare, typename Allocator>
struct is_associative_container<std::set<Key, Compare, Allocator>> : std::true_type {};

template <typename Key, typename T, typename Compare, typename Allocator>
struct is_associative_container<std::map<Key, T, Compare, Allocator>> : std::true_type {};

template <typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct is_associative_container<std::unordered_set<Key, Hash, KeyEqual, Allocator>> : std::true_type {};

template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
struct is_associative_container<std::unordered_map<Key, T, Hash, KeyEqual, Allocator>> : std::true_type {};

// 通用移动函数，支持移动语义的容器
template<typename SrcContainer, typename DestContainer>
typename std::enable_if<
    !is_associative_container<SrcContainer>::value && 
    std::is_move_constructible<typename SrcContainer::value_type>::value>::type
moveElements(SrcContainer& source, DestContainer& destination) {
    std::cout << "Moving elements..." << std::endl;
    destination.insert(destination.end(),
                       std::make_move_iterator(source.begin()),
                       std::make_move_iterator(source.end()));
    source.clear();  // 清空源容器
}

// 通用复制函数，处理关联容器或不支持移动的容器
template<typename SrcContainer, typename DestContainer>
typename std::enable_if<
    is_associative_container<SrcContainer>::value || 
    !std::is_move_constructible<typename SrcContainer::value_type>::value>::type
moveElements(SrcContainer& source, DestContainer& destination) {
    std::cout << "Copying elements..." << std::endl;
    destination.insert(destination.end(), source.begin(), source.end());
    source.clear();  // 清空源容器（模拟“移动”）
}

template<typename Container>
void printContainer(const Container& container) {
    for (const auto& item : container) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

// 自定义类型，用于测试
class MyClass {
public:
    MyClass(int v) : value(v) {}
    MyClass(const MyClass&) = delete;  // 禁用拷贝构造
    MyClass(MyClass&&) = default;      // 启用移动构造
    int getValue() const { return value; }
private:
    int value;
};

std::ostream& operator<<(std::ostream& os, const MyClass& obj) {
    return os << obj.getValue();
}

int main() {
    // 测试 std::set
    std::set<std::string> sourceSet = {"apple", "banana", "cherry"};
    std::list<std::string> destinationList1;
    moveElements(sourceSet, destinationList1);
    std::cout << "After moving from set: ";
    printContainer(destinationList1);

    // 测试 std::map
    std::map<int, std::string> sourceMap = {{1, "one"}, {2, "two"}, {3, "three"}};
    std::vector<std::pair<int, std::string>> destinationVector1;
    moveElements(sourceMap, destinationVector1);
    std::cout << "After moving from map: ";
    for (const auto& pair : destinationVector1) {
        std::cout << "{" << pair.first << ", " << pair.second << "} ";
    }
    std::cout << std::endl;

    // 测试 std::unordered_set
    std::unordered_set<int> sourceUSet = {1, 2, 3, 4, 5};
    std::list<int> destinationList2;
    moveElements(sourceUSet, destinationList2);
    std::cout << "After moving from unordered_set: ";
    printContainer(destinationList2);

    // 测试 std::unordered_map
    std::unordered_map<char, int> sourceUMap = {{'a', 1}, {'b', 2}, {'c', 3}};
    std::vector<std::pair<char, int>> destinationVector2;
    moveElements(sourceUMap, destinationVector2);
    std::cout << "After moving from unordered_map: ";
    for (const auto& pair : destinationVector2) {
        std::cout << "{" << pair.first << ", " << pair.second << "} ";
    }
    std::cout << std::endl;

    // 测试 std::vector（非关联容器，支持移动）
    std::vector<std::string> sourceVector = {"dog", "cat", "bird"};
    std::list<std::string> destinationList3;
    moveElements(sourceVector, destinationList3);
    std::cout << "After moving from vector: ";
    printContainer(destinationList3);

    // 测试 std::deque
    std::deque<int> sourceDeque = {10, 20, 30, 40, 50};
    std::vector<int> destinationVector3;
    moveElements(sourceDeque, destinationVector3);
    std::cout << "After moving from deque: ";
    printContainer(destinationVector3);

    // 测试 std::array（注意：std::array 不能被清空）
    // std::array<int, 3> sourceArray = {100, 200, 300};
    // std::vector<int> destinationVector4;
    // moveElements(sourceArray, destinationVector4);
    // std::cout << "After moving from array: ";
    // printContainer(destinationVector4);
    // std::cout << "Original array after move: ";
    // printContainer(sourceArray);

    // 测试 std::forward_list
    std::forward_list<std::string> sourceForwardList = {"hello", "world", "forward"};
    std::list<std::string> destinationList4;
    moveElements(sourceForwardList, destinationList4);
    std::cout << "After moving from forward_list: ";
    printContainer(destinationList4);

    // 测试自定义类型 MyClass
    std::vector<MyClass> sourceMyClassVector;
    sourceMyClassVector.emplace_back(1000);
    sourceMyClassVector.emplace_back(2000);
    sourceMyClassVector.emplace_back(3000);
    std::list<MyClass> destinationMyClassList;
    moveElements(sourceMyClassVector, destinationMyClassList);
    std::cout << "After moving MyClass objects: ";
    printContainer(destinationMyClassList);

    return 0;
}
