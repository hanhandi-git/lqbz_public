#pragma once
#include <vector>
#include <array>
#include <string>
#include <algorithm>

using std::vector;
using std::array;
using std::string;
using std::min;
using std::max;

class CacheAwareStringMatcher {
private:
    static const int CACHE_LINE_SIZE = 64;
    static const int CHUNK_SIZE = CACHE_LINE_SIZE * 4;
    
public:
    // KMP算法的缓存优化版本
    static vector<int> findAll(const string& text, const string& pattern) {
        vector<int> result;
        const int m = pattern.length();
        const int n = text.length();
        
        if(m == 0 || n == 0) return result;
        
        // 计算前缀函数
        vector<int> pi = computePrefixFunction(pattern);
        
        // 分块处理文本
        int q = 0;  // 已匹配的长度
        for(int i = 0; i < n; i += CHUNK_SIZE) {
            int chunk_end = min(i + CHUNK_SIZE, n);
            
            // 预取下一个块
            if(chunk_end < n) {
                __builtin_prefetch(&text[chunk_end], 0, 3);
            }
            
            // 处理当前块
            for(int j = i; j < chunk_end; j++) {
                while(q > 0 && pattern[q] != text[j])
                    q = pi[q-1];
                    
                if(pattern[q] == text[j])
                    q++;
                    
                if(q == m) {
                    result.push_back(j - m + 1);
                    q = pi[q-1];
                }
            }
        }
        
        return result;
    }
    
    // Boyer-Moore算法的缓存优化版本
    static vector<int> findAllBM(const string& text, const string& pattern) {
        vector<int> result;
        const int m = pattern.length();
        const int n = text.length();
        
        if(m == 0 || n == 0) return result;
        
        // 使用局部变量存储pattern以提高缓存命中率
        array<char, 1024> local_pattern;
        std::copy_n(pattern.begin(), min(1024UL, pattern.length()), 
               local_pattern.begin());
        
        // 预处理坏字符规则
        array<int, 256> badChar;
        preprocessBadCharacter(local_pattern.data(), m, badChar);
        
        // 分块处理文本
        for(int i = 0; i < n; i += CHUNK_SIZE) {
            int chunk_end = min(i + CHUNK_SIZE, n);
            
            // 预取下一个块
            if(chunk_end < n) {
                __builtin_prefetch(&text[chunk_end], 0, 3);
            }
            
            // 处理当前块
            int j = i;
            while(j <= chunk_end - m) {
                int k = m - 1;
                while(k >= 0 && local_pattern[k] == text[j + k])
                    k--;
                    
                if(k < 0) {
                    result.push_back(j);
                    j += 1;
                } else {
                    j += max(1, k - badChar[text[j + k]]);
                }
            }
        }
        
        return result;
    }
    
private:
    static vector<int> computePrefixFunction(const string& pattern) {
        const int m = pattern.length();
        vector<int> pi(m);
        
        // 预取pattern数据
        for(int i = 0; i < m; i += 16) {
            __builtin_prefetch(&pattern[min(i + 16, m)], 0, 3);
        }
        
        int k = 0;
        for(int q = 1; q < m; q++) {
            while(k > 0 && pattern[k] != pattern[q])
                k = pi[k-1];
                
            if(pattern[k] == pattern[q])
                k++;
                
            pi[q] = k;
        }
        
        return pi;
    }
    
    static void preprocessBadCharacter(const char* pattern, int size, 
                                     array<int, 256>& badChar) {
        badChar.fill(-1);
        for(int i = 0; i < size; i++) {
            badChar[pattern[i]] = i;
        }
    }
};

class CacheAwareStringSorter {
private:
    static const int CHUNK_SIZE = 64 * 4;  // 使用多个缓存行
    
public:
    // 基数排序的缓存优化版本
    static void radixSort(vector<string>& strings) {
        if(strings.empty()) return;
        
        // 找到最长字符串
        size_t maxLength = 0;
        for(const auto& s : strings) {
            maxLength = max(maxLength, s.length());
        }
        
        vector<string> temp(strings.size());
        
        // 按chunk处理字符串
        for(size_t pos = 0; pos < maxLength; pos += CHUNK_SIZE) {
            size_t chunk_end = min(pos + CHUNK_SIZE, maxLength);
            
            // 对当前chunk中的每个位置进行计数排序
            for(size_t p = pos; p < chunk_end; p++) {
                array<vector<string>, 256> buckets;
                
                // 分配到桶中
                for(const auto& s : strings) {
                    // 预取下一个字符串
                    if(p + CHUNK_SIZE < s.length()) {
                        __builtin_prefetch(&s[p + CHUNK_SIZE], 0, 0);
                    }
                    
                    char c = p < s.length() ? s[p] : 0;
                    buckets[c].push_back(s);
                }
                
                // 收集结果
                size_t index = 0;
                for(const auto& bucket : buckets) {
                    for(const auto& s : bucket) {
                        strings[index++] = s;
                    }
                }
            }
        }
    }
}; 