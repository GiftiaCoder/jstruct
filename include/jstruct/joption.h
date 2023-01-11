#pragma once

#include <cstddef>

namespace jstruct {

struct Option {
    enum Type {
        IGNORE_MISSING_FIELD = 1,
        IGNORE_ERROR_FIELD = 2,
    };
    uint64_t ignore_flags = 0;

    inline Option & set_ignore_missing_field() { ignore_flags |= IGNORE_MISSING_FIELD; return *this; }
    inline bool ignore_missing_field() const { return (bool) (ignore_flags & IGNORE_MISSING_FIELD); }

    inline Option & set_ignore_error_field() { ignore_flags |= IGNORE_ERROR_FIELD; return *this; }
    inline bool ignore_error_field() const { return (bool) (ignore_flags & IGNORE_ERROR_FIELD); }
};

} // ns jstruct
