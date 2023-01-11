#pragma once

#include "allocer_base.h"
#include <parallel_hashmap/phmap.h>

namespace jstruct {

template < typename Base >
class StrDedupPlugin : public Base {
public:
    using Base::Base;

    std::string_view get_string(const char * str, size_t len) override {
        if (len > 0) {
            std::string_view sv = { str, len };
            auto it = _string_dedup_set.find(sv);
            if (it == _string_dedup_set.end()) {
                sv = Base::get_string(str, len);
                _string_dedup_set.insert(sv);
                return sv;
            } else {
                return *it;
            }
        } else {
            return std::string_view();
        }
    }
    void clear() override {
        _string_dedup_set.clear();
        Base::clear();
    }

private:
    ::phmap::node_hash_set<std::string_view> _string_dedup_set;
};

} // ns jstruct
