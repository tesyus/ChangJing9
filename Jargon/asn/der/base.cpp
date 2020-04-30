#include <Windows.h>

#include "asn/der/base.hpp"

#include "datum/box.hpp"
#include "datum/flonum.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

/*************************************************************************************************/
static const uint8 default_real_base = 2; // TODO: implememnt base 10 representation

static inline size_t fill_ufixnum_octets(uint8* pool, unsigned long long n, size_t capacity, size_t payload) {
    do {
        payload++;
        pool[capacity - payload] = (uint8)(n & 0xFF);
        n >>= 8U;
    } while (n > 0U);

    return payload;
}

static inline size_t fill_fixnum_octets(uint8* pool, long long n, size_t capacity, size_t payload) {
    if (n >= 0) {
        payload = fill_ufixnum_octets(pool, n, capacity, payload);

        if (pool[capacity - payload] >= 0b10000000) {
            pool[capacity - (++payload)] = 0;
        }
    } else {
        do {
            payload++;
            pool[capacity - payload] = (uint8)(n & 0xFF);
            n >>= 8U;
        } while (n < -1);
    }

    return payload;
}

static inline size_t fill_length_octets(uint8* pool, size_t length, size_t capacity) {
    size_t payload = 1U;
    
    if (length <= 127) {
        pool[capacity - payload] = (uint8)(length);
    } else {
        payload = fill_ufixnum_octets(pool, length, capacity, payload - 1);
        pool[capacity - (++payload)] = (uint8)(0b10000000 | payload);
    }

    return payload;
}

static inline void substitude_trailing_zero(long long* E, long long* N, uint8 delta, uint64 mask, uint8 rshift) {
    while (((*N) & mask) == 0) {
        (*E) += delta;
        (*N) >>= rshift;
    }
}

static void fill_real_binary(double real, double base, long long* E, long long* N) {
    double r = real;

    (*E) = 0;

    // TODO: implement Racket `integer?`
    while (flfloor(r) != r) {
        r *= base;
        (*E) -= 1U;
    }

    (*N) = (long long)flfloor(r);

    if (base == 16.0) {
        substitude_trailing_zero(E, N, 2, 0xFF, -8);
    } else if (base == 2.0) {
        substitude_trailing_zero(E, N, 8, 0xFF, -8);
    } else {
        substitude_trailing_zero(E, N, 1, 0b111, -3);
    }
}

template<typename N>
static inline void fill_integer_from_bytes(N* n, const uint8* pool, size_t start, size_t end, bool check_sign = false) {
    if (check_sign) {
        (*n) = ((pool[start] >= 0b10000000) ? -1 : 0);
    } else {
        (*n) = 0;
    }

    for (size_t idx = start; idx < end; idx++) {
        (*n) = ((*n) << 8U) | pool[idx];
    }
}

static inline size_t utf8_string_span(const wchar_t* src, size_t length) {
    return WideCharToMultiByte(CP_UTF8, 0, src, int(length), nullptr, 0, NULL, NULL);
}

static size_t utf8_string_into_octets(const wchar_t* src, size_t size, uint8* octets, size_t offset, size_t end) {
    int payload = WideCharToMultiByte(CP_UTF8, 0, src, int(size), (char*)(octets + offset), int(end - offset), NULL, NULL);

    if (payload > 0) {
        offset += payload;
    }

    return offset;
}

static std::wstring make_wide_string(const uint8* src, size_t size) {
    wchar_t wpool[1024];
    wchar_t* pool = wpool;
    size_t wsize = sizeof(wpool) / sizeof(wchar_t);
    std::wstring wide;

    if (size > wsize) {
        pool = new wchar_t[size];
        wsize = size;
    }

    wsize = (size_t)(MultiByteToWideChar(CP_UTF8, 0, (char*)src, int(size), pool, int(wsize)));

    if (wsize > 0) {
        wide = std::wstring(pool, wsize);
    }

    if (pool != wpool) {
        delete[] pool;
    }

    return wide;
}

static uint8 make_real_infoctet(double real, uint8 base, size_t E_size, size_t binary_scaling_factor) {
    uint8 infoctet = ((real > 0.0) ? 0b10000000 : 0b11000000);
    
    switch (base) {
    case 8:  infoctet ^= 0b00010000U; break;
    case 16: infoctet ^= 0b00100000U; break;
    }

    switch (binary_scaling_factor) {
    case 0:  infoctet ^= 0b00000000; break;
    case 1:  infoctet ^= 0b00000100; break;
    case 2:  infoctet ^= 0b00001000; break;
    default: infoctet ^= 0b00001100;
    }

    switch (E_size) {
    case 2:  infoctet ^= 0b00000001U; break;
    case 3:  infoctet ^= 0b00000010U; break;
    }

    if (E_size >= 4) {
        infoctet ^= 0b00000011U;
    }

    return infoctet;
}

/*************************************************************************************************/
size_t WarGrey::GYDM::asn_length_span(size_t length) {
    size_t span = 1;

    if (length > 127) {
        do {
            span++;
            length >>= 8U;
        } while (length > 0U);
    }

    return span;
}

octets WarGrey::GYDM::asn_length_to_octets(size_t length) {
    uint8 pool[10];
    size_t capacity = sizeof(pool) / sizeof(uint8);
    size_t payload = fill_length_octets(pool, length, capacity);

    return octets(pool + (capacity - payload), payload);
}

size_t WarGrey::GYDM::asn_length_into_octets(size_t length, uint8* octets, size_t offset) {
    size_t span = asn_length_span(length);

    fill_length_octets(octets + offset, length, span);

    return offset + span;
}

size_t WarGrey::GYDM::asn_octets_to_length(const uint8* blength, size_t* offset) {
    size_t idx = ((offset == nullptr) ? 0 : (*offset));
    size_t length = blength[idx];

    if (length > 0b10000000) {
        size_t size = length & 0b01111111;

        fill_integer_from_bytes(&length, blength, idx + 1, idx + size + 1, false);

        if (offset != nullptr) {
            (*offset) += (size + 1);
        }
    } else if (offset != nullptr) {
        (*offset) += 1;
    }

    return length;
}

/*************************************************************************************************/
bool WarGrey::GYDM::asn_primitive_predicate(ASNPrimitive type, const uint8* content, size_t offset) {
    return (asn_primitive_identifier_octet(type) == content[offset]);
}

bool WarGrey::GYDM::asn_primitive_predicate(ASNPrimitive type, octets& content, size_t offset) {
    return (asn_primitive_identifier_octet(type) == content[offset]);
}

bool WarGrey::GYDM::asn_constructed_predicate(ASNConstructed type, const uint8* content, size_t offset) {
    return (asn_constructed_identifier_octet(type) == content[offset]);
}

bool WarGrey::GYDM::asn_constructed_predicate(ASNConstructed type, octets& content, size_t offset) {
    return (asn_constructed_identifier_octet(type) == content[offset]);
}

octets WarGrey::GYDM::asn_octets_box(uint8 tag, octets& content, size_t size) {
    uint8 pool[10];
    size_t capacity = sizeof(pool) / sizeof(uint8);
    size_t payload = fill_length_octets(pool, size, capacity);

    pool[capacity - (++payload)] = tag;

    return octets(content, 0, size).insert(0, pool + (capacity - payload), payload);
}

size_t WarGrey::GYDM::asn_octets_unbox(const uint8* basn, size_t* offset) {
    size_t content_idx = ((offset == nullptr) ? 0 : (*offset)) + 1U;
    size_t size = asn_octets_to_length(basn, &content_idx);

    SET_BOX(offset, (content_idx + size));

    return size;
}

/*************************************************************************************************/
size_t WarGrey::GYDM::asn_boolean_span(bool b) {
    return 1;
}

octets WarGrey::GYDM::asn_boolean_to_octets(bool b) {
    octets bbool(3, '\0');

    asn_boolean_into_octets(b, (uint8*)bbool.c_str(), 0);

    return bbool;
}

size_t WarGrey::GYDM::asn_boolean_into_octets(bool b, uint8* octets, size_t offset) {
    size_t span = asn_boolean_span(b);

    octets[offset++] = asn_primitive_identifier_octet(ASNPrimitive::Boolean);
    octets[offset++] = (uint8)span;
    octets[offset++] = (b ? 0xFF : 0x00);

    return offset;
}

bool WarGrey::GYDM::asn_octets_to_boolean(const uint8* bnat, size_t* offset0) {
    size_t offset = ((offset0 == nullptr) ? 0 : (*offset0));
    size_t size = asn_octets_unbox(bnat, &offset);

    SET_BOX(offset0, offset);

    return (bnat[offset - size] > 0x00);
}

size_t WarGrey::GYDM::asn_fixnum_span(long long fixnum) {
    size_t span = 0;

    if (fixnum >= 0) {
        while (fixnum > 0xFF) {
            span++;
            fixnum >>= 8U;
        }

        span += ((fixnum >= 0b10000000) ? 2 : 1);
    } else {
        do {
            span++;
            fixnum >>= 8U;
        } while (fixnum < -1);
    }

    return span;
}

octets WarGrey::GYDM::asn_int64_to_octets(long long fixnum, ASNPrimitive id) {
    uint8 pool[12];
    size_t capacity = sizeof(pool) / sizeof(uint8);
    size_t payload = fill_fixnum_octets(pool, fixnum, capacity, 0U);

    pool[capacity - (++payload)] = (uint8)payload;
    pool[capacity - (++payload)] = asn_primitive_identifier_octet(id);

    return octets(pool + (capacity - payload), payload);
}

size_t WarGrey::GYDM::asn_int64_into_octets(long long fixnum, uint8* octets, size_t offset, ASNPrimitive id) {
    size_t span = asn_fixnum_span(fixnum);
    
    octets[offset++] = asn_primitive_identifier_octet(id);
    octets[offset++] = (uint8)span;

    fill_fixnum_octets(octets + offset, fixnum, span, 0U);

    return offset + span;
}

long long WarGrey::GYDM::asn_octets_to_fixnum(const uint8* bfixnum, size_t* offset0) {
    size_t offset = ((offset0 == nullptr) ? 0 : (*offset0));
    size_t size = asn_octets_unbox(bfixnum, &offset);
    long long integer = 0;

    fill_integer_from_bytes(&integer, bfixnum, offset - size, offset, true);

    SET_BOX(offset0, offset);

    return integer;
}

size_t WarGrey::GYDM::asn_natural_span(Natural& nat) {
    size_t span = nat.length();

    if (nat[0] >= 0b10000000) {
        span += 1;
    }

    return span;
}

octets WarGrey::GYDM::asn_natural_to_octets(Natural& nat) {
    octets payload = nat.to_bytes();
    size_t size = nat.length();
    
    if (payload[0] >= 0b10000000) {
        payload.insert(0, 1, '\x00');
        size += 1;
    }

    return asn_octets_box(asn_primitive_identifier_octet(ASNPrimitive::Integer), payload, size);
}

size_t WarGrey::GYDM::asn_natural_into_octets(Natural& nat, uint8* octets, size_t offset) {
    size_t size = nat.length();
    size_t span = asn_natural_span(nat);

    octets[offset + 0] = asn_primitive_identifier_octet(ASNPrimitive::Integer);
    offset = asn_length_into_octets(span, octets, offset + 1);

    if (span > size) {
        octets[offset++] = '\x00';
    }

    memcpy(octets + offset, &nat[0], size);

    return offset + size;
}

Natural WarGrey::GYDM::asn_octets_to_natural(const uint8* bnat, size_t* offset0) {
    size_t offset = ((offset0 == nullptr) ? 0 : (*offset0));
    size_t size = asn_octets_unbox(bnat, &offset);
    Natural nat(bnat, offset - size, offset);

    SET_BOX(offset0, offset);

    return nat;
}

size_t WarGrey::GYDM::asn_null_span(std::nullptr_t placeholder) {
    return 0;
}

octets WarGrey::GYDM::asn_null_to_octets(std::nullptr_t placeholder) {
    uint8 pool[2];

    asn_null_into_octets(placeholder, pool, 0);

    return octets(pool, sizeof(pool) / sizeof(uint8));
}

size_t WarGrey::GYDM::asn_null_into_octets(std::nullptr_t placeholder, uint8* octets, size_t offset) {
    octets[offset++] = asn_primitive_identifier_octet(ASNPrimitive::Null);
    octets[offset++] = 0x00;

    return offset;
}

std::nullptr_t WarGrey::GYDM::asn_octets_to_null(const uint8* bnull, size_t* offset0) {
    size_t offset = ((offset0 == nullptr) ? 0 : (*offset0));
    size_t size = asn_octets_unbox(bnull, &offset);

    SET_BOX(offset0, offset);

    return nullptr;
}

size_t WarGrey::GYDM::asn_real_span(double real) {
    size_t span = 1;
    uint8 base = 2;

    if (flisfinite(real)) {
        // WARNING: 0.0 is +0.0, hence 1.0
        if (real == 0.0) {
            if (flsign(real) == 1.0) {
                span = 0;
            }
        } else {
            long long E, N;
            size_t E_size;

            fill_real_binary(flabs(real), double(base), &E, &N);
            E_size = asn_fixnum_span(E);
            
            span = 1 + E_size + asn_fixnum_span(N);
            
            if (E_size >= 4) {
                span += 1;
            }
        }
    }

    return span;
}

octets WarGrey::GYDM::asn_real_to_octets(double real) {
    uint8 pool[64];
    size_t capacity = sizeof(pool) / sizeof(uint8);
    size_t payload = 0U;
    
    if (!flisfinite(real)) {
        payload = 1U;
        
        if (real > 0.0) {
            pool[capacity - payload] = 0x40;
        } else if (real < 0.0) {
            pool[capacity - payload] = 0x41;
        } else {
            pool[capacity - payload] = 0x42;
        }
    } else if (real == 0.0) {
        // WARNING: 0.0 is +0.0, hence 1.0
        if (flsign(real) == -1.0) {
            payload = 1U;
            pool[capacity - payload] = 0x43;
        }
    } else {
        size_t E_size;
        long long E, N;
        
        fill_real_binary(flabs(real), double(default_real_base), &E, &N);
        E_size = fill_fixnum_octets(pool, N, capacity, payload);
        payload = fill_fixnum_octets(pool, E, capacity, E_size);
        E_size = payload - E_size;

        if (E_size >= 4) {
            pool[capacity - (++payload)] = (uint8)E_size;
        }

        pool[capacity - (++payload)] = make_real_infoctet(real, default_real_base, E_size, 0);
    }

    pool[capacity - (++payload)] = (uint8)payload;
    pool[capacity - (++payload)] = asn_primitive_identifier_octet(ASNPrimitive::Real);

    return octets(pool + (capacity - payload), payload);
}

size_t WarGrey::GYDM::asn_real_into_octets(double real, uint8* octets, size_t offset) {
    octets[offset++] = asn_primitive_identifier_octet(ASNPrimitive::Real);

    if (!flisfinite(real)) {
        octets[offset++] = 1;

        if (real > 0.0) {
            octets[offset++] = 0x40;
        } else if (real < 0.0) {
            octets[offset++] = 0x41;
        } else {
            octets[offset++] = 0x42;
        }
    } else if (real == 0.0) {
        // WARNING: 0.0 is +0.0, hence 1.0
        if (flsign(real) == -1.0) {
            octets[offset++] = 1U;
            octets[offset++] = 0x43;
        } else {
            octets[offset++] = 0U;
        }
    } else {
        size_t E_size, N_size;
        long long E, N;

        fill_real_binary(flabs(real), double(default_real_base), &E, &N);
        E_size = asn_fixnum_span(E);
        N_size = asn_fixnum_span(N);

        offset = asn_length_into_octets(1 + ((E_size >= 4) ? 1 : 0) + E_size + N_size, octets, offset);
        octets[offset++] = make_real_infoctet(real, default_real_base, E_size, 0);

        if (E_size >= 4) {
            octets[offset++] = (uint8)E_size;
        }

        offset += E_size;
        fill_fixnum_octets(octets, E, offset, 0);
        offset += N_size;
        fill_fixnum_octets(octets, N, offset, 0);
    }

    return offset;
}

double WarGrey::GYDM::asn_octets_to_real(const uint8* breal, size_t* offset0) {
    size_t offset = ((offset0 == nullptr) ? 0 : (*offset0));
    size_t size = asn_octets_unbox(breal, &offset);
    double real = flnan;

    if (size > 0) {
        uint8 infoctet = breal[offset - size];

        if ((infoctet & (0b1 << 7)) > 0) {
            double sign = ((infoctet & (0b1 << 6)) > 0) ? -1.0 : 1.0;
            double base = flnan;
            uint8 N_lshift = ((infoctet & 0b1100) >> 2);
            size_t E_start = offset - size + 1;
            size_t E_end = offset;

            switch ((infoctet & 0b110000) >> 4) {
            case 0b00: base = 2.0; break;
            case 0b01: base = 8.0; break;
            case 0b10: base = 16.0; break;
            }

            switch (infoctet & 0b11) {
            case 0b00: E_end = E_start + 1; break;
            case 0b01: E_end = E_start + 2; break;
            case 0b11: E_end = E_start + 3; break;
            default: E_start ++; E_end = E_start + breal[E_start - 1];
            }

            if (E_end < offset) {
                long long E, N;

                fill_integer_from_bytes(&E, breal, E_start, E_end, true);
                fill_integer_from_bytes(&N, breal, E_end, offset, true);
                real = sign * double(N << N_lshift) * flexpt(base, double(E));
            } else if (E_end == offset) {
                real = 0.0;
            }
        } else {
            switch (infoctet) {
            case 0b01000000: real = +infinity; break;
            case 0b01000001: real = -infinity; break;
            case 0b01000010: real = flnan; break;
            case 0b01000011: real = -0.0; break;
            }
        }
    } else if (size == 0) {
        real = 0.0;
    }

    SET_BOX(offset0, offset);

    return real;
}

size_t WarGrey::GYDM::asn_ia5_span(std::string& str) {
    return str.length();
}

octets WarGrey::GYDM::asn_ia5_to_octets(std::string& str) {
    size_t size = str.length();
    octets payload((uint8*)str.c_str(), 0, size);
   
    return asn_octets_box(asn_primitive_identifier_octet(ASNPrimitive::IA5_String), payload, size);
}

size_t WarGrey::GYDM::asn_ia5_into_octets(std::string& str, uint8* octets, size_t offset) {
    size_t size = str.length();

    octets[offset++] = asn_primitive_identifier_octet(ASNPrimitive::IA5_String);
    offset = asn_length_into_octets(size, octets, offset);
    memcpy(octets + offset, (uint8*)str.c_str(), size);

    return offset + size;
}

std::string WarGrey::GYDM::asn_octets_to_ia5(const uint8* bia5, size_t* offset0) {
    size_t offset = ((offset0 == nullptr) ? 0 : (*offset0));
    size_t size = asn_octets_unbox(bia5, &offset);

    SET_BOX(offset0, offset);

    return std::string((char*)(bia5 + (offset - size)), size);
}

size_t WarGrey::GYDM::asn_utf8_span(Platform::String^ str) {
    return utf8_string_span(str->Data(), str->Length());
}

size_t WarGrey::GYDM::asn_utf8_span(std::wstring& str) {
    return utf8_string_span(str.c_str(), str.size());
}

octets WarGrey::GYDM::asn_utf8_to_octets(Platform::String^ str) {
    size_t payload = asn_utf8_span(str);
    octets pool(asn_span(payload), '\0');

    asn_utf8_into_octets(str, (uint8*)pool.c_str(), 0U);

    return pool;
}

octets WarGrey::GYDM::asn_utf8_to_octets(std::wstring& str) {
    size_t payload = asn_utf8_span(str);
    octets pool(asn_span(payload), '\0');

    asn_utf8_into_octets(str, (uint8*)pool.c_str(), 0U);

    return pool;
}

size_t WarGrey::GYDM::asn_utf8_into_octets(Platform::String^ str, uint8* octets, size_t offset) {
    size_t size = asn_utf8_span(str);
    
    octets[offset++] = asn_primitive_identifier_octet(ASNPrimitive::UTF8_String);
    offset = asn_length_into_octets(size, octets, offset);

    return utf8_string_into_octets(str->Data(), str->Length(), octets, offset, offset + size);
}

size_t WarGrey::GYDM::asn_utf8_into_octets(std::wstring& str, uint8* octets, size_t offset) {
    size_t size = asn_utf8_span(str);
    
    octets[offset++] = asn_primitive_identifier_octet(ASNPrimitive::UTF8_String);
    offset = asn_length_into_octets(size, octets, offset);

    return utf8_string_into_octets(str.c_str(), str.size(), octets, offset, offset + size);
}

std::wstring WarGrey::GYDM::asn_octets_to_utf8(const uint8* butf8, size_t* offset0) {
    size_t offset = ((offset0 == nullptr) ? 0 : (*offset0));
    size_t size = asn_octets_unbox(butf8, &offset);
    
    SET_BOX(offset0, offset);

    return make_wide_string(butf8 + (offset - size), size);
}
