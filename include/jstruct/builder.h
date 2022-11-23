#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <json/json.h>
#include "allocer.h"
#include "jstruct.h"
#include "jparser.h"

namespace jstruct {

// jstruct builder
class JBuilderBase {
public:
    virtual void reg(JBuilderBase * field, const char * name, size_t nmlen) {
        (void) field;
        (void) name;
        (void) nmlen;
    }
    virtual void init() {}
    virtual size_t msize() const = 0;
    virtual size_t malign() const = 0;

    virtual bool load(const ::Json::Value & jval, void * cval, Allocer * allocer) const = 0;
};

template < typename T, bool is_jstruct > class JFieldBuilder;
template < typename T >
class JFieldBuilder < T, false > : public JBuilderBase {
public:
    template < size_t N >
    JFieldBuilder(JBuilderBase * root, const char (&name)[N]) {
        root->reg(this, name, N - 1);
    }
    size_t msize() const override {
        return sizeof(T);
    }
    size_t malign() const override {
        return alignof(T);
    }

    bool load(const ::Json::Value & jval, void * cval, Allocer * allocer) const override {
        return JParser<T>::parse(jval, (T *) cval, allocer);
    }
};
template < typename T >
class JFieldBuilder < T, true > : public JBuilderBase {
public:
    template < size_t N >
    JFieldBuilder(JBuilderBase * root, const char (&name)[N]) {
        root->reg(this, name, N - 1);
    }
    void init() {
        _builder.init();
    }
    size_t msize() const override {
        return _builder.msize();
    }
    size_t malign() const override {
        return _builder.malign();
    }

    bool load(const ::Json::Value & jval, void * cval, Allocer * allocer) const override {
        return _builder.load(jval, cval, allocer);
    }

private:
    T _builder;
};

template < typename T, bool is_jstruct > class JArrayBuilder;
template < typename T >
class JArrayBuilder < T, false > : public JBuilderBase {
public:
    template < size_t N >
    JArrayBuilder(JBuilderBase * root, const char (&name)[N]) {
        root->reg(this, name, N - 1);
    }
    size_t msize() const override {
        return sizeof(ArrayView<int>);
    }
    size_t malign() const override {
        return alignof(ArrayView<int>);
    }

    bool load(const ::Json::Value & jval, void * cval, Allocer * allocer) const override {
        if (!jval.isArray()) {
            // TODO
            return false;
        }
        int n = jval.size();
        T * buf = allocer->get<T>(n);
        ((ArrayView<char> *) cval)->reset((char *) buf, ((char *) buf) + (n * sizeof(T)));
        for (int i = 0; i < n; ++i, ++buf) {
            if (!JParser<T>::parse(jval[i], buf, allocer)) {
                // TODO
                return false;
            }
        }
        return true;
    }
};
template < typename T >
class JArrayBuilder < T, true > : public JBuilderBase {
public:
    template < size_t N >
    JArrayBuilder(JBuilderBase * root, const char (&name)[N]) {
        root->reg(this, name, N - 1);
    }
    void init() {
        _builder.init();
    }
    size_t msize() const override {
        return sizeof(ArrayView<int>);
    }
    size_t malign() const override {
        return alignof(ArrayView<int>);
    }

    bool load(const ::Json::Value & jval, void * cval, Allocer * allocer) const override {
        if (!jval.isArray()) {
            // TODO
            return false;
        }
        int n = jval.size();
        char * buf = allocer->get<char>(n * _builder.msize());
        ((ArrayView<char> *) cval)->reset(buf, buf + (n * _builder.msize()));
        for (int i = 0; i < n; ++i, buf += _builder.msize()) {
            if (!_builder.load(jval[i], buf, allocer)) {
                // TODO
                return false;
            }
        }
        return true;
    }

private:
    T _builder;
};

class JStructBuilder : public JBuilderBase {
public:
    JStructBuilder() : _msize(0), _malign(0) {}
    template < size_t N >
    JStructBuilder(JBuilderBase * root, const char (&name)[N]) : _msize(0), _malign(0) {
        root->reg(this, name, N - 1);
    }

public:
    template < typename T > using Field = JFieldBuilder < T, std::is_base_of<JStructBuilder, T>::value >;
    template < typename T > using Array = JArrayBuilder < T, std::is_base_of<JStructBuilder, T>::value >;

public:
    void init() {
        const char * cur = nullptr;
        size_t space = -1LLU;
        size_t max_align = 0;
        for (auto & field : _fields) {
            field.field->init();
            std::align(field.field->malign(), field.field->msize(), (void *&) cur, space);
            field.offset = (size_t) cur;
            cur += field.field->msize();
            space -= field.field->msize();

            max_align = (max_align > field.field->malign()) ? max_align : field.field->malign();
        }
        std::align(max_align, (size_t) cur, (void *&) cur, space);

        _msize = (size_t) cur;
        _malign = max_align;
    }
    size_t msize() const override {
        return _msize;
    }
    size_t malign() const override {
        return _malign;
    }

    bool load(const ::Json::Value & jval, void * cval, Allocer * allocer) const override {
        if (!jval.isObject()) {
            // TODO
            return false;
        }
        for (auto & field : _fields) {
            auto * jsub = jval.find(field.name.c_str(), field.name.c_str() + field.name.size());
            if (!jsub) {
                // TODO
                return false;
            }
            char * ploc = field.offset + ((char *) cval);
            if (!field.field->load(*jsub, ploc, allocer)) {
                // TODO
                return false;
            }
        }
        return true;
    }

public:
    void reg(JBuilderBase * field, const char * name, size_t nmlen) override {
        FieldInfo info;
        info.field = field;
        info.name = { name, nmlen };
        _fields.push_back(info);
    }
    
private:
    struct FieldInfo {
        JBuilderBase * field = nullptr;
        std::string name;
        size_t offset = 0;
    };
    size_t _msize, _malign;
    std::vector<FieldInfo> _fields;
};

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
const T<JStruct> * load(const ::Json::Value & jval, Allocer * allocer) {
    using Data = T<JStruct>;
    Data * data = allocer->get<Data>(1);
    if (!get_builder<T>().load(jval, data, allocer)) {
        return nullptr;
    }
    return data;
}

template < template < typename Impl > typename T >
const T<JStruct> * load(const ::Json::Value & jval, int n, Allocer * allocer) {
    using Data = T<JStruct>;
    if (!jval.isArray() || jval.size() < n) {
        // TODO
        return nullptr;
    }
    Data * data = allocer->get<Data>(n);
    for (int i = 0; i < n; ++i) {
        if (!get_builder<T>().load(jval[i], data + i, allocer)) {
            return nullptr;
        }
    }
    return data;
}

} // ns jstruct
