#include "asn/der/sequence.hpp"

#include "datum/box.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

/*************************************************************************************************/
IASNSequence::IASNSequence(size_t count) : count(count) {}

size_t IASNSequence::span() {
    size_t payload = 0;

    for (size_t idx = 0; idx < this->count; idx++) {        
        payload += asn_span(this->field_payload_span(idx));
    }

    return payload;
}

octets IASNSequence::to_octets() {
    size_t payload_span = this->span();
    octets basn(asn_span(payload_span), '\0');

    this->into_octets((uint8*)basn.c_str(), 0);

    return basn;
}

size_t IASNSequence::into_octets(uint8* octets, size_t offset) {
    size_t payload_span = this->span();

    octets[offset++] = asn_constructed_identifier_octet(ASNConstructed::Sequence);
    offset = asn_length_into_octets(payload_span, octets, offset);

    for (size_t idx = 0; idx < this->count; idx++) {
        offset = this->fill_field(idx, octets, offset);
    }

    return offset;
}

void IASNSequence::from_octets(const uint8* basn, size_t* offset0) {
    size_t offset = ((offset0 == nullptr) ? 0 : (*offset0));
    size_t size = asn_octets_unbox(basn, &offset);
    size_t position = offset - size;

    SET_BOX(offset0, offset);

    for (size_t idx = 0; idx < this->count; idx++) {
        this->extract_field(idx, basn, &position);
    }
}
