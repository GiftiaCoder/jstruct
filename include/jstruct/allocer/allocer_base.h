#pragma once

#include <cstddef>
#include <stdint.h>
#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <parallel_hashmap/phmap.h>

namespace jstruct {

class AllocerBase {
public:
    template < typename T >
    inline T * get(size_t n) {
        return (T *) malloc(sizeof(T) * n);
    }

    template < char >
    inline char * get(size_t n) {
        return (char *) malloc(n + 1);
    }

    inline std::string_view get_string(const std::string & str) {
        return get_string(str.c_str(), str.size());
    }
    template < size_t N >
    inline std::string_view get_string(const char (&str)[N]) {
        return get_string(str, N - 1);
    }

public: // string optimized
    virtual std::string_view get_string(const char * str, size_t len) {
        char * buf = this->template get<char>(len);
        std::memcpy(buf, str, len);
        buf[len] = '\0';
        return std::string_view(buf, len);
    }

    virtual void * malloc(size_t sz) = 0;
    virtual void clear() = 0;
    virtual size_t space_used() = 0;
    virtual size_t total_space() = 0;
};

} // ns jstruct
