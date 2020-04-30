#pragma once

namespace WarGrey::GYDM {
	unsigned short checksum_ipv4(const uint8* message, size_t start, size_t end);
	unsigned short checksum_ipv4(unsigned short accumulated_crc, const uint8* message, size_t start, size_t end);
	unsigned short checksum_ipv4(unsigned short* accumulated_crc, const uint8* message, size_t start, size_t end);

	template<typename B, size_t N>
	unsigned short checksum_ipv4(const B(&message)[N], size_t start = 0) {
		return checksum_ipv4((const uint8*)message, start, N - 1);
	}

	template<typename B, size_t N>
	unsigned short checksum_ipv4(unsigned short accumulated_crc, const B(&message)[N], size_t start = 0) {
		return checksum_ipv4(accumulated_crc, (const uint8*)message, start, N - 1);
	}

	template<typename B, size_t N>
	unsigned short checksum_ipv4(unsigned short* accumulated_crc, const B(&message)[N], size_t start = 0) {
		return checksum_ipv4(accumulated_crc, (const uint8*)message, start, N - 1);
	}
}
