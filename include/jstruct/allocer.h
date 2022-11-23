#pragma once

#include <cstddef>
#include <stdint.h>
#include <vector>
#include <string>
#include <string_view>
#include <parallel_hashmap/phmap.h>

namespace jstruct {

class Allocer {
public:
    template < typename T >
    inline T * get(size_t n) {
        return (T *) malloc(sizeof(T) * n);
    }

    template < char >
    inline char * get(size_t n) {
        return (char *) malloc(n + 1);
    }

public: // string optimized
    std::string_view get_string(const char * str, size_t len) {
        if (len > 0) {
            std::string_view sv = { str, len };
            auto it = _string_dedup_set.find(sv);
            if (it == _string_dedup_set.end()) {
                char * buf = this->template get<char>(len);
                std::memcpy(buf, str, len);
                buf[len] = '\0';
                _string_dedup_set.insert(sv = { buf, len });
                return sv;
            } else {
                return *it;
            }
        } else {
            return std::string_view();
        }
    }
    std::string_view get_string(const std::string & str) {
        return get_string(str.c_str(), str.size());
    }
    template < size_t N >
    std::string_view get_string(const char (&str)[N]) {
        return get_string(str, N - 1);
    }

public:
    virtual void clear() {
        _string_dedup_set.clear();
    }

protected:
    virtual void * malloc(size_t sz) = 0;

private:
    ::phmap::node_hash_set<std::string_view> _string_dedup_set;
};

} // ns jstruct
