#pragma once
#include "array_view.h"

namespace jstruct {

// jfield
template < typename T >
class JField {
public:
    inline JField(void *, const char *) {}
    inline ~JField() {}

    inline const T & operator() () const { return _val; }

private:
    T _val;
};


// jarray
template < typename T >
class JArray {
public:
    inline JArray(void *, const char *) {}
    inline ~JArray() {}

    inline size_t size() const { return _val.size(); }

    inline const ArrayView<T> & operator() () const { return _val; }
    inline const T & operator() (size_t i) const { return _val[i]; }
    inline const T & operator[] (size_t i) const { return _val[i]; }

private:
    ArrayView<T> _val;
};

// jstruct
class JStruct {
public:
    inline JStruct() {}
    inline JStruct(void *, const char *) {}
    inline ~JStruct() {}

    template < typename T > using Field = JField<T>;
    template < typename T > using Array = JArray<T>;
};

} // ns jstruct
