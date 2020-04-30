#pragma once

#include <string>

#include "datum/natural.hpp"

#include "asn/der/identifier.hpp"

namespace WarGrey::GYDM {
    typedef std::basic_string<unsigned char> octets;

    bool asn_primitive_predicate(WarGrey::GYDM::ASNPrimitive type, const uint8* content, size_t offset = 0);
    bool asn_primitive_predicate(WarGrey::GYDM::ASNPrimitive type, WarGrey::GYDM::octets& content, size_t offset = 0);
    bool asn_constructed_predicate(WarGrey::GYDM::ASNConstructed type, const uint8* content, size_t offset = 0);
    bool asn_constructed_predicate(WarGrey::GYDM::ASNConstructed type, WarGrey::GYDM::octets& content, size_t offset = 0);

    size_t asn_length_span(size_t length);
    WarGrey::GYDM::octets asn_length_to_octets(size_t length);
    size_t asn_length_into_octets(size_t length, uint8* octects, size_t offset = 0);
    size_t asn_octets_to_length(const uint8* blength, size_t* offset = nullptr);
    inline size_t asn_octets_to_length(WarGrey::GYDM::octets& blength, size_t* offset = nullptr) { return asn_octets_to_length(blength.c_str(), offset); }
    
    WarGrey::GYDM::octets asn_octets_box(uint8 tag, WarGrey::GYDM::octets& content, size_t size);
    size_t asn_octets_unbox(const uint8* basn, size_t* offset = nullptr);
    inline size_t asn_octets_unbox(WarGrey::GYDM::octets& basn, size_t* offset = nullptr) { return asn_octets_unbox(basn.c_str(), offset); }
    WarGrey::GYDM::octets asn_int64_to_octets(long long integer, WarGrey::GYDM::ASNPrimitive id = ASNPrimitive::Integer);
    size_t asn_int64_into_octets(long long integer, uint8* octets, size_t offset, WarGrey::GYDM::ASNPrimitive id = ASNPrimitive::Integer);

    // NOTE: `asn_xxx_into_octets` does not check the boundary, please ensure that the octets is sufficient. 
    // NOTE: `asn_octets_to_xxx` does not check the tag, please ensure that the octets is really what it should be.
    size_t asn_boolean_span(bool b);
    WarGrey::GYDM::octets asn_boolean_to_octets(bool b);
    size_t asn_boolean_into_octets(bool b, uint8* octets, size_t offset);
    bool asn_octets_to_boolean(const uint8* bbool, size_t* offset = nullptr);
    inline bool asn_octets_to_boolean(WarGrey::GYDM::octets& bbool, size_t* offset = nullptr) { return asn_octets_to_boolean(bbool.c_str(), offset); }

    size_t asn_fixnum_span(long long integer);
    inline WarGrey::GYDM::octets asn_fixnum_to_octets(long long integer) { return asn_int64_to_octets(integer, ASNPrimitive::Integer); }
    inline size_t asn_fixnum_into_octets(long long integer, uint8* octets, size_t offset = 0) { return asn_int64_into_octets(integer, octets, offset, ASNPrimitive::Integer); }
    long long asn_octets_to_fixnum(const uint8* bint, size_t* offset = nullptr);
    inline long long asn_octets_to_fixnum(WarGrey::GYDM::octets& bint, size_t* offset = nullptr) { return asn_octets_to_fixnum(bint.c_str(), offset); }

    size_t asn_natural_span(WarGrey::GYDM::Natural& nat);
    WarGrey::GYDM::octets asn_natural_to_octets(WarGrey::GYDM::Natural& nat);
    size_t asn_natural_into_octets(WarGrey::GYDM::Natural& nat, uint8* octets, size_t offset = 0);
    WarGrey::GYDM::Natural asn_octets_to_natural(const uint8* bnat, size_t* offset = nullptr);
    inline WarGrey::GYDM::Natural asn_octets_to_natural(WarGrey::GYDM::octets& bnat, size_t* offset = nullptr) { return asn_octets_to_natural(bnat.c_str(), offset); }

    size_t asn_null_span(std::nullptr_t placeholder);
    WarGrey::GYDM::octets asn_null_to_octets(std::nullptr_t placeholder);
    size_t asn_null_into_octets(std::nullptr_t placeholder, uint8* octets, size_t offset = 0);
    std::nullptr_t asn_octets_to_null(const uint8* bnull, size_t* offset = nullptr);
    inline std::nullptr_t asn_octets_to_null(WarGrey::GYDM::octets& bnull, size_t* offset = nullptr) { return asn_octets_to_null(bnull.c_str(), offset); }

    size_t asn_real_span(double real);
    WarGrey::GYDM::octets asn_real_to_octets(double real);
    size_t asn_real_into_octets(double real, uint8* octets, size_t offset = 0);
    double asn_octets_to_real(const uint8* breal, size_t* offset = nullptr);
    inline double asn_octets_to_real(WarGrey::GYDM::octets& breal, size_t* offset = nullptr) { return asn_octets_to_real(breal.c_str(), offset); }

    size_t asn_ia5_span(std::string& ia5_str);
    WarGrey::GYDM::octets asn_ia5_to_octets(std::string& ia5_str);
    size_t asn_ia5_into_octets(std::string& ia5_str, uint8* octets, size_t offset = 0);
    std::string asn_octets_to_ia5(const uint8* bia5, size_t* offset = nullptr);
    inline std::string asn_octets_to_ia5(WarGrey::GYDM::octets& bia5, size_t* offset = nullptr) { return asn_octets_to_ia5(bia5.c_str(), offset); }

    size_t asn_utf8_span(Platform::String^ wide_str);
    size_t asn_utf8_span(std::wstring& wide_str);
    WarGrey::GYDM::octets asn_utf8_to_octets(Platform::String^ wide_str);
    WarGrey::GYDM::octets asn_utf8_to_octets(std::wstring& wide_str);
    size_t asn_utf8_into_octets(Platform::String^ wide_str, uint8* octets, size_t offset = 0);
    size_t asn_utf8_into_octets(std::wstring& wide_str, uint8* octets, size_t offset = 0);
    std::wstring asn_octets_to_utf8(const uint8* butf8, size_t* offset = nullptr);
    inline std::wstring asn_octets_to_utf8(WarGrey::GYDM::octets& butf8, size_t* offset = nullptr) { return asn_octets_to_utf8(butf8.c_str(), offset); }

    inline size_t asn_span(size_t payload_span) { return 1 + asn_length_span(payload_span) + payload_span; }

    template<typename Span, typename T>
    inline size_t asn_span(Span span, T v) { return asn_span(span(v)); }

    template<class C>
    inline size_t asn_span(C constructed) { return asn_span(constructed->span()); }
}
