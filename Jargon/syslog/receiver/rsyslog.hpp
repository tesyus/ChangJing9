#pragma once

#include <deque>
#include <mutex>

#include "syslog/logging.hpp"
#include "syslog/metainfo.hpp"

namespace WarGrey::GYDM {
	private class RsyslogReceiver : public WarGrey::GYDM::ISyslogReceiver {
	public:
		RsyslogReceiver(Platform::String^ multicast_group, unsigned short service,
			WarGrey::GYDM::Log level = WarGrey::GYDM::Log::Debug,
			Platform::String^ topic = "");

	protected:
		void on_log_message(WarGrey::GYDM::Log level, Platform::String^ message,
			WarGrey::GYDM::SyslogMetainfo* data, Platform::String^ topic) override;

	protected:
		~RsyslogReceiver() noexcept;

	private:
		void send_drift_messages();

	private:
		std::mutex section;
		std::deque<WarGrey::GYDM::Log> levels;
		std::deque<Platform::String^> messages;
		std::deque<WarGrey::GYDM::SyslogMetainfo> infoes;
		std::deque<Platform::String^> topics;

	private:
		Windows::Networking::Sockets::DatagramSocket^ socket;
		Windows::Storage::Streams::DataWriter^ udpout;
	};
}
