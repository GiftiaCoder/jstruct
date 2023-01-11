#include "data_def.h"

#include <iostream>
#include <jstruct/jloader.h>
#include <jstruct/allocer.h>
#include <cassert>

static std::string JSON = "{ \
    \"unit_id\": 100, \
    \"is_ad\": true, \
    \"bidprice\": 999, \
    \"vendor\": 6000, \
    \"creative_list\":[{ \
        \"creative_id\": 200, \
        \"image_url\": \"http://www.giftialab.org/image/anya.png\", \
        \"material_tags\": [255,256,257]\
    },{ \
        \"creative_id\": 201, \
        \"image_url\": \"http://www.giftialab.org/image/anya.png\", \
        \"material_tags\": [355,356,357]\
    }], \
    \"freq_ctls\": { \
        \"freq_info_1\": \"freq_key_1\", \
        \"freq_info_2\": 22, \
        \"freq_info_3\": 3.14, \
        \"freq_info_4\": false, \
        \"freq_info_5\": { \
            \"sub_info_1\": 1, \
            \"sub_info_2\": 2 \
        }\
    } \
}";

int main() {
    // prepare json
    ::rapidjson::Document value;
    std::cout << value.Parse(JSON).HasParseError() << std::endl;

    // load
    std::cout << std::boolalpha;
    // ::jstruct::A8Allocer<::jstruct::StrDedupPlugin> allocer;
    ::jstruct::A8Allocer<> allocer;
    ::jstruct::Option opt;
    ::jstruct::Error err;
    auto * p_unit = ::jstruct::load<Unit>(value, &allocer, &opt, &err);
    if (!p_unit) {
        std::cout << err.c_str() << std::endl;
        std::cout << "load failed, exit" << std::endl;
        return -1;
    }

    // detail
    std::cout << "memory_used: " << allocer.space_used() << std::endl;
    std::cout << "class Unit size: " << sizeof(*p_unit) << " align: " << alignof(*p_unit) << std::endl;

#define TEST(_obj_, _name_, _val_) \
    std::cout << "  " << #_name_ << " = " << _obj_._name_() << std::endl; \
    assert(_obj_._name_() == _val_);

    auto & unit = *p_unit;
    auto & freq_ctls = unit.freq_ctls();

    std::cout << "unit:" << std::endl;
    TEST(unit, unit_id, 100);
    TEST(unit, is_ad, true);
    TEST(unit, bidprice, 999);
    TEST(unit, vendor, 6000);

    std::cout << "freq_ctls:" << std::endl;
    TEST(freq_ctls, freq_info_1, "freq_key_1");
    TEST(freq_ctls, freq_info_2, 22);
    TEST(freq_ctls, freq_info_3, (float) 3.14);
    TEST(freq_ctls, freq_info_4, false);
    auto & sub_freq = freq_ctls.freq_info_5();
    TEST(sub_freq, sub_info_1, 1);
    TEST(sub_freq, sub_info_2, 2);

    assert(unit.creative_list().size() == 2);

    auto & crea1 = unit.creative_list(0);
    std::cout << "creative[" << crea1.creative_id() << "]:" << std::endl;
    TEST(crea1, creative_id, 200);
    TEST(crea1, image_url, "http://www.giftialab.org/image/anya.png");
    
    auto & crea2 = unit.creative_list()[1];
    std::cout << "creative[" << crea2.creative_id() << "]:" << std::endl;
    TEST(crea2, creative_id, 201);
    TEST(crea2, image_url, "http://www.giftialab.org/image/anya.png");

#undef PRINT_FIELD

    return 0;
}

