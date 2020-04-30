#include "taskexn.hpp"

#include "datum/string.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

/*************************************************************************************************/
void WarGrey::SCADA::task_fatal() {
	throw task_terminated();
}

void WarGrey::SCADA::task_fatal(Syslog* logger, Platform::String^ message) {
	logger->log_message(Log::Critical, message);
	task_fatal();
}

void WarGrey::SCADA::task_fatal(Syslog* logger, const wchar_t *fmt, ...) {
	VSNWPRINT(pool, 2048, fmt);
	task_fatal(logger, ref new Platform::String(pool));
}

/*************************************************************************************************/
void WarGrey::SCADA::task_discard() {
	throw task_discarded();
}

void WarGrey::SCADA::task_discard(Syslog* logger, Platform::String^ message) {
	logger->log_message(Log::Error, message);
	task_discard();
}

void WarGrey::SCADA::task_discard(Syslog* logger, const wchar_t *fmt, ...) {
	VSNWPRINT(pool, 2048, fmt);
	task_discard(logger, ref new Platform::String(pool));
}
