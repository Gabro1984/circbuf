/* The MailBox implementation which is based on a lock-free circular buffer */
/* Author: Roman Shishko */

#include "IMailBox.hpp"
#include <atomic>

constexpr uint32_t MAIL_BOX_SIZE = 16;

template <typename T>
class MailBoxImpl : public IMailBox<T> {
public:
    MailBoxImpl() : _tail(0), _head(0){}

    bool IsEmpty() override {
        return (_head.load() == _tail.load());
    }

    bool IsFull() override {
        const auto next_tail = posIncrement(_tail.load());
        return (next_tail == _head.load());
    }

    ErrorCode SendData(const T &data) override {
        auto current_tail = _tail.load();
        auto next_tail = posIncrement(current_tail);

        if(next_tail != _head.load()) {
            _array[current_tail] = data;
            _tail.store(next_tail);
            return ErrorCode::OK;
        }
        return ErrorCode::ERROR;  // buf is full
    }

    ErrorCode GetData(std::optional<T> &data) override {
        const auto current_head = _head.load();

        if(current_head == _tail.load()) {
            data = std::nullopt;
            return ErrorCode::ERROR;   // buf is empty
        }

        data = _array[current_head];
        _head.store(posIncrement(current_head));
        return ErrorCode::OK;
    }

private:
    std::atomic<size_t>  _tail;
    T              _array[MAIL_BOX_SIZE];
    std::atomic<size_t>  _head;

    size_t posIncrement(size_t idx) const  {
        return (idx + 1) % MAIL_BOX_SIZE;
    }
};
