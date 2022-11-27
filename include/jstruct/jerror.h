#pragma once

#include <cstddef>
#include <cstring>
#include <string>
#include <string_view>

#include <json/value.h>

namespace jstruct {

class Error final {
public:
    inline Error() : _pos(_buf + sizeof(_buf) - 1) {
    	*_pos = '\0';
    }
    Error & append(const char * msg, size_t len) {
        std::memcpy(_pos -= len, msg, len);
        return *this;
    }
    inline const char * c_str() const { return _pos; }
private:
    char _buf[1024];
    char * _pos;
};

Error & operator<< (Error & error, const std::string & msg) {
    return error.append(msg.c_str(), msg.size());
}
Error & operator<< (Error & error, const std::string_view & msg) {
    return error.append(msg.data(), msg.size());
}
Error & operator<< (Error & error, const char * msg) {
    return error.append(msg, std::strlen(msg));
}
template < size_t N >
Error & operator<< (Error & error, const char (&msg)[N]) {
    return error.append(msg, N - 1);
}
Error & operator<< (Error & error, const ::Json::Value & msg) {
    std::string smsg = std::move(msg.asString());
    if (msg.isString()) {
        return error << "\"" << smsg << "\"";
    } else {
        return error << smsg;
    }
}
template < typename T >
Error & operator<< (Error & error, const T & msg) {
    return error << std::to_string(msg);
}

} // ns jstruct
