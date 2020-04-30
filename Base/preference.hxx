#pragma once

namespace WarGrey::SCADA {
	Windows::Storage::ApplicationDataContainer^ make_preference_zone(Platform::String^ name, Windows::Storage::ApplicationDataContainer^ parent = nullptr);
	
	bool has_preference(Platform::String^ name, Windows::Storage::ApplicationDataContainer^ zone = nullptr);
	void put_preference(Platform::String^ name, Platform::Object^ value, Windows::Storage::ApplicationDataContainer^ zone = nullptr);
	
	Platform::Object^ get_preference(Platform::String^ name, Windows::Storage::ApplicationDataContainer^ zone = nullptr);
	Platform::Object^ get_preference(Platform::String^ name, Platform::Object^ defvalue, Windows::Storage::ApplicationDataContainer^ zone = nullptr);

	template<typename T>
	T get_preference(Platform::String^ name, T defvalue, Windows::Storage::ApplicationDataContainer^ zone = nullptr) {
		return (T)WarGrey::SCADA::get_preference(name, (Platform::Object^)defvalue, zone);
	}
}
