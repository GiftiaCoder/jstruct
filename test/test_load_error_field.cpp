#include "data_def.h"

#include <iostream>
#include <jstruct/jloader.h>
#include <jstruct/align_allocer.h>

#include <cstring>

static std::string JSON1 = "{ \
    \"unit_id\": 100, \
    \"is_ad\": \"true\", \
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
        \"creative_id\": 201, \
        \"image_url\": \"http://www.giftialab.org/image/aira.png\", \
        \"material_tags\": [355,356,3.57]\
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
            \"sub_info_1\": 1, \
            \"sub_info_2\": \"2\" \
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
    ::jstruct::Error err;
    auto * p_unit = ::jstruct::load<Unit>(value, &allocer, nullptr, &err);
    if (!p_unit) {
        std::cout << err.c_str() << std::endl;
        std::cout << "load failed, exit" << std::endl;

        assert(std::strcmp(err.c_str(), err_msg) == 0);
        return -1;
    }

    return 0;
}

int main() {
    test_load(JSON1, ".is_ad=\"true\" is not bool");
    test_load(JSON2, ".creative_list[1].material_tags[2]=3.57 is not int32");
    test_load(JSON3, ".freq_ctls.freq_info_5.sub_info_2=\"2\" is not int32");
    return 0;
}

