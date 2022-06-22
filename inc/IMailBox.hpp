#pragma once

#include <optional>

enum class ErrorCode {
    OK,
    ERROR
};

template <typename T>
class IMailBox {
public:
    virtual bool IsEmpty() = 0;
    virtual bool IsFull() = 0;
    virtual ErrorCode SendData(const T &data) = 0;
    virtual ErrorCode GetData(std::optional<T> &data) = 0;
};