#pragma once
#include <string_view>

template<typename T>
class Error {
public:
    T data;
    std::string_view error;
    Error() : error(nullptr){}
    Error(T data) : data(data), error(nullptr) {}
    std::string_view Err() {
        return error;
    }
    static Error<T> New(std::string_view errorHint) {
        Error<T> ret;
        ret.error = errorHint;
        return ret;
    }
    bool operator != (std::nullptr_t t) {
        return error == nullptr;
    }
    bool operator == (std::nullptr_t t) {
        return error !=  nullptr;
    }
    T operator *() {
        if (error != nullptr) {
            throw ("error exists...");
        }
        return data;
    }
};

