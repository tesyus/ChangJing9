#pragma once

namespace WarGrey::SCADA {
	Windows::Networking::Sockets::StreamSocket^ make_stream_socket();
	Windows::Networking::Sockets::DatagramSocket^ make_datagram_socket();

	Platform::String^ socket_remote_description(Windows::Networking::Sockets::StreamSocket^ socket);
	Platform::String^ socket_remote_description(Windows::Networking::Sockets::DatagramSocket^ socket);
	Platform::String^ socket_local_description(Windows::Networking::Sockets::StreamSocket^ socket);
	Platform::String^ socket_local_description(Windows::Networking::Sockets::DatagramSocket^ socket);

	Windows::Storage::Streams::DataReader^ make_socket_reader(Windows::Networking::Sockets::StreamSocket^ socket);
	Windows::Storage::Streams::DataReader^ make_socket_available_reader(Windows::Networking::Sockets::StreamSocket^ socket);
	Windows::Storage::Streams::DataWriter^ make_socket_writer(Windows::Networking::Sockets::StreamSocket^ socket);

	void socket_reader_standardize(Windows::Storage::Streams::DataReader^ sktin);
	void socket_writer_standardize(Windows::Storage::Streams::DataWriter^ sktout);
}
