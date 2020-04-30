#pragma once

#include <cmath>

namespace WarGrey::SCADA {
#define fxin(open, v, close) ((open <= v) && (v <= close))
#define fxout(open, v, close) ((v < open) || (v > close))

	int inline fxabs(int fx) { return std::abs(fx); }
	long long inline fxabs(long long fx) { return std::abs(fx); }

	char inline fxmax(char fx1, char fx2) { return ((fx1 < fx2) ? fx2 : fx1); }
	unsigned char inline fxmax(unsigned char fx1, unsigned char fx2) { return ((fx1 < fx2) ? fx2 : fx1); }
	unsigned short inline fxmax(unsigned short fx1, unsigned short fx2) { return ((fx1 < fx2) ? fx2 : fx1); }
	int inline fxmax(int fx1, int fx2) { return ((fx1 < fx2) ? fx2 : fx1); }
	unsigned int inline fxmax(unsigned int fx1, unsigned int fx2) { return ((fx1 < fx2) ? fx2 : fx1); }
	long long inline fxmax(long long fx1, long long fx2) { return ((fx1 < fx2) ? fx2 : fx1); }
	unsigned long long inline fxmax(unsigned long long fx1, unsigned long long fx2) { return ((fx1 < fx2) ? fx2 : fx1); }
	
	char inline fxmin(char fx1, char fx2) { return ((fx1 < fx2) ? fx1 : fx2); }
	unsigned char inline fxmin(unsigned char fx1, unsigned char fx2) { return ((fx1 < fx2) ? fx1 : fx2); }
	unsigned short inline fxmin(unsigned short fx1, unsigned short fx2) { return ((fx1 < fx2) ? fx1 : fx2); }
	int inline fxmin(int fx1, int fx2) { return ((fx1 < fx2) ? fx1 : fx2); }
	unsigned int inline fxmin(unsigned int fx1, unsigned int fx2) { return ((fx1 < fx2) ? fx1 : fx2); }
	long long inline fxmin(long long fx1, long long fx2) { return ((fx1 < fx2) ? fx1 : fx2); }
	unsigned long long inline fxmin(unsigned long long fx1, unsigned long long fx2) { return ((fx1 < fx2) ? fx1 : fx2); }
	
	long long inline fxround(long long fx, float precision) { return (long long)(std::roundf(float(fx) * precision)); }
	long long inline fxround(long long fx, double precision) { return (long long)(std::round(double(fx) * precision)); }

	long long inline fxfloor(long long fx, float precision) { return (long long)(std::floorf(float(fx) * precision)); }
	long long inline fxfloor(long long fx, double precision) { return (long long)(std::floor(double(fx) * precision)); }

	long long inline fxceiling(long long fx, float precision) { return (long long)(std::ceilf(float(fx) * precision)); }
	long long inline fxceiling(long long fx, double precision) { return (long long)(std::ceil(double(fx) * precision)); }

	static inline size_t integer_length(unsigned long long n) { size_t s = 0;  while (n) { n >>= 1; s++; } return s; }
}
