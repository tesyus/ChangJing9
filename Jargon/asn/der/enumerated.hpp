#pragma once

#include "asn/der/base.hpp"
#include "asn/der/identifier.hpp"

namespace WarGrey::GYDM {
#define define_asn_enum(e, E, ...) \
private enum class E { __VA_ARGS__ }; \
inline size_t asn_##e##_span(E e) { return asn_enum_span(e); } \
inline WarGrey::GYDM::octets asn_##e##_to_octets(E e) { return asn_enum_to_octets(e); } \
inline size_t asn_##e##_into_octets(E e, uint8* octets, size_t offset = 0) { return asn_enum_into_octets(e, octets, offset); } \
inline E asn_octets_to_##e(const uint8* benum, size_t* offset = nullptr) { return asn_octets_to_enum<E>(benum, offset); } \
inline E asn_octets_to_##e(WarGrey::GYDM::octets& benum, size_t* offset = nullptr) { return asn_octets_to_enum<E>(benum.c_str(), offset); }

    template<typename E>
    inline size_t asn_enum_span(E enumerated) {
        return asn_fixnum_span(static_cast<long long>(enumerated));
    }

    template<typename E>
    inline WarGrey::GYDM::octets asn_enum_to_octets(E enumerated) {
        return asn_int64_to_octets(static_cast<long long>(enumerated), ASNPrimitive::Enumerated);
    }

    template<typename E>
    inline size_t asn_enum_into_octets(E enumerated, uint8* octets, size_t offset = 0) {
        return asn_int64_into_octets(static_cast<long long>(enumerated), octets, offset, ASNPrimitive::Enumerated);
    }

    template<typename E>
    inline E asn_octets_to_enum(const uint8* benum, size_t* offset = nullptr) {
        return static_cast<E>(asn_octets_to_fixnum(benum, offset));
    }
}
