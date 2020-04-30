#pragma once

#include <list>
#include <unordered_map>
#include <functional>

#include "syslog/logging.hpp"

#include "network/udp.hpp"
#include "asn/der.hpp"
#include "datum/enum.hpp"

namespace WarGrey::GYDM {
	typedef std::function<void(Platform::String^, uint16, unsigned int, double, Platform::String^ exn_msg)> slang_cast_task_then_t;

	void slang_cast_log_message(Platform::String^ host, uint16 port, unsigned int size, double span_ms, Platform::String^ exn_msg);

	void slang_cast(uint16 peer_port, Platform::Array<uint8>^ payload, uint8 type = 0U, uint16 response_port = 0U, uint16 transaction = 0U,
		bool checksum = true, WarGrey::GYDM::slang_cast_task_then_t fthen = slang_cast_log_message);
	void slang_cast(Platform::String^ peer, uint16 peer_port, Platform::Array<uint8>^ payload, uint8 type = 0U, uint16 response_port = 0U, uint16 transaction = 0U,
		bool checksum = true, WarGrey::GYDM::slang_cast_task_then_t fthen = slang_cast_log_message);
	
	void slang_cast(uint16 peer_port, const WarGrey::GYDM::octets& payload, uint8 type = 0U, uint16 response_port = 0U, uint16 transaction = 0U,
		bool checksum = true, WarGrey::GYDM::slang_cast_task_then_t fthen = slang_cast_log_message);
	void slang_cast(Platform::String^ peer, uint16 peer_port, const WarGrey::GYDM::octets& payload, uint8 type = 0U, uint16 response_port = 0U, uint16 transaction = 0U,
		bool checksum = true, WarGrey::GYDM::slang_cast_task_then_t fthen = slang_cast_log_message);

	void slang_cast(uint16 peer_port,  WarGrey::GYDM::IASNSequence* payload, uint8 type = 0U, uint16 response_port = 0U, uint16 transaction = 0U,
		bool checksum = true, WarGrey::GYDM::slang_cast_task_then_t fthen = slang_cast_log_message);
	void slang_cast(Platform::String^ peer, uint16 peer_port, WarGrey::GYDM::IASNSequence* payload, uint8 type = 0U, uint16 response_port = 0U, uint16 transaction = 0U,
		bool checksum = true, WarGrey::GYDM::slang_cast_task_then_t fthen = slang_cast_log_message);

	template<typename E>
	void slang_cast(uint16 peer_port, Platform::Array<uint8>^ payload, E type, uint16 response_port = 0U, uint16 transaction = 0U
		, bool checksum = true, WarGrey::GYDM::slang_cast_task_then_t fthen = slang_cast_log_message) {
		return WarGrey::GYDM::slang_cast(peer_port, payload, _I(type), response_port, transaction, checksum, fthen);
	}

	template<typename E>
	void slang_cast(Platform::String^ peer, uint16 peer_port, Platform::Array<uint8>^ payload, E type, uint16 response_port = 0U, uint16 transaction = 0U
		, bool checksum = true, WarGrey::GYDM::slang_cast_task_then_t fthen = slang_cast_log_message) {
		return WarGrey::GYDM::slang_cast(peer, peer_port, payload, _C(type), response_port, transaction, checksum, fthen);
	}

	template<typename E>
	void slang_cast(uint16 peer_port, const uint8* payload, size_t size, E type, uint16 response_port = 0U, uint16 transaction = 0U
		, bool checksum = true, WarGrey::GYDM::slang_cast_task_then_t fthen = slang_cast_log_message) {
		return WarGrey::GYDM::slang_cast(nullptr, peer_port, payload, size, _C(type), response_port, transaction, checksum, fthen);
	}
	
	template<typename E>
	void slang_cast(Platform::String^ peer, uint16 peer_port, const uint8* payload, size_t size, E type, uint16 response_port = 0U, uint16 transaction = 0U
		, bool checksum = true, WarGrey::GYDM::slang_cast_task_then_t fthen = slang_cast_log_message) {
		return WarGrey::GYDM::slang_cast(peer, peer_port, payload, size, _C(type), response_port, transaction, checksum, fthen);
	}

	template<typename E>
	void slang_cast(uint16 peer_port, WarGrey::GYDM::IASNSequence* payload, E type, uint16 response_port = 0U, uint16 transaction = 0U
		, bool checksum = true, WarGrey::GYDM::slang_cast_task_then_t fthen = slang_cast_log_message) {
		return WarGrey::GYDM::slang_cast(nullptr, peer_port, payload, _C(type), response_port, transaction, checksum, fthen);
	}
	
	template<typename E>
	void slang_cast(Platform::String^ peer, uint16 peer_port, WarGrey::GYDM::IASNSequence* payload, E type, uint16 response_port = 0U, uint16 transaction = 0U
		, bool checksum = true, WarGrey::GYDM::slang_cast_task_then_t fthen = slang_cast_log_message) {
		return WarGrey::GYDM::slang_cast(peer, peer_port, payload, _C(type), response_port, transaction, checksum, fthen);
	}

	/**********************************************************************************************/
	private class ISlangLocalPeer abstract {
	public:
		virtual bool absent() { return false; }

	public:
		virtual void pre_read_message(WarGrey::GYDM::Syslog* logger) = 0;
		virtual void post_read_message(WarGrey::GYDM::Syslog* logger) = 0;
	};

	private class ISlangDaemon abstract : public WarGrey::GYDM::IUDPStatedDaemon {
    public:
        virtual ~ISlangDaemon() noexcept;
		ISlangDaemon(WarGrey::GYDM::Syslog* logger, uint16 service, WarGrey::GYDM::ISlangLocalPeer* local_peer = nullptr);
		ISlangDaemon(WarGrey::GYDM::Syslog* logger, uint16 service, size_t recv_size, WarGrey::GYDM::ISlangLocalPeer* local_peer = nullptr);

	public:
		WarGrey::GYDM::Syslog* get_logger() override;
		Platform::String^ daemon_hostname() override;
		Platform::String^ daemon_description() override;
		
	public:
		void push_slang_local_peer(WarGrey::GYDM::ISlangLocalPeer* confirmation);

	public:
		void join_multicast_group(Platform::String^ group_ipv4);
		void set_target_multicast_group(Platform::String^ group_ipv4);
		void bind_multicast_service(uint16 service = 0U);
		void enable_checksum(bool yes_no);

	public:
		void cast(Platform::String^ peer, uint16 peer_port, const WarGrey::GYDM::octets& payload, uint8 type = 0U, uint16 transaction = 0U);
		void cast(Platform::String^ peer, const WarGrey::GYDM::octets& payload, uint8 type = 0U, uint16 transaction = 0U);
		void cast(uint16 peer_port, const WarGrey::GYDM::octets& payload, uint8 type = 0U, uint16 transaction = 0U);
		void cast(const WarGrey::GYDM::octets& payload, uint8 type = 0U, uint16 transaction = 0U);
		void multicast(uint16 peer_port, const WarGrey::GYDM::octets& payload, uint8 type = 0U, uint16 transaction = 0U);
		void multicast(const WarGrey::GYDM::octets& payload, uint8 type = 0U, uint16 transaction = 0U);

		void cast(Platform::String^ peer, uint16 peer_port, WarGrey::GYDM::IASNSequence* payload, uint8 type = 0U, uint16 transaction = 0U);
		void cast(Platform::String^ peer, WarGrey::GYDM::IASNSequence* payload, uint8 type = 0U, uint16 transaction = 0U);
		void cast(uint16 peer_port, WarGrey::GYDM::IASNSequence* payload, uint8 type = 0U, uint16 transaction = 0U);
		void cast(WarGrey::GYDM::IASNSequence* payload, uint8 type = 0U, uint16 transaction = 0U);
		void multicast(uint16 peer_port, WarGrey::GYDM::IASNSequence* payload, uint8 type = 0U, uint16 transaction = 0U);
		void multicast(WarGrey::GYDM::IASNSequence* payload, uint8 type = 0U, uint16 transaction = 0U);

		template<typename E>
		void cast(Platform::String^ peer, uint16 peer_port, const WarGrey::GYDM::octets& payload, E type, uint16 transaction = 0U) {
			this->cast(peer, peer_port, payload, _C(type), this->service, transaction);
		}

		template<typename E>
		void cast(Platform::String^ peer, const WarGrey::GYDM::octets& payload, E type, uint16 transaction = 0U) {
			this->cast(peer, this->service, payload, _C(type), this->service, transaction);
		}

		template<typename E>
		void cast(uint16 peer_port, const WarGrey::GYDM::octets& payload, E type, uint16 transaction = 0U) {
			this->cast(peer_port, payload, _C(type), this->service, transaction);
		}

		template<typename E>
		void cast(const WarGrey::GYDM::octets& payload, E type, uint16 transaction = 0U) {
			this->cast(this->service, peer_port, payload, _C(type), this->service, transaction);
		}

		template<typename E>
		void multicast(uint16 peer_port, const WarGrey::GYDM::octets& payload, E type, uint16 transaction = 0U) {
			this->multicast(peer_port, payload, _C(type), transaction);
		}

		template<typename E>
		void multicast(const WarGrey::GYDM::octets& payload, E type, uint16 transaction = 0U) {
			this->multicast(this->service, payload, _C(type), transaction);
		}

		template<typename E>
		void cast(Platform::String^ peer, uint16 peer_port, WarGrey::GYDM::IASNSequence* payload, E type, uint16 transaction = 0U) {
			this->cast(peer, peer_port, payload, _C(type), this->service, transaction);
		}

		template<typename E>
		void cast(Platform::String^ peer, WarGrey::GYDM::IASNSequence* payload, E type, uint16 transaction = 0U) {
			this->cast(peer, this->service, payload, _C(type), this->service, transaction);
		}

		template<typename E>
		void cast(uint16 peer_port, WarGrey::GYDM::IASNSequence* payload, E type, uint16 transaction = 0U) {
			this->cast(peer_port, payload, _C(type), this->service, transaction);
		}

		template<typename E>
		void cast(WarGrey::GYDM::IASNSequence* payload, E type, uint16 transaction = 0U) {
			this->cast(this->service, peer_port, payload, _C(type), this->service, transaction);
		}

		template<typename E>
		void multicast(uint16 peer_port, WarGrey::GYDM::IASNSequence* payload, E type, uint16 transaction = 0U) {
			this->multicast(peer_port, payload, _C(type), transaction);
		}

		template<typename E>
		void multicast(WarGrey::GYDM::IASNSequence* payload, E type, uint16 transaction = 0U) {
			this->multicast(this->service, payload, _C(type), transaction);
		}

	protected:
		virtual Platform::String^ message_typename(uint8 type);
		virtual void apply_message(long long timepoint_ms, WarGrey::GYDM::ISlangLocalPeer* peer,
			Platform::String^ remote_peer, uint16 port, uint8 type, const uint8* message) = 0;
		
	private:
		void bind_if_has_not();
		void clear_if_peer_broken();
		void clear_binded_services();
		void on_message(long long timepoint_ms, Platform::String^ remote_peer, uint16 port, uint8 type, const uint8* message);
		void on_message(long long timepoint_ms, Platform::String^ remote_peer, uint16 port, uint16 transaction, uint16 response_port, uint8 type, const uint8* message);		
		void cast_then(Platform::String^ host, uint16 port, unsigned int size, double span_ms, Platform::String^ exn_msg, uint8 type, uint16 transaction);
		void multicast(Windows::Storage::Streams::DataWriter^ udpout, uint16 peer_port, Platform::Array<uint8>^ payload, uint8 type, uint16 transaction);
		Windows::Networking::HostName^ multicast_host();
		Platform::String^ multicast_hostname();

	protected:
		std::list<WarGrey::GYDM::ISlangLocalPeer*> local_peers;
		WarGrey::GYDM::ISlangLocalPeer* current_peer;
		WarGrey::GYDM::Syslog* logger;

	private:
		ref class GhostDaemon; // delegate only accepts C++/CX class
		Windows::Networking::Sockets::DatagramSocket^ socket;

    private:
		std::unordered_map<uint16, Windows::Storage::Streams::DataWriter^> grpouts;
		WarGrey::GYDM::ISlangDaemon::GhostDaemon^ ghostcat;
		Windows::Networking::HostName^ group;
		Windows::Networking::HostName^ target;
		unsigned short service;
		bool unsafe;
    };

	/**********************************************************************************************/
	template<typename E>
	private class SlangLocalPeer : public WarGrey::GYDM::ISlangLocalPeer {
	public:
		void pre_read_message(WarGrey::GYDM::Syslog* logger) override {}
		void post_read_message(WarGrey::GYDM::Syslog* logger) override {}

	public:
		virtual void on_message(long long timepoint_ms,
			Platform::String^ remote_peer, uint16 port, E type, const uint8* message,
			WarGrey::GYDM::Syslog* logger) = 0;
	};

	template<typename E>
	private class SlangDaemon : public WarGrey::GYDM::ISlangDaemon {
	public:
		SlangDaemon(WarGrey::GYDM::Syslog* logger, uint16 port, ISlangLocalPeer* confirmation = nullptr)
			: ISlangDaemon(logger, port, confirmation) {}

		SlangDaemon(WarGrey::GYDM::Syslog* logger, uint16 port, size_t recv_size, ISlangLocalPeer* confirmation = nullptr)
			: ISlangDaemon(logger, port, recv_size, confirmation) {}

	protected:
		Platform::String^ message_typename(uint8 type) override {
			return _E(E, type).ToString();
		}

		void apply_message(long long timepoint_ms, WarGrey::GYDM::ISlangLocalPeer* local_peer
			, Platform::String^ remote_peer, uint16 port, uint8 type, const uint8* message) override {
			auto peer = static_cast<WarGrey::GYDM::SlangLocalPeer<E>*>(local_peer);

			if (peer != nullptr) {
				peer->on_message(timepoint_ms, remote_peer, port, _E(E, type), message, this->logger);
			}
		};
	};
}
