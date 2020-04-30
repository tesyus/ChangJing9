#include "network/socket.hpp"

using namespace WarGrey::SCADA;

using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;

/*************************************************************************************************/
StreamSocket^ WarGrey::SCADA::make_stream_socket() {
	auto socket = ref new StreamSocket();
	auto control = socket->Control;

	control->KeepAlive = false;
	control->QualityOfService = SocketQualityOfService::LowLatency;

	return socket;
}

DatagramSocket^ WarGrey::SCADA::make_datagram_socket() {
	auto socket = ref new DatagramSocket();
	auto control = socket->Control;

	control->QualityOfService = SocketQualityOfService::LowLatency;

	return socket;
}

Platform::String^ WarGrey::SCADA::socket_remote_description(StreamSocket^ client) {
	return client->Information->RemoteHostName->RawName + ":" + client->Information->RemotePort;
}

Platform::String^ WarGrey::SCADA::socket_remote_description(DatagramSocket^ client) {
	return client->Information->RemoteAddress->RawName + ":" + client->Information->RemotePort;
}

Platform::String^ WarGrey::SCADA::socket_local_description(StreamSocket^ client) {
	return client->Information->LocalAddress->RawName + ":" + client->Information->LocalPort;
}

Platform::String^ WarGrey::SCADA::socket_local_description(DatagramSocket^ client) {
	return client->Information->LocalAddress->RawName + ":" + client->Information->LocalPort;
}

DataReader^ WarGrey::SCADA::make_socket_reader(StreamSocket^ socket) {
	DataReader^ sktin = ref new DataReader(socket->InputStream);
	
	socket_reader_standardize(sktin);

	return sktin;
}

DataReader^ WarGrey::SCADA::make_socket_available_reader(StreamSocket^ socket) {
	DataReader^ sktin = make_socket_reader(socket);

	sktin->InputStreamOptions = InputStreamOptions::Partial;

	return sktin;
}

DataWriter^ WarGrey::SCADA::make_socket_writer(StreamSocket^ socket) {
	DataWriter^ sktout = ref new DataWriter(socket->OutputStream);

	socket_writer_standardize(sktout);

	return sktout;
}

void WarGrey::SCADA::socket_reader_standardize(DataReader^ sktin) {
	sktin->UnicodeEncoding = UnicodeEncoding::Utf8;
	sktin->ByteOrder = ByteOrder::BigEndian;
}

void WarGrey::SCADA::socket_writer_standardize(DataWriter^ sktout) {
	sktout->UnicodeEncoding = UnicodeEncoding::Utf8;
	sktout->ByteOrder = ByteOrder::BigEndian;
}
