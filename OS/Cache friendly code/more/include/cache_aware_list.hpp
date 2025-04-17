#pragma once
#include <memory>
#include <vector>

template<typename T>
class CacheAwareList {
private:
    static const int POOL_SIZE = 1024;
    static const int NODES_PER_CHUNK = 64;  // 一个缓存行可以容纳的节点数
    
    struct Node {
        T data;
        Node* next;
    };
    
    // 节点内存池
    struct NodeChunk {
        alignas(64) Node nodes[NODES_PER_CHUNK];
        NodeChunk* next;
    };
    
    NodeChunk* chunks;
    Node* freeList;
    Node* head;
    size_t size;

public:
    CacheAwareList() : chunks(nullptr), freeList(nullptr), head(nullptr), size(0) {}
    
    ~CacheAwareList() {
        // 释放所有chunks
        while(chunks) {
            NodeChunk* next = chunks->next;
            delete chunks;
            chunks = next;
        }
    }
    
    void push_front(const T& value) {
        Node* node = allocateNode();
        node->data = value;
        node->next = head;
        head = node;
        size++;
    }
    
    bool pop_front(T& value) {
        if(!head) return false;
        
        Node* node = head;
        value = node->data;
        head = head->next;
        freeNode(node);
        size--;
        return true;
    }
    
    size_t get_size() const { return size; }

private:
    Node* allocateNode() {
        if(!freeList) {
            NodeChunk* chunk = new NodeChunk();
            chunk->next = chunks;
            chunks = chunk;
            
            // 初始化free list
            for(int i = 0; i < NODES_PER_CHUNK - 1; i++) {
                chunk->nodes[i].next = &chunk->nodes[i + 1];
            }
            chunk->nodes[NODES_PER_CHUNK-1].next = nullptr;
            freeList = &chunk->nodes[0];
        }
        
        Node* node = freeList;
        freeList = freeList->next;
        return node;
    }
    
    void freeNode(Node* node) {
        node->next = freeList;
        freeList = node;
    }
}; 