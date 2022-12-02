#pragma once

#include <cstddef>
#include <string_view>
#include "jerror.h"

namespace jstruct {

class Allocer;

template < typename T > struct JParser;
template <> struct JParser < bool > {
    static bool parse(const JsonValue & jval, bool * cval, Allocer *, Error * err) {
        if (jval.IsBool()) {
            *cval = jval.GetBool();
            return true;
        } else {
            if (err) *err << " is not bool" << jval << "=";
            return false;
        }
    }
};
template <> struct JParser < int32_t > {
     static bool parse(const JsonValue & jval, int32_t * cval, Allocer *, Error * err) {
        if (jval.IsInt()) {
            *cval = jval.GetInt();
            return true;
        } else {
            if (err) *err << " is not int32" << jval << "=";
            return false;
        }
    }
};
template <> struct JParser < int64_t > {
     static bool parse(const JsonValue & jval, int64_t * cval, Allocer *, Error * err) {
        if (jval.IsInt64()) {
            *cval = jval.GetInt64();
            return true;
        } else {
            if (err) *err << " is not int64" << jval << "=";
            return false;
        }
    }
};
template <> struct JParser < float > {
     static bool parse(const JsonValue & jval, float * cval, Allocer *, Error * err) {
        if (jval.IsDouble()) {
            *cval = (float) jval.GetDouble();
            return true;
        } else {
            if (err) *err << " is not float" << jval << "=";
            return false;
        }
    }
};
template <> struct JParser < double > {
     static bool parse(const JsonValue & jval, double * cval, Allocer *, Error * err) {
        if (jval.IsDouble()) {
            *cval = jval.GetDouble();
            return true;
        } else {
            if (err) *err << " is not double" << jval << "=";
            return false;
        }
    }
};
template <> struct JParser < std::string_view > {
     static bool parse(const JsonValue & jval, std::string_view * cval, Allocer * allocer, Error * err) {
        if (jval.IsString()) {
            *cval = allocer->get_string(jval.GetString(), jval.GetStringLength());
            return true;
        } else {
            if (err) *err << " is not string" << jval << "=";
            return false;
        }
    }
};

} // ns jstruct
