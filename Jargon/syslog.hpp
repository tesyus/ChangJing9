#pragma once

#include "syslog/logging.hpp"
#include "syslog/metainfo.hpp"
#include "syslog/receiver/rsyslog.hpp"
#include "syslog/receiver/windows.hpp"
#include "syslog/receiver/vstudio.hpp"

namespace WarGrey::GYDM {
    void set_default_logging_level(WarGrey::GYDM::Log level = WarGrey::GYDM::Log::Debug);
    void set_default_logging_topic(Platform::String^ topic = "WinSCADA");
    void set_default_rsyslog_target(Platform::String^ ipv4, unsigned short port = 0U);

    WarGrey::GYDM::Syslog* default_logger();
    WarGrey::GYDM::Syslog* make_silent_logger(Platform::String^ topic);
    WarGrey::GYDM::Syslog* make_system_logger(Platform::String^ topic);
    WarGrey::GYDM::Syslog* make_system_logger(WarGrey::GYDM::Log level, Platform::String^ topic);
    WarGrey::GYDM::Syslog* make_logger(WarGrey::GYDM::Log level, Platform::String^ topic, WarGrey::GYDM::Syslog* parent = nullptr);

    WarGrey::GYDM::RsyslogReceiver* default_rsyslog_logging_receiver();
    WarGrey::GYDM::WindowsReceiver* default_windows_logging_receiver();

    void syslog(WarGrey::GYDM::Log level, Platform::String^ message);
    void syslog(WarGrey::GYDM::Log level, const wchar_t* fmt, ...);

#define declare_syslog(level) \
    void syslog_##level(const wchar_t *fmt, ...); \
    void syslog_##level(Platform::String^ message);

    declare_syslog(debug)
    declare_syslog(info)
    declare_syslog(notice)
    declare_syslog(warning)
    declare_syslog(error)
    declare_syslog(critical)
    declare_syslog(alert)
    declare_syslog(panic)

#undef declare_syslog
}
