#include <ppltasks.h>
#include <collection.h>
#include <list>

#include "datum/box.hpp"
#include "datum/time.hpp"

#include "system.hpp"
#include "syslog.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

using namespace Concurrency;

using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;

using namespace Windows::Graphics::Display;
using namespace Windows::Storage;

using namespace Windows::System;
using namespace Windows::System::Diagnostics;

using namespace Windows::Devices::Power;
using namespace Windows::Devices::WiFi;

using namespace Windows::Networking;
using namespace Windows::Networking::Connectivity;

using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::ViewManagement;

typedef TypedEventHandler<Battery^, Platform::Object^> BatteryUpdateHandler;
typedef TypedEventHandler<WiFiAdapter^, Platform::Object^> WiFiUpdateHandler;

static UISettings^ sysUI = nullptr;
static CoreDispatcher^ ui_thread = nullptr;

static inline Size adjust_size(float Width, float Height, FrameworkElement^ workspace) {
    auto margin = workspace->Margin;

    float width = Width - float(margin.Left + margin.Right);
    float height = Height - float(margin.Top + margin.Bottom);
    return Size(width, height);
}

/*************************************************************************************************/
Size WarGrey::SCADA::adjusted_workspace_size(Rect region, FrameworkElement^ workspace) {
	return adjust_size(region.Width, region.Height, workspace);
}

Size WarGrey::SCADA::system_screen_size() {
	DisplayInformation^ master = DisplayInformation::GetForCurrentView();
    double scaling = master->RawPixelsPerViewPixel;
	unsigned int fxwidth = master->ScreenWidthInRawPixels;
	unsigned int fxheight = master->ScreenHeightInRawPixels;
	Size screen;

	screen.Width = float(fxwidth / scaling);
	screen.Height = float(fxheight / scaling);

    return screen;
}

Color WarGrey::SCADA::system_color(UIColorType type) {
    if (sysUI == nullptr) {
        sysUI = ref new UISettings();
    }

    return sysUI->GetColorValue(type);
}

Color WarGrey::SCADA::system_color(UIElementType type) {
    if (sysUI == nullptr) {
        sysUI = ref new UISettings();
    }

    return sysUI->UIElementColor(type);
}

/*************************************************************************************************/
void WarGrey::SCADA::ui_thread_initialize() {
	if (ui_thread == nullptr) {
		ui_thread = CoreWindow::GetForCurrentThread()->Dispatcher;
	}
}

bool WarGrey::SCADA::ui_thread_accessed() {
	return ui_thread->HasThreadAccess;
}

IAsyncAction^ WarGrey::SCADA::ui_thread_run_async(DispatchedHandler^ handler, CoreDispatcherPriority priority) {
	return ui_thread->RunAsync(priority, handler);
}

/*************************************************************************************************/
/** CRITICAL WARNING
 * Don't ask me, Microsoft is an asshole.
 *
 * `BrightnessOverride::IsSupported` always returns false,
 * Even the device is its own Surface Book Pro 2, 2018.
 * 
 * `BrightnessOverride::GetDefaultForSystem()` throws "Unspecified Error"
 */

double WarGrey::SCADA::system_screen_brightness(double defval_if_not_available) {
	BrightnessOverride^ bo = BrightnessOverride::GetForCurrentView();

	return (bo->IsSupported ? bo->BrightnessLevel : defval_if_not_available);
}

void WarGrey::SCADA::system_try_change_screen_brightness(double brightness, DisplayBrightnessOverrideOptions option) {
	BrightnessOverride^ bo = BrightnessOverride::GetForCurrentView();

	bo->SetBrightnessLevel(brightness, option);
	bo->StartOverride();
}

/*************************************************************************************************/
unsigned long long WarGrey::SCADA::system_physical_memory_usage(unsigned long long* virtual_usage, AppMemoryUsageLevel* level) {
	SET_BOX(virtual_usage, MemoryManager::AppMemoryUsage); // same as `GetAppMemoryReport()->TotalCommitUsage`;
	SET_BOX(level, MemoryManager::AppMemoryUsageLevel);

	return MemoryManager::GetProcessMemoryReport()->PrivateWorkingSetUsage;
}

float WarGrey::SCADA::system_battery_capacity(float defval_if_no_battery) {
	auto info = Battery::AggregateBattery->GetReport();
	auto maybe_remaining = info->RemainingCapacityInMilliwattHours;
	float capacity = defval_if_no_battery;

	if (maybe_remaining != nullptr) {
		auto remaining = float(info->RemainingCapacityInMilliwattHours->Value);
		auto full = float(info->FullChargeCapacityInMilliwattHours->Value);

		capacity = remaining / full;
	}

	return capacity;
}

Platform::String^ WarGrey::SCADA::system_wifi_ssid(char* signal) {
	auto nics = NetworkInformation::GetConnectionProfiles();
	Platform::String^ ssid = nullptr;

	for (unsigned int i = nics->Size; i > 0; --i) {
		auto nic = nics->GetAt(i - 1U);

		if (nic->IsWlanConnectionProfile) {
			ssid = nic->WlanConnectionProfileDetails->GetConnectedSsid();

			if (ssid != nullptr) {
				SET_BOX(signal, nic->GetSignalBars()->Value);
				break;
			}
		}
	}

	return ssid;
}

Platform::String^ WarGrey::SCADA::system_ipv4_address(Platform::String^ defval_if_no_nic, Platform::String^ subnet_prefix) {
	Platform::String^ ipv4 = defval_if_no_nic;
	Platform::String^ first_one = nullptr;

	try {
		auto names = NetworkInformation::GetHostNames(); // DbgPrint says "invalid parameter passed to C runtime"
		bool matched = false;

		for (unsigned int i = 0; i < names->Size; ++i) {
			auto host = names->GetAt(i);

			if (host->Type == HostNameType::Ipv4) {
				if (subnet_prefix == nullptr) {
					ipv4 = host->RawName;
					matched = true;
					break;
				} else {
					Platform::String^ target = host->RawName;

					if (first_one == nullptr) {
						first_one = target;
					}
					
					if (subnet_prefix->Equals(ref new Platform::String(target->Data(), subnet_prefix->Length()))) {
						ipv4 = target;
						matched = true;
						break;
					}
				}
			}
		}

		if ((!matched) && (first_one != nullptr)) {
			ipv4 = first_one;
		}
	} catch (Platform::Exception^ e) {
		// Do nothing, Stupid Microsoft
		// See: https://github.com/ms-iot/samples/commit/c0da021a312a631c8a26771a0d203e0de80fc597
	}
	
	return ipv4;
}

/*************************************************************************************************/
unsigned int WarGrey::SCADA::system_process_id() {
	return ProcessDiagnosticInfo::GetForCurrentProcess()->ProcessId;
}

/*************************************************************************************************/
private ref class SystemState sealed {
public:
	static SystemState^ get_status_info_provider() {
		static SystemState^ singleton = ref new SystemState();

		return singleton;
	}

internal:
	void add_status_listener(ISystemStatusListener* listener) {
		if (listener != nullptr) {
			char wifi_signal_strength;
			Platform::String^ ssid = system_wifi_ssid(&wifi_signal_strength);

			this->listeners.push_back(listener);

			listener->on_timestamp_changed(update_nowstamp(false));
			listener->on_battery_capacity_changed(system_battery_capacity());
			listener->on_brightness_changed(system_screen_brightness());
			listener->on_wifi_signal_strength_changed(ssid, wifi_signal_strength);
			listener->on_available_storage_changed(0L, 0L);
			listener->on_ipv4_address_changed(system_ipv4_address(nullptr, this->subnet_prefix));
		}
	}

	void set_subnet_prefix(Platform::String^ prefix) {
		if (!this->subnet_prefix->Equals(prefix)) {
			this->subnet_prefix = prefix;
			this->report_ipv4_changed(nullptr);
		}
	}

private:
	void report_timestamp(Platform::Object^ sender, Platform::Object^ e) {
		TimeSpan ts;
		int l00ns;
		Platform::String^ timestamp = update_nowstamp(false, &l00ns);

		ts.Duration = 10000000LL - l00ns;
		this->clock->Interval = ts;

		for (auto listener : this->listeners) {
			listener->on_timestamp_changed(timestamp);
		}

		this->report_available_storage_if_changed();
		this->report_wifiinfo_if_changed();
	}

	void report_powerinfo(Battery^ sender, Platform::Object^ e) {
		float capacity = system_battery_capacity();

		for (auto listener : this->listeners) {
			listener->on_battery_capacity_changed(capacity);
		}
	}

	void report_brightness(BrightnessOverride^ bo, Platform::Object^ e) {
		for (auto listener : this->listeners) {
			listener->on_brightness_changed(bo->BrightnessLevel);
		}
	}

	void report_wifiinfo_if_changed() {
		char signal;
		Platform::String^ ssid = system_wifi_ssid(&signal);

		if ((signal != this->last_wifi_strength)
			|| (ssid != this->last_wifi_ssid)
			|| ((ssid != nullptr) && (!ssid->Equals(this->last_wifi_ssid)))) {
			for (auto listener : this->listeners) {
				listener->on_wifi_signal_strength_changed(ssid, signal);
			}

			this->last_wifi_strength = signal;
			this->last_wifi_ssid = ssid;
		}
	}

	void report_ipv4_changed(Platform::Object^ whocares) {
		Platform::String^ ipv4 = system_ipv4_address(nullptr, this->subnet_prefix);

		for (auto listener : this->listeners) {
			listener->on_ipv4_address_changed(ipv4);
		}
	}

	void report_available_storage_if_changed() {
		static Vector<Platform::String^>^ properties = ref new Vector<Platform::String^>();
		StorageFolder^ local = ApplicationData::Current->LocalFolder;

		if (properties->Size == 0) {
			properties->Append("System.FreeSpace");
			properties->Append("System.Capacity");
		}

		create_task(local->Properties->RetrievePropertiesAsync(properties)).then([=](task<IMap<Platform::String^, Platform::Object^>^> t) {
			try {
				IMap<Platform::String^, Platform::Object^>^ ps = t.get();
				uint64 fs = safe_cast<uint64>(ps->Lookup("System.FreeSpace"));
				uint64 ts = safe_cast<uint64>(ps->Lookup("System.Capacity"));

				if (fs != this->last_freespace) {
					for (auto listener : this->listeners) {
						listener->on_available_storage_changed(fs, ts);
					}

					this->last_freespace = fs;
				}
			} catch (task_canceled&) {
			} catch (Platform::Exception^ e) {
				syslog(Log::Warning, e->Message);
			}
		});
	}

private:
	SystemState() {
		BrightnessOverride^ bo = BrightnessOverride::GetForCurrentView();
		
		Battery::AggregateBattery->ReportUpdated += ref new BatteryUpdateHandler(this, &SystemState::report_powerinfo);
		NetworkInformation::NetworkStatusChanged += ref new NetworkStatusChangedEventHandler(this, &SystemState::report_ipv4_changed);
		
		bo->BrightnessLevelChanged += ref new TypedEventHandler<BrightnessOverride^, Platform::Object^>(this, &SystemState::report_brightness);

		this->clock = ref new DispatcherTimer();
		this->clock->Tick += ref new EventHandler<Object^>(this, &SystemState::report_timestamp);
		this->report_timestamp(nullptr, nullptr);
		this->clock->Start();
	}

	~SystemState() {
		this->clock->Stop();
	}


private:
	std::list<ISystemStatusListener*> listeners;
	DispatcherTimer^ clock;

private:
	Platform::String^ last_wifi_ssid = nullptr;
	Platform::String^ subnet_prefix = nullptr;
	char last_wifi_strength = -1;
	long long last_freespace = -1L;
};

void WarGrey::SCADA::register_system_status_listener(ISystemStatusListener* listener) {
	SystemState::get_status_info_provider()->add_status_listener(listener);
}

void WarGrey::SCADA::system_set_subnet_prefix(Platform::String^ prefix) {
	SystemState::get_status_info_provider()->set_subnet_prefix(prefix);
}
