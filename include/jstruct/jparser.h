#pragma once

#include <cstddef>
#include <string_view>
#include <json/json.h>
#include "jerror.h"
#include "joption.h"

namespace jstruct {

class Allocer;

template < typename T > struct JParser;
template <> struct JParser < bool > {
    static bool parse(const ::Json::Value & jval, bool * cval, Allocer *, Option * option, Error * err) {
        if (jval.isBool()) {
            *cval = jval.asBool();
        } else if (!option->ignore_error_field()) {
            if (err) *err << " is not bool" << jval << "=";
            return false;
        }
        return true;
    }
};
template <> struct JParser < int32_t > {
     static bool parse(const ::Json::Value & jval, int32_t * cval, Allocer *, Option * option, Error * err) {
        if (jval.isInt()) {
            *cval = jval.asInt();
        } else if (!option->ignore_error_field()) {
            if (err) *err << " is not int32" << jval << "=";
            return false;
        }
        return true;
    }
};
template <> struct JParser < int64_t > {
     static bool parse(const ::Json::Value & jval, int64_t * cval, Allocer *, Option * option, Error * err) {
        if (jval.isInt64()) {
            *cval = jval.asInt64();
        } else if (!option->ignore_error_field()) {
            if (err) *err << " is not int64" << jval << "=";
            return false;
        }
        return true;
    }
};
template <> struct JParser < float > {
     static bool parse(const ::Json::Value & jval, float * cval, Allocer *, Option * option, Error * err) {
        if (jval.isDouble()) {
            *cval = jval.asFloat();
        } else if (!option->ignore_error_field()) {
            if (err) *err << " is not float" << jval << "=";
            return false;
        }
        return true;
    }
};
template <> struct JParser < double > {
     static bool parse(const ::Json::Value & jval, double * cval, Allocer *, Option * option, Error * err) {
        if (jval.isDouble()) {
            *cval = jval.asDouble();
        } else if (!option->ignore_error_field()) {
            if (err) *err << " is not double" << jval << "=";
            return false;
        }
        return true;
    }
};
template <> struct JParser < std::string_view > {
     static bool parse(const ::Json::Value & jval, std::string_view * cval, Allocer * allocer, Option * option, Error * err) {
        if (jval.isString()) {
            const char * beg = nullptr, * end = nullptr;
            jval.getString(&beg, &end);
	    if (beg <= end) {
                *cval = allocer->get_string(beg, end - beg);
	    } else if (!option->ignore_error_field()) {
                if (err) *err << " wrongjson string" << jval << "=";
                return false;
            }
        } else if (!option->ignore_error_field()) {
            if (err) *err << " is not string" << jval << "=";
            return false;
        }
        return true;
    }
};

} // ns jstruct
