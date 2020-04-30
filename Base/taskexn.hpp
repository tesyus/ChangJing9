#pragma once

#include <exception>

#include "syslog.hpp"

namespace WarGrey::SCADA {
	private class task_terminated : public std::exception {
	public:
		task_terminated() noexcept : exception() {}
	};

	private class task_discarded : public std::exception {
	public:
		task_discarded() noexcept : exception() {}
	};

	/************************************************************************************************/
	void task_fatal();
	void task_fatal(WarGrey::GYDM::Syslog* logger, Platform::String^ message);
	void task_fatal(WarGrey::GYDM::Syslog* logger, const wchar_t *fmt, ...);

	void task_discard();
	void task_discard(WarGrey::GYDM::Syslog* logger, Platform::String^ message);
	void task_discard(WarGrey::GYDM::Syslog* logger, const wchar_t *fmt, ...);
}
