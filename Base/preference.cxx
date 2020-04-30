#include "preference.hxx"

using namespace WarGrey::SCADA;

using namespace Windows::Storage;

/*************************************************************************************************/
ApplicationDataContainer^ WarGrey::SCADA::make_preference_zone(Platform::String^ name, ApplicationDataContainer^ parent) {
	ApplicationDataCreateDisposition adcd = ApplicationDataCreateDisposition::Always;
	ApplicationDataContainer^ zone = nullptr;

	if (parent == nullptr) {
		zone = ApplicationData::Current->LocalSettings->CreateContainer(name, adcd);
	} else {
		parent->CreateContainer(name, adcd);
	}

	return zone;
}

bool WarGrey::SCADA::has_preference(Platform::String^ name, ApplicationDataContainer^ pref) {
	ApplicationDataContainer^ zone = ((pref == nullptr) ? ApplicationData::Current->LocalSettings : pref);

	return zone->Values->HasKey(name);
}

void WarGrey::SCADA::put_preference(Platform::String^ name, Platform::Object^ value, ApplicationDataContainer^ pref) {
	ApplicationDataContainer^ zone = ((pref == nullptr) ? ApplicationData::Current->LocalSettings : pref);

	if (value != nullptr) {
		zone->Values->Insert(name, value); // insert or replace
	} else if (zone->Values->HasKey(name)) {
		zone->Values->Remove(name);
	}
}

Platform::Object^ WarGrey::SCADA::get_preference(Platform::String^ name, ApplicationDataContainer^ pref) {
	ApplicationDataContainer^ zone = ((pref == nullptr) ? ApplicationData::Current->LocalSettings : pref);

	return zone->Values->Lookup(name);
}

Platform::Object^ WarGrey::SCADA::get_preference(Platform::String^ name, Platform::Object^ defvalue, ApplicationDataContainer^ pref) {
	ApplicationDataContainer^ zone = ((pref == nullptr) ? ApplicationData::Current->LocalSettings : pref);
	Platform::Object^ value = defvalue;

	if (zone->Values->HasKey(name)) {
		value = zone->Values->Lookup(name);
	}

	return value;
}
