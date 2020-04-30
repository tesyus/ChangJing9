#pragma once

namespace WarGrey::SCADA {
	Platform::String^ socket_strerror(Platform::Exception^ e);
	Windows::Networking::Sockets::SocketErrorStatus socket_error(Platform::Exception^ e);
}
