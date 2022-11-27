#pragma once

#include <cstddef>

namespace jstruct {

template < typename T >
class ArrayView final {
public:
    inline ArrayView() : _beg(nullptr), _end(nullptr) {}
    inline ArrayView(T * beg, T * end) : _beg(beg), _end(end) {}

    inline void reset(T * beg, T * end) {
        _beg = beg;
        _end = end;
    }

public:
    inline void clear() { _end = _beg; }

    inline size_t size() const { return _end - _beg; }
    inline bool empty() const { return _beg == _end; }

    inline T * data() { return _beg; }
    inline const T * data() const { return _beg; }

public:
    inline T & at(size_t i) { return _beg[i]; }
    inline T & operator[] (size_t i) { return _beg[i]; }

    inline const T & at(size_t i) const { return _beg[i]; }
    inline const T & operator[] (size_t i) const { return _beg[i]; }

public:
    using iterator = T *;
    using const_iterator = T const *;
    inline iterator begin() { return _beg; }
    inline iterator end() { return _end; }
    inline const_iterator begin() const { return _beg; }
    inline const_iterator end() const { return _end; }

private:
    T * _beg, *_end;
};

template < typename S, typename E >
S & operator<< (S & s, const ArrayView<E> & ary) {
    size_t sz = ary.size();
    if (sz > 0) {
        s << "[" << ary[0];
        for (size_t i = 1; i < sz; ++i) {
            s << "," << ary[i];
        }
        return s << "]";
    } else {
        return s << "[]";
    }
}

} // ns jstruct
