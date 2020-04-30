#pragma once

#include <cstdint>
#include <list>

#include "dbsystem.hpp"
#include "syslog.hpp"

namespace WarGrey::SCADA {
	typedef void mssql_t;
	typedef void mssql_stmt_t;

#define SQLITE_OK   0
#define SQLITE_BUSY 5
#define SQLITE_ROW  100
#define SQLITE_DONE 101

#define SQLITE_TRACE_STMT    0x01
#define SQLITE_TRACE_PROFILE 0x02
#define SQLITE_TRACE_ROW     0x04
#define SQLITE_TRACE_CLOSE   0x08

	typedef int (*mssql_trace_f)(unsigned int, void*, void*, void*);
	typedef int (*mssql_busy_handler_f)(void*, int);

	private enum class MSSQLDataType { Integer = 1, Float = 2, Text = 3, Bytes = 4, Null = 5 };

	private struct MSSQLTableInfo { // for pragma.table_info
		int cid;
		std::string name;
		std::string type;
		bool notnull;
		std::string dflt_value;
		int pk;
	};

	int mssql_default_trace_callback(unsigned int reason, void* pCxt, void* P, void* X);

	private class IMSSQL abstract : public WarGrey::SCADA::IDBSystem {
	public:
		virtual ~IMSSQL() noexcept;

		IMSSQL(WarGrey::GYDM::Syslog* logger);

	public:
		int libversion();

	public:
		virtual std::list<WarGrey::SCADA::MSSQLTableInfo> table_info(const char* name) = 0;
		virtual void set_busy_handler(mssql_busy_handler_f handler = nullptr, void* args = nullptr) = 0;
		virtual void set_busy_handler(int timeout_ms) = 0;

	public:
		virtual std::string filename(const char* dbname = "main") = 0;
		virtual int changes(bool total = false) = 0;
		virtual int64 last_insert_rowid() = 0;

	protected:
		WarGrey::SCADA::IVirtualSQL* new_sql_factory(WarGrey::SCADA::TableColumnInfo* columns, size_t count) override;
	};

	private class MSSQLStatement : public WarGrey::SCADA::IPreparedStatement {
	public:
		virtual ~MSSQLStatement() noexcept;

		MSSQLStatement(WarGrey::SCADA::IMSSQL* db, mssql_stmt_t* stmt);

	public:
		unsigned int parameter_count() override;
		void bind_parameter(unsigned int pid_starts_with_0) override;
		void bind_parameter(unsigned int pid_starts_with_0, int32 v) override;
		void bind_parameter(unsigned int pid_starts_with_0, int64 v) override;
		void bind_parameter(unsigned int pid_starts_with_0, double v) override;
		void bind_parameter(unsigned int pid_starts_with_0, const char* text) override;

	public:
		std::string column_text(unsigned int cid_starts_with_0) override;
		int32 column_int32(unsigned int cid_starts_with_0) override;
		int64 column_int64(unsigned int cid_starts_with_0) override;
		double column_double(unsigned int cid_starts_with_0) override;

	public:
		int column_data_count() override;
		bool column_is_null(unsigned int cid_starts_with_0) override;
		std::string column_database_name(unsigned int cid_starts_with_0) override;
		std::string column_table_name(unsigned int cid_starts_with_0) override;
		std::string column_name(unsigned int cid_starts_with_0) override;
		std::string column_decltype(unsigned int cid_starts_with_0) override;
		MSSQLDataType column_type(unsigned int cid_starts_with_0);

	public:
		std::string description(bool expand = true) override;
		bool step(int* data_count = nullptr, const char* error_src = "step") override;
		void reset(bool reset_bindings = true) override;
		void clear_bindings() override;

	private:
		WarGrey::SCADA::IMSSQL* master;
		WarGrey::SCADA::mssql_stmt_t* stmt;
	};

	private class MSSQL : public WarGrey::SCADA::IMSSQL {
	public:
		virtual ~MSSQL() noexcept;

		MSSQL(const wchar_t* connection = nullptr, WarGrey::GYDM::Syslog* logger = nullptr, mssql_trace_f xCallback = nullptr);

	public:
		std::list<std::string> list_tables() override;
		std::string last_error_message() override;
		int last_errno(int* extended_errno) override;
		WarGrey::SCADA::IPreparedStatement* prepare(const std::string& sql) override;

	public:
		std::list<WarGrey::SCADA::MSSQLTableInfo> table_info(const char* name) override;
		void set_busy_handler(mssql_busy_handler_f handler = nullptr, void* args = nullptr) override;
		void set_busy_handler(int timeout_ms) override;

	public:
		std::string filename(const char* dbname = "main") override;
		int changes(bool total = false) override;
		int64 last_insert_rowid() override;

	private:
		WarGrey::SCADA::mssql_t* db;
	};
}
