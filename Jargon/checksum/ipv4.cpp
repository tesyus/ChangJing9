#include "checksum/ipv4.hpp"

//// https://tools.ietf.org/html/rfc1071

using namespace WarGrey::GYDM;

/*************************************************************************************************/
static unsigned short update_sum(unsigned short sum, const uint8* message, size_t start, size_t end) {
    size_t count = end - start;
    unsigned short H = (sum >> 8U);
    unsigned short L = (sum & 0xFFU);

    if ((count & 0x01) == 1) {
        H += message[--end];
    }

    for (size_t idx = start; idx < end; idx += 2) {
        H += message[idx];
        L += message[idx + 1];
    }

    while ((H > 0xFF) || (L > 0xFF)) {
        unsigned short Hcarry = H >> 8U;
        unsigned short Lcarry = L >> 8U;

        H = (H & 0xFF) + Lcarry;
        L = (L & 0xFF) + Hcarry;
    }

    return ~((H << 8U) ^ L);
}

/*************************************************************************************************/
unsigned short WarGrey::GYDM::checksum_ipv4(const uint8* message, size_t start, size_t end) {
    return update_sum(0xFFFFL, message, start, end);
}

unsigned short WarGrey::GYDM::checksum_ipv4(unsigned short acc_crc, const uint8* message, size_t start, size_t end) {
    return update_sum(~acc_crc, message, start, end);
}

unsigned short  WarGrey::GYDM::checksum_ipv4(unsigned short* acc_crc, const uint8* message, size_t start, size_t end) {
    unsigned short sum = 0UL;

    if (acc_crc == nullptr) {
        sum = checksum_ipv4(message, start, end);
    } else {
        sum = update_sum(~(*acc_crc), message, start, end);
        (*acc_crc) = sum;
    }

    return sum;
}
