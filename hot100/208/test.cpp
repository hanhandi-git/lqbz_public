#include <iostream>
#include <vector>
#include <string>
#include <functional>

// 前缀树节点类
class TrieNode {
public:
    std::vector<TrieNode*> childrens;
    bool isEnd;
    TrieNode() : childrens(std::vector<TrieNode*>(26, nullptr)), isEnd(false) {}
};
// 前缀树类
class Trie {
private:
    TrieNode* root;  // 前缀树的根节点
    
public:
    Trie() {
        root = new TrieNode();
    }
    
    // 插入单词
    void insert(const std::string& word) {
       auto node = root;
       for (char c : word)
       {
            int index = c - 'a';
            if (!node->childrens[index]) 
            {
                node->childrens[index] = new TrieNode();
            }
            node = node->childrens[index];
       }
       node->isEnd = true;
    }
    
    // 查找单词
    bool search(const std::string& word) {
        auto node = root;
        for (char c : word)
        {
            int index = c - 'a';
            if (!node->childrens[index])
            {
                return false;
            }
            node = node->childrens[index];
        }
        return node->isEnd;
    }
    
    // 查找前缀
    bool startsWith(const std::string& prefix) {
        auto node = root;
        for (char c : prefix)
        {
            int index = c - 'a';
            if (!node->childrens[index])
            {
                return false;
            }
            node = node->childrens[index];
        }
        return true;
    }
};

class TestFramework {
private:
    Trie trie;
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
        // 测试基本插入和查找
        runTest("测试用例1 - 基本插入和查找", [this]() {
            trie.insert("apple");
            return trie.search("apple") && !trie.search("app");
        });

        // 测试前缀查找
        runTest("测试用例2 - 前缀查找", [this]() {
            return trie.startsWith("app");
        });

        // 测试不存在的单词
        runTest("测试用例3 - 不存在的单词", [this]() {
            return !trie.search("banana") && !trie.startsWith("ban");
        });

        // 测试空字符串
        runTest("测试用例4 - 空字符串", [this]() {
            trie.insert("");
            return trie.search("") && trie.startsWith("");
        });

        // 测试重复插入
        runTest("测试用例5 - 重复插入", [this]() {
            trie.insert("apple");  // 重复插入相同单词
            return trie.search("apple");
        });

        // 测试多个单词
        runTest("测试用例6 - 多个单词", [this]() {
            trie.insert("app");
            return trie.search("app") && trie.search("apple") && 
                   trie.startsWith("app") && !trie.search("ap");
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