#include <ppltasks.h>

#include "syslog/receiver/rsyslog.hpp"

#include "network/socket.hpp"
#include "network/hostname.hpp"

#include "datum/string.hpp"
#include "datum/time.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

using namespace Concurrency;

using namespace Windows::Storage::Streams;

/*************************************************************************************************/
static void rsyslog_send(DataWriter^ udpout, Log level, Platform::String^ message, SyslogMetainfo* data, Platform::String^ topic) {
	// https://tools.ietf.org/html/rfc5424#section-6.2.1
	int pri = 1 /* user level message */ * 8 + (7 - static_cast<int>(level));
	Platform::String^ ts = make_timestamp_utc(data->timestamp / 1000, false);

	udpout->WriteByte('<');
	udpout->WriteString(pri.ToString());
	udpout->WriteString(">1 ");
	
	udpout->WriteString(ts);
	udpout->WriteByte('.');
	udpout->WriteString((data->timestamp % 1000).ToString());
	udpout->WriteString("Z ");

	udpout->WriteString(data->localname);
	udpout->WriteByte(' ');

	udpout->WriteString(data->pname);
	udpout->WriteByte(' ');
	udpout->WriteString(data->pid.ToString());
	udpout->WriteByte(' ');

	udpout->WriteString(topic);
	udpout->WriteString(" -"); // no structured data

	if (message != nullptr) {
		udpout->WriteString(" BOM");
		udpout->WriteString(message);
	}

	udpout->StoreAsync();
}

/*************************************************************************************************/
RsyslogReceiver::RsyslogReceiver(Platform::String^ host, unsigned short service, Log level, Platform::String^ topic) : ISyslogReceiver(level, topic), udpout(nullptr) {
	this->socket = make_datagram_socket();
	this->socket->Control->InboundBufferSizeInBytes = 2048;

	create_task(this->socket->GetOutputStreamAsync(hostname_ref(host), service.ToString())).then([=](task<IOutputStream^> opening) {
		this->section.lock();

		try {
			this->udpout = ref new DataWriter(opening.get());
			socket_writer_standardize(this->udpout);
			this->send_drift_messages();
		} catch (task_canceled&) {
		} catch (Platform::Exception^ e) {
		}
		
		this->section.unlock();
		});
}

RsyslogReceiver::~RsyslogReceiver() {
	if (this->udpout != nullptr) {
		this->udpout->DetachStream();
	}

	if (this->socket != nullptr) {
		delete this->socket;
	}
}

void RsyslogReceiver::on_log_message(Log level, Platform::String^ message, SyslogMetainfo* data, Platform::String^ topic) {
	if (this->udpout == nullptr) {
		this->section.lock();

		this->levels.push_back(level);
		this->messages.push_back(message);
		this->infoes.push_back(*data);
		this->topics.push_back(topic);

		this->section.unlock();
	} else {
		rsyslog_send(this->udpout, level, message, data, topic);
	}
}

void RsyslogReceiver::send_drift_messages() {
	for (size_t idx = 0; idx < this->levels.size(); idx++) {
		rsyslog_send(this->udpout, this->levels[idx], this->messages[idx], &this->infoes[idx], this->topics[idx]);
	}

	this->levels.clear();
	this->messages.clear();
	this->infoes.clear();
	this->topics.clear();
}
