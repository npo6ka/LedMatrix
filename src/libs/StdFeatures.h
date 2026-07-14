#pragma once

#include <memory>
#include <utility>

namespace std {
    // Реализация make_unique для C++11
    template <typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}