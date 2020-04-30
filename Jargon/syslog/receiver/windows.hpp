#pragma once

#include "syslog/logging.hpp"
#include "syslog/metainfo.hpp"

namespace WarGrey::GYDM {
	private class WindowsReceiver : public WarGrey::GYDM::ISyslogReceiver {
	public:
		WindowsReceiver(Platform::String^ name, WarGrey::GYDM::Log level = WarGrey::GYDM::Log::Debug, Platform::String^ topic = "");

	protected:
		void on_log_message(WarGrey::GYDM::Log level, Platform::String^ message,
			WarGrey::GYDM::SyslogMetainfo* data, Platform::String^ topic) override;

	protected:
		~WindowsReceiver() noexcept;

	private:
		ref class Session;
		WarGrey::GYDM::WindowsReceiver::Session^ session;
	};
}
