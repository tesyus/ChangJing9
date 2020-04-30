#include "syslog.hpp"

#include "datum/string.hpp"

using namespace WarGrey::GYDM;

using namespace Windows::ApplicationModel;

/*************************************************************************************************/
static Platform::String^ default_rsyslog_group = nullptr;
static unsigned short default_rsyslog_port = 0U;

static Platform::String^ default_logging_topic = "WarGrey";
static Log default_logging_level = Log::Debug;

/*************************************************************************************************/
RsyslogReceiver* WarGrey::GYDM::default_rsyslog_logging_receiver() {
	static RsyslogReceiver* rsyslog = nullptr;

	if ((rsyslog == nullptr) && (default_rsyslog_group != nullptr)) {
		rsyslog = new RsyslogReceiver(default_rsyslog_group, default_rsyslog_port, Log::Debug);
		rsyslog->reference();
	}

	return rsyslog;
}

WindowsReceiver* WarGrey::GYDM::default_windows_logging_receiver() {
	static WindowsReceiver* etl = nullptr;

	if (etl == nullptr) {
		etl = new WindowsReceiver(Package::Current->DisplayName, Log::Debug);
		etl->reference();
	}

	return etl;
}

/*************************************************************************************************/
void WarGrey::GYDM::set_default_logging_level(Log level) {
	default_logging_level = level;
}

void WarGrey::GYDM::set_default_logging_topic(Platform::String^ topic) {
	default_logging_topic = topic;
}

void WarGrey::GYDM::set_default_rsyslog_target(Platform::String^ ipv4, unsigned short port) {
	default_rsyslog_group = ipv4;

	if (port == 0) {
		default_rsyslog_port = 514;
	} else {
		default_rsyslog_port = port;
	}
}

/*************************************************************************************************/
Syslog* WarGrey::GYDM::default_logger() {
	static Syslog* winlog;

	if (winlog == nullptr) {
		RsyslogReceiver* racket = default_rsyslog_logging_receiver();
		WindowsReceiver* win_etl = default_windows_logging_receiver();

		winlog = make_logger(default_logging_level, default_logging_topic, nullptr);
		winlog->push_log_receiver(new VisualStudioReceiver());

		if (racket != nullptr) {
			winlog->push_log_receiver(racket);
		}

		if (win_etl != nullptr) {
			winlog->push_log_receiver(win_etl);
		}

		winlog->reference();
	}

	return winlog;
}

void WarGrey::GYDM::syslog(Log level, Platform::String^ message) {
	auto self = default_logger();

	self->log_message(level, message);
}

void WarGrey::GYDM::syslog(Log level, const wchar_t *fmt, ...) {
	VSWPRINT(message, fmt);

	syslog(level, message);
}

/*************************************************************************************************/
#define implement_syslog(fname, level) \
void syslog_##fname(const wchar_t *fmt, ...) { VSWPRINT(message, fmt); syslog(level, message); } \
void syslog_##fname(Platform::String^ message) { syslog(level, message); }

implement_syslog(debug,    Log::Debug)
implement_syslog(info,     Log::Info)
implement_syslog(notice,   Log::Notice)
implement_syslog(warning,  Log::Warning)
implement_syslog(error,    Log::Error)
implement_syslog(critical, Log::Critical)
implement_syslog(alert,    Log::Alarm)
implement_syslog(panic,    Log::Panic)

#undef implement_syslog

/*************************************************************************************************/
Syslog* WarGrey::GYDM::make_logger(Log level, Platform::String^ topic, Syslog* parent) {
	return new Syslog(level, topic, parent);
}

Syslog* WarGrey::GYDM::make_silent_logger(Platform::String^ topic) {
	return make_logger(Log::_, topic);
}

Syslog* WarGrey::GYDM::make_system_logger(Log level, Platform::String^ topic) {
	return make_logger(level, topic, default_logger());
}

Syslog* WarGrey::GYDM::make_system_logger(Platform::String^ topic) {
	return make_logger(default_logging_level, topic, default_logger());
}
