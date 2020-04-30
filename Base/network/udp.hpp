#pragma once

#include <list>

#include "syslog.hpp"

namespace WarGrey::GYDM {
	private class IUDPDaemon abstract {
	public:
		virtual ~IUDPDaemon() noexcept {}
		IUDPDaemon() {}

	public:
		virtual Platform::String^ daemon_hostname() = 0;
		virtual Platform::String^ daemon_description() = 0;

	public:
		virtual WarGrey::GYDM::Syslog* get_logger() = 0;
	};

	private class IUDPStateListener {
	public:
		virtual void on_send_message(WarGrey::GYDM::IUDPDaemon* master, long long bytes, double span_ms, double timestamp_ms) {}
		virtual void on_unbox_message(WarGrey::GYDM::IUDPDaemon* master, long long bytes, double span_ms, double timestamp_ms) {}
		virtual void on_apply_message(WarGrey::GYDM::IUDPDaemon* master, long long bytes, double span_ms, double timestamp_ms) {}
	};

	template<class UDPStateListener>
	private class IUDPFeedBackDaemon abstract : public WarGrey::GYDM::IUDPDaemon {
	public:
		void notify_message_sent(long long bytes, double span_ms) {
			double timestamp = current_inexact_milliseconds();

			for (auto it = this->listeners.begin(); it != this->listeners.end(); it++) {
				(*it)->on_send_message(this, bytes, span_ms, timestamp);
			}
		}

		void notify_message_unboxed(long long bytes, double span_ms) {
			double timestamp = current_inexact_milliseconds();

			for (auto it = this->listeners.begin(); it != this->listeners.end(); it++) {
				(*it)->on_unbox_message(this, bytes, span_ms, timestamp);
			}
		}

		void notify_message_applied(long long bytes, double span_ms) {
			double timestamp = current_inexact_milliseconds();

			for (auto it = this->listeners.begin(); it != this->listeners.end(); it++) {
				(*it)->on_apply_message(this, bytes, span_ms, timestamp);
			}
		}

	protected:
		std::list<UDPStateListener*> listeners;
	};

	typedef WarGrey::GYDM::IUDPFeedBackDaemon<WarGrey::GYDM::IUDPStateListener> IUDPStatedDaemon;
}
