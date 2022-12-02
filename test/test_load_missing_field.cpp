#include "data_def.h"

#include <iostream>
#include <jstruct/jloader.h>
#include <jstruct/align_allocer.h>

static std::string JSON1 = "{ \
    \"unit_id\": 100, \
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
static std::string JSON2 = "{ \
    \"unit_id\": 100, \
    \"is_ad\": true, \
    \"bidprice\": 999, \
    \"vendor\": 6000, \
    \"creative_list\":[{ \
        \"creative_id\": 200, \
        \"image_url\": \"http://www.giftialab.org/image/anya.png\", \
        \"material_tags\": [255,256,257]\
    },{ \
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
static std::string JSON3 = "{ \
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
            \"sub_info_1\": 1 \
        }\
    } \
}";

int test_load(const std::string & js, const char * err_msg) {
    // prepare json
    ::rapidjson::Document value;
    std::cout << value.Parse(js).HasParseError() << std::endl;

    // load
    std::cout << std::boolalpha;
    ::jstruct::AlignAllocer allocer;
    ::jstruct::Option opt;
    ::jstruct::Error err;
    auto * p_unit = ::jstruct::load<Unit>(value, &allocer, &opt, &err);
    if (!p_unit) {
        std::cout << err.c_str() << std::endl;
        std::cout << "load failed, exit" << std::endl;
        assert(std::strcmp(err.c_str(), err_msg) == 0);
        return -1;
    }
    return 0;
}

int main() {
    test_load(JSON1, ".is_ad field not found");
    test_load(JSON2, ".creative_list[1].creative_id field not found");
    test_load(JSON3, ".freq_ctls.freq_info_5.sub_info_2 field not found");
    return 0;
}

