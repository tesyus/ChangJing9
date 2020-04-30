#include "network/tcp.hpp"

using namespace WarGrey::SCADA;

using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;

/*************************************************************************************************/
namespace {
	private ref class TCPKiller {
	public:
	internal:
		TCPKiller(ITCPConnection* master) : master(master) {
			this->timer = ref new DispatcherTimer();
			this->timer->Tick += ref new EventHandler<Platform::Object^>(this, &TCPKiller::check_timeout);
		}

	public:
		void set_timeout(long long ms) {
			this->timeout = ms;

			if (ms > 0) {
				this->timer->Interval = make_timespan_from_milliseconds(ms);
				this->timer->Start(); // starting or restarting
				this->timer->Start(); // ensured restarting
			} else {
				this->timer->Stop();
			}
		}

	public:
		void check_timeout(Platform::Object^ whocares, Platform::Object^ useless) {
			this->master->suicide_if_timeout(this->timeout);
		}

	private:
		Windows::UI::Xaml::DispatcherTimer^ timer;
		long long timeout;

	private:
		ITCPConnection* master;
	};
}

/*************************************************************************************************/
ITCPConnection::~ITCPConnection() {
	this->set_suicide_timeout(0);
}

void ITCPConnection::set_suicide_timeout(long long ms) {
	TCPKiller^ killer = ((this->killer == nullptr)
		? (ref new TCPKiller(this))
		: dynamic_cast<TCPKiller^>(this->killer));

	killer->set_timeout(ms);
	this->reset_heartbeat();

	if (this->killer == nullptr) {
		this->killer = killer;
	}
}

bool ITCPConnection::authorized() {
	return (this->mode != TCPMode::User);
}

void ITCPConnection::set_mode(WarGrey::SCADA::TCPMode mode) {
	this->mode = mode;
}
