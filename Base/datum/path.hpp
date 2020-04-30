#pragma once

namespace WarGrey::SCADA {
	int path_next_slash_position(Platform::String^ path, int start = 0, int fallback = -1);

	Platform::String^ path_only(Windows::Foundation::Uri^ uri);
	Platform::String^ path_only(Platform::String^ path);

	Platform::String^ file_name_from_path(Windows::Foundation::Uri^ uri);
	Platform::String^ file_name_from_path(Platform::String^ path);

	Platform::String^ file_basename_from_path(Windows::Foundation::Uri^ uri);
	Platform::String^ file_basename_from_path(Platform::String^ path);

	Platform::String^ file_extension_from_path(Windows::Foundation::Uri^ uri);
	Platform::String^ file_extension_from_path(Platform::String^ path);

	Platform::String^ file_basename_from_second(long long utc_s, bool locale = true);

	Windows::Foundation::Uri^ ms_appx_file(Platform::String^ file, Platform::String^ ext, Platform::String^ rootdir = "graphlet");
	Windows::Foundation::Uri^ ms_appdata_file(Platform::String^ file, Platform::String^ ext, Platform::String^ rootdir = "");
	Platform::String^ ms_apptemp_file(Platform::String^ file, Platform::String^ ext);
}
