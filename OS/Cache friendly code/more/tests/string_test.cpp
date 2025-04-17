#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <random>
#include <iomanip>
#include <algorithm>
#include "cache_aware_string.hpp"

using namespace std;
using namespace std::chrono;

/**
 * @brief 测试字符串处理的缓存优化效果
 */
class StringTester {
private:
    static const size_t TEXT_SIZE = 10 * 1024 * 1024;  // 10MB
    static const size_t PATTERN_SIZE = 1000;
    static const size_t STRING_COUNT = 1000000;
    static const size_t ITERATIONS = 10;
    
    string text;
    string pattern;
    vector<string> strings_to_sort;
    
public:
    StringTester() {
        // 生成随机文本
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(32, 126);  // 可打印字符
        
        text.reserve(TEXT_SIZE);
        for(size_t i = 0; i < TEXT_SIZE; i++) {
            text.push_back(dis(gen));
        }
        
        // 生成模式串
        pattern.reserve(PATTERN_SIZE);
        for(size_t i = 0; i < PATTERN_SIZE; i++) {
            pattern.push_back(dis(gen));
        }
        
        // 生成待排序字符串
        strings_to_sort.reserve(STRING_COUNT);
        uniform_int_distribution<> len_dis(10, 100);
        for(size_t i = 0; i < STRING_COUNT; i++) {
            string s;
            int len = len_dis(gen);
            s.reserve(len);
            for(int j = 0; j < len; j++) {
                s.push_back(dis(gen));
            }
            strings_to_sort.push_back(s);
        }
    }
    
    void runTest() {
        cout << "开始字符串处理性能测试...\n" << endl;
        
        auto std_search_time = testStdSearch();
        auto kmp_time = testKMP();
        auto bm_time = testBM();
        auto std_sort_time = testStdSort();
        auto radix_time = testRadixSort();
        
        cout << "\n测试结果汇总:" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "算法" << setw(15) << "耗时(ms)" << endl;
        cout << "----------------------------------------" << endl;
        cout << setw(20) << "std::search" << setw(15) << std_search_time << endl;
        cout << setw(20) << "优化KMP" << setw(15) << kmp_time << endl;
        cout << setw(20) << "优化BM" << setw(15) << bm_time << endl;
        cout << setw(20) << "std::sort" << setw(15) << std_sort_time << endl;
        cout << setw(20) << "优化基数排序" << setw(15) << radix_time << endl;
        cout << "----------------------------------------" << endl;
        
        if(std_search_time > 0) {
            cout << "字符串搜索优化提升:" << endl;
            cout << "KMP: " << fixed << setprecision(2) 
                 << static_cast<double>(std_search_time)/kmp_time << "x" << endl;
            cout << "BM: " << fixed << setprecision(2) 
                 << static_cast<double>(std_search_time)/bm_time << "x" << endl;
        }
        
        if(std_sort_time > 0) {
            cout << "字符串排序优化提升: " << fixed << setprecision(2) 
                 << static_cast<double>(std_sort_time)/radix_time << "x" << endl;
        }
    }
    
private:
    double testStdSearch() {
        cout << "测试std::search..." << endl;
        
        auto start = high_resolution_clock::now();
        volatile size_t count = 0;
        
        for(size_t iter = 0; iter < ITERATIONS; iter++) {
            auto it = text.begin();
            while(true) {
                it = search(it, text.end(), pattern.begin(), pattern.end());
                if(it == text.end()) break;
                count++;
                ++it;
            }
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
    
    double testKMP() {
        cout << "测试优化的KMP算法..." << endl;
        
        auto start = high_resolution_clock::now();
        volatile size_t count = 0;
        
        for(size_t iter = 0; iter < ITERATIONS; iter++) {
            auto matches = CacheAwareStringMatcher::findAll(text, pattern);
            count += matches.size();
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
    
    double testBM() {
        cout << "测试优化的Boyer-Moore算法..." << endl;
        
        auto start = high_resolution_clock::now();
        volatile size_t count = 0;
        
        for(size_t iter = 0; iter < ITERATIONS; iter++) {
            auto matches = CacheAwareStringMatcher::findAllBM(text, pattern);
            count += matches.size();
        }
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
    
    double testStdSort() {
        cout << "测试std::sort..." << endl;
        
        vector<string> strings = strings_to_sort;
        
        auto start = high_resolution_clock::now();
        
        sort(strings.begin(), strings.end());
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
    
    double testRadixSort() {
        cout << "测试优化的基数排序..." << endl;
        
        vector<string> strings = strings_to_sort;
        
        auto start = high_resolution_clock::now();
        
        CacheAwareStringSorter::radixSort(strings);
        
        auto end = high_resolution_clock::now();
        return duration_cast<milliseconds>(end - start).count();
    }
};

int main() {
    StringTester tester;
    tester.runTest();
    return 0;
} 