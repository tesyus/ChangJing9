#include <map>

#include "network/hostname.hpp"

using namespace WarGrey::GYDM;

using namespace Windows::Networking;

static HostName^ broadcast_host = ref new HostName("255.255.255.255");
static std::map<Platform::String^, HostName^> hosts;

/*************************************************************************************************/
HostName^ WarGrey::GYDM::hostname_ref(Platform::String^ host) {
	HostName^ name = broadcast_host;

	if (host != nullptr) {
		auto maybe_host = hosts.find(host);

		if (maybe_host == hosts.end()) {
			name = ref new HostName(host);
			hosts.insert(std::pair<Platform::String^, HostName^>(host, name));
		} else {
			name = maybe_host->second;
		}
	}

	return name;
}
