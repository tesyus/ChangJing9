#pragma once

#include <Windows.h>

#include "syslog.hpp"

namespace WarGrey::SCADA {
#define win32_fetch(m, fobj, type_t, logger) fobj = (type_t)win32_fetch_foreign_object(m, #fobj, logger)

    typedef int (*_fun__int)();
    typedef void(*_fun__void)();
    typedef void(*_fun_destructor)(void*);

    HMODULE win32_load_foreign_library(Platform::String^ dllname, WarGrey::GYDM::Syslog* logger = nullptr);
    bool win32_unload_foreign_library(HMODULE m, WarGrey::GYDM::Syslog* logger = nullptr);
    FARPROC win32_fetch_foreign_object(HMODULE m, LPCSTR name, WarGrey::GYDM::Syslog* logger = nullptr);

    Platform::String^ win32_last_strerror();
}
