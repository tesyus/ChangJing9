#pragma once

namespace WarGrey::GYDM {
    private enum class ASNPrimitive {
        Boolean, Integer, /* BitString, OctetString, */ Null,
        /* ObjectIdentifier, ObjectDescriptor, External, */
        Real, Enumerated,
        UTF8_String, IA5_String
    };

    private enum class ASNConstructed {
        Sequence
    };

    private enum class ASN1TagClass { Universal, Application, ContextSpecific, Private };

    uint8 asn_identifier_octet(uint8 tag, bool constructed = false, WarGrey::GYDM::ASN1TagClass type = ASN1TagClass::Universal);

    uint8 asn_identifier_tag(uint8 octet);
    WarGrey::GYDM::ASN1TagClass asn_identifier_class(uint8 octet);
    bool asn_identifier_constructed(uint8 octet);

    uint8 asn_primitive_identifier_octet(WarGrey::GYDM::ASNPrimitive type);
    uint8 asn_constructed_identifier_octet(WarGrey::GYDM::ASNConstructed type);
}
