#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <cxxabi.h>
#include <memory>

// 用于存储成员变量信息的结构体
struct MemberInfo {
    std::string name;        // 成员名称
    std::string type;        // 类型名称
    size_t size;            // 大小
    size_t alignment;       // 对齐要求
    size_t offset;          // 偏移量
    size_t padding;         // 填充字节数
    
    MemberInfo(const std::string& n, const std::string& t, size_t s, size_t a)
        : name(n), type(t), size(s), alignment(a), offset(0), padding(0) {}
};

// 用于获取类型的真实名称
std::string demangle(const char* name) {
    int status;
    char* demangledName = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    std::string result = (status == 0) ? demangledName : name;
    free(demangledName);
    return result;
}

class MemoryLayoutAnalyzer {
public:
    // 添加成员变量信息
    void addMember(const std::string& name, const std::string& type, size_t size, size_t alignment) {
        members.emplace_back(name, type, size, alignment);
    }

    // 分析当前内存布局
    void analyzeCurrentLayout() {
        size_t currentOffset = 0;
        size_t totalSize = 0;
        
        for (auto& member : members) {
            // 计算需要的填充
            size_t padding = (member.alignment - (currentOffset % member.alignment)) % member.alignment;
            member.padding = padding;
            member.offset = currentOffset + padding;
            
            // 更新偏移量
            currentOffset = member.offset + member.size;
            totalSize += member.size + padding;
        }
        
        // 考虑最终对齐
        size_t finalPadding = (8 - (totalSize % 8)) % 8;
        totalSize += finalPadding;
        
        originalSize = totalSize;
    }

    // 优化内存布局
    void optimizeLayout() {
        // 按照大小和对齐要求排序
        std::sort(members.begin(), members.end(), 
            [](const MemberInfo& a, const MemberInfo& b) {
                if (a.size != b.size)
                    return a.size > b.size;
                return a.alignment > b.alignment;
            });

        // 重新计算布局
        size_t currentOffset = 0;
        size_t totalSize = 0;
        
        for (auto& member : members) {
            size_t padding = (member.alignment - (currentOffset % member.alignment)) % member.alignment;
            member.padding = padding;
            member.offset = currentOffset + padding;
            currentOffset = member.offset + member.size;
            totalSize += member.size + padding;
        }
        
        // 考虑最终对齐
        size_t finalPadding = (8 - (totalSize % 8)) % 8;
        totalSize += finalPadding;
        
        optimizedSize = totalSize;
    }

    // 打印内存布局信息
    void printLayout(bool isOptimized = false) {
        std::cout << (isOptimized ? "Optimized" : "Original") << " Memory Layout:\n";
        std::cout << "Total size: " << (isOptimized ? optimizedSize : originalSize) << " bytes\n\n";
        
        for (const auto& member : members) {
            std::cout << "Member: " << member.name << "\n";
            std::cout << "  Type: " << member.type << "\n";
            std::cout << "  Size: " << member.size << " bytes\n";
            std::cout << "  Alignment: " << member.alignment << " bytes\n";
            std::cout << "  Offset: " << member.offset << " bytes\n";
            std::cout << "  Padding: " << member.padding << " bytes\n\n";
        }
    }

    // 获取优化建议
    void printOptimizationSuggestions() {
        std::cout << "Memory Optimization Suggestions:\n";
        std::cout << "Original size: " << originalSize << " bytes\n";
        std::cout << "Optimized size: " << optimizedSize << " bytes\n";
        std::cout << "Memory saved: " << originalSize - optimizedSize << " bytes\n\n";
        
        std::cout << "Suggested member order:\n";
        for (const auto& member : members) {
            std::cout << member.type << " " << member.name << ";\n";
        }
    }

private:
    std::vector<MemberInfo> members;
    size_t originalSize = 0;
    size_t optimizedSize = 0;
};

// 使用示例
int main() {
    MemoryLayoutAnalyzer analyzer;
    
    // 添加ComplexExample的成员
    analyzer.addMember("vptr", "virtual_ptr", 8, 8);
    analyzer.addMember("a", "char", 1, 1);
    analyzer.addMember("b", "int", 4, 4);
    analyzer.addMember("vec", "std::vector<int>", 24, 8);
    analyzer.addMember("ptr", "std::shared_ptr<int>", 16, 8);
    analyzer.addMember("atomicVar", "std::atomic<int>", 4, 4);
    analyzer.addMember("d", "double", 8, 8);
    analyzer.addMember("funcPtr", "void(*)()", 8, 8);

    // 分析当前布局
    analyzer.analyzeCurrentLayout();
    analyzer.printLayout();

    // 优化布局
    analyzer.optimizeLayout();
    analyzer.printLayout(true);

    // 打印优化建议
    analyzer.printOptimizationSuggestions();

    return 0;
}