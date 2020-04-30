#pragma once

#include "asn/der/base.hpp"

namespace WarGrey::GYDM {
    private class IASNSequence {
    public:
        virtual ~IASNSequence() noexcept {}
        IASNSequence(size_t count);

    public:
        size_t span();
        WarGrey::GYDM::octets to_octets();
        size_t into_octets(uint8* octets, size_t offset = 0);
        void from_octets(const uint8* basn, size_t* offset = nullptr);
        inline void from_octets(WarGrey::GYDM::octets& basn, size_t* offset = nullptr) { this->from_octets(basn.c_str(), offset); }

    protected:
        virtual size_t field_payload_span(size_t idx) = 0;
        virtual size_t fill_field(size_t idx, uint8* octets, size_t offset) = 0;
        virtual void extract_field(size_t idx, const uint8* basn, size_t* offset) = 0;

    private:
        size_t count;
    };
}
