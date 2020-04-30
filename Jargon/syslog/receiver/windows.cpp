#include "syslog/receiver/windows.hpp"

#include "datum/string.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

using namespace Windows::Foundation;
using namespace Windows::Foundation::Diagnostics;

using namespace Windows::UI::Xaml;

/*************************************************************************************************/
ref class WarGrey::GYDM::WindowsReceiver::Session sealed {
public:
	virtual ~Session() {
		if (this->channel != nullptr) {
			delete this->channel;
		}

		if (this->session != nullptr) {
			delete this->session;
		}
	}

	Session(Platform::String^ name) {
		this->channel = ref new LoggingChannel(name, nullptr);
		this->session = ref new FileLoggingSession(this->channel->Name);

		this->session->AddLoggingChannel(this->channel, LoggingLevel::Verbose);
	}

internal:
	void on_log_message(Log level, Platform::String^ message, SyslogMetainfo* data, Platform::String^ topic) {
		LoggingLevel etl_level = LoggingLevel::Verbose;

		switch (level) {
		case Log::Warning: etl_level = LoggingLevel::Warning; break;
		case Log::Error: etl_level = LoggingLevel::Error; break;
		case Log::Info: case Log::Notice: etl_level = LoggingLevel::Information;
		case Log::Alarm: case Log::Panic: case Log::Critical: etl_level = LoggingLevel::Critical; break;
		}

		this->channel->LogMessage(message, etl_level);
		this->channel->LogValuePair("timestamp", int(data->timestamp));
	}

private:
	Windows::Foundation::Diagnostics::LoggingChannel^ channel;
	Windows::Foundation::Diagnostics::FileLoggingSession^ session;
};

/*************************************************************************************************/
WindowsReceiver::WindowsReceiver(Platform::String^ name, Log level, Platform::String^ topic) : ISyslogReceiver(level, topic) {
	this->session = ref new WindowsReceiver::Session(name);
}

WindowsReceiver::~WindowsReceiver() {
	delete this->session;
}

void WindowsReceiver::on_log_message(Log level, Platform::String^ message, SyslogMetainfo* data, Platform::String^ topic) {
	this->session->on_log_message(level, message, data, topic);
}
