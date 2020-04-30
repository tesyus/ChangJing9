#include "network/netexn.hpp"

using namespace WarGrey::SCADA;

using namespace Windows::Networking::Sockets;

Platform::String^ WarGrey::SCADA::socket_strerror(Platform::Exception^ e) {
	return socket_error(e).ToString();
}

SocketErrorStatus WarGrey::SCADA::socket_error(Platform::Exception^ e) {
	return SocketError::GetStatus(e->HResult);
}
