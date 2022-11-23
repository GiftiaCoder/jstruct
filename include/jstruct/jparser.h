#pragma once

#include <cstddef>
#include <string_view>
#include <json/json.h>

namespace jstruct {

class Allocer;

template < typename T > struct JParser;
template <> struct JParser < bool > {
    static bool parse(const ::Json::Value & jval, bool * cval, Allocer *) {
        if (jval.isBool()) {
            *cval = jval.asBool();
            return true;
        } else {
            // TODO
            return false;
        }
    }
};
template <> struct JParser < int32_t > {
     static bool parse(const ::Json::Value & jval, int32_t * cval, Allocer *) {
        if (jval.isInt()) {
            *cval = jval.asInt();
            return true;
        } else {
            // TODO
            return false;
        }
    }
};
template <> struct JParser < int64_t > {
     static bool parse(const ::Json::Value & jval, int64_t * cval, Allocer *) {
        if (jval.isInt64()) {
            *cval = jval.asInt64();
            return true;
        } else {
            // TODO
            return false;
        }
    }
};
template <> struct JParser < float > {
     static bool parse(const ::Json::Value & jval, float * cval, Allocer *) {
        if (jval.isDouble()) {
            *cval = jval.asFloat();
            return true;
        } else {
            // TODO
            return false;
        }
    }
};
template <> struct JParser < double > {
     static bool parse(const ::Json::Value & jval, double * cval, Allocer *) {
        if (jval.isDouble()) {
            *cval = jval.asDouble();
            return true;
        } else {
            // TODO
            return false;
        }
    }
};
template <> struct JParser < std::string_view > {
     static bool parse(const ::Json::Value & jval, std::string_view * cval, Allocer * allocer) {
        if (jval.isString()) {
            const char * beg = nullptr, * end = nullptr;
            jval.getString(&beg, &end);
            if (beg > end) {
                // TODO
                return false;
            }
            *cval = allocer->get_string(beg, end - beg);
            return true;
        } else {
            // TODO
            *cval = { nullptr, 0 };
            return false;
        }
    }
};

} // ns jstruct
