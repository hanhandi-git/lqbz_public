#ifndef FICUS_PRODUCT_OPOD_REALTIME_CLUSTER_PROTO_CODING_H
#define FICUS_PRODUCT_OPOD_REALTIME_CLUSTER_PROTO_CODING_H


#define FICUS_SUCC 0
#define FICUS_UNEXPECTED_ERROR -1

#include <string>
#include <type_traits>

template <typename T>
class ProtoCoding {
public:
    int Init(bool isEncrypted) {
        mIsEncrypted = isEncrypted;
        return FICUS_SUCC;
    }

    // 用于检查 id() 返回类型的辅助模板
    template <typename U>
    struct id_type {
        typedef decltype(std::declval<U>().id()) type;
    };

    // 处理 id() 返回 std::string 的情况
    template <typename U = T>
    typename std::enable_if<std::is_same<typename id_type<U>::type, std::string>::value, int>::type
    GetID(const T& source, std::string& target) const {
        target = source.id();
        return FICUS_SUCC;
    }

    // 处理 id() 返回 int64_t 的情况
    template <typename U = T>
    typename std::enable_if<std::is_same<typename id_type<U>::type, int64_t>::value, int>::type
    GetID(const T& source, std::string& target) const {
        target = std::to_string(source.id());
        return FICUS_SUCC;
    }

    // 处理其他情况（如果需要的话）
    template <typename U = T>
    typename std::enable_if<!std::is_same<typename id_type<U>::type, std::string>::value &&
                            !std::is_same<typename id_type<U>::type, int64_t>::value, int>::type
    GetID(const T& source, std::string& target) const {
        // 如果 id() 返回其他类型，可能需要添加更多的处理逻辑
        return FICUS_UNEXPECTED_ERROR;
    }

private:
    bool mIsEncrypted;
};


#endif // FICUS_PRODUCT_OPOD_REALTIME_CLUSTER_PROTO_CODING_H