#include "data_def.h"

#include <iostream>
#include <jstruct/jloader.h>
#include <jstruct/allocer.h>
#include <cassert>

static std::string JSON = "{ \
    \"unit_id\": 100, \
    \"creative_list\":[{ \
        \"creative_id\": 200, \
	    \"material_tags\": [255,256,257]\
    },{ \
	    \"image_url\": \"http://www.giftialab.org/image/aira.png\" \
    }], \
    \"freq_ctls\": { \
        \"freq_info_1\": \"freq_key_1\", \
        \"freq_info_3\": 3.14, \
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
    ::jstruct::A8Allocer<> allocer;
    ::jstruct::Error err;
    auto * p_unit = ::jstruct::load<Unit>(value, &allocer, nullptr, &err);
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
    TEST(unit, is_ad, false);
    TEST(unit, bidprice, 0);
    TEST(unit, vendor, 0);

    std::cout << "freq_ctls:" << std::endl;
    TEST(freq_ctls, freq_info_1, "freq_key_1");
    TEST(freq_ctls, freq_info_2, 0);
    std::cout << "  " << "freq_info_3" << " = " << freq_ctls.freq_info_3() << std::endl;
    assert(((int) (freq_ctls.freq_info_3() * 100)) == (3.14 * 100));
    TEST(freq_ctls, freq_info_4, false);
    auto & sub_freq = freq_ctls.freq_info_5();
    std::cout << "sub_freq:" << std::endl;
    TEST(sub_freq, sub_info_1, 1);
    TEST(sub_freq, sub_info_2, 2);

    assert(unit.creative_list().size() == 2);

    auto & crea1 = unit.creative_list(0);
    std::cout << "creative_list[0]:" << std::endl;
    TEST(crea1, creative_id, 200);
    TEST(crea1, image_url, "");
    std::cout << "  material_tags" << " = " << crea1.material_tags() << std::endl;
    assert(crea1.material_tags().size() == 3);
    assert(crea1.material_tags()[0] == 255);
    assert(crea1.material_tags()[1] == 256);
    assert(crea1.material_tags()[2] == 257);

    auto & crea2 = unit.creative_list()[1];
    std::cout << "creative_list[1]:" << std::endl;
    TEST(crea2, creative_id, 0);
    TEST(crea2, image_url, "http://www.giftialab.org/image/aira.png");
    assert(crea2.material_tags().size() == 0);

#undef TEST

    return 0;
}

