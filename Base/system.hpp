#pragma once

namespace WarGrey::SCADA {
	private class ISystemStatusListener abstract {
	public:
		virtual void on_timestamp_changed(Platform::String^ timestamp) {}
		virtual void on_battery_capacity_changed(float capacity) {}
		virtual void on_brightness_changed(double brightness) {}
		virtual void on_available_storage_changed(unsigned long long free_bytes, unsigned long long total_bytes) {}
		virtual void on_ipv4_address_changed(Platform::String^ ipv4) {}
		virtual void on_wifi_signal_strength_changed(Platform::String^ ssid, char strength) {}
	};

	void register_system_status_listener(WarGrey::SCADA::ISystemStatusListener* listener);

	Windows::Foundation::Size adjusted_workspace_size(Windows::Foundation::Rect region, Windows::UI::Xaml::FrameworkElement^ ws);
	Windows::Foundation::Size system_screen_size();

	Windows::UI::Color system_color(Windows::UI::ViewManagement::UIColorType type);
	Windows::UI::Color system_color(Windows::UI::ViewManagement::UIElementType type);

	double system_screen_brightness(double defval_if_not_available = 0.0);
	void system_try_change_screen_brightness(double brightness, Windows::Graphics::Display::DisplayBrightnessOverrideOptions opt
		= Windows::Graphics::Display::DisplayBrightnessOverrideOptions::UseDimmedPolicyWhenBatteryIsLow);

	unsigned long long system_physical_memory_usage(unsigned long long* virtual_usage = nullptr, Windows::System::AppMemoryUsageLevel* level = nullptr);
	float system_battery_capacity(float defval_if_no_battery = 1.0F);
	Platform::String^ system_wifi_ssid(char* signal_strength = nullptr);
	Platform::String^ system_ipv4_address(Platform::String^ defval_if_no_nic = nullptr, Platform::String^ subnet_prefix = nullptr);
	void system_set_subnet_prefix(Platform::String^ subnet_prefix);

	unsigned int system_process_id();

	void ui_thread_initialize();
	bool ui_thread_accessed();
	Windows::Foundation::IAsyncAction^ ui_thread_run_async(Windows::UI::Core::DispatchedHandler^ task,
		Windows::UI::Core::CoreDispatcherPriority priority = Windows::UI::Core::CoreDispatcherPriority::Normal);

	template<typename L>
	Windows::Foundation::IAsyncAction^ ui_thread_run_async(L task, Windows::UI::Core::CoreDispatcherPriority priority = Windows::UI::Core::CoreDispatcherPriority::Normal) {
		return ui_thread_run_async(ref new Windows::UI::Core::DispatchedHandler(task), priority);
	}
}
