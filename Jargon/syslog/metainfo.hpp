#pragma once

#include "datum/object.hpp"

namespace WarGrey::GYDM {
	private class SyslogMetainfo : public WarGrey::SCADA::SharedObject {
	public:
		SyslogMetainfo();

	public:
		Platform::String^ pname;
		unsigned int pid;

	public:
		Platform::String^ localname;
		long long timestamp;
	};
}
