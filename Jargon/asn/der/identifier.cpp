#include "asn/der/identifier.hpp"

using namespace WarGrey::GYDM;

static uint8 primitive_identifiers[37];
static uint8 constructed_identifiers[37];
static bool initialized = false;

/*************************************************************************************************/
static void initialize_identifiers() {
    if (!initialized) {
        size_t ptotal = sizeof(primitive_identifiers) / sizeof(uint8);
        size_t ctotal = sizeof(constructed_identifiers) / sizeof(uint8);

        for (size_t idx = 0; idx < ptotal; idx++) {
            primitive_identifiers[idx] = asn_identifier_octet((uint8)idx, false);
        }

        for (size_t idx = 0; idx < ctotal; idx++) {
            constructed_identifiers[idx] = asn_identifier_octet((uint8)idx, true);
        }

        initialized = true;
    }
}

/*************************************************************************************************/
uint8 WarGrey::GYDM::asn_identifier_octet(uint8 tag, bool constructed, ASN1TagClass type) {
    uint8 type_octet = 0b00000000;

    switch (type) {
    case ASN1TagClass::Application:     type_octet = 0b01000000; break;
    case ASN1TagClass::ContextSpecific: type_octet = 0b10000000; break;
    case ASN1TagClass::Private:         type_octet = 0b11000000; break;
    }

    return type_octet | (constructed ? 0b00100000 : 0b00000000) | tag;
}

uint8 WarGrey::GYDM::asn_identifier_tag(uint8 octet) {
    return octet & 0b00011111;
}

ASN1TagClass WarGrey::GYDM::asn_identifier_class(uint8 octet) {
    ASN1TagClass tag = ASN1TagClass::Universal;

    switch (octet >> 6U) {
    case 0b01: tag = ASN1TagClass::Application; break;
    case 0b10: tag = ASN1TagClass::ContextSpecific; break;
    case 0b11: tag = ASN1TagClass::Private; break;
    }

    return tag;
}

bool WarGrey::GYDM::asn_identifier_constructed(uint8 octet) {
    return ((octet & 0b00100000) > 0U);
}

/*************************************************************************************************/
uint8 WarGrey::GYDM::asn_primitive_identifier_octet(ASNPrimitive type) {
    size_t idx = 0;

    initialize_identifiers();

    switch (type) {
    case ASNPrimitive::Boolean:     idx = 0x01; break;
    case ASNPrimitive::Integer:     idx = 0x02; break;
    case ASNPrimitive::Null:        idx = 0x05; break;
    case ASNPrimitive::Real:        idx = 0x09; break;
    case ASNPrimitive::Enumerated:  idx = 0x0A; break;
    case ASNPrimitive::UTF8_String: idx = 0x0C; break;
    case ASNPrimitive::IA5_String:  idx = 0x16; break;
    }

    return primitive_identifiers[idx];
}

uint8 WarGrey::GYDM::asn_constructed_identifier_octet(ASNConstructed type) {
    size_t idx = 0;

    initialize_identifiers();

    switch (type) {
    case ASNConstructed::Sequence:  idx = 0x10; break;
    }

    return constructed_identifiers[idx];
}
