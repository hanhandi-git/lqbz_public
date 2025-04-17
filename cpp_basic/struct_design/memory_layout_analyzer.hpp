// memory_layout_analyzer.hpp
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <cstddef>
#include <algorithm>

// 用于存储成员信息的结构体
struct MemberInfo {
    const char* name;
    size_t offset;
    size_t size;
    size_t alignment;
    
    MemberInfo(const char* n, size_t o, size_t s, size_t a)
        : name(n), offset(o), size(s), alignment(a) {}
};

// 内存布局分析器
class MemoryLayoutAnalyzer {
    std::vector<MemberInfo> members;
    const char* className;

public:
    MemoryLayoutAnalyzer(const char* name) : className(name) {}

    // 添加成员信息的宏
    #define ADD_MEMBER(Class, Member) \
        addMember(#Member, offsetof(Class, Member), sizeof(((Class*)nullptr)->Member), alignof(decltype(((Class*)nullptr)->Member)))

    void addMember(const char* name, size_t offset, size_t size, size_t alignment) {
        members.emplace_back(name, offset, size, alignment);
    }

    void analyze() {
        std::cout << "\nAnalyzing class: " << className << "\n";
        std::cout << "Total size: " << members.back().offset + members.back().size << " bytes\n\n";

        size_t currentOffset = 0;
        for (size_t i = 0; i < members.size(); ++i) {
            const auto& member = members[i];
            size_t padding = member.offset - currentOffset;

            std::cout << "Member: " << member.name << "\n";
            std::cout << "  Offset: " << member.offset << " bytes\n";
            std::cout << "  Size: " << member.size << " bytes\n";
            std::cout << "  Alignment: " << member.alignment << " bytes\n";
            std::cout << "  Padding before: " << padding << " bytes\n";

            if (i < members.size() - 1) {
                size_t nextOffset = members[i + 1].offset;
                size_t paddingAfter = nextOffset - (member.offset + member.size);
                std::cout << "  Padding after: " << paddingAfter << " bytes\n";
            }
            std::cout << "\n";

            currentOffset = member.offset + member.size;
        }

        suggestOptimization();
    }

private:
    void suggestOptimization() {
        // 按大小和对齐要求排序
        auto sortedMembers = members;
        std::sort(sortedMembers.begin(), sortedMembers.end(),
            [](const MemberInfo& a, const MemberInfo& b) {
                if (a.alignment != b.alignment)
                    return a.alignment > b.alignment;
                return a.size > b.size;
            });

        std::cout << "Suggested member order to minimize padding:\n";
        for (const auto& member : sortedMembers) {
            std::cout << "  " << member.name << " (size: " << member.size 
                     << ", alignment: " << member.alignment << ")\n";
        }
    }
};