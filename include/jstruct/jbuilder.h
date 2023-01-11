#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <rapidjson/document.h>
#include "allocer.h"
#include "jstruct.h"
#include "jparser.h"
#include "jerror.h"
#include "joption.h"

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

    virtual bool load(const JsonValue & jval, void * cval, AllocerBase * allocer, Option * option, Error * err) const = 0;
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

    bool load(const JsonValue & jval, void * cval, AllocerBase * allocer, Option *, Error * err) const override {
        return JParser<T>::parse(jval, (T *) cval, allocer, err);
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

    bool load(const JsonValue & jval, void * cval, AllocerBase * allocer, Option * option, Error * err) const override {
        return _builder.load(jval, cval, allocer, option, err);
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

    bool load(const JsonValue & jval, void * cval, AllocerBase * allocer, Option *, Error * err) const override {
        if (!jval.IsArray()) {
	        if (err) *err << " is not array";
            return false;
        }
        int n = jval.Size(), i = 0;
        T * buf = allocer->get<T>(n);
        while (i < n) {
            if (JParser<T>::parse(jval[i], buf + i, allocer, err)) {
                ++i;
                continue;
            }
		    if (err) *err << "]" << i << "[";
            return false;
        }
        ((ArrayView<char> *) cval)->reset((char *) buf, ((char *) buf) + (i * sizeof(T)));
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

    bool load(const JsonValue & jval, void * cval, AllocerBase * allocer, Option * option, Error * err) const override {
        if (!jval.IsArray()) {
            if (err) *err << " is not array";
            return false;
        }
        int n = jval.Size(), i = 0;
        char * buf = allocer->get<char>(n * _builder.msize());
        while (i < n) {
            if (_builder.load(jval[i], buf + (i * _builder.msize()), allocer, option, err)) {
                ++i;
                continue;
            }
            if (err) *err << "]" << i << "[";
            return false;
        }
        ((ArrayView<char> *) cval)->reset(buf, buf + (i * _builder.msize()));
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

    bool load(const JsonValue & jval, void * cval, AllocerBase * allocer, Option * option, Error * err) const override {
        if (!jval.IsObject()) {
            if (err) *err << " is not object";
            return false;
        }
        for (auto & field : _fields) {
            auto jsub = jval.FindMember(field.name);
            if (jsub == jval.MemberEnd()) {
                if (!option->ignore_missing_field()) {
                    if (err) *err << " field not found" << field.name << ".";
                    return false;
                } else {
                    continue;
                }
            }
            char * ploc = field.offset + ((char *) cval);
            if (!field.field->load(jsub->value, ploc, allocer, option, err)) {
                if (err) *err << field.name << ".";
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

} // ns jstruct
