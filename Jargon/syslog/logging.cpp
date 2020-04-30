#include "logging.hpp"

#include "datum/string.hpp"
#include "datum/time.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

/*************************************************************************************************/
void ISyslogReceiver::log_message(Log level, Platform::String^ message, SyslogMetainfo* data, Platform::String^ topic) {
	if (level >= this->level) {
		if ((this->topic == nullptr) || (this->topic->Equals(topic))) {
			this->on_log_message(level, message, data, topic);
		}
	}
}

/*************************************************************************************************/
Syslog::Syslog(Log level, Platform::String^ topic, Syslog* parent) : level(level), topic(topic) {
	if (parent != nullptr) {
		this->parent = parent;
		this->parent->reference();
	}

	this->metadata = new SyslogMetainfo();
	this->metadata->reference();
}

Syslog::~Syslog() {
	while (!this->receivers.empty()) {
		this->receivers.front()->destroy();
		this->receivers.pop_front();
	}

	if (parent != nullptr) {
		this->parent->destroy();
	}

	this->metadata->destroy();
}

Platform::String^ Syslog::get_name() {
	return this->topic;
}

void Syslog::push_log_receiver(ISyslogReceiver* receiver) {
	if (receiver != nullptr) {
		receiver->reference();
		this->receivers.push_back(receiver);
	}
}

void Syslog::log_message(WarGrey::GYDM::Log level, Platform::String^ message) {
	if (level >= this->level) {
		this->do_log_message(level, message, this->topic, true);
	}
}

void Syslog::log_message(Platform::String^ alt_topic, Log level, Platform::String^ message) {
	if (level >= this->level) {
		this->do_log_message(level, message, alt_topic, true);
	}
}

void Syslog::log_message(Log level, const wchar_t* msgfmt, ...) {
	if (level >= this->level) {
		VSWPRINT(message, msgfmt);
		this->do_log_message(level, message, this->topic, true);
	}
}

void Syslog::log_message(Platform::String^ alt_topic, Log level, const wchar_t* msgfmt, ...) {
	if (level >= this->level) {
		VSWPRINT(message, msgfmt);
		this->do_log_message(level, message, alt_topic, true);
	}
}

void Syslog::do_log_message(Log level, Platform::String^ message, Platform::String^ topic, bool prefix) {
	auto actual_topic = ((topic == nullptr) ? this->topic : topic);
	auto actual_message = (((!prefix) || (actual_topic == nullptr)) ? message : (actual_topic + ": " + message));
	Syslog* logger = this;

	this->metadata->timestamp = current_milliseconds();

	while (logger != nullptr) {
		for (auto r : logger->receivers) {
			r->log_message(level, actual_message, this->metadata, actual_topic);
		}

		// TODO: do we need propagated level?
		logger = logger->parent;
	}
}
