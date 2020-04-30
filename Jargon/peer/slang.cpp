#include <ppltasks.h>

#include "peer/slang.hpp"

#include "checksum/ipv4.hpp"

#include "datum/time.hpp"
#include "datum/bytes.hpp"
#include "datum/string.hpp"

#include "network/socket.hpp"
#include "network/netexn.hpp"
#include "network/hostname.hpp"

#include "syslog.hpp"
#include "taskexn.hpp"

using namespace WarGrey::GYDM;
using namespace WarGrey::SCADA;

using namespace Concurrency;

using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;

using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;

/*************************************************************************************************/
/**
 * slang message structure:
 *  / magic number:   2 bytes, constant '#~
 *  | version number: 1 byte, default 0
 *  | payload type:   1 byte, hint for decoding the payload
 *  | checksum:       2 bytes, checksum of entire message [algorithm is the same as that used in IP/UDP/TCP]
 *  | [additional fields based on version]
 *  \ [payload that encoded as ASN.1 DER, usually it is an ASN.1 Sequence]
 *
 * for version 1 message, add two fields:
 *  | transaction id: 2 bytes, initialized by client and recopied by server
 *  | response port:  2 bytes, initialized by client for server to respond
 *
 * NOTE
 * The size of header (plus additional fields) should be even
 *  or the checksum algorithm may fail to verify the sum since
 *  it would insert a virtual nil byte before summing up the payload
 */

static constexpr size_t slang_checksum_idx = 4;
static constexpr size_t slang_checksum_size = 2;
static constexpr size_t slang_message_metadata_upsize = (slang_checksum_idx + slang_checksum_size /* header */) + 4U /* version 1 fields */;
static constexpr uint16 slang_message_magic = 0x237E; // '#~'

typedef std::function<void(DataWriter^ udpout)> slang_cast_stream_then_t;

#define SlangBytes(p) reinterpret_cast<Platform::Array<uint8>^>(new Platform::ArrayReference<uint8>((uint8*)p.c_str(), static_cast<unsigned int>(p.size())))

static void slang_cast_stream_detach(DataWriter^ udpout) {
	udpout->DetachStream();
}

static void slang_cast_stream_do_nothing(DataWriter^ udpout) {
	// Empty function
}

static inline Platform::Array<uint8>^ slang_bytes(IASNSequence* payload) {
	size_t payload_span = payload->span();
	auto basn = ref new Platform::Array<uint8>((unsigned int)(asn_span(payload_span)));

	payload->into_octets((uint8*)basn->Data, 0);

	return basn;
}

static void slang_do_cast(DataWriter^ udpout, HostName^ peer_host, uint16 peer_port, Platform::Array<uint8>^ payload, uint8 type, uint16 response_port, uint16 transaction
	, bool checksumed, slang_cast_task_then_t fthen, slang_cast_stream_then_t fwthen) {
	static auto stupid_cx = ref new Platform::Array<uint8>(slang_message_metadata_upsize);
	static uint8* metainfo = stupid_cx->Data;

	uint8 version = ((response_port == 0) ? 0 : 1);
	double sending_ms = current_inexact_milliseconds();
	size_t header_size = slang_checksum_idx + slang_checksum_size;

	{ // initialize header
		bigendian_uint16_set(metainfo, 0, slang_message_magic);
		bigendian_uint8_set(metainfo, 2, version);
		bigendian_uint8_set(metainfo, 3, type);
		bigendian_uint16_set(metainfo, slang_checksum_idx, 0); // clear checksum

		switch (version) { // write additional fields
		case 1: {
			bigendian_uint16_set(metainfo, header_size, transaction);
			header_size += 2;
			bigendian_uint16_set(metainfo, header_size, response_port);
			header_size += 2;
		}; break;
		}

		if (checksumed) { // calculate checksum
			unsigned short checksum = 0;

			checksum_ipv4(&checksum, metainfo, 0, header_size);
			checksum_ipv4(&checksum, payload->Data, 0, payload->Length);

			if (checksum == 0) {
				bigendian_uint16_set(metainfo, slang_checksum_idx, 0xFFFFU);
			} else {
				bigendian_uint16_set(metainfo, slang_checksum_idx, checksum);
			}
		}
	}

	{ // send message
		if (header_size == slang_message_metadata_upsize) {
			udpout->WriteBytes(stupid_cx);
		} else {
			udpout->WriteBytes(reinterpret_cast<Platform::Array<uint8>^>(new Platform::ArrayReference<uint8>((uint8*)metainfo, (unsigned int)header_size)));
		}

		udpout->WriteBytes(payload);
	}

	create_task(udpout->StoreAsync()).then([=](task<unsigned int> sending) {
		try {
			unsigned int total = sending.get();

			fthen(peer_host->CanonicalName, peer_port, total, current_inexact_milliseconds() - sending_ms, nullptr);
			fwthen(udpout);
		} catch (task_canceled&) {
		} catch (Platform::Exception^ e) {
			fthen(peer_host->CanonicalName, peer_port, 0, 0.0, e->Message);
		}
		});
}

// delegate only accepts C++/CX class
ref class WarGrey::GYDM::ISlangDaemon::GhostDaemon {
internal:
	GhostDaemon(ISlangDaemon* master) : master(master) {
		this->logger = master->get_logger();
	}

public:
	void on_message(DatagramSocket^ sender, DatagramSocketMessageReceivedEventArgs^ args) {
		double unboxing_ts = current_inexact_milliseconds();

		if (this->master->local_peers.size() > 0) { // the message would be dealt with only when at least one local peers exist
			DataReader^ udpin = args->GetDataReader();
			unsigned int total = udpin->UnconsumedBufferLength;
			Platform::WriteOnlyArray<unsigned char>^ pool = ref new Platform::Array<unsigned char>(total);
			Platform::String^ peer = args->RemoteAddress->RawName;
			uint16 port = (uint16)(string_to_fixnum(args->RemotePort));
			const uint8* message = pool->Data;
			
			socket_reader_standardize(udpin);
			udpin->ReadBytes(pool);

			try {
				if ((total > (slang_checksum_idx + slang_checksum_size))
					|| (bigendian_uint16_ref(message, 0) == slang_message_magic)) {
					unsigned short checksum = bigendian_uint16_ref(message, slang_checksum_idx);

					if (checksum == 0x0000U) { // checksum is disabled
						this->dispatch_message(message, total, unboxing_ts, peer, port);
					} else {
						checksum = checksum_ipv4(message, 0, total);

						if (checksum == 0U) {
							this->dispatch_message(message, total, unboxing_ts, peer, port);
						} else {
							task_fatal(this->logger, L"%s:%d: unverifiable slang message[checksum: 0x%04X]", peer->Data(), port, checksum);
						}
					}
				} else {
					task_fatal(this->logger, L"%s:%d: invalid slang message", peer->Data(), port);
				}
			} catch (task_discarded&) {
			} catch (task_terminated&) {
			} catch (task_canceled&) {
			} catch (Platform::Exception ^ e) {
				this->logger->log_message(Log::Warning, e->Message);
				this->master->clear_if_peer_broken();
			}
		}
	}

private:
	void dispatch_message(const uint8* message, unsigned int total, double unboxing_ts, Platform::String^ peer, uint16 port) {
		uint8 version = bigendian_uint8_ref(message, 2);
		uint8 type = bigendian_uint8_ref(message, 3);
		size_t cursor = slang_checksum_idx + slang_checksum_size;
		uint16 transaction = 0;
		uint16 response_port = 0;

		switch (version) {
		case 1: {
			transaction = bigendian_uint16_ref(message, cursor);
			cursor += 2;
			response_port = bigendian_uint16_ref(message, cursor);
			cursor += 2;
		}; break;
		}

		{ // extract payload
			const uint8* payload = message + cursor;
			
			asn_octets_unbox(message, &cursor);

			if (cursor == total) { // is this necessary?
				double applying_ms = current_inexact_milliseconds();
				long long now_ms = current_milliseconds();

				this->master->notify_message_unboxed(total, applying_ms - unboxing_ts);
				this->logger->log_message(Log::Debug, L"<recieved %u-byte slang message(%s, %u, %u) from %s:%u>",
					total, this->master->message_typename(type)->Data(), transaction, response_port, peer->Data(), port);

				switch (version) {
				case 1: this->master->on_message(now_ms, peer, response_port, transaction, response_port, type, payload); break;
				default: this->master->on_message(now_ms, peer, response_port, type, payload);
				}

				this->master->notify_message_applied(total, current_inexact_milliseconds() - applying_ms);
			} else {
				task_discard(this->logger, L"%s:%d: discard truncated slang message", peer->Data(), port);
			}
		}
	}

private:
	ISlangDaemon* master;
	Syslog* logger;
};

void WarGrey::GYDM::slang_cast(Platform::String^ peer, uint16 peer_port, Platform::Array<uint8>^ payload, uint8 type, uint16 response_port, uint16 transaction
	, bool checksumed, slang_cast_task_then_t fthen) {
	static DatagramSocket^ socket = make_datagram_socket();
	auto peer_host = hostname_ref(peer);

	create_task(socket->GetOutputStreamAsync(peer_host, peer_port.ToString())).then([=](task<IOutputStream^> opening) {
		try {
			auto udpout = ref new DataWriter(opening.get());

			socket_writer_standardize(udpout);
			slang_do_cast(udpout, peer_host, peer_port, payload, type, response_port, transaction, checksumed, fthen, slang_cast_stream_detach);
		} catch (task_canceled&) {
		} catch (Platform::Exception^ e) {
			fthen(peer_host->CanonicalName, peer_port, 0, 0.0, e->Message);
		}});
}

void WarGrey::GYDM::slang_cast(uint16 peer_port, Platform::Array<uint8>^ payload, uint8 type, uint16 response_port, uint16 transaction, bool checksum, slang_cast_task_then_t fthen) {
	return slang_cast(nullptr, peer_port, payload, type, response_port, transaction, checksum, fthen);
}

void WarGrey::GYDM::slang_cast(uint16 peer_port, const octets& payload, uint8 type, uint16 response_port, uint16 transaction, bool checksum, slang_cast_task_then_t fthen) {
	return slang_cast(nullptr, peer_port, payload, type, response_port, transaction, checksum, fthen);
}

void WarGrey::GYDM::slang_cast(Platform::String^ peer, uint16 peer_port, const octets& payload, uint8 type, uint16 response_port, uint16 transaction, bool checksum, slang_cast_task_then_t fthen) {
	return slang_cast(peer, peer_port, SlangBytes(payload), type, response_port, transaction, checksum, fthen);
}

void WarGrey::GYDM::slang_cast(uint16 peer_port, IASNSequence* payload, uint8 type, uint16 response_port, uint16 transaction, bool checksum, slang_cast_task_then_t fthen) {
	return slang_cast(nullptr, peer_port, payload, type, response_port, transaction, checksum, fthen);
}

void WarGrey::GYDM::slang_cast(Platform::String^ peer, uint16 peer_port, IASNSequence* payload, uint8 type, uint16 response_port, uint16 transaction, bool checksum, slang_cast_task_then_t fthen) {
	return slang_cast(peer, peer_port, slang_bytes(payload), type, response_port, transaction, checksum, fthen);
}

void WarGrey::GYDM::slang_cast_log_message(Platform::String^ host, uint16 port, unsigned int size, double span_ms, Platform::String^ exn_msg) {
	if (exn_msg != nullptr) {
		syslog(Log::Warning, exn_msg);
	} else {
		syslog(Log::Debug, L"<sent %d-byte slang message to %s:%u", size, host->Data(), port);
	}
}

/*************************************************************************************************/
ISlangDaemon::ISlangDaemon(Syslog* sl, uint16 p, ISlangLocalPeer* cf) : ISlangDaemon(sl, p, 512U, cf) {}
ISlangDaemon::ISlangDaemon(Syslog* sl, uint16 p, size_t recv_buf, ISlangLocalPeer* cf) : service(p), group(nullptr), target(nullptr), unsafe(false), ghostcat(nullptr) {
	this->logger = ((sl == nullptr) ? make_silent_logger("Silent Slang Daemon") : sl);
	this->logger->reference();

	this->socket = make_datagram_socket();
		
	if (recv_buf > 0) {
		this->socket->Control->InboundBufferSizeInBytes = (unsigned int)(recv_buf + slang_message_metadata_upsize);
	}

	this->push_slang_local_peer(cf);
};

ISlangDaemon::~ISlangDaemon() {
	this->clear_binded_services();

	if (this->socket != nullptr) {
		delete this->socket;
	}

	this->logger->destroy();
}

Platform::String^ ISlangDaemon::daemon_hostname() {
	return this->socket->Information->RemoteAddress->DisplayName;
}

Platform::String^ ISlangDaemon::daemon_description() {
	return socket_remote_description(this->socket);
}

Syslog* ISlangDaemon::get_logger() {
	return this->logger;
}

Platform::String^ ISlangDaemon::message_typename(uint8 type) {
	return type.ToString();
}

void ISlangDaemon::push_slang_local_peer(ISlangLocalPeer* peer) {
	if (peer != nullptr) {
		this->bind_if_has_not();
		this->local_peers.push_back(peer);
	}
}

void ISlangDaemon::bind_if_has_not() {
	if (this->ghostcat == nullptr) {
		auto bind_task = create_task(this->socket->BindServiceNameAsync(this->service.ToString()));
		
		this->ghostcat = ref new ISlangDaemon::GhostDaemon(this);
		this->socket->MessageReceived += ref new TypedEventHandler<DatagramSocket^, DatagramSocketMessageReceivedEventArgs^>(this->ghostcat, &ISlangDaemon::GhostDaemon::on_message);

		bind_task.then([=](task<void> binding) {
			try {
				binding.get();

				if (this->service == 0) {
					this->service = (uint16)string_to_fixnum(this->socket->Information->LocalPort);
				}

				this->logger->log_message(Log::Info, L"## binding on 0.0.0.0:%u [buffer size: %d]",
					this->service, this->socket->Control->InboundBufferSizeInBytes - slang_message_metadata_upsize);
			} catch (task_canceled&) {
			} catch (Platform::Exception ^ e) {
				this->logger->log_message(Log::Warning, e->Message);
			}
		});
	}
}

void ISlangDaemon::join_multicast_group(Platform::String^ ipv4) {
	// TODO: rejoin after network adapter changed if necessary.

	this->bind_if_has_not(); // joining a multicast group is only meaningful for slang servers
	this->socket->JoinMulticastGroup(hostname_ref(ipv4));

	if (this->target == nullptr) {
		this->clear_binded_services();
	}

	this->logger->log_message(Log::Info, L"# joined multicast group %s", ipv4->ToString()->Data());
}

void ISlangDaemon::set_target_multicast_group(Platform::String^ ipv4) {
	// NOTE: This API is designed for cases that only slang client is needed

	if (ipv4 == nullptr) {
		if (this->target != nullptr) {
			this->target = nullptr;
			this->clear_binded_services();
		}
	} else {
		if ((this->target == nullptr) || (this->target->CanonicalName != ipv4)) {
			this->target = hostname_ref(ipv4);
			this->clear_binded_services();
		}
	}
}

void ISlangDaemon::bind_multicast_service(uint16 service) {
	HostName^ host = this->multicast_host();

	if (service == 0) {
		service = this->service;
	}

	if ((this->group != nullptr) && (service > 0)) {
		auto maybe_exists = this->grpouts.find(service);

		if (maybe_exists == this->grpouts.end()) {
			create_task(this->socket->GetOutputStreamAsync(host, service.ToString())).then([=](task<IOutputStream^> opening) {
				try {
					auto udpout = ref new DataWriter(opening.get());

					socket_writer_standardize(udpout);
					this->grpouts.insert(std::pair<uint16, DataWriter^>(service, udpout));
					this->logger->log_message(Log::Info, L"# binded multicast service %d", service);
				} catch (task_canceled&) {
				} catch (Platform::Exception^ e) {
					this->get_logger()->log_message(Log::Error, L"unable to bind the multicast service[%d]: %s", service, e->Message->Data());
				}});
		}
	}
}


void ISlangDaemon::enable_checksum(bool yes_no) {
	this->unsafe = (!yes_no);
}

void ISlangDaemon::on_message(long long timepoint, Platform::String^ remote_peer, uint16 port, uint16 transaction, uint16 response_port, uint8 type, const uint8* message) {
	 for (auto peer : this->local_peers) {
		this->current_peer = peer;

		if (!peer->absent()) {
			this->current_peer->pre_read_message(this->logger);
			this->apply_message(timepoint, this->current_peer, remote_peer, port, type, message);
			this->current_peer->post_read_message(this->logger);
		}
	}

	this->current_peer = nullptr;
}

void ISlangDaemon::on_message(long long timepoint, Platform::String^ remote_peer, uint16 port, uint8 type, const uint8* message) {
	for (auto peer : this->local_peers) {
		this->current_peer = peer;

		if (!peer->absent()) {
			this->current_peer->pre_read_message(this->logger);
			this->apply_message(timepoint, this->current_peer, remote_peer, port, type, message);
			this->current_peer->post_read_message(this->logger);
		}
	}

	this->current_peer = nullptr;
}

void ISlangDaemon::clear_if_peer_broken() {
	/** NOTE
	 * The peer may throw exceptions which will be caught by Network IO thread.
	 * If the peer is using a lock, it should have a change to release the lock.
	 */

	if (this->current_peer != nullptr) {
		this->current_peer->post_read_message(this->logger);
		this->current_peer = nullptr;
	}
}

void ISlangDaemon::clear_binded_services() {
	for (auto it = this->grpouts.begin(); it != this->grpouts.end(); it++) {
		it->second->DetachStream();
	}

	this->grpouts.clear();
}

HostName^ ISlangDaemon::multicast_host() {
	return (this->target != nullptr) ? this->target : this->group;
}

Platform::String^ ISlangDaemon::multicast_hostname() {
	HostName^ host = this->multicast_host();

	return (host == nullptr) ? nullptr : host->CanonicalName;
}

/*************************************************************************************************/
void ISlangDaemon::cast_then(Platform::String^ host, uint16 port, unsigned int size, double span_ms, Platform::String^ exn_msg, uint8 type, uint16 transaction) {
	if (exn_msg == nullptr) {
		this->notify_message_sent(size, span_ms);
		this->logger->log_message(Log::Debug, L"<sent %u-byte slang message(%s, %u, %u) to %s:%u>",
			size, this->message_typename(type)->Data(), transaction, this->service, host->Data(), port);
	} else {
		this->logger->log_message(Log::Warning, exn_msg->Data());
	}
}

void ISlangDaemon::multicast(DataWriter^ udpout, uint16 peer_port, Platform::Array<uint8>^ payload, uint8 type, uint16 transaction) {
	slang_do_cast(udpout, this->multicast_host(), peer_port, payload, type, this->service, transaction, !this->unsafe,
		[=](Platform::String^ host, uint16 port, unsigned int bytes, double span_ms, Platform::String^ exn_msg) {
			this->cast_then(host, port, bytes, span_ms, exn_msg, type, transaction);
		}, slang_cast_stream_do_nothing);
}

void ISlangDaemon::cast(Platform::String^ peer, uint16 peer_port, const octets& payload, uint8 type, uint16 transaction) {
	slang_cast(peer, peer_port, payload, type, this->service, transaction, !this->unsafe,
		[=](Platform::String^ host, uint16 port, unsigned int bytes, double span_ms, Platform::String^ exn_msg) {
			this->cast_then(host, port, bytes, span_ms, exn_msg, type, transaction);
		});
}

void ISlangDaemon::cast(Platform::String^ peer, uint16 peer_port, IASNSequence* payload, uint8 type, uint16 transaction) {
	slang_cast(peer, peer_port, payload, type, this->service, transaction, !this->unsafe,
		[=](Platform::String^ host, uint16 port, unsigned int bytes, double span_ms, Platform::String^ exn_msg) {
			this->cast_then(host, port, bytes, span_ms, exn_msg, type, transaction);
		});
}

void ISlangDaemon::multicast(uint16 peer_port, const octets& payload, uint8 type, uint16 transaction) {
	auto maybe_binded_service = this->grpouts.find(peer_port);

	if (maybe_binded_service == this->grpouts.end()) {
		this->cast(this->multicast_hostname(), peer_port, payload, type, transaction);
	} else {
		this->multicast(maybe_binded_service->second, peer_port, SlangBytes(payload), type, transaction);
	}
}

void ISlangDaemon::multicast(uint16 peer_port, IASNSequence* payload, uint8 type, uint16 transaction) {
	auto maybe_binded_service = this->grpouts.find(peer_port);

	if (maybe_binded_service == this->grpouts.end()) {
		this->cast(this->multicast_hostname(), peer_port, payload, type, transaction);
	} else {
		this->multicast(maybe_binded_service->second, peer_port, slang_bytes(payload), type, transaction);
	}
}

void ISlangDaemon::cast(Platform::String^ peer, const octets& payload, uint8 type, uint16 transaction) {
	this->cast(peer, this->service, payload, type, transaction);
}

void ISlangDaemon::cast(uint16 peer_port, const octets& payload, uint8 type, uint16 transaction) {
	this->cast(nullptr, peer_port, payload, type, transaction);
}

void ISlangDaemon::cast(const octets& payload, uint8 type, uint16 transaction) {
	this->cast(this->service, payload, type, transaction);
}

void ISlangDaemon::multicast(const octets& payload, uint8 type, uint16 transaction) {
	this->multicast(this->service, payload, type, transaction);
}

void ISlangDaemon::cast(Platform::String^ peer, IASNSequence* payload, uint8 type, uint16 transaction) {
	this->cast(peer, this->service, payload, type, transaction);
}

void ISlangDaemon::cast(uint16 peer_port, IASNSequence* payload, uint8 type, uint16 transaction) {
	this->cast(nullptr, peer_port, payload, type, transaction);
}

void ISlangDaemon::cast(IASNSequence* payload, uint8 type, uint16 transaction) {
	this->cast(this->service, payload, type, transaction);
}

void ISlangDaemon::multicast(IASNSequence* payload, uint8 type, uint16 transaction) {
	this->multicast(this->service, payload, type, transaction);
}
