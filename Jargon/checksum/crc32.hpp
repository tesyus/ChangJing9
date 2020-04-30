#pragma once

namespace WarGrey::GYDM {
	unsigned long checksum_crc32(const uint8* message, size_t start, size_t end);
	unsigned long checksum_crc32(unsigned long accumulated_crc, const uint8* message, size_t start, size_t end);
	unsigned long checksum_crc32(unsigned long* accumulated_crc, const uint8* message, size_t start, size_t end);

	template<typename B, size_t N>
	unsigned long checksum_crc32(const B(&message)[N], size_t start = 0) {
		return checksum_crc32((const uint8*)message, start, N - 1);
	}

	template<typename B, size_t N>
	unsigned long checksum_crc32(unsigned long accumulated_crc, const B(&message)[N], size_t start = 0) {
		return checksum_crc32(accumulated_crc, (const uint8*)message, start, N - 1);
	}

	template<typename B, size_t N>
	unsigned long checksum_crc32(unsigned long* accumulated_crc, const B(&message)[N], size_t start = 0) {
		return checksum_crc32(accumulated_crc, (const uint8*)message, start, N - 1);
	}
}
