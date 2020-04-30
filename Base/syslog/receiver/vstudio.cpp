#include <windows.h>

#include "syslog/receiver/vstudio.hpp"

using namespace WarGrey::SCADA;

void VisualStudioReceiver::on_log_message(Log level, Platform::String^ message, SyslogMetainfo& whocares, Platform::String^ topic) {
	auto actual_message = "[" + level.ToString() + "] " + message;

	OutputDebugString(actual_message->Data());
	OutputDebugString(L"\n");
}
