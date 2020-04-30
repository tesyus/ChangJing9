#include "datum/natural.hpp"

#include "datum/bytes.hpp"
#include "datum/fixnum.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

/*************************************************************************************************/
template<typename BYTE>
static size_t natural_from_base16(uint8* natural, const BYTE n[], size_t nstart, size_t nend, size_t capacity) {
	size_t slot = capacity - 1;
	size_t payload = 0U;

	do {
		uint8 lsb = ((nend > nstart) ? byte_to_hexadecimal((uint8)n[--nend], 0U) : 0U);
		uint8 msb = ((nend > nstart) ? byte_to_hexadecimal((uint8)n[--nend], 0U) : 0U);

		natural[slot--] = (msb << 4U | lsb);

		if (natural[slot + 1] > 0) {
			payload = capacity - (slot + 1);
		}
	} while (nstart < nend);

	return payload;
}

template<typename BYTE>
static size_t natural_from_base(uint8 base, uint8* natural, const BYTE n[], int nstart, int nend, size_t capacity) {
	size_t cursor = capacity - 2;
	size_t payload = 0U;

	natural[capacity - 1] = 0;

	do {
		uint16 decimal = byte_to_decimal((uint8)n[nstart++], 0U);
		
		for (size_t idx = capacity - 1; idx > cursor; idx--) {
			uint16 digit = natural[idx] * base + decimal;

			natural[idx] = (uint8)(digit & 0xFFU);
			decimal = digit >> 8;
		}

		if (decimal > 0) {
			payload = capacity - cursor;
			natural[cursor--] = (uint8)decimal;
			
		}
	} while (nstart < nend);

	return payload;
}

static inline size_t fixnum_length(size_t payload, size_t modulus) {
	return payload / modulus + ((payload % modulus > 0) ? 1 : 0);
}

template<typename UI>
static UI fixnum_ref(const uint8* natural, size_t payload, size_t capacity, int slot_idx, size_t offset, size_t size) {
	UI n = 0U;

	if (payload > 0U) {
		size_t start0 = capacity - payload;
		size_t start, end;

		if (slot_idx >= 0) {
			start = capacity - (fixnum_length(payload, size) - slot_idx) * size;
		} else {
			start = capacity + slot_idx * size;
		}

		start += offset;
		end = start + size;

		start = fxmax(start0, start);
		end = fxmin(end, capacity);

		for (size_t idx = start; idx < end; idx++) {
			n = (n << 8U) ^ natural[idx];
		}
	}

	return n;
}

template<typename Q, typename C>
static Q u_vq_hat(uint8 u, uint8 v, Q q_hat, C* carry, uint8* borrowed) {
	Q diff = v * q_hat + (*carry) + (*borrowed);

	if (diff <= 0xFFU) {
		(*carry) = 0U;
	} else {
		(*carry) = diff >> 8U;
		diff &= 0xFFU;
	}

	if (u < diff) {
		diff = 0x100U + u - diff;
		(*borrowed) = 1U;
	} else {
		diff = u - diff;
		(*borrowed) = 0U;
	}

	return diff;
}

template<typename Q>
static inline Q add(uint8 lhs, uint8 rhs, Q* carry) {
	Q digit = lhs + rhs + (*carry);

	if (digit > 0xFFU) {
		digit &= 0xFFU;
		(*carry) = 1U;
	} else {
		(*carry) = 0U;
	}

	return digit;
}

#define NATURAL_MODULAR_EXPT(self, me, b, n) { \
	self->quotient_remainder(n, self); \
\
	for (size_t bidx = 1; bidx < b.payload; bidx++) { \
		uint8 bself = b.natural[b.capacity - bidx]; \
\
		for (uint16 bmask = 0b1U; bmask < 0x100U; bmask <<= 1U) { \
			if ((bself & bmask) > 0U) { \
				me.operator*=(*self).quotient_remainder(n, &me); \
			} \
\
			self->operator*=(*self).quotient_remainder(n, self); \
		} \
	} \
    /* All leading zeros of the leading byte do not change the result but do make some useless computations. */ \
	natural_modular_expt(self, &me, b.natural[b.capacity - b.payload], n); \
}

template<typename N>
static void natural_modular_expt(Natural* self, Natural* me, unsigned long long b, N n) {
	/** NOTE
	 * This function also serves the NATURAL_MODULAR_EXPT for the leading byte of b's payload
	 * Thus, invokers of this function should do all the preparations.
	 */

	do {
		if ((b & 0b1U) > 0U) {
			me->operator*=(*self).quotient_remainder(n, me);
		}

		self->operator*=(*self).quotient_remainder(n, self);
		b >>= 1U;
	} while (b > 0U);

	(*self) = (*me);
}

/*************************************************************************************************/
Natural::~Natural() {
	if (this->natural != nullptr) {
		delete [] this->natural;
	}
}

Natural::Natural() : Natural(0ULL) {}

Natural::Natural(unsigned long long n) : natural(nullptr), capacity(sizeof(unsigned long long)), payload(0U) {
	this->natural = this->malloc(this->capacity);
	this->replaced_by_fixnum(n);
}

Natural::Natural(bytes& nstr, size_t nstart, size_t nend)
	: Natural(nstr.c_str(), nstart, ((nend <= nstart) ? nstr.size() : nend)) {}

Natural::Natural(std::string& nstr, size_t nstart, size_t nend)
	: Natural((const uint8*)nstr.c_str(), nstart, ((nend <= nstart) ? nstr.size() : nend)) {}

Natural::Natural(std::wstring& nstr, size_t nstart, size_t nend)
	: Natural((const uint16*)nstr.c_str(), nstart, ((nend <= nstart) ? nstr.size() : nend)) {}

Natural::Natural(Platform::String^ nstr, size_t nstart, size_t nend)
	: Natural((const uint16*)nstr->Data(), nstart, ((nend <= nstart) ? nstr->Length() : nend)) {}

Natural::Natural(uint8 base, bytes& nstr, size_t nstart, size_t nend)
	: Natural(base, nstr.c_str(), nstart, ((nend <= nstart) ? nstr.size() : nend)) {}

Natural::Natural(uint8 base, std::string& nstr, size_t nstart, size_t nend)
	: Natural(base, (const uint8*)nstr.c_str(), nstart, ((nend <= nstart) ? nstr.size() : nend)) {}

Natural::Natural(uint8 base, std::wstring& nstr, size_t nstart, size_t nend)
	: Natural(base, (const uint16*)nstr.c_str(), nstart, ((nend <= nstart) ? nstr.size() : nend)) {}

Natural::Natural(uint8 base, Platform::String^ nstr, size_t nstart, size_t nend)
	: Natural(base, (const uint16*)nstr->Data(), nstart, ((nend <= nstart) ? nstr->Length() : nend)) {}

bool Natural::is_zero() const {
	return ((this->payload == 0)
		|| ((this->payload == 1) // redundant checking
			&& (this->natural[this->capacity - 1] == 0)));
}

bool Natural::is_one() const {
	return ((this->payload == 1)
		&& (this->natural[this->capacity - 1] == 1));
}

bool Natural::is_fixnum() const {
	return (this->payload <= sizeof(unsigned long long));
}

bool Natural::is_odd() const {
	return ((this->payload > 0U) && ((this->natural[this->capacity - 1U] & 0x1U) == 0x1U));
}

bool Natural::is_even() const {
	return ((this->payload == 0U) || ((this->natural[this->capacity - 1U] & 0x1U) == 0x0U));
}

size_t Natural::length() const {
	return this->payload;
}

size_t Natural::integer_length(uint8 alignment) const {
	size_t s = 0;

	if (this->payload > 0) {
		s = (this->payload - 1) * 8;
		s += ::integer_length(this->natural[this->capacity - this->payload]);
	}

	if (alignment > 0) {
		if (s > 0) {
			size_t remainder = s % alignment;

			if (remainder > 0) {
				s += (alignment - remainder);
			}
		} else {
			s = alignment;
		}
	}

	return s;
}

bytes Natural::to_bytes() const {
	return bytes(this->natural + (this->capacity - this->payload), this->payload);
}

bytes Natural::to_hexstring() const {
	bytes hex(fxmax((unsigned int)this->payload, 1U) * 2, '0');
	size_t payload_idx = this->capacity - this->payload;
	size_t msb_idx = 0U;

	for (size_t idx = 0; idx < this->payload; idx++) {
		uint8 ubyte = this->natural[idx + payload_idx];
		
		if (ubyte <= 0xF) {
			msb_idx++;
			hex[msb_idx++] = hexadecimal_to_byte(ubyte);
		} else {
			hex[msb_idx++] = hexadecimal_to_byte(ubyte >> 4);
			hex[msb_idx++] = hexadecimal_to_byte(ubyte & 0xF);
		}
	}

	return hex;
}

bytes Natural::to_binstring(uint8 alignment) const {
	size_t bsize = this->integer_length(alignment);
	bytes bin(bsize, '0');
	size_t lsb_idx = bsize - 1U;

	for (size_t idx = 0; idx < this->payload; idx++) {
		uint8 ubyte = this->natural[this->capacity - idx - 1];
		
		for (size_t bits = 0; bits < 8; bits++) {
			if ((ubyte & (0x1U << bits)) > 0) {
				bin[lsb_idx] = '1';
			}
			
			if (lsb_idx > 0) {
				lsb_idx--;
			} else {
				break;
			}
		};
	}

	return bin;
}

/*************************************************************************************************/
Natural::Natural(const Natural& n) : natural(nullptr), capacity(fxmax(n.payload, sizeof(unsigned long long))), payload(n.payload) {
	this->natural = this->malloc(this->capacity);

	if (this->payload > 0) {
		memcpy(this->natural + (this->capacity - this->payload),
			n.natural + (n.capacity - n.payload),
			this->payload);
	}
}

Natural::Natural(Natural&& n) : natural(n.natural), capacity(n.capacity), payload(n.payload) {
	n.on_moved();
}

Natural& Natural::operator=(unsigned long long n) {
	this->replaced_by_fixnum(n);

	return (*this);
}

Natural& Natural::operator=(const Natural& n) {
	if (n.payload > this->capacity) {
		if (this->natural != nullptr) {
			delete[] this->natural;
		}

		this->capacity = n.payload;
		this->natural = this->malloc(this->capacity);
	}

	this->payload = n.payload;
	if (this->payload > 0) {
		size_t payload_idx = this->capacity - this->payload;
		size_t n_idx = n.capacity - this->payload;

		memcpy(this->natural + payload_idx, n.natural + n_idx, this->payload);
	}

	return (*this);
}

Natural& Natural::operator=(Natural&& n) {
	if (this != &n) {
		if (this->natural != nullptr) {
			delete[] this->natural;
		}

		this->natural = n.natural;
		this->capacity = n.capacity;
		this->payload = n.payload;

		n.on_moved();
	}

	return (*this);
}

void Natural::on_moved() {
	this->capacity = 0U;
	this->payload = 0U;
	this->natural = nullptr;
}

/*************************************************************************************************/
int Natural::compare(unsigned long long rhs) const {
	int cmp = 0;

	if (this->is_fixnum()) {
		for (size_t slot = sizeof(unsigned long long); slot > 0; slot--) {
			uint8 ldigit = ((slot > this->payload) ? 0U : this->natural[this->capacity - slot]);
			uint8 rdigit = (rhs >> ((slot - 1) * 8)) & 0xFFU;

			if (ldigit == rdigit) {
				continue;
			}

			cmp = ((ldigit < rdigit) ? -1 : 1);
			break;
		}
	} else {
		cmp = 1;
	}

	return cmp;
}

int Natural::compare(const Natural& rhs) const {
	int cmp = ((this->payload < rhs.payload) ? -1 : +1);

	if (this->payload == rhs.payload) {
		cmp = memcmp(this->natural + (this->capacity - this->payload),
			rhs.natural + (rhs.capacity - rhs.payload),
			this->payload);
	}

	return cmp;
}

/*************************************************************************************************/
Natural& Natural::operator++() {
	this->add_digit(1U);

	return (*this);
}

Natural& Natural::operator--() {
	this->decrease_from_slot(1U);

	return (*this);
}

Natural& Natural::operator+=(unsigned long long rhs) {
	if (rhs > 0xFFU) {
		size_t digits = fxmax(this->payload, sizeof(unsigned long long));
		size_t addend_idx = this->capacity - 1;
		
		if (this->capacity <= digits) {
			this->recalloc(digits + 1);
			addend_idx = this->capacity - 1;
		}

		do {
			uint16 digit = this->natural[addend_idx] + (rhs & 0xFFU);

			if (digit > 0xFF) {
				this->natural[addend_idx] = (uint8)(digit & 0xFFU);
				rhs = (rhs >> 8U) + 1U;
			} else {
				this->natural[addend_idx] = (uint8)digit;
				rhs >>= 8U;
			}

			addend_idx--;
		} while (rhs > 0U);

		this->payload = fxmax(this->payload, (this->capacity - addend_idx - 1));
	} else {
		this->add_digit((uint8)rhs);
	}

	return (*this);
}

Natural& Natural::operator+=(const Natural& rhs) {
	// NOTE: the rhs may refer to (*this)

	if (rhs.payload > 1U) {
		size_t digits = fxmax(this->payload, rhs.payload);
		size_t cdigits = fxmin(this->payload, rhs.payload);
		size_t ddigits = digits - cdigits;
		size_t lcapacity = this->capacity;
		uint8* lsrc = this->natural;
		uint16 carry = 0U;

		if (this->capacity <= digits) { // no `this->expand`ing to avoid redundant copy
			this->capacity = digits + 1;
			this->natural = this->malloc(this->capacity);
		}

		for (size_t idx = 1; idx <= cdigits; idx++) {
			this->natural[this->capacity - idx] = (uint8)add(lsrc[lcapacity - idx], rhs.natural[rhs.capacity - idx], &carry);
		}

		if (ddigits > 0) {
			if (this->payload < rhs.payload) {
				memcpy(this->natural + (this->capacity - digits), rhs.natural + (rhs.capacity - rhs.payload), ddigits);
			} else if (lsrc != this->natural) {
				memcpy(this->natural + (this->capacity - digits), lsrc + (lcapacity - this->payload), ddigits);
			}

			if (carry == 1) {
				for (size_t idx = cdigits + 1; idx <= digits; idx++) {
					size_t slot = this->capacity - idx;

					if (this->natural[slot] == 0xFFU) {
						this->natural[slot] = 0U;
					} else {
						this->natural[slot] += 1;
						carry = 0U;
						break;
					}
				}
			}
		}

		if (carry == 1) {
			this->payload = digits + carry;
			this->natural[this->capacity - this->payload] = 1;
		} else {
			this->payload = digits;
		}

		if (this->natural != lsrc) {
			delete[] lsrc;
		}
	} else if (rhs.payload == 1U) {
		this->add_digit(rhs.natural[rhs.capacity - 1U]);
	}

	return (*this);
}

Natural& Natural::operator-=(unsigned long long rhs) {
	if ((!this->is_zero()) && (rhs > 0U)) {
		size_t rhs_payloadp1 = 1U;
		uint8 borrowed = 0U;

		while ((rhs_payloadp1 <= this->payload) && (rhs > 0U)) {
			uint16 minuend = this->natural[this->capacity - rhs_payloadp1];
			uint16 subtrachend = (rhs & 0xFFU) + borrowed;

			if (minuend >= subtrachend) {
				this->natural[this->capacity - rhs_payloadp1] = (uint8)(minuend - subtrachend);
				borrowed = 0U;
			} else {
				this->natural[this->capacity - rhs_payloadp1] = (uint8)(0x100U + minuend - subtrachend);
				borrowed = 1U;
			}

			rhs >>= 8U;
			rhs_payloadp1++;
		}

		if (rhs > 0U) {
			this->bzero();
		} else if (borrowed > 0U) {
			if (this->payload + 1U == rhs_payloadp1) {
				this->bzero();
			} else {
				this->decrease_from_slot(rhs_payloadp1);
				this->skip_leading_zeros(this->payload);
			}
		} else {
			this->skip_leading_zeros(this->payload);
		}
	}

	return (*this);
}

Natural& Natural::operator-=(const Natural& rhs) {
	if (!rhs.is_zero()) {
		if (this->payload >= rhs.payload) {
			uint8 borrowed = 0U;

			for (size_t idx = 1; idx <= rhs.payload; idx++) {
				uint16 minuend = this->natural[this->capacity - idx];
				uint16 subtrachend = rhs.natural[rhs.capacity - idx] + borrowed;

				if (minuend >= subtrachend) {
					this->natural[this->capacity - idx] = (uint8)(minuend - subtrachend);
					borrowed = 0U;
				} else {
					this->natural[this->capacity - idx] = (uint8)(0x100U + minuend - subtrachend);
					borrowed = 1U;
				}
			}

			if (borrowed > 0U) {
				if (this->payload == rhs.payload) {
					this->bzero();
				} else {
					this->decrease_from_slot(rhs.payload + 1U);
					this->skip_leading_zeros(this->payload);
				}
			} else if (this->payload == rhs.payload) {
				this->skip_leading_zeros(this->payload);
			}
		} else {
			this->bzero();
		}
	}

	return (*this);
}

Natural& Natural::operator*=(unsigned long long rhs) {
	if (!this->is_zero()) {
		if (rhs > 0xFFU) {
			size_t digits = this->payload + sizeof(unsigned long long);
			uint8* product = this->natural;
			size_t j = 0U;

			if (this->capacity < digits + this->payload) {
				product = this->malloc(digits);
			}

			memset(product + (digits - this->payload), '\0', this->payload);

			do {
				uint16 carry = 0U;
				uint8 v = (uint8)(rhs & 0xFFU);

				if (v > 0) {
					for (size_t i = 1; i <= this->payload; i++) {
						size_t ij = digits - i - j;
						uint16 digit = this->natural[this->capacity - i] * v + product[ij] + carry;

						product[ij] = (uint8)(digit & 0xFFU);
						carry = (digit >> 8);
					}
				}

				product[digits - this->payload - (++j)] = (uint8)carry;
				rhs >>= 8;
			} while (rhs > 0U);

			if (this->natural != product) {
				delete[] this->natural;
				this->natural = product;
				this->capacity = digits;
			} else {
				memmove(this->natural + (this->capacity - digits), this->natural, digits);
			}

			this->skip_leading_zeros(this->payload + j);
		} else {
			if (rhs == 0U) {
				this->bzero();
			} else {
				this->times_digit((uint8)rhs);
			}
		}
	}

	return (*this);
}

Natural& Natural::operator*=(const Natural& rhs) {
	// NOTE: the rhs may refer to (*this)

	if (!this->is_zero()) {
		if (rhs.payload > 1U) {
			size_t digits = this->payload + rhs.payload;
			uint8* product = this->natural;

			if (this->capacity < this->payload + digits) {
				product = this->malloc(digits);
			}
			
			memset(product + (digits - this->payload), '\0', this->payload);

			for (size_t j = 1; j <= rhs.payload; j++) {
				uint16 carry = 0U;
				
				if (rhs.natural[rhs.capacity - j] > 0) {
					for (size_t i = 1; i <= this->payload; i++) {
						size_t ij = digits - i - j + 1;
						uint16 digit = this->natural[this->capacity - i] * rhs.natural[rhs.capacity - j] + product[ij] + carry;

						product[ij] = (uint8)(digit & 0xFFU);
						carry = (digit >> 8);
					}
				}

				product[digits - this->payload - j] = (uint8)carry;
			}

			if (this->natural != product) {
				delete[] this->natural;
				this->natural = product;
				this->capacity = digits;
			} else {
				memmove(this->natural + (this->capacity - digits), this->natural, digits);
			}

			this->skip_leading_zeros(digits);
		} else {
			if (rhs.is_zero()) {
				this->bzero();
			} else {
				this->times_digit(rhs.natural[rhs.capacity - 1U]);
			}
		}
	}

	return (*this);
}

Natural& Natural::quotient_remainder(unsigned long long rhs, Natural* oremainder) {
	// WARNING: `rhs` may refer to `(*this)`, `oremainder` may point to `this`.
	
	if (!this->is_zero()) {
		if (rhs > 0xFFU) {
			size_t payload_idx = this->capacity - this->payload;
			size_t divisor_payload = fixnum_length(::integer_length(rhs), 8U);
			size_t quotient_idx = payload_idx + divisor_payload - 1U;

			if (this->payload >= divisor_payload) {
				unsigned long long remainder = 0U;
				size_t idx = payload_idx;

				while (idx < quotient_idx) {
					remainder = (remainder << 8U) ^ this->natural[idx++];
				}

				do {
					remainder = (remainder << 8U) ^ this->natural[idx];

					if (remainder < rhs) {
						this->natural[idx++] = 0U;
					} else {
						this->natural[idx++] = (uint8)(remainder / rhs);
						remainder = remainder % rhs;
					}
				} while (idx < this->capacity);

				this->skip_leading_zeros(this->capacity - quotient_idx);
				
				if (oremainder != nullptr) {
					(*oremainder) = remainder;
				}
			} else if (oremainder == nullptr) {
				this->bzero();
			} else if (oremainder != nullptr) {	
				if (this != oremainder) {
					oremainder->operator=(*this);
					this->bzero();
				}
			}
		} else if (rhs > 0U) {
			this->divide_digit((uint8)rhs, oremainder);
		}
	} else if (oremainder != nullptr) {
		oremainder->bzero();
	}

	return (*this);
}

Natural& Natural::quotient_remainder(const Natural& rhs, Natural* oremainder) {
	// Algorithm: classic method that to estimate the pencil-and-paper long division.
	
	/** Theorem
	 * u = (u_n u_n-1 ... u_1 u_0)b
	 * v =     (v_n-1 ... v_1 v_0)b
	 * where u/v < b <==> u/b < v <==> floor(u/b) < v
	 * thus, q = floor(u/v), r = u - qv
	 *
	 * In order to eliminate the guesswork for q, let's find a
	 *     q^ = min(floor((u_n * b + u_n-1) / v_n-1), b - 1)
	 *   thus
	 *   A). q^ >= q;
	 *   B). v_n-1 >= floor(b/2) ==> q^-2 <= q <= q^
	 */

	// WARNING: `rhs` may refer to `(*this)`, `oremainder` may point to `this`.

	if (!this->is_zero()) {
		if (!rhs.is_fixnum()) {
			int cmp = this->compare(rhs);

			if (cmp > 0) { // `this` will be the remainder
				size_t quotient_size = (this->payload - rhs.payload) + 1U;
				uint8* quotient = ((this == oremainder) ? nullptr : this->natural);
				uint8 vn_1 = rhs.natural[rhs.capacity - rhs.payload];
				uint8 vn_2 = rhs.natural[rhs.capacity - rhs.payload + 1U];
				uint8 shifts = 0U;

				{ // normalization
					if (vn_1 < 0x80U) { // 0x80 is `base/2`
						uint32 virtual_shifted = (vn_1 << 16U) ^ (vn_2 << 8U) ^ rhs.natural[rhs.capacity - rhs.payload + 2U];

						while (virtual_shifted < 0x80000000U) {
							virtual_shifted <<= 1U;
							shifts += 1U;
						}

						vn_1 = (virtual_shifted >> 24U) & 0xFFU;
						vn_2 = (virtual_shifted >> 16U) & 0xFFU;
					}

					{ // m+n-bytes dividend / n-bytes divisor = m+1-bytes quotient ... n-bytes remainder
						if (quotient == nullptr) {
							if (this->capacity == this->payload) {
								this->expand(1U);
							}
						} else {
							if (this->capacity < this->payload + quotient_size + 1U) {
								this->expand(this->payload + quotient_size + 1U - this->capacity);
								quotient = this->natural;
							}
						}

						this->payload += 1U;
						this->natural[this->capacity - this->payload] = '\0';
					}
				}

				{ // long division
					size_t quotient_idx0 = this->capacity - quotient_size;

					for (size_t j_idx = 0; j_idx < quotient_size; j_idx++) {
						size_t j = quotient_idx0 + j_idx;
						size_t jn_idx = quotient_idx0 + j_idx - rhs.payload;
						uint64 ujn_0 = this->natural[jn_idx + 0U];
						uint64 ujn_1 = this->natural[jn_idx + 1U];
						uint64 ujn_2 = this->natural[jn_idx + 2U];
						uint64 ujnb_ujn_1 = (ujn_0 << 8U) ^ ujn_1;
						uint64 q_hat = 0U;

						if (shifts > 0U) {
							uint64 virtual_shifted = ((ujnb_ujn_1 << 16U) ^ (ujn_2 << 8U) ^ this->natural[jn_idx + 3U]) << shifts;

							ujnb_ujn_1 = (virtual_shifted >> 24U) & 0xFFFFU;
							ujn_2 = (virtual_shifted >> 16U) & 0xFFU;
						}

						{ // The q^ found here is guaranteed accurate (almost) 
							uint64 r_hat = ujnb_ujn_1 % vn_1;

							q_hat = ujnb_ujn_1 / vn_1;

							if (q_hat > 0xFFU) {
								r_hat += (vn_1 * (q_hat - 0xFFU));
								q_hat = 0xFFU;
							}

							while ((q_hat * vn_2) > ((r_hat << 8U) ^ ujn_2)) {
								q_hat -= 1U;
								r_hat += vn_1;
							}
						}

						{ // in-place: r = u - v(q^)
							size_t divisor_diff = rhs.capacity - j - 1U;
							uint64 carry = 0U;
							uint8 borrowed = 0U;

							for (size_t idx = j; idx > jn_idx; idx--) {
								this->natural[idx] = (uint8)u_vq_hat(this->natural[idx], rhs.natural[idx + divisor_diff], q_hat, &carry, &borrowed);
							}
							
							this->natural[jn_idx] = (uint8)u_vq_hat(this->natural[jn_idx], 0U, q_hat, &carry, &borrowed);

							if (borrowed > 0U) { // the probability of this case is `2/base`.
								q_hat -= 1U;
								carry = 0U;

								for (size_t idx = j; idx > jn_idx; idx--) {
									this->natural[idx] = (uint8)add(this->natural[idx], rhs.natural[idx + divisor_diff], &carry);
								}

								this->natural[jn_idx] = (uint8)add(this->natural[jn_idx], 0U, &carry);
								// Ignore the carry here since it is triggered by the borrowing.
							}

							if (quotient != nullptr) {
								quotient[j_idx] = (uint8)q_hat;
							}
						}
					}
				}

				if (oremainder != nullptr) {
					this->skip_leading_zeros(rhs.payload);

					if (this != oremainder) {
						(*oremainder) = (*this);
					}
				}

				if (quotient != nullptr) { // <==> (this != oremainder)
					memmove(this->natural + (this->capacity - quotient_size), this->natural, quotient_size);
					this->skip_leading_zeros(quotient_size);
				}
			} else if (cmp == 0) {
				this->payload = 1U;
				this->natural[this->capacity - 1U] = 1U;

				if (oremainder != nullptr) {
					oremainder->bzero();
				}
			} else if (oremainder == nullptr) {
				this->bzero();
			} else if (oremainder != nullptr) {
				if (this != oremainder) {
					oremainder->operator=(*this);
					this->bzero();
				}
			}
		} else if (rhs.payload > 1U) {
			this->quotient_remainder(rhs.fixnum64_ref(0), oremainder);
		} else if (rhs.payload == 1U) {
			this->divide_digit(rhs.natural[rhs.capacity - 1U], oremainder);
		}
	} else if (oremainder != nullptr) {
		oremainder->bzero();
	}

	return (*this);
}

Natural& Natural::expt(unsigned long long n) {
	Natural Z = (*this);
	
	(*this) = 1U;

	while (n > 0U) {
		if ((n & 0x1U) > 0) {
			this->operator*=(Z);
		}

		n >>= 1U;
		Z *= Z;
	}

	return (*this);
}

Natural& Natural::expt(const Natural& n) {
	// Algorithm: Right-to-Left binary method (also known as "Russian Peasant Method")
	Natural Z = (*this);
	Natural N = n;

	(*this) = 1U;

	while (N.payload > 0U) {
		if (N.is_odd()) {
			this->operator*=(Z);
		}

		N >>= 1U;
		Z *= Z;
	}

	return (*this);
}

Natural& Natural::modular_expt(unsigned long long b, unsigned long long n) {
	if (b > 0U) {
		size_t product_size = sizeof(unsigned long long) * 2U;
		Natural me = 1U;
		
		me.smart_prealloc(product_size);
		this->smart_prealloc(product_size);
		this->quotient_remainder(n, this);
		natural_modular_expt(this, &me, b, n);
	} else if (b == 0U) {
		(*this) = 1U;
	} else {
		this->quotient_remainder(n, this);
	}

	return (*this);
}

Natural& Natural::modular_expt(const Natural& b, unsigned long long n) {
	if (b.is_fixnum()) {
		this->modular_expt(b.fixnum64_ref(0U), n);
	} else {
		size_t product_size = sizeof(unsigned long long) * 2U;
		Natural me = 1U;

		me.smart_prealloc(product_size);
		this->smart_prealloc(product_size);
		NATURAL_MODULAR_EXPT(this, me, b, n);
	}

	return (*this);
}

Natural& Natural::modular_expt(const Natural& b, const Natural& n) {
	// Algorithm: repeated modular multiplication and squaring (the idea is the same as `expt`)

	/** Theorem
	 * f(a, b)
	 *   = 1 % n,              b = 0;
	 *   = a % n,              b = 1;
	 *   = f^2(a, b >> 1) % n, b is even;
	 *   = a*f(a, b - 1) % n,  b is odd;
	 */

	bool fixed_b = b.is_fixnum();
	bool fixed_n = n.is_fixnum();

	if (fixed_b || fixed_b) {
		if (fixed_b && fixed_b) {
			this->modular_expt(b.fixnum64_ref(0U), n.fixnum64_ref(0U));
		} else if (fixed_b) {
			this->modular_expt(b.fixnum64_ref(0U), n);
		} else if (fixed_n) {
			this->modular_expt(b, n.fixnum64_ref(0U));
		}
	} else {
		size_t product_size = n.payload * 2U;
		Natural me = 1U;

		me.smart_prealloc(product_size);
		this->smart_prealloc(product_size);
		NATURAL_MODULAR_EXPT(this, me, b, n);
	}

	return (*this);
}

Natural& Natural::modular_expt(unsigned long long b, const Natural& n) {
	if (b > 0U) {
		if (n.is_fixnum()) {
			this->modular_expt(b, n.fixnum64_ref(0));
		} else {
			size_t product_size = n.payload * 2U;
			Natural me = 1U;

			me.smart_prealloc(product_size);
			this->smart_prealloc(product_size);
			this->quotient_remainder(n, this);
			natural_modular_expt(this, &me, b, n);
		}
	} else if (b == 0U) {
		(*this) = 1U;
	} else {
		this->quotient_remainder(n, this);
	}

	return (*this);
}

/*************************************************************************************************/
Natural Natural::operator~() {
	Natural ones_complement(*this);

	
	for (size_t idx = ones_complement.capacity - ones_complement.payload; idx < ones_complement.capacity; idx++) {
		ones_complement.natural[idx] = ~ones_complement.natural[idx];
	}

	ones_complement.skip_leading_zeros(ones_complement.payload);

	return ones_complement;
}

Natural& Natural::operator<<=(unsigned long long rhs) {	
	if ((!this->is_zero()) && (rhs > 0U)) {
		size_t shift_byts = (size_t)(rhs / 8);
		size_t shift_bits = (size_t)(rhs % 8);
		size_t shift_load = this->payload + shift_byts;
		size_t original_idx0 = this->capacity - this->payload;
		size_t total = shift_load;

		if (shift_bits == 0U) {
			if (this->capacity < total) {
				this->recalloc(total, shift_byts);
			} else if (shift_byts > 0U) {
				memmove(this->natural + (this->capacity - shift_load), this->natural + original_idx0, this->payload);
				memset(this->natural + (this->capacity - shift_byts), '\0', shift_byts);
			}
		} else {
			const uint8* src = this->natural;
			size_t shift_diff = this->capacity - shift_load - original_idx0;
			
			if (((uint16)(src[original_idx0]) << shift_bits) > 0xFFU) {
				total++;
			}

			if (this->capacity < total) { // no `this->expand`ing to avoid redundant copy 
				shift_diff += (total - this->capacity);

				this->capacity = total;
				this->natural = this->malloc(this->capacity);
				
				/** NOTE
				 * There is no need to set other slots of `natural` to zero since
				 *   after shifting all slots will be set by algorithm.
				 */
			}

			if ((shift_byts > 0) || (total > shift_load)) {
				this->natural[this->capacity - total] = '\0';
			}

			for (size_t idx = original_idx0; idx < original_idx0 + this->payload; idx++) {
				uint16 digit = ((uint16)(src[idx]) << shift_bits);
				size_t shift_idx = idx + shift_diff;

				if (digit <= 0xFFU) {
					this->natural[shift_idx] = (uint8)digit;
				} else {
					this->natural[shift_idx] = (uint8)(digit & 0xFFU);
					this->natural[shift_idx - 1] ^= (uint8)(digit >> 8U);
				}
			}

			if (shift_byts > 0U) {
				memset(this->natural + (this->capacity - shift_byts), '\0', shift_byts);
			}

			if (src != this->natural) {
				delete[] src;
			}
		}

		this->payload = total;
	}

	return (*this);
}

Natural& Natural::operator>>=(unsigned long long rhs) {
	if ((!this->is_zero()) && (rhs != 0U)) {
		size_t shift_byts = (size_t)(rhs / 8);
		
		if (this->payload <= shift_byts) {
			this->bzero();
		} else {
			size_t shift_bits = (size_t)(rhs % 8);
			size_t shift_cbits = 8 - shift_bits;
			size_t original_idx0 = this->capacity - this->payload;
			
			this->payload -= shift_byts;

			if (shift_bits == 0U) {
				memmove(this->natural + (this->capacity - this->payload), this->natural + original_idx0, this->payload);
				//memset(this->natural + original_idx0, '\0', shift_byts); // reduntant
			} else {
				size_t shift_endp1 = original_idx0 + this->payload;
				size_t shift_diff = this->capacity - shift_endp1 - 1;
				size_t mask = (1 << shift_bits) - 1;

				this->natural[this->capacity - 1] = (this->natural[shift_endp1 - 1] >> shift_bits);
				
				for (size_t idx = shift_endp1 - 1; idx > original_idx0; idx--) {
					size_t shift_idx = idx + shift_diff;
					uint8 digit = this->natural[idx - 1];
					uint8 tail = digit & mask;
					
					this->natural[shift_idx] = (digit >> shift_bits);

					if (tail > 0U) {
						this->natural[shift_idx + 1] ^= (tail << shift_cbits);
					}
				}

				// reduntant zeroing since other operators do not assume it
				//if (shift_byts > 0U) {
				//	memset(this->natural + original_idx0, '\0', shift_byts);
				//}

				if (this->natural[this->capacity - this->payload] == 0U) {
					this->payload--;
				}
			}
		}
	}

	return (*this);
}

Natural& Natural::operator&=(unsigned long long rhs) {
	size_t idx0 = this->capacity - fxmin(this->payload, sizeof(unsigned long long));

	this->payload = 0U;
	for (size_t idx = this->capacity; (rhs > 0U) && (idx > idx0); idx--) {
		size_t payload_idx = idx - 1U;

		this->natural[payload_idx] &= (rhs & 0xFFU);

		if (this->natural[payload_idx] > 0U) {
			this->payload = this->capacity - payload_idx;
		}

		rhs >>= 8ULL;
	}

	return (*this);
}

Natural& Natural::operator&=(const Natural& rhs) {
	size_t upsize = fxmin(this->payload, rhs.payload);

	this->payload = 0U;
	for (size_t idx = 1; idx <= upsize; idx++) {
		size_t payload_idx = this->capacity - idx;

		this->natural[payload_idx] &= rhs.natural[rhs.capacity - idx];

		if (this->natural[payload_idx] > 0U) {
			this->payload = this->capacity - payload_idx;
		}
	}

	return (*this);
}

Natural& Natural::operator|=(unsigned long long rhs) {
	if (rhs > 0U) {
		size_t digits = fxmax(this->payload, sizeof(unsigned long long));
		size_t nidx = this->capacity - 1;

		if (this->capacity < digits) {
			this->recalloc(digits);
			nidx = this->capacity - 1;
		} else {
			memset(this->natural, '\0', this->capacity - this->payload);
		}

		do {
			this->natural[nidx--] |= (rhs & 0xFFU);
			rhs >>= 8U;
		} while (rhs > 0U);

		this->payload = fxmax(this->payload, (this->capacity - nidx - 1));
	}

	return (*this);
}

Natural& Natural::operator|=(const Natural& rhs) {
	if (!rhs.is_zero()) {
		size_t digits = fxmax(this->payload, rhs.payload);
		size_t cdigits = fxmin(this->payload, rhs.payload);
		size_t ddigits = digits - cdigits;
		size_t lcapacity = this->capacity;
		uint8* lsrc = this->natural;
		
		if (this->capacity < digits) {
			this->capacity = digits;
			this->natural = this->malloc(this->capacity);
		}

		for (size_t idx = 1; idx <= cdigits; idx++) {
			this->natural[this->capacity - idx] = (lsrc[lcapacity - idx] | rhs.natural[rhs.capacity - idx]);
		}

		if (ddigits > 0U) {
			if (this->payload < rhs.payload) {
				memcpy(this->natural + (this->capacity - digits), rhs.natural + (rhs.capacity - rhs.payload), ddigits);
			} else if (lsrc != this->natural) {
				memcpy(this->natural + (this->capacity - digits), lsrc + (lcapacity - this->payload), ddigits);
			}
		}

		this->payload = digits;
		
		if (this->natural != lsrc) {
			delete[] lsrc;
		}
	}

	return (*this);
}

Natural& Natural::operator^=(unsigned long long rhs) {
	if (rhs > 0U) {
		size_t digits = fxmax(this->payload, sizeof(unsigned long long));
		size_t nidx = this->capacity - 1;

		if (this->capacity < digits) {
			this->recalloc(digits);
			nidx = this->capacity - 1;
		} else {
			memset(this->natural, '\0', this->capacity - this->payload);
		}

		do {
			this->natural[nidx--] ^= (rhs & 0xFFU);
			rhs >>= 8U;
		} while (rhs > 0U);

		{ // resolve the payload
			size_t rhs_payload = (this->capacity - nidx - 1);

			if (this->payload == rhs_payload) {
				this->skip_leading_zeros(rhs_payload);
			} else if (this->payload < rhs_payload) {
				this->payload = rhs_payload;
			}
		}
	}

	return (*this);
}

Natural& Natural::operator^=(const Natural& rhs) {
	if (!rhs.is_zero()) {
		size_t digits = fxmax(this->payload, rhs.payload);
		size_t cdigits = fxmin(this->payload, rhs.payload);
		size_t ddigits = digits - cdigits;
		size_t lcapacity = this->capacity;
		uint8* lsrc = this->natural;
		
		if (this->capacity < digits) {
			this->capacity = digits;
			this->natural = this->malloc(this->capacity);
		}

		for (size_t idx = 1; idx <= cdigits; idx++) {
			this->natural[this->capacity - idx] = (lsrc[lcapacity - idx] ^ rhs.natural[rhs.capacity - idx]);
		}

		if (ddigits > 0U) {
			if (this->payload < rhs.payload) {
				memcpy(this->natural + (this->capacity - digits), rhs.natural + (rhs.capacity - rhs.payload), ddigits);
			} else if (lsrc != this->natural) {
				memcpy(this->natural + (this->capacity - digits), lsrc + (lcapacity - this->payload), ddigits);
			}

			this->payload = digits;
		} else {
			this->skip_leading_zeros(cdigits);
		}

		if (this->natural != lsrc) {
			delete[] lsrc;
		}
	}

	return (*this);
}

bool Natural::is_bit_set(unsigned long long m) {
	bool set = false;

	if (m <= (this->payload * 8U)) {
		uint64 q = m / 8;
		uint64 r = m % 8;
		uint8 digit = this->natural[this->capacity - 1 - q];

		set = ((digit & (1 << r)) > 0U);
	}

	return set;
}

Natural Natural::bit_field(unsigned long long start, unsigned long long end) { // counting from right side
	size_t startq = size_t(start / 8);
	size_t endq = size_t(end) / 8 + 1;
	size_t endr = size_t(end % 8);
	Natural sub(nullptr, 0LL);

	if (startq < this->payload) {
		if (endq > this->payload) {
			endq = this->payload;
			endr = 0U;
		}

		if (endq > startq) {
			size_t startr = (size_t)(start % 8);
			
			sub.payload = endq - startq;

			if (sub.payload > sub.capacity) {
				sub.expand(sub.payload - sub.capacity);
			}
			
			memcpy(sub.natural + (sub.capacity - sub.payload), this->natural + (this->capacity - endq), sub.payload);

			if (endr > 0U) {
				sub.natural[sub.capacity - sub.payload] &= ((1 << endr) - 1);
			}

			sub >>= startr;
		}
	}

	return sub;
}

unsigned long long Natural::bitfield(unsigned long long start, unsigned long long end0) { // counting from right side
	unsigned long long sub = 0x0U;
	size_t end = fxmin(start + 64, end0);
	size_t startq = size_t(start / 8);
	size_t endq = size_t(end) / 8 + 1;
	size_t endr = size_t(end % 8);
	
	if (startq < this->payload) {
		if (endq > this->payload) {
			endq = this->payload;
			endr = 0U;
		}

		if (endq > startq) {
			size_t startr = (size_t)(start % 8);
			size_t idx = this->capacity - endq;
			size_t terminator = (endq - startq) + idx;

			sub = this->natural[idx++];
			
			if (endr > 0U) {
				sub &= ((1 << endr) - 1);
			}

			while (idx < terminator) {
				sub <<= 8;
				sub |= this->natural[idx++];
			}

			sub >>= startr;
		}
	}

	return sub;
}

long long Natural::signed_bitfield(unsigned long long start, unsigned long long end) { // counting from right side
	unsigned long long mask_length = fxmin(end - start, 64ULL) - 1;
	unsigned long long raw = this->bitfield(start, end);
	long long sint = 0LL;

	if ((raw >> mask_length) > 0) {
		unsigned long long mask = (1 << mask_length) - 1U;
		
		sint = static_cast<long long>(raw | ((~0ULL) & (~mask)));
	} else {
		sint = static_cast<long long>(raw);
	}

	return sint;
}

/*************************************************************************************************/
uint8& Natural::operator[](int idx) {
	size_t bidx = 0U;

	if (this->payload == 0U) {
		// WARNING: this is an undefined behavior.
	} else if (idx >= 0) {
		if (((size_t)idx) < this->payload) {
			bidx = this->capacity - this->payload + idx;
		}
	} else {
		if (idx >= -int(this->payload)) {
			bidx = this->capacity + idx;
		}
	}

	return this->natural[bidx];
}

size_t Natural::fixnum_count(Fixnum type) const {
	size_t modulus = 8U;

	switch (type) {
	case Fixnum::Uint64: modulus = 8U; break;
	case Fixnum::Uint32: modulus = 4U; break;
	case Fixnum::Uint16: modulus = 2U; break;
	}

	return fixnum_length(this->payload, modulus);
}

uint16 Natural::fixnum16_ref(int slot_idx, size_t offset) const {
	return fixnum_ref<uint16>(this->natural, this->payload, this->capacity, slot_idx, offset, 2U);
}

uint32 Natural::fixnum32_ref(int slot_idx, size_t offset) const {
	return fixnum_ref<uint32>(this->natural, this->payload, this->capacity, slot_idx, offset, 4U);
}

uint64 Natural::fixnum64_ref(int slot_idx, size_t offset) const {
	return fixnum_ref<uint64>(this->natural, this->payload, this->capacity, slot_idx, offset, 8U);
}

/*************************************************************************************************/
size_t Natural::expand(size_t size) {
	if (size > 0) {
		if (this->natural != nullptr) {
			this->recalloc(this->capacity + size);
		} else {
			this->capacity += size;
			this->natural = this->malloc(this->capacity);
		}
	}

	return this->capacity;
}

/*************************************************************************************************/
Natural::Natural(void* null, long long capacity) : natural(nullptr), capacity(0L), payload(0L) {
	this->capacity = ((capacity > 0) ? (size_t)capacity : sizeof(unsigned long long));
	this->natural = this->malloc(this->capacity);
}

void Natural::replaced_by_fixnum(unsigned long long n) {
	this->payload = 0U;

	while (n > 0U) {
		this->payload++;
		this->natural[this->capacity - this->payload] = (uint8)(n & 0xFFU);
		
		n >>= 8U;
	}
}

void Natural::from_memory(const uint8 nbytes[], size_t nstart, size_t nend) {
	if (nend > nstart) {
		this->capacity = nend - nstart;
		this->natural = this->malloc(this->capacity);

		for (size_t idx = 0; idx < this->capacity; idx++) {
			size_t sidx = idx + nstart;

			if (nbytes[sidx] == 0) {
				this->natural[idx] = 0;
			} else {
				this->payload = this->capacity - idx;
				memcpy(this->natural + idx, nbytes + sidx, nend - sidx);
				break;
			}
		}
	}
}

void Natural::from_memory(const uint16 nchars[], size_t nstart, size_t nend) {
	if (nend > nstart) {
		this->capacity = (nend - nstart) * 2;
		this->natural = this->malloc(this->capacity);
		size_t idx = 0U;

		for (size_t sidx = nstart; sidx < nend; sidx++) {
			uint16 ch = (uint16)(nchars[sidx]);

			this->natural[idx++] = (uint8)(ch >> 8);
			this->natural[idx++] = (uint8)(ch & 0xFFU);

			if (this->payload == 0) {
				if (ch > 0xFFU) {
					this->payload = this->capacity - (idx - 2);
				} else if (ch > 0) {
					this->payload = this->capacity - (idx - 1);
				}
			}
		}
	}
}

void Natural::from_base16(const uint8 nbytes[], size_t nstart, size_t nend) {
	if (nend > nstart) {
		size_t span = nend - nstart;
		
		this->capacity = span / 2 + (span % 2);
		this->natural = this->malloc(this->capacity);
		this->payload = natural_from_base16(this->natural, nbytes, nstart, nend, this->capacity);
	}
}

void Natural::from_base16(const uint16 nchars[], size_t nstart, size_t nend) {
	if (nend > nstart) {
		size_t span = nend - nstart;
		
		this->capacity = span / 2 + (span % 2);
		this->natural = this->malloc(this->capacity);
		this->payload = natural_from_base16(this->natural, nchars, nstart, nend, this->capacity);
	}
}

void Natural::from_base10(const uint8 nbytes[], size_t nstart, size_t nend) {
	if (nend > nstart) {
		this->capacity = nend - nstart;
		this->natural = this->malloc(this->capacity);
		this->payload = natural_from_base(10U, this->natural, nbytes, int(nstart), int(nend), this->capacity);
	}
}

void Natural::from_base10(const uint16 nchars[], size_t nstart, size_t nend) {
	if (nend > nstart) {
		this->capacity = nend - nstart;
		this->natural = this->malloc(this->capacity);
		this->payload = natural_from_base(10U, this->natural, nchars, int(nstart), int(nend), this->capacity);
	}
}

void Natural::from_base8(const uint8 nbytes[], size_t nstart, size_t nend) {
	if (nend > nstart) {
		size_t span = nend - nstart;
		
		this->capacity = (span / 3 + ((span % 3 == 0) ? 0 : 1)) * 2;
		this->natural = this->malloc(this->capacity);
		this->payload = natural_from_base(8U, this->natural, nbytes, int(nstart), int(nend), this->capacity);
	}
}

void Natural::from_base8(const uint16 nchars[], size_t nstart, size_t nend) {
	if (nend > nstart) {
		size_t span = nend - nstart;
		
		this->capacity = (span / 3 + ((span % 3 == 0) ? 0 : 1)) * 2;
		this->natural = this->malloc(this->capacity);
		this->payload = natural_from_base(8U, this->natural, nchars, int(nstart), int(nend), this->capacity);
	}
}

/*************************************************************************************************/
void Natural::add_digit(uint8 digit) {
	size_t idx = this->capacity - 1U;
	
	if (this->payload == 0) {
		this->natural[idx] = digit;
		this->payload++;
	} else {
		while (digit > 0U) {
			uint16 sum = this->natural[idx] + digit;

			if (sum <= 0xFF) {
				this->natural[idx] = (uint8)sum;
				digit = 0U;
			} else {
				size_t payload_idx = this->capacity - this->payload;

				this->natural[idx] = (uint8)(sum & 0xFFU);
				digit = sum >> 8U;

				if (idx > payload_idx) {
					idx--;
					continue;
				}
				
				if (idx == 0U) {
					this->expand(1U);
					this->natural[0] = digit;
				} else if (idx == payload_idx) {
					this->natural[idx - 1] = digit;
				}

				this->payload++;
				digit = 0U;
			}
		}
	}
}

void Natural::times_digit(uint8 rhs) {
	if (rhs > 1ULL) {
		uint16 carry = 0U;

		for (size_t idx = 1U; idx <= this->payload; idx++) {
			uint16 digit = this->natural[this->capacity - idx] * rhs + carry;

			if (digit <= 0xFFU) {
				this->natural[this->capacity - idx] = (uint8)digit;
				carry = 0U;
			} else {
				this->natural[this->capacity - idx] = (uint8)(digit & 0xFFU);
				carry = digit >> 8U;
			}
		}

		if (carry > 0U) {
			if (this->capacity == this->payload) {
				this->expand(1U);
			} 
			
			this->payload++;
			this->natural[this->capacity - this->payload] = (uint8)carry;
		}
	} else if (rhs == 0U) {
		this->bzero();
	}
}

void Natural::divide_digit(uint8 divisor, Natural* oremainder) {
	if (divisor > 1ULL) {
		uint16 remainder = 0U;

		for (size_t idx = this->capacity - this->payload; idx < this->capacity; idx++) {
			uint16 dividend = (remainder << 8U) ^ this->natural[idx];

			if (dividend < divisor) {
				this->natural[idx] = 0U;
				remainder = dividend;
			} else {
				this->natural[idx] = (uint8)(dividend / divisor);
				remainder = dividend % divisor;
			}
		}

		this->skip_leading_zeros(this->payload);

		if (oremainder != nullptr) {
			(*oremainder) = remainder;
		}
	} else if (divisor == 1ULL) {
		if (oremainder != nullptr) {
			oremainder->bzero();
		}
	}
}

int Natural::compare_to_one() const {
	int cmp = int(this->payload) - 1;

	if (cmp == 0) {
		cmp = this->natural[this->capacity - 1U] - 1;
	}

	return cmp;
}

/*************************************************************************************************/
void Natural::bzero() {
	this->payload = 0U;

	// Method should not assume zeroed memory.
	//if (this->natural != nullptr) {
	//	memset(this->natural, '\0', this->capacity);
	//}
}

void Natural::skip_leading_zeros(size_t new_payload) {
	uint8* cursor = this->natural + (this->capacity - new_payload);

	// WARNING: Invokers take responsibilities to ensure that `payload` is not out of index.

	this->payload = new_payload;
	while ((this->payload > 0U) && ((*cursor++) == 0U)) {
		this->payload--;
	}
}

void Natural::decrease_from_slot(size_t slot) {
	while (slot <= this->payload) {
		uint8 digit = this->natural[this->capacity - slot];

		if (digit > 0U) {
			this->natural[this->capacity - slot] = digit - 1U;

			if ((digit == 1) && (slot == this->payload)) {
				this->payload--;
			}

			break;
		} else {
			if (slot < this->payload) {
				this->natural[this->capacity - slot] = 0xFFU;
				slot++;
			} else {
				this->bzero();
				break;
			}
		}
	}
}

uint8* Natural::malloc(size_t size) {
	uint8* memory = new uint8[size];

	// NOTE: Method should not assume zeroed memory.

#ifdef _DEBUG
	memset(memory, int(size), size);
#endif

	return memory;
}

void Natural::recalloc(size_t newsize, size_t shift) {
	uint8* src = this->natural;
	size_t zero_size = (this->capacity - this->payload);

	this->capacity = newsize;
	this->natural = this->malloc(this->capacity);

	{ // do copying and shifting
		size_t payload_idx0 = this->capacity - this->payload /* this is not the `zero_size` */ - shift;

		memset(this->natural, '\0', this->capacity);
		memcpy(this->natural + payload_idx0, src + zero_size, this->payload);
	}

	delete[] src;
}

void Natural::smart_prealloc(size_t size) {
	if (size + this->payload > this->capacity) {
		this->expand(size + this->payload - this->capacity);
	}
}
