#pragma once

#include <string>
#include <cwchar>
#include <cstdarg>

#include <list>

namespace WarGrey::SCADA {
#define VSNWPRINT(pool, size, fmt) \
    static wchar_t pool[size]; \
    va_list argl; \
    va_start(argl, fmt); \
    vswprintf(pool, size, fmt, argl); \
    va_end(argl);

#define VSWPRINT(retval, fmt) \
Platform::String^ retval; { \
    const int DEFAULT_POOL_SIZE = 1024; \
    wchar_t wpool[DEFAULT_POOL_SIZE]; \
    int bigSize = DEFAULT_POOL_SIZE - 1; \
    wchar_t* pool; \
    va_list argl; \
    do { \
	    pool = (bigSize < DEFAULT_POOL_SIZE) ? wpool : (new wchar_t[bigSize + 1]); \
    	va_start(argl, fmt); \
    	int status = vswprintf(pool, bigSize + 1, fmt, argl); \
	    va_end(argl); \
    	if (status == -1) { \
	    	bigSize = bigSize * 2 + 1; \
		    if (pool != wpool) delete[] pool; \
		    pool = nullptr; \
	    } \
    } while (pool == nullptr); \
    retval = ref new Platform::String(pool); \
    if (pool != wpool) delete[] pool; \
}

#define VSNPRINT(retval, fmt) \
std::string retval; { \
    const int DEFAULT_POOL_SIZE = 1024; \
    char chpool[DEFAULT_POOL_SIZE]; \
    int bigSize = DEFAULT_POOL_SIZE - 1; \
    char* pool; \
    va_list argl; \
    do { \
	    pool = (bigSize < DEFAULT_POOL_SIZE) ? chpool : (new char[bigSize + 1]); \
    	va_start(argl, fmt); \
    	int status = vsnprintf(pool, bigSize + 1, fmt, argl); \
	    va_end(argl); \
    	if (status == -1) { \
	    	bigSize = bigSize * 2 + 1; \
		    if (pool != chpool) delete[] pool; \
		    pool = nullptr; \
	    } \
    } while (pool == nullptr); \
    retval = std::string(pool); \
    if (pool != chpool) delete[] pool; \
}

	/************************************************************************************************/
	Platform::String^ substring(Platform::String^ src, int start, int endplus1 = -1);
	Platform::String^ flstring(double flonum, int precision);
	Platform::String^ fxstring(long long fixnum, int width);
	Platform::String^ sstring(unsigned long long bytes, int precision);
	Platform::String^ gpstring(double position, char suffix, int precision = 3);

	Platform::String^ make_wstring(const wchar_t* fmt, ...);
	Platform::String^ make_wstring(const char* bytes);
	Platform::String^ make_wstring(std::string& bytes);
	Platform::String^ make_wstring(std::wstring& bytes);
	Platform::String^ make_wstring(char ch);
	size_t wstrlen(const wchar_t* content);

	long long string_to_fixnum(Platform::String^ string);

	std::string make_nstring(const char* fmt, ...);
	std::string make_nstring(const wchar_t* wbytes);
	std::string make_nstring(Platform::String^ wstr);

	Platform::String^ string_first_line(Platform::String^ src);
	std::list<Platform::String^> string_lines(Platform::String^ src, bool skip_empty_line = false);

	/************************************************************************************************/
	std::basic_string<unsigned char> binumber(unsigned long long n, size_t bitsize = 0);
	std::basic_string<unsigned char> hexnumber(unsigned long long n, size_t bytecount = 0);

	unsigned long long scan_natural(const unsigned char* src, size_t* pos, size_t end, bool skip_trailing_space = true);
	long long scan_integer(const unsigned char* src, size_t* pos, size_t end, bool skip_trailing_space = true);
	double scan_flonum(const unsigned char* src, size_t* pos, size_t end, bool skip_trailing_space = true);
	void scan_bytes(const unsigned char* src, size_t* pos, size_t end, unsigned char* bs, size_t bs_start, size_t bs_end, bool terminating = true);
	
	size_t scan_skip_space(const unsigned char* src, size_t* pos, size_t end);
	size_t scan_skip_newline(const unsigned char* src, size_t* pos, size_t end);
	size_t scan_skip_this_line(const unsigned char* src, size_t* pos, size_t end);

	template<typename B, size_t N>
	void scan_bytes(const unsigned char* src, size_t* pos, size_t end, B (&bs)[N], size_t start = 0U, bool terminating = true) {
		scan_bytes(src, pos, end, (unsigned char*)bs, 0, N, terminating);
	}
}