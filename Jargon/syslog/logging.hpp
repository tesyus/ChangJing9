#pragma once

#include <deque>

#include "datum/object.hpp"
#include "syslog/metainfo.hpp"

namespace WarGrey::GYDM {
	private enum class Log : unsigned int { Debug, Info, Notice, Warning, Error, Critical, Alarm, Panic, _ };

	private class ISyslogReceiver abstract : public WarGrey::SCADA::SharedObject {
	public:
		ISyslogReceiver(WarGrey::GYDM::Log level, Platform::String^ topic = "")
			: level(level), topic(topic) {};

	public:
		void log_message(WarGrey::GYDM::Log level, Platform::String^ message,
			WarGrey::GYDM::SyslogMetainfo* data, Platform::String^ topic);

	protected:
		virtual void on_log_message(
			WarGrey::GYDM::Log level,
			Platform::String^ message,
			WarGrey::GYDM::SyslogMetainfo* data,
			Platform::String^ topic) = 0;

	protected:
		~ISyslogReceiver() noexcept {}

	private:
		WarGrey::GYDM::Log level;
		Platform::String^ topic;
	};

	private class Syslog final : public WarGrey::SCADA::SharedObject {
	public:
		Syslog(WarGrey::GYDM::Log level, Platform::String^ topic = "", WarGrey::GYDM::Syslog* parent = nullptr);

	public:
		Platform::String^ get_name();

	public:
		void push_log_receiver(WarGrey::GYDM::ISyslogReceiver* receiver);
		
		void log_message(WarGrey::GYDM::Log level, Platform::String^ message);
		void log_message(WarGrey::GYDM::Log level, const wchar_t* msgfmt, ...);
		
		void log_message(Platform::String^ alt_topic, WarGrey::GYDM::Log level, Platform::String^ message);
		void log_message(Platform::String^ alt_topic, WarGrey::GYDM::Log level, const wchar_t* msgfmt, ...);

	protected:
		~Syslog() noexcept;

	private:
		void do_log_message(WarGrey::GYDM::Log level, Platform::String^ message, Platform::String^ alt_topic, bool prefix);

	private:
		WarGrey::GYDM::Log level;
		WarGrey::GYDM::SyslogMetainfo* metadata;
		Platform::String^ topic;
		Syslog* parent;

	private:
		std::deque<WarGrey::GYDM::ISyslogReceiver*> receivers;
	};
}
