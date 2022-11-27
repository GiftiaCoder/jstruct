#include "data_def.h"

#include <iostream>
#include <jstruct/jloader.h>
#include <jstruct/align_allocer.h>

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
	\"image_url\": \"http://www.giftialab.org/image/aira.png\", \
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
    ::Json::Reader reader;
    ::Json::Value value;
    reader.parse(JSON, value);

    // load
    std::cout << std::boolalpha;
    ::jstruct::AlignAllocer allocer;
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

#define PRINT_FIELD(_obj_, _name_) \
    std::cout << "  " << #_name_ << " = " << _obj_._name_() << std::endl;

    auto & unit = *p_unit;
    auto & freq_ctls = unit.freq_ctls();

    std::cout << "unit:" << std::endl;
    PRINT_FIELD(unit, unit_id);
    PRINT_FIELD(unit, is_ad);
    PRINT_FIELD(unit, bidprice);
    PRINT_FIELD(unit, vendor);

    std::cout << "freq_ctls:" << std::endl;
    PRINT_FIELD(freq_ctls, freq_info_1);
    PRINT_FIELD(freq_ctls, freq_info_2);
    PRINT_FIELD(freq_ctls, freq_info_3);
    PRINT_FIELD(freq_ctls, freq_info_4);
    auto & sub_freq = freq_ctls.freq_info_5();
    PRINT_FIELD(sub_freq, sub_info_1);
    PRINT_FIELD(sub_freq, sub_info_2);

    for (auto & creative : unit.creative_list()) {
        std::cout << "creative[" << creative.creative_id() << "]:" << std::endl;
        PRINT_FIELD(creative, image_url);
        PRINT_FIELD(creative, material_tags);
    }

#undef PRINT_FIELD

    return 0;
}

