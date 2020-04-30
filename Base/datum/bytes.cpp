#include "datum/bytes.hpp"

#include "datum/flonum.hpp"

using namespace WarGrey::SCADA;

/*************************************************************************************************/
int WarGrey::SCADA::read_bits(uint8 *src, uint16 address, uint16 quantity, uint8 *dest) {
    uint8 shift = 0;
    uint8 byte = 0;
	uint8 d_idx = 0;

    for (uint16 i = address; i < address + quantity; i++) {
        byte |= (src[i] << shift);
        if (shift < 7) {
            shift += 1;
        } else {
			dest[d_idx++] = byte;
            byte = shift = 0;
        }
    }

    if (shift != 0) {
		dest[d_idx++] = byte;
    }

    return d_idx;
}

int WarGrey::SCADA::read_words(uint16 *src, uint16 address, uint16 quantity, uint8 *dest) {
	uint8 d_idx = 0;

	for (uint16 i = address; i < address + quantity; i++) {
		dest[d_idx++] = src[i] >> 8;
		dest[d_idx++] = src[i] & 0xFF;
	}

	return d_idx;
}

void WarGrey::SCADA::write_words(uint16 *dest, uint16 address, uint16 quantity, uint8 *src) {
	uint8 d_idx = 0;

	for (size_t i = 0; i < quantity; i++) {
		dest[address + i] = GET_INT16_FROM_INT8(src, d_idx);
		d_idx += 2;
	}
}

void WarGrey::SCADA::set_bits_from_byte(uint8 *dest, uint16 idx, uint8 src) {
    for (unsigned int i = 0; i < 8; i++) {
        dest[idx + i] = (src & (1 << i)) ? 1 : 0;
    }
}


void WarGrey::SCADA::set_bits_from_bytes(uint8 *dest, uint16 idx, uint16 count, const uint8 *src) {
    uint8 shift = 0;

    for (unsigned short i = idx; i < idx + count; i++) {
        dest[i] = ((src[(i - idx) / 8] & (1 << shift)) ? 1 : 0);
        shift = (shift + 1) % 8;
    }
}

uint8 WarGrey::SCADA::get_byte_from_bits(const uint8 *src, uint16 idx, uint16 count) {
    uint8 value = 0;

	if (count > 8) {
		// In fact, the count as an input argument should not be greater than 8
		count = 8;
	}

    for (size_t i = 0; i < count; i++) {
        value |= (src[idx + i] << i);
    }

    return value;
}

/*************************************************************************************************/
bool WarGrey::SCADA::quantity_bit_ref(const uint8* src, size_t idx, uint8 bit_idx) {
	return ((src[idx] & (0b1U << bit_idx)) != 0);
}

void WarGrey::SCADA::quantity_bit_set(uint8* src, size_t idx, uint8 bit_idx) {
	src[idx] = src[idx] | (0b1U << bit_idx);
}

uint8 WarGrey::SCADA::bigendian_uint8_ref(const uint8* src, size_t idx) {
	return src[idx];
}

void WarGrey::SCADA::bigendian_uint8_set(uint8* dest, size_t idx, uint8 x) {
	dest[idx] = x;
}

uint16 WarGrey::SCADA::bigendian_uint16_ref(const uint8* src, size_t idx) {
	return GET_INT16_FROM_INT8(src, idx);
}

void WarGrey::SCADA::bigendian_uint16_set(uint8* dest, size_t idx, uint16 x) {
	SET_INT16_TO_INT8(dest, idx, x);
}

uint32 WarGrey::SCADA::bigendian_uint32_ref(const uint8* src, size_t idx) {
	uint32 msb16 = GET_INT16_FROM_INT8(src, idx);
	uint32 lsb16 = GET_INT16_FROM_INT8(src, idx + 2);

	return (msb16 << 16) + lsb16;
}

void WarGrey::SCADA::bigendian_uint32_set(uint8* dest, size_t idx, uint32 x) {
	uint16 msb16 = x >> 16;
	uint16 lsb16 = x & 0xFFFF;

	SET_INT16_TO_INT8(dest, idx + 0, msb16);
	SET_INT16_TO_INT8(dest, idx + 2, lsb16);
}

uint64 WarGrey::SCADA::bigendian_uint64_ref(const uint8* src, size_t idx) {
	uint64 msb32 = bigendian_uint32_ref(src, idx);
	uint64 lsb32 = bigendian_uint32_ref(src, idx + 4);

	return (msb32 << 32) + lsb32;
}

void WarGrey::SCADA::bigendian_uint64_set(uint8* dest, size_t idx, uint64 x) {
	uint32 msb32 = x >> 32;
	uint32 lsb32 = x & 0xFFFFFFFF;

	bigendian_uint32_set(dest, idx, msb32);
	bigendian_uint32_set(dest, idx + 4, lsb32);
}

float WarGrey::SCADA::bigendian_float_ref(const uint8* src, size_t idx) {
	uint8 flbytes[4];
	float dest = 0.0F;

	flbytes[0] = src[idx + 3];
	flbytes[1] = src[idx + 2];
	flbytes[2] = src[idx + 1];
	flbytes[3] = src[idx + 0];

	memcpy((void*)&dest, (void*)flbytes, 4);

	return dest;
}

void WarGrey::SCADA::bigendian_float_set(uint8* dest, size_t idx, float src) {
	uint8 flbytes[4];

	memcpy((void*)flbytes, (void*)&src, 4);

	dest[idx + 3] = flbytes[0];
	dest[idx + 2] = flbytes[1];
	dest[idx + 1] = flbytes[2];
	dest[idx + 0] = flbytes[3];
}

float WarGrey::SCADA::bigendian_flword_ref(const uint8* src, size_t idx, float scale) {
	uint16 u16 = bigendian_uint16_ref(src, idx);

	return float(u16) / scale;
}

void WarGrey::SCADA::bigendian_flword_set(uint8* dest, size_t idx, float x, float scale) {
	uint16 u16 = (uint16)(flround(x * scale));

	bigendian_uint16_set(dest, idx, u16);
}

void WarGrey::SCADA::read_bigendian_floats(uint8* src, size_t address, size_t quantity, float* dest) {
	for (size_t i = 0; i < quantity; i++) {
		dest[i] = bigendian_float_ref(src, address + i * sizeof(float));
	}
}

/*************************************************************************************************/
uint8 WarGrey::SCADA::byte_to_hexadecimal(uint8 ch, uint8 fallback_value) {
	if ((ch >= '0') && (ch <= '9')) {
		ch = ch - '0';
	} else if ((ch >= 'a') && (ch <= 'f')) {
		ch = ch - 'a' + 10;
	} else if ((ch >= 'A') && (ch <= 'F')) {
		ch = ch - 'A' + 10;
	} else {
		ch = fallback_value;
	}

	return ch;
}

uint8 WarGrey::SCADA::byte_to_decimal(uint8 ch, uint8 fallback_value) {
	if ((ch >= '0') && (ch <= '9')) {
		ch = ch - '0';
	} else {
		ch = fallback_value;
	}

	return ch;
}

uint8 WarGrey::SCADA::byte_to_octal(uint8 ch, uint8 fallback_value) {
	if ((ch >= '0') && (ch <= '7')) {
		ch = ch - '0';
	} else {
		ch = fallback_value;
	}

	return ch;
}

uint8 WarGrey::SCADA::hexadecimal_to_byte(uint8 ch) {
	return ((ch >= 10) ? (ch - 10 + 'A') : (ch + '0'));
}

uint8 WarGrey::SCADA::decimal_to_byte(uint8 ch) {
	return (ch + '0');
}

uint8 WarGrey::SCADA::octal_to_byte(uint8 ch) {
	return (ch + '0');
}

uint8 WarGrey::SCADA::hexadecimal_ref(const uint8* src, size_t idx, uint8 fallback_value) {
	return byte_to_hexadecimal(src[idx], fallback_value);
}

void WarGrey::SCADA::hexadecimal_set(uint8* dest, size_t idx, uint8 ch) {
	dest[idx] = hexadecimal_to_byte(ch);
}

uint8 WarGrey::SCADA::decimal_ref(const uint8* src, size_t idx, uint8 fallback_value) {
	return byte_to_decimal(src[idx], fallback_value);
}

void WarGrey::SCADA::decimal_set(uint8* dest, size_t idx, uint8 ch) {
	dest[idx] = decimal_to_byte(ch);
}
