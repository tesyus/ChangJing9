#pragma once

#include "syslog/logging.hpp"
#include "syslog/metainfo.hpp"

namespace WarGrey::GYDM {
	private class VisualStudioReceiver : public WarGrey::GYDM::ISyslogReceiver {
	public:
		VisualStudioReceiver(WarGrey::GYDM::Log level = WarGrey::GYDM::Log::Debug, Platform::String^ topic = "")
			: ISyslogReceiver(level, topic) {};

	protected:
		void on_log_message(WarGrey::GYDM::Log level, Platform::String^ message,
			WarGrey::GYDM::SyslogMetainfo* data, Platform::String^ topic) override;

	protected:
		~VisualStudioReceiver() noexcept {}
	};
}
