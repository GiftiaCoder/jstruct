#pragma once

#include "jbuilder.h"

namespace jstruct {

template < template < typename Impl > typename T >
const T<JStructBuilder> & get_builder() {
    class Builder final {
    public:
        inline Builder() { _builder.init(); }
        inline const T<JStructBuilder> & operator() () const { return _builder; }
    private:
        T<JStructBuilder> _builder;
    };
    static Builder builder;
    return builder();
}

template < template < typename Impl > typename T >
const T<JStruct> * load(const JsonValue & jval, Allocer * allocer, Option * option = nullptr, Error * err = nullptr) {
    static Option DFT_OPTION = Option().set_ignore_missing_field();
    if (!option) option = &DFT_OPTION;

    using Data = T<JStruct>;
    Data * data = new(allocer->get<Data>(1)) Data;
    if (!get_builder<T>().load(jval, data, allocer, option, err)) {
        return nullptr;
    }
    return data;
}

} // ns jstruct
