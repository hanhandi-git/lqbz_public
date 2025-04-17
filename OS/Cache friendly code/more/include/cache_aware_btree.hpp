#pragma once
#include <memory>
#include <vector>
#include <algorithm>

template<typename K, typename V>
class CacheAwareBPlusTree {
private:
    // 计算最优节点大小
    static const int CACHE_LINE_SIZE = 64;
    static const int NODE_SIZE = CACHE_LINE_SIZE * 4;  // 使用多个缓存行
    static const int KEY_SIZE = sizeof(K);
    static const int PTR_SIZE = sizeof(void*);
    static const int ORDER = (NODE_SIZE - PTR_SIZE) / (KEY_SIZE + PTR_SIZE);
    
    struct alignas(CACHE_LINE_SIZE) Node {
        bool isLeaf;
        int count;
        K keys[ORDER];
        union {
            Node* children[ORDER + 1];    // 内部节点
            V values[ORDER];              // 叶子节点
        };
        Node* next;  // 叶子节点链表
        
        Node(bool leaf = true) : isLeaf(leaf), count(0), next(nullptr) {
            if(!isLeaf) {
                for(int i = 0; i <= ORDER; i++) {
                    children[i] = nullptr;
                }
            }
        }
        
        // 预取优化
        void prefetchChildren() {
            if(!isLeaf) {
                for(int i = 0; i < count + 1; i++) {
                    __builtin_prefetch(children[i], 0, 3);
                }
            }
        }
    };
    
    Node* root;
    
public:
    CacheAwareBPlusTree() : root(new Node()) {}
    
    void insert(const K& key, const V& value) {
        if(root->count == ORDER) {
            // 分裂根节点
            Node* newRoot = new Node(false);
            newRoot->children[0] = root;
            splitChild(newRoot, 0);
            root = newRoot;
        }
        insertNonFull(root, key, value);
    }
    
    bool find(const K& key, V& value) {
        Node* node = root;
        while(!node->isLeaf) {
            node->prefetchChildren();  // 预取子节点
            int i = 0;
            while(i < node->count && key >= node->keys[i]) {
                i++;
            }
            node = node->children[i];
        }
        
        // 在叶子节点中查找
        for(int i = 0; i < node->count; i++) {
            if(node->keys[i] == key) {
                value = node->values[i];
                return true;
            }
        }
        return false;
    }
    
private:
    void splitChild(Node* parent, int index) {
        Node* child = parent->children[index];
        Node* newNode = new Node(child->isLeaf);
        
        // 分裂节点
        int mid = ORDER / 2;
        newNode->count = ORDER - mid - 1;
        
        for(int i = 0; i < newNode->count; i++) {
            newNode->keys[i] = child->keys[mid + 1 + i];
            if(child->isLeaf) {
                newNode->values[i] = child->values[mid + 1 + i];
            } else {
                newNode->children[i] = child->children[mid + 1 + i];
            }
        }
        
        if(!child->isLeaf) {
            newNode->children[newNode->count] = child->children[ORDER];
        }
        
        child->count = mid;
        
        // 更新父节点
        for(int i = parent->count; i > index; i--) {
            parent->keys[i] = parent->keys[i-1];
            parent->children[i+1] = parent->children[i];
        }
        
        parent->keys[index] = child->keys[mid];
        parent->children[index+1] = newNode;
        parent->count++;
        
        // 维护叶子节点链表
        if(child->isLeaf) {
            newNode->next = child->next;
            child->next = newNode;
        }
    }
    
    void insertNonFull(Node* node, const K& key, const V& value) {
        int i = node->count - 1;
        
        if(node->isLeaf) {
            while(i >= 0 && key < node->keys[i]) {
                node->keys[i+1] = node->keys[i];
                node->values[i+1] = node->values[i];
                i--;
            }
            node->keys[i+1] = key;
            node->values[i+1] = value;
            node->count++;
        } else {
            while(i >= 0 && key < node->keys[i]) {
                i--;
            }
            i++;
            
            if(node->children[i]->count == ORDER) {
                splitChild(node, i);
                if(key > node->keys[i]) {
                    i++;
                }
            }
            insertNonFull(node->children[i], key, value);
        }
    }
}; 