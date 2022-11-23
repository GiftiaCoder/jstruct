#include <iostream>
#include <jstruct/builder.h>
#include <jstruct/align_allocer.h>

template < typename Impl >
class Creative : public Impl {
public:
    template < typename T > using Field = typename Impl::template Field<T>;
    template < typename T > using Array = typename Impl::template Array<T>;

    using Impl::Impl;

public:
    Field<int64_t> creative_id = { this, "creative_id" };
    Field<std::string_view> image_url = { this, "image_url" };
    Array<int32_t> cmatch = { this, "cmatch" };
};

std::ostream & operator<< (std::ostream & out, const Creative<::jstruct::JStruct> & creative) {
    return out << "{" << "\n"
           << "        creative_id    : \t" << creative.creative_id() << "\n"
           << "        image_url      : \t" << creative.image_url() << "\n"
           << "        cmatch         : \t" << creative.cmatch() << "\n"
           << "    }";
}

template < typename Impl >
class Unit : public Impl {
public:
    template < typename T > using Field = typename Impl::template Field<T>;
    template < typename T > using Array = typename Impl::template Array<T>;

    using Impl::Impl;

public:
    Field<int64_t> unit_id = { this, "unit_id" };
    Field<bool> is_ad = { this, "is_ad" };
    Field<int32_t> bidprice = { this, "bidprice" };
    Array<Creative<Impl>> creative_list = { this, "creative_list" };
};

std::ostream & operator<< (std::ostream & out, const Unit<::jstruct::JStruct> & unit) {
    return out << "{" << "\n"
           << "    unit_id        : \t" << unit.unit_id() << "\n"
           << "    is_ad          : \t" << unit.is_ad() << "\n"
           << "    bidprice       : \t" << unit.bidprice() << "\n"
           << "    creative_list  : \t" << unit.creative_list() << "\n"
           << "}";
}

int main() {
    // prepare json
    ::Json::Reader reader;
    ::Json::Value value;
    reader.parse("{\"unit_id\":1234,\"is_ad\":true,\"creative_list\":[{\"creative_id\":5678,\"image_url\":\"www.giftialab.com\",\"cmatch\":[424,425,426]},{\"creative_id\":5678,\"image_url\":\"www.giftialab.com\",\"cmatch\":[424,425,426]}],\"bidprice\":10}", value);
    std::cout << value << std::endl;

    // load
    std::cout << "\nload:\n" << std::boolalpha;

    ::jstruct::AlignAllocer allocer;
    auto * unit = ::jstruct::load<Unit>(value, &allocer);
    if (!unit) {
        std::cout << "load failed, exit" << std::endl;
	return -1;
    }

    // detail
    std::cout << *unit << "\n\n";
    std::cout << "memory_used: " << allocer.space_used() << std::endl;

    return 0;
}
