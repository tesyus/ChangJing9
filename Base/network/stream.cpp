#include <ppltasks.h>
#include <map>

#include "network/stream.hpp"

using namespace WarGrey::SCADA;

using namespace Concurrency;

using namespace Windows::Foundation;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;

/*************************************************************************************************/
namespace {
	// delegate only accepts C++/CX class
	private ref class GhostListener sealed {
	internal:
		GhostListener(IStreamAcceptPort* master) : master(master) {}

	public:
		void respond(StreamSocketListener^ listener, StreamSocketListenerConnectionReceivedEventArgs^ e) {
			this->master->on_socket(e->Socket);
		}

	private:
		IStreamAcceptPort* master;
	};
}

static std::map<int, GhostListener^> ghosts;

/*************************************************************************************************/
StreamListener::StreamListener() {
	this->entity = ref new StreamSocketListener();
	
	this->entity->Control->QualityOfService = SocketQualityOfService::LowLatency;
	this->entity->Control->KeepAlive = false;
	this->entity->Control->NoDelay = true;
}

StreamListener::~StreamListener() {
	auto uuid = this->entity->GetHashCode();
	auto self = ghosts.find(uuid);

	if (self != ghosts.end()) {
		ghosts.erase(self);
	}
}

void StreamListener::listen(IStreamAcceptPort* master, Platform::String^ service) {
	auto delegate = ref new GhostListener(master);
	auto uuid = this->entity->GetHashCode();

	ghosts.insert(std::pair<int, GhostListener^>(uuid, delegate));

	create_task(this->entity->BindEndpointAsync(nullptr, service)).wait();

	this->entity->ConnectionReceived
		+= ref new TypedEventHandler<StreamSocketListener^, StreamSocketListenerConnectionReceivedEventArgs^>(
			delegate, &GhostListener::respond);
}

/*************************************************************************************************/
unsigned int WarGrey::SCADA::discard_dirty_bytes(DataReader^ din) {
	unsigned int rest = din->UnconsumedBufferLength;

	if (rest > 0) {
		din->ReadBuffer(rest);
	}

	return rest;
}
