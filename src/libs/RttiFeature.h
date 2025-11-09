#pragma once

#include <string>

template <typename T>
constexpr std::string type_name() {
#ifdef __clang__
    std::string p = __PRETTY_FUNCTION__; // "constexpr std::string_view type_name() [T = int]"
    auto start = p.find("[T = ") + 5;
    auto end = p.find(']', start);
    return p.substr(start, end - start);
#elif defined(__GNUC__)
    std::string p = __PRETTY_FUNCTION__; // "constexpr std::string_view type_name() [with T = int]"
    auto start = p.find("T = ") + 4;
    auto end = p.find(']', start);
    return p.substr(start, end - start);
#elif defined(_MSC_VER)
    std::string p = __FUNCSIG__; // "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl type_name<int>(void)"
    auto start = p.find("type_name<") + 10;
    auto end = p.find('>', start);
    return p.substr(start, end - start);
#else
#   error Unsupported compiler
#endif
}