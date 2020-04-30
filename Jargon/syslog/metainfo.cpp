#include "syslog/metainfo.hpp"

#include "asn/der.hpp"
#include "system.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

using namespace Windows::ApplicationModel;

/*************************************************************************************************/
SyslogMetainfo::SyslogMetainfo() {
	this->localname = system_ipv4_address();
	this->pname = Package::Current->DisplayName;
	this->pid = system_process_id();
}
