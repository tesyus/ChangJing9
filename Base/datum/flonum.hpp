#pragma once

#include <cmath>
#include <limits>

namespace WarGrey::SCADA {
	private struct double2 {
	public:
		double2(double x = 0.0, double y = 0.0)
			: x(x), y(y) {}

	public:
		double x;
		double y;
	};

	private struct double3 {
	public:
		double3(double x = 0.0, double y = 0.0, double z = 0.0)
			: x(x), y(y), z(z) {}

	public:
		double x;
		double y;
		double z;
	};

	/************************************************************************************************/
#define flin(open, v, close) ((open <= v) && (v <= close))
#define flout(open, v, close) ((v < open) || (v > close))

	static const float flnan_f = std::nanf("");
	static const double flnan = std::nanf("");
	static const long double flnan_t = std::nanf("");

	static const float pi_f       = 3.1415927f;
	static const double pi        = 3.141592653589793;
	static const long double pi_t = 3.1415926535897932385L;

	static const float infinity_f        = std::numeric_limits<float>::infinity();
	static const double infinity         = std::numeric_limits<double>::infinity();
	static const long double infinity_t  = std::numeric_limits<long double>::infinity();
	
	bool inline flisnan(float f) { return std::isnan(f); }
	bool inline flisnan(double fl) { return std::isnan(fl); }
	bool inline flisnan(long double fl) { return std::isnan(fl); }

	bool inline flisfinite(float f) { return std::isfinite(f); }
	bool inline flisfinite(double fl) { return std::isfinite(fl); }
	bool inline flisfinite(long double fl) { return std::isfinite(fl); }

	float inline flsafe(float v, float fallback = 0.0F) { return (flisnan(v) ? fallback : v); }
	double inline flsafe(double v, double fallback = 0.0) { return (flisnan(v) ? fallback : v); }
	long double inline flsafe(long double v, long double fallback = 0.0L) { return (flisnan(v) ? fallback : v); }
	
	// for non-flonums
	template<typename T> T inline flsafe(T v, T fallback) { return v; }
	template<typename T> bool inline flisnan(T fl) { return false; }

	template<typename T> T inline flsafe(T v, T min, T max) {
		if (v < min) {
			v = min;
		} else if (v > max) {
			v = max;
		} else if (flisnan(v)) {
			v = max;
		}

		return v;
	}

	float inline flabs(float f) { return std::fabsf(f); }
	double inline flabs(double fl) { return std::abs(fl); }
	long double inline flabs(long double fl) { return std::abs(fl); }

	// WARNING: 0.0 is +0.0, hence, flsign(0.0) => 1.0
	float inline flsign(float f) { return std::copysignf(1.0f, f); }
	double inline flsign(double fl) { return std::copysign(1.0, fl); }
	long double inline flsign(long double fl) { return std::copysignl(1.0l, fl); }

	float inline fllog(float f) { return std::logf(f); }
	double inline fllog(double fl) { return std::log(fl); }
	long double inline fllog(long double fl) { return std::logl(fl); }

	float inline fllog(float f, float b) { return std::logf(f) / std::logf(b); }
	double inline fllog(double fl, double b) { return std::log(fl) / std::log(b); }
	long double inline fllog(long double fl, long double b) { return std::logl(fl) / std::logl(b); }

	float inline flsqr(float f) { return f * f; }
	double inline flsqr(double fl) { return fl * fl; }
	long double inline flsqr(long double fl) { return fl * fl; }

	float inline flsqrt(float f) { return std::sqrtf(f); }
	double inline flsqrt(double fl) { return std::sqrt(fl); }
	long double inline flsqrt(long double fl) { return std::sqrtl(fl); }

	float inline flexpt(float fb, float fe) { return std::powf(fb, fe); }
	double inline flexpt(double flb, double fle) { return std::pow(flb, fle); }
	long double inline flexpt(long double flb, long double fle) { return std::powl(flb, fle); }

	float inline flmin(float f1, float f2) { return std::fminf(f1, f2); }
	double inline flmin(double fl1, double fl2) { return std::fmin(fl1, fl2); }
	long double inline flmin(long double fl1, long double fl2) { return std::fminl(fl1, fl2); }

	float inline flmax(float f1, float f2) { return std::fmaxf(f1, f2); }
	double inline flmax(double fl1, double fl2) { return std::fmax(fl1, fl2); }
	long double inline flmax(long double fl1, long double fl2) { return std::fmaxl(fl1, fl2); }

	float inline flround(float f) { return std::roundf(f); }
	double inline flround(double fl) { return std::round(fl); }
	long double inline flround(long double fl) { return std::roundl(fl); }

	float inline flfloor(float f) { return std::floorf(f); }
	double inline flfloor(double fl) { return std::floor(fl); }
	long double inline flfloor(long double fl) { return std::floorl(fl); }

	float inline flceiling(float f) { return std::ceilf(f); }
	double inline flceiling(double fl) { return std::ceil(fl); }
	long double inline flceiling(long double fl) { return std::ceill(fl); }

	float inline flsin(float f) { return std::sinf(f); }
	double inline flsin(double fl) { return std::sin(fl); }
	long double inline flsin(long double fl) { return std::sinl(fl); }

	float inline flasin(float f) { return std::asinf(f); }
	double inline flasin(double fl) { return std::asin(fl); }
	long double inline flasin(long double fl) { return std::asinl(fl); }

	float inline flcos(float f) { return std::cosf(f); }
	double inline flcos(double fl) { return std::cos(fl); }
	long double inline flcos(long double fl) { return std::cosl(fl); }

	float inline flacos(float f) { return std::acosf(f); }
	double inline flacos(double fl) { return std::acos(fl); }
	long double inline flacos(long double fl) { return std::acosl(fl); }

	float inline fltan(float f) { return std::tanf(f); }
	double inline fltan(double fl) { return std::tan(fl); }
	long double inline fltan(long double fl) { return std::tanl(fl); }

	float inline flatan(float fy, float fx) { return std::atan2f(fy, fx); }
	double inline flatan(double fly, double flx) { return std::atan2(fly, flx); }
	long double inline flatan(long double fly, long double flx) { return std::atan2l(fly, flx); }
}
