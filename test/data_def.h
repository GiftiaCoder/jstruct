#pragma once

#include <stdint.h>
#include <string_view>
#include <jstruct/jstruct.h>

#define JSTRUCT_COMMON_DEFS()                                                   \
        template < typename T > using Field = typename Impl::template Field<T>; \
        template < typename T > using Array = typename Impl::template Array<T>; \
        using Impl::Impl;                                                       \

// Creative
template < typename Impl >
class Creative : public Impl {
public:
    JSTRUCT_COMMON_DEFS();
public:
    Field<int64_t> creative_id = { this, "creative_id" };
    Field<std::string_view> image_url = { this, "image_url" };
    Array<int32_t> material_tags = { this, "material_tags" };
};

// FrequencyControls
template < typename Impl >
class SubFreqInfo : public Impl {
public:
    JSTRUCT_COMMON_DEFS();
public:
    Field<int32_t> sub_info_1 = { this, "sub_info_1" };
    Field<int32_t> sub_info_2 = { this, "sub_info_2" };
};
template < typename Impl >
class FrequencyControls : public Impl {
public:
    JSTRUCT_COMMON_DEFS();
public:
    Field<std::string_view> freq_info_1 = { this, "freq_info_1" };
    Field<int32_t> freq_info_2 = { this, "freq_info_2" };
    Field<float> freq_info_3 = { this, "freq_info_3" };
    Field<bool> freq_info_4 = { this, "freq_info_4" };
    Field<SubFreqInfo<Impl>> freq_info_5 = { this, "freq_info_5" };
};

// Unit
template < typename Impl >
class Unit : public Impl {
public:
    JSTRUCT_COMMON_DEFS();
public:
    Field<int64_t> unit_id = { this, "unit_id" };
    Field<bool> is_ad = { this, "is_ad" };
    Field<int32_t> bidprice = { this, "bidprice" };
    Array<Creative<Impl>> creative_list = { this, "creative_list" };
    Field<int32_t> vendor = { this, "vendor" };
    Field<FrequencyControls<Impl>> freq_ctls = { this, "freq_ctls" };
};

#undef JSTRUCT_COMMON_DEFS
