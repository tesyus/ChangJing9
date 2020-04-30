#pragma once

#include "datum/bytes.hpp"

namespace WarGrey::GYDM {
	private enum class Fixnum { Uint16, Uint32, Uint64 };

	private class Natural {
	public:
		~Natural() noexcept;
		
		Natural();
		Natural(unsigned long long n);

		Natural(WarGrey::SCADA::bytes& nstr, size_t nstart = 0, size_t nend = 0);
		Natural(std::string& nstr, size_t nstart = 0, size_t nend = 0);
		Natural(std::wstring& nstr, size_t nstart = 0, size_t nend = 0);
		Natural(Platform::String^ nstr, size_t nstart = 0, size_t nend = 0);
		Natural(uint8 base, WarGrey::SCADA::bytes& nstr, size_t nstart = 0, size_t nend = 0);
		Natural(uint8 base, std::string& nstr, size_t nstart = 0, size_t nend = 0);
		Natural(uint8 base, std::wstring& nstr, size_t nstart = 0, size_t nend = 0);
		Natural(uint8 base, Platform::String^ nstr, size_t nstart = 0, size_t nend = 0);

		template<typename BYTE, size_t N>
		Natural(const BYTE(&ns)[N], size_t nstart = 0, size_t nend = N)
			: Natural((const BYTE*)ns, nstart, nend) {}

		template<typename BYTE, size_t N>
		Natural(uint8 base, const BYTE(&ns)[N], size_t nstart = 0, size_t nend = N)
			: Natural(base, (const BYTE*)ns, nstart, nend) {}

		template<typename BYTE>
		Natural(const BYTE ns[], size_t nstart, size_t nend)
			: Natural((uint8)0U, ns, nstart, nend) {}

		template<typename BYTE>
		Natural(uint8 base, const BYTE ns[], size_t nstart, size_t nend) : natural(nullptr), capacity(0U), payload(0U) {
			switch (base) {
			case 16: this->from_base16(ns, nstart, nend); break;
			case 10: this->from_base10(ns, nstart, nend); break;
			case 8:  this->from_base8(ns, nstart, nend); break;
			default: this->from_memory(ns, nstart, nend);
			}
		}

	public:
		Natural(const WarGrey::GYDM::Natural& n);
		Natural(WarGrey::GYDM::Natural&& n);

		WarGrey::GYDM::Natural& operator=(unsigned long long n);
		WarGrey::GYDM::Natural& operator=(const WarGrey::GYDM::Natural& n);
		WarGrey::GYDM::Natural& operator=(WarGrey::GYDM::Natural&& n);

	public: // NOTE: C++20 has operator<=>
		int compare(unsigned long long rhs) const;
		int compare(const WarGrey::GYDM::Natural& rhs) const;

		friend inline bool operator<(const WarGrey::GYDM::Natural& lhs, const WarGrey::GYDM::Natural& rhs) { return (lhs.compare(rhs) < 0); }
		friend inline bool operator<(const WarGrey::GYDM::Natural& lhs, unsigned long long rhs) { return (lhs.compare(rhs) < 0); }
		friend inline bool operator<(unsigned long long lhs, const WarGrey::GYDM::Natural& rhs) { return (rhs.compare(lhs) > 0); }
		friend inline bool operator==(const WarGrey::GYDM::Natural& lhs, const WarGrey::GYDM::Natural& rhs) { return (lhs.compare(rhs) == 0); }
		friend inline bool operator==(const WarGrey::GYDM::Natural& lhs, unsigned long long rhs) { return (lhs.compare(rhs) == 0); }
		friend inline bool operator==(unsigned long long lhs, const WarGrey::GYDM::Natural& rhs) { return (rhs.compare(lhs) == 0); }

		friend inline bool operator>(const WarGrey::GYDM::Natural& lhs, const WarGrey::GYDM::Natural& rhs) { return (lhs.compare(rhs) > 0); }
		friend inline bool operator>(const WarGrey::GYDM::Natural& lhs, unsigned long long rhs) { return (lhs.compare(rhs) > 0); }
		friend inline bool operator>(unsigned long long lhs, const WarGrey::GYDM::Natural& rhs) { return (rhs.compare(lhs) < 0); }
		friend inline bool operator<=(const WarGrey::GYDM::Natural& lhs, const WarGrey::GYDM::Natural& rhs) { return (lhs.compare(rhs) <= 0); }
		friend inline bool operator<=(const WarGrey::GYDM::Natural& lhs, unsigned long long rhs) { return (lhs.compare(rhs) <= 0); }
		friend inline bool operator<=(unsigned long long lhs, const WarGrey::GYDM::Natural& rhs) { return (rhs.compare(lhs) >= 0); }
		friend inline bool operator>=(const WarGrey::GYDM::Natural& lhs, const WarGrey::GYDM::Natural& rhs) { return (lhs.compare(rhs) >= 0); }
		friend inline bool operator>=(const WarGrey::GYDM::Natural& lhs, unsigned long long rhs) { return (lhs.compare(rhs) >= 0); }
		friend inline bool operator>=(unsigned long long lhs, const WarGrey::GYDM::Natural& rhs) { return (rhs.compare(lhs) <= 0); }
		friend inline bool operator!=(const WarGrey::GYDM::Natural& lhs, const WarGrey::GYDM::Natural& rhs) { return (lhs.compare(rhs) != 0); }
		friend inline bool operator!=(const WarGrey::GYDM::Natural& lhs, unsigned long long rhs) { return (lhs.compare(rhs) != 0); }
		friend inline bool operator!=(unsigned long long lhs, const WarGrey::GYDM::Natural& rhs) { return (rhs.compare(lhs) != 0); }

	public:
		WarGrey::GYDM::Natural& operator++();
		inline WarGrey::GYDM::Natural operator++(int postfix) { Natural snapshot(*this); this->operator++(); return snapshot; }

		WarGrey::GYDM::Natural& operator--();
		inline WarGrey::GYDM::Natural operator--(int postfix) { Natural snapshot(*this); this->operator--(); return snapshot; }

		WarGrey::GYDM::Natural& operator+=(unsigned long long rhs);
		WarGrey::GYDM::Natural& operator+=(const WarGrey::GYDM::Natural& rhs);

		WarGrey::GYDM::Natural& operator-=(unsigned long long rhs);
		WarGrey::GYDM::Natural& operator-=(const WarGrey::GYDM::Natural& rhs);
		
		WarGrey::GYDM::Natural& operator*=(unsigned long long rhs);
		WarGrey::GYDM::Natural& operator*=(const WarGrey::GYDM::Natural& rhs);

		inline WarGrey::GYDM::Natural& operator/=(unsigned long long rhs) { return this->quotient_remainder(rhs, nullptr); }
		inline WarGrey::GYDM::Natural& operator/=(const WarGrey::GYDM::Natural& rhs) { return this->quotient_remainder(rhs, nullptr); }
		inline WarGrey::GYDM::Natural& operator%=(unsigned long long rhs) { return this->quotient_remainder(rhs, this); };
		inline WarGrey::GYDM::Natural& operator%=(const WarGrey::GYDM::Natural& rhs) { return this->quotient_remainder(rhs, this); };

		friend inline WarGrey::GYDM::Natural operator+(WarGrey::GYDM::Natural lhs, unsigned long long rhs) { return lhs += rhs; }
		friend inline WarGrey::GYDM::Natural operator+(unsigned long long lhs, WarGrey::GYDM::Natural rhs) { return rhs += lhs; }
		friend inline WarGrey::GYDM::Natural operator+(WarGrey::GYDM::Natural lhs, const WarGrey::GYDM::Natural& rhs) { return lhs += rhs; }

		// NOTE: the compiler will cast the number into Natural when encountered `n - Natural`;
		friend inline WarGrey::GYDM::Natural operator-(WarGrey::GYDM::Natural lhs, unsigned long long rhs) { return lhs -= rhs; }
		friend inline WarGrey::GYDM::Natural operator-(WarGrey::GYDM::Natural lhs, const WarGrey::GYDM::Natural& rhs) { return lhs -= rhs; }

		friend inline WarGrey::GYDM::Natural operator*(WarGrey::GYDM::Natural lhs, unsigned long long rhs) { return lhs *= rhs; }
		friend inline WarGrey::GYDM::Natural operator*(unsigned long long lhs, WarGrey::GYDM::Natural rhs) { return rhs *= lhs; }
		friend inline WarGrey::GYDM::Natural operator*(WarGrey::GYDM::Natural lhs, const WarGrey::GYDM::Natural& rhs) { return lhs *= rhs; }

		// NOTE: the compiler will cast the number into Natural when encountered `n / Natural` or `n % Natural`
		friend inline WarGrey::GYDM::Natural operator/(WarGrey::GYDM::Natural lhs, unsigned long long rhs) { return lhs /= rhs; }
		friend inline WarGrey::GYDM::Natural operator/(WarGrey::GYDM::Natural lhs, const WarGrey::GYDM::Natural& rhs) { return lhs /= rhs; }
		friend inline WarGrey::GYDM::Natural operator%(WarGrey::GYDM::Natural lhs, unsigned long long rhs) { return lhs %= rhs; }
		friend inline WarGrey::GYDM::Natural operator%(WarGrey::GYDM::Natural lhs, const WarGrey::GYDM::Natural& rhs) { return lhs %= rhs; }

	public:
		WarGrey::GYDM::Natural& expt(unsigned long long e);
		WarGrey::GYDM::Natural& expt(const WarGrey::GYDM::Natural& e);

		WarGrey::GYDM::Natural& modular_expt(unsigned long long b, unsigned long long n);
		WarGrey::GYDM::Natural& modular_expt(unsigned long long b, const WarGrey::GYDM::Natural& n);
		WarGrey::GYDM::Natural& modular_expt(const WarGrey::GYDM::Natural& b, unsigned long long n);
		WarGrey::GYDM::Natural& modular_expt(const WarGrey::GYDM::Natural& b, const WarGrey::GYDM::Natural& n);

		WarGrey::GYDM::Natural& quotient_remainder(unsigned long long divisor, Natural* remainder = nullptr);
		WarGrey::GYDM::Natural& quotient_remainder(const WarGrey::GYDM::Natural& divisor, Natural* remainder = nullptr);

		friend inline WarGrey::GYDM::Natural expt(WarGrey::GYDM::Natural b, unsigned long long e) { return b.expt(e); }
		friend inline WarGrey::GYDM::Natural expt(unsigned long long b, WarGrey::GYDM::Natural e) { return Natural(b).expt(e); }
		friend inline WarGrey::GYDM::Natural expt(WarGrey::GYDM::Natural b, const WarGrey::GYDM::Natural& e) { return b.expt(e); }

		friend inline WarGrey::GYDM::Natural modular_expt(WarGrey::GYDM::Natural a, unsigned long long b, unsigned long long n) { return a.modular_expt(b, n); }
		friend inline WarGrey::GYDM::Natural modular_expt(WarGrey::GYDM::Natural a, unsigned long long b, const WarGrey::GYDM::Natural& n) { return a.modular_expt(b, n); }
		friend inline WarGrey::GYDM::Natural modular_expt(WarGrey::GYDM::Natural a, const WarGrey::GYDM::Natural& b, unsigned long long n) { return a.modular_expt(b, n); }
		friend inline WarGrey::GYDM::Natural modular_expt(WarGrey::GYDM::Natural a, const WarGrey::GYDM::Natural& b, const WarGrey::GYDM::Natural& n) { return a.modular_expt(b, n); }

	public:
		WarGrey::GYDM::Natural operator~();

		WarGrey::GYDM::Natural& operator<<=(unsigned long long rhs);
		WarGrey::GYDM::Natural& operator>>=(unsigned long long rhs);

		WarGrey::GYDM::Natural& operator&=(unsigned long long rhs);
		WarGrey::GYDM::Natural& operator&=(const WarGrey::GYDM::Natural& rhs);
		WarGrey::GYDM::Natural& operator|=(unsigned long long rhs);
		WarGrey::GYDM::Natural& operator|=(const WarGrey::GYDM::Natural& rhs);
		WarGrey::GYDM::Natural& operator^=(unsigned long long rhs);
		WarGrey::GYDM::Natural& operator^=(const WarGrey::GYDM::Natural& rhs);

		friend inline WarGrey::GYDM::Natural operator<<(WarGrey::GYDM::Natural lhs, unsigned long long rhs) { return lhs <<= rhs; }
		friend inline WarGrey::GYDM::Natural operator>>(WarGrey::GYDM::Natural lhs, unsigned long long rhs) { return lhs >>= rhs; }

		friend inline WarGrey::GYDM::Natural operator&(WarGrey::GYDM::Natural lhs, unsigned long long rhs) { return lhs &= rhs; }
		friend inline WarGrey::GYDM::Natural operator&(unsigned long long lhs, WarGrey::GYDM::Natural rhs) { return rhs &= lhs; }
		friend inline WarGrey::GYDM::Natural operator&(WarGrey::GYDM::Natural lhs, const WarGrey::GYDM::Natural& rhs) { return lhs &= rhs; }
		friend inline WarGrey::GYDM::Natural operator|(WarGrey::GYDM::Natural lhs, unsigned long long rhs) { return lhs |= rhs; }
		friend inline WarGrey::GYDM::Natural operator|(unsigned long long lhs, WarGrey::GYDM::Natural rhs) { return rhs |= lhs; }
		friend inline WarGrey::GYDM::Natural operator|(WarGrey::GYDM::Natural lhs, const WarGrey::GYDM::Natural& rhs) { return lhs |= rhs; }
		friend inline WarGrey::GYDM::Natural operator^(WarGrey::GYDM::Natural lhs, unsigned long long rhs) { return lhs ^= rhs; }
		friend inline WarGrey::GYDM::Natural operator^(unsigned long long lhs, WarGrey::GYDM::Natural rhs) { return rhs ^= lhs; }
		friend inline WarGrey::GYDM::Natural operator^(WarGrey::GYDM::Natural lhs, const WarGrey::GYDM::Natural& rhs) { return lhs ^= rhs; }

		bool is_bit_set(unsigned long long m);
		WarGrey::GYDM::Natural bit_field(unsigned long long start, unsigned long long end);
		unsigned long long bitfield(unsigned long long start, unsigned long long end);
		long long signed_bitfield(unsigned long long start, unsigned long long end);

	public:
		uint8& operator[] (int idx);
		uint16 fixnum16_ref(int idx, size_t offset = 0U) const;
		uint32 fixnum32_ref(int idx, size_t offset = 0U) const;
		uint64 fixnum64_ref(int idx, size_t offset = 0U) const;

	public:
		bool is_zero() const;
		bool is_one() const;
		bool is_fixnum() const;
		bool is_odd() const;
		bool is_even() const;

	public:
		size_t length() const;
		size_t integer_length(uint8 alignment = 0U) const;
		size_t fixnum_count(WarGrey::GYDM::Fixnum type = Fixnum::Uint64) const;

	public:
		size_t expand(size_t size);

	public:
		WarGrey::SCADA::bytes to_bytes() const;
		WarGrey::SCADA::bytes to_hexstring() const;
		WarGrey::SCADA::bytes to_binstring(uint8 alignment = 0U) const;

	private:
		Natural(void* null, long long capacity);
		void replaced_by_fixnum(unsigned long long n);
		void from_memory(const uint8 nbytes[], size_t nstart, size_t nend);
		void from_memory(const uint16 nchars[], size_t nstart, size_t nend);
		void from_base16(const uint8 nbytes[], size_t nstart, size_t nend);
		void from_base16(const uint16 nchars[], size_t nstart, size_t nend);
		void from_base10(const uint8 nbytes[], size_t nstart, size_t nend);
		void from_base10(const uint16 nchars[], size_t nstart, size_t nend);
		void from_base8(const uint8 nbytes[], size_t nstart, size_t nend);
		void from_base8(const uint16 nchars[], size_t nstart, size_t nend);

	private:
		void add_digit(uint8 digit);
		void times_digit(uint8 digit);
		void divide_digit(uint8 digit, WarGrey::GYDM::Natural* remainder);
		int compare_to_one() const;

	private:
		void on_moved();
		void bzero();
		void skip_leading_zeros(size_t new_payload);
		void decrease_from_slot(size_t slot);
		uint8* malloc(size_t size);
		void recalloc(size_t new_size, size_t shift = 0U);
		void smart_prealloc(size_t size);
		
	private:
		uint8* natural;
		size_t capacity;
		size_t payload;
	};
}
