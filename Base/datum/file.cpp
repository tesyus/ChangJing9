#include <string>
#include <winerror.h>
#include <Windows.h>

#include "datum/file.hpp"
#include "datum/string.hpp"
#include "datum/char.hpp"

using namespace WarGrey::SCADA;

static inline bool char_whitespace(char ch) {
	return (ch == space);
}

template<typename C>
static void read_basic_text(std::basic_string<C>& str, std::filebuf& src, bool (*end_of_text)(char)) {
	char ch;

	discard_space(src);

	while ((ch = src.sbumpc()) != EOF) {
		if (end_of_text(ch)) {
			src.sungetc();
			break;
		}

		str.push_back((C)ch);
	}
}

/*************************************************************************************************/
bool WarGrey::SCADA::open_input_binary(std::filebuf& src, Platform::String^ in_port) {
	src.open(in_port->Data(), std::ios::in | std::ios::binary);

	return src.is_open();
}

bool WarGrey::SCADA::open_output_binary(std::wofstream& src, Platform::String^ out_port, unsigned int flprecision) {
	src.open(out_port->Data(), std::ios::out | std::ios::binary);

	src.setf(std::ios_base::fixed | std::ios_base::showpoint);

	if (flprecision > 0) {
		src.precision(flprecision);
	}

	return src.is_open();
}

/************************************************************************************************/
bool WarGrey::SCADA::char_end_of_word(char ch) {
	return (char_whitespace(ch) || char_end_of_line(ch));
}

bool WarGrey::SCADA::char_end_of_line(char ch) {
	return (ch == linefeed) || (ch == carriage_return);
}

bool WarGrey::SCADA::char_end_of_field(char ch) {
	return ((ch == comma) || char_end_of_line(ch));
}

/************************************************************************************************/
char WarGrey::SCADA::peek_char(std::filebuf& src) {
	return src.sgetc();
}

char WarGrey::SCADA::read_char(std::filebuf& src) {
	discard_space(src);

	return src.sbumpc();
}

size_t WarGrey::SCADA::read_bytes(std::filebuf& src, char* bs, size_t start, size_t end, bool terminating) {
	size_t idx = start;
	char ch;

	if (terminating) {
		end--;
	}

	while ((idx < end) && ((ch = src.sbumpc()) != EOF)) {
		bs[idx++] = ch;
	}

	if (terminating) {
		bs[idx - start] = '\0';
	}

	return (idx - start);
}

bool WarGrey::SCADA::read_bool(std::filebuf& src) {
	long long n = read_integer(src);

	/** WARNING
	 * Boolean datum in project's files are padded with leading 0s
	 * Stupid design.
	 */

	return (n > 0);
}

std::basic_string<unsigned char> WarGrey::SCADA::read_bytes(std::filebuf& src, bool (*end_of_text)(char)) {
	std::basic_string<unsigned char> str;
	
	read_basic_text(str, src, end_of_text);

	return str;
}

std::string WarGrey::SCADA::read_text(std::filebuf& src, bool (*end_of_text)(char)) {
	std::string str;
	
	read_basic_text(str, src, end_of_text);

	return str;
}

Platform::String^ WarGrey::SCADA::read_wtext(std::filebuf& src, bool (*end_of_text)(char)) {
	return make_wstring(read_text(src, end_of_text));
}

Platform::String^ WarGrey::SCADA::read_wgb18030(std::filebuf& src, bool (*end_of_text)(char)) {
	Platform::String^ wstr = nullptr;
	std::string str;
	bool gb18030 = false;
	char ch;

	discard_space(src);

	while ((ch = src.sbumpc()) != EOF) {
		if (end_of_text(ch)) {
			src.sungetc();
			break;
		}

		if ((!gb18030) && (!((0 <= ch) && (ch < 0x80)))) { // ch is actually unsigned int
			gb18030 = true;
		}

		str.push_back(ch);
	}

	if (gb18030) { // https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
		wchar_t wpool[1024]; // DIG only allows 50 chars.
		size_t msize = str.length();
		int wsize = MultiByteToWideChar(54936, 0, str.c_str(), int(msize), wpool, int(sizeof(wpool) / sizeof(wchar_t)));

		if (wsize > 0) {
			wstr = ref new Platform::String(wpool, (unsigned int)wsize);
		} else {
			switch (GetLastError()) {
			case ERROR_INSUFFICIENT_BUFFER: wstr = "insufficient buffer"; break;
			case ERROR_INVALID_FLAGS: wstr = "invalid flags"; break;
			case ERROR_INVALID_PARAMETER: wstr = "invalid parameters"; break;
			case ERROR_NO_UNICODE_TRANSLATION: wstr = "no unicode translation"; break;
			default: wstr = "unknown error occured"; break;
			}
		}
	} else {
		wstr = make_wstring(str);
	}

	return wstr;
}

unsigned long long WarGrey::SCADA::read_natural(std::filebuf& src) {
	unsigned long long n = 0;
	char ch;

	discard_space(src);

	while ((ch = src.sbumpc()) != EOF) {
		if ((ch < zero) || (ch > nine)) {
			src.sungetc();
			break;
		}

		n = n * 10 + (ch - zero);
	}

	return n;
}

long long WarGrey::SCADA::read_integer(std::filebuf& src) {
	long long n = 0;
	long long sign = 1;
	char ch;

	discard_space(src);
	
	ch = src.sgetc();
	if (ch == minus) {
		sign = -1;
		src.snextc();
	} else if (ch == plus) {
		src.snextc();
	}

	while ((ch = src.sbumpc()) != EOF) {
		if ((ch < zero) || (ch > nine)) {
			src.sungetc();
			break;
		}

		n = n * 10 + (ch - zero);
	}

	return n * sign;
}

double WarGrey::SCADA::read_flonum(std::filebuf& src) {
	double flonum = flnan;
	double i_acc = 10.0;
	double f_acc = 1.0;
	double sign = 1.0;
	long long int e = 1;
	char ch;

	discard_space(src);

	ch = src.sgetc();
	if (ch == minus) {
		sign = -1.0;
		src.snextc();
	} else if (ch == plus) {
		src.snextc();
	}

	while ((ch = src.sbumpc()) != EOF) {
		if ((ch < zero) || (ch > nine)) {
			if (ch == dot) {
				if (f_acc == 1.0) {
					i_acc = 1.0;
					f_acc = 0.1;
					continue;
				} else {
					src.sungetc();
					break;
				}
			} else if ((ch == exponent) || (ch == Exponent)) {
				e = read_integer(src);
				break;
			} else {
				src.sungetc();
				break;
			}
		}

		if (std::isnan(flonum)) {
			flonum = 0.0;
		}

		flonum = flonum * i_acc + double(ch - zero) * f_acc;

		if (f_acc != 1.0) {
			f_acc *= 0.1;
		}
	}

	return flonum * sign * ((e == 1) ? 1.0 : flexpt(10.0, double(e)));
}

float WarGrey::SCADA::read_single_flonum(std::filebuf& src) {
	return float(read_flonum(src));
}

void WarGrey::SCADA::discard_space(std::filebuf& src) {
	char ch;

	while ((ch = src.sbumpc()) != EOF) {
		if (!char_whitespace(ch)) {
			src.sungetc();
			break;
		}
	}
}

void WarGrey::SCADA::discard_newline(std::filebuf& src) {
	char ch;
	
	while ((ch = src.sbumpc()) != EOF) {
		if (!char_end_of_line(ch)) {
			src.sungetc();
			break;
		}
	}
}

void WarGrey::SCADA::discard_this_line(std::filebuf& src) {
	char ch;

	while ((ch = src.sbumpc()) != EOF) {
		if (char_end_of_line(ch)) {
			discard_newline(src);
			break;
		}
	}
}

/*************************************************************************************************/
std::wostream& WarGrey::SCADA::write_bool(std::wostream& stream, bool b) {
	return stream << (b ? 1 : 0);
}

std::wostream& WarGrey::SCADA::write_wtext(std::wostream& stream, Platform::String^ text) {
	return stream.write(text->Data(), text->Length());
}

std::wostream& WarGrey::SCADA::write_position(std::wostream& stream, double2 position, Platform::String^ sep) {
	stream.write(sep->Data(), sep->Length()) << position.x;
	
	return stream.write(sep->Data(), sep->Length()) << position.y;
}

std::wostream& WarGrey::SCADA::write_newline(std::wostream& stream) {
	// NOTE: `std::endl` does not put newline for `std::ios::binary`

	return stream << "\n\r" << std::endl;
}
