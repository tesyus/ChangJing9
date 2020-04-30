#pragma once

namespace WarGrey::SCADA {
	unsigned int discard_dirty_bytes(Windows::Storage::Streams::DataReader^ din);
	
	private class IStreamAcceptPort abstract {
	public:
		virtual void on_socket(Windows::Networking::Sockets::StreamSocket^ socket) = 0;
	};

	private class StreamListener {
	public:
		virtual ~StreamListener() noexcept;

		StreamListener();

	public:
		void listen(IStreamAcceptPort* master, Platform::String^ service);

	private:
		Windows::Networking::Sockets::StreamSocketListener^ entity;
	};
}
