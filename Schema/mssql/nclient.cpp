#include <map>

#include "sqlite3/vsqlite3.hpp"
#include "sqlite3/sqlite_master.hpp"

#include "mssql/nclient.hpp"

#include "datum/box.hpp"
#include "datum/string.hpp"

#include "win32.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::GYDM;

typedef int (*_fun__wchar__sqlite3__int)(const wchar_t*, mssql_t**);
typedef const char* (*_fun__sqlite3__char)(mssql_t*);
typedef int (*_fun__sqlite3__int)(mssql_t*);
typedef int64 (*_fun__sqlite3__int64)(mssql_t*);
typedef int(*_fun__sqlite3__uint__trace__void__int)(mssql_t*, unsigned int, mssql_trace_f, void*);
typedef int(*_fun__sqlite3__uint__busy_handler__void__int)(mssql_t*, mssql_busy_handler_f, void*);
typedef const char* (*_fun__sqlite3__char__char)(mssql_t*, const char*);

typedef int (*_fun__sqlite3__char__stmt__void__int)(mssql_t*, const char*, size_t, mssql_stmt_t**, const void**);
typedef int (*_fun__stmt__int)(mssql_stmt_t*);
typedef int (*_fun__stmt__int__int)(mssql_stmt_t*, int);
typedef int (*_fun__stmt__int__int32__int)(mssql_stmt_t*, int, int32);
typedef int (*_fun__stmt__int__int64__int)(mssql_stmt_t*, int, int64);
typedef int (*_fun__stmt__int__real__int)(mssql_stmt_t*, int, double);
typedef int (*_fun__stmt__int__blob__fptr__int)(mssql_stmt_t*, int, const void*, size_t, _fun_destructor);
typedef int (*_fun__stmt__int__char__fptr__int)(mssql_stmt_t*, int, const char*, size_t, _fun_destructor);

typedef const void* (*_fun__stmt__int__blob)(mssql_stmt_t*, int);
typedef const char* (*_fun__stmt__int__char)(mssql_stmt_t*, int);
typedef int32 (*_fun__stmt__int__int32)(mssql_stmt_t*, int);
typedef int64 (*_fun__stmt__int__int64)(mssql_stmt_t*, int);
typedef double (*_fun__stmt__int__real)(mssql_stmt_t*, int);

typedef const char* (*_fun__stmt__char)(mssql_stmt_t*);
typedef char* (*_fun__stmt__malloc)(mssql_stmt_t*);

static HMODULE sqlnclient = nullptr;
static int references = 0;

static _fun__int sqlite3_libversion_number;
static _fun__wchar__sqlite3__int sqlite3_open16;
static _fun__sqlite3__int sqlite3_close;
static _fun_destructor sqlite3_free;
static _fun__sqlite3__char sqlite3_errmsg;
static _fun__sqlite3__uint__trace__void__int sqlite3_trace_v2;
static _fun__sqlite3__uint__busy_handler__void__int sqlite3_busy_handler;
static _fun__stmt__int__int sqlite3_busy_timeout;
static _fun__sqlite3__char__char sqlite3_db_filename;

static _fun__sqlite3__char__stmt__void__int sqlite3_prepare_v2;
static _fun__stmt__int sqlite3_reset;
static _fun__stmt__int sqlite3_step;
static _fun__stmt__int sqlite3_finalize;

static _fun__stmt__int sqlite3_bind_parameter_count;
static _fun__stmt__int sqlite3_clear_bindings;
static _fun__stmt__int__int sqlite3_bind_null;
static _fun__stmt__int__int32__int sqlite3_bind_int;
static _fun__stmt__int__int64__int sqlite3_bind_int64;
static _fun__stmt__int__real__int sqlite3_bind_double;
static _fun__stmt__int__blob__fptr__int sqlite3_bind_blob;
static _fun__stmt__int__char__fptr__int sqlite3_bind_text;

static _fun__stmt__int sqlite3_data_count;
static _fun__stmt__int sqlite3_column_count;
static _fun__stmt__int__char sqlite3_column_database_name;
static _fun__stmt__int__char sqlite3_column_table_name;
static _fun__stmt__int__char sqlite3_column_origin_name;
static _fun__stmt__int__char sqlite3_column_decltype;
static _fun__stmt__int__int sqlite3_column_bytes;
static _fun__stmt__int__int sqlite3_column_type;
static _fun__stmt__int__blob sqlite3_column_blob;
static _fun__stmt__int__char sqlite3_column_text;
static _fun__stmt__int__int32 sqlite3_column_int;
static _fun__stmt__int__int64 sqlite3_column_int64;
static _fun__stmt__int__real sqlite3_column_double;

static _fun__stmt__char sqlite3_sql;
static _fun__stmt__malloc sqlite3_expanded_sql;

static _fun__sqlite3__int sqlite3_changes;
static _fun__sqlite3__int sqlite3_total_changes;
static _fun__sqlite3__int sqlite3_errcode;
static _fun__sqlite3__int sqlite3_extended_errcode;
static _fun__sqlite3__int64 sqlite3_last_insert_rowid;

static const _fun_destructor SQLITE_STATIC    = ((_fun_destructor)0);
static const _fun_destructor SQLITE_TRANSIENT = ((_fun_destructor)-1);

static void load_sqlnclient(Syslog* logger) {
	if (sqlnclient == nullptr) {
		sqlnclient = win32_load_foreign_library("sqlncli11", logger);

		if (sqlnclient != nullptr) {
			win32_fetch(sqlnclient, sqlite3_libversion_number, _fun__int, logger);
			win32_fetch(sqlnclient, sqlite3_open16, _fun__wchar__sqlite3__int, logger);
			win32_fetch(sqlnclient, sqlite3_close, _fun__sqlite3__int, logger);
			win32_fetch(sqlnclient, sqlite3_free, _fun_destructor, logger);
			win32_fetch(sqlnclient, sqlite3_errmsg, _fun__sqlite3__char, logger);
			win32_fetch(sqlnclient, sqlite3_errcode, _fun__sqlite3__int, logger);
			win32_fetch(sqlnclient, sqlite3_extended_errcode, _fun__sqlite3__int, logger);
			win32_fetch(sqlnclient, sqlite3_trace_v2, _fun__sqlite3__uint__trace__void__int, logger);
			win32_fetch(sqlnclient, sqlite3_busy_handler, _fun__sqlite3__uint__busy_handler__void__int, logger);
			win32_fetch(sqlnclient, sqlite3_busy_timeout, _fun__stmt__int__int, logger);
			win32_fetch(sqlnclient, sqlite3_db_filename, _fun__sqlite3__char__char, logger);

			win32_fetch(sqlnclient, sqlite3_prepare_v2, _fun__sqlite3__char__stmt__void__int, logger);
			win32_fetch(sqlnclient, sqlite3_reset, _fun__stmt__int, logger);
			win32_fetch(sqlnclient, sqlite3_step, _fun__stmt__int, logger);
			win32_fetch(sqlnclient, sqlite3_finalize, _fun__stmt__int, logger);

			win32_fetch(sqlnclient, sqlite3_bind_parameter_count, _fun__stmt__int, logger);
			win32_fetch(sqlnclient, sqlite3_clear_bindings, _fun__stmt__int, logger);
			win32_fetch(sqlnclient, sqlite3_bind_null, _fun__stmt__int__int, logger);
			win32_fetch(sqlnclient, sqlite3_bind_int, _fun__stmt__int__int32__int, logger);
			win32_fetch(sqlnclient, sqlite3_bind_int64, _fun__stmt__int__int64__int, logger);
			win32_fetch(sqlnclient, sqlite3_bind_double, _fun__stmt__int__real__int, logger);
			win32_fetch(sqlnclient, sqlite3_bind_blob, _fun__stmt__int__blob__fptr__int, logger);
			win32_fetch(sqlnclient, sqlite3_bind_text, _fun__stmt__int__char__fptr__int, logger);

			win32_fetch(sqlnclient, sqlite3_data_count, _fun__stmt__int, logger);
			win32_fetch(sqlnclient, sqlite3_column_count, _fun__stmt__int, logger);
			win32_fetch(sqlnclient, sqlite3_column_database_name, _fun__stmt__int__char, logger);
			win32_fetch(sqlnclient, sqlite3_column_table_name, _fun__stmt__int__char, logger);
			win32_fetch(sqlnclient, sqlite3_column_origin_name, _fun__stmt__int__char, logger);
			win32_fetch(sqlnclient, sqlite3_column_decltype, _fun__stmt__int__char, logger);
			win32_fetch(sqlnclient, sqlite3_column_bytes, _fun__stmt__int__int, logger);
			win32_fetch(sqlnclient, sqlite3_column_type, _fun__stmt__int__int, logger);
			win32_fetch(sqlnclient, sqlite3_column_blob, _fun__stmt__int__blob, logger);
			win32_fetch(sqlnclient, sqlite3_column_text, _fun__stmt__int__char, logger);
			win32_fetch(sqlnclient, sqlite3_column_int, _fun__stmt__int__int32, logger);
			win32_fetch(sqlnclient, sqlite3_column_int64, _fun__stmt__int__int64, logger);
			win32_fetch(sqlnclient, sqlite3_column_double, _fun__stmt__int__real, logger);

			win32_fetch(sqlnclient, sqlite3_sql, _fun__stmt__char, logger);
			win32_fetch(sqlnclient, sqlite3_expanded_sql, _fun__stmt__malloc, logger);

			win32_fetch(sqlnclient, sqlite3_changes, _fun__sqlite3__int, logger);
			win32_fetch(sqlnclient, sqlite3_total_changes, _fun__sqlite3__int, logger);
			win32_fetch(sqlnclient, sqlite3_last_insert_rowid, _fun__sqlite3__int64, logger);
		}
	}

	references += 1;
}

static void unload_sqlnclient(Syslog* logger) {
	if (sqlnclient != nullptr) {
		references -= 1;

		if (references <= 0) {
			win32_unload_foreign_library(sqlnclient, logger);
			sqlnclient = nullptr;
		}
	}
}

static std::string sqlite3_statement_description(mssql_stmt_t* stmt, bool expand) {
	static size_t buffer_size = 64;
	char* raw = (expand ? sqlite3_expanded_sql(stmt) : (char *)(sqlite3_sql(stmt)));
	std::string desc(raw);
	
	if (expand) {
		sqlite3_free(raw);
	}

	return desc;
}

static inline std::string make_safe_bytes(const char* blob) {
	return ((blob == nullptr) ? std::string() : std::string(blob));
}

static inline std::wstring make_safe_wstring(const wchar_t* text) {
	return ((text == nullptr) ? std::wstring() : std::wstring(text));
}

/*************************************************************************************************/
int WarGrey::SCADA::mssql_default_trace_callback(unsigned int type, void* pCxt, void* P, void* X) {
	IMSSQL* self = static_cast<IMSSQL*>(pCxt);

	switch (type) {
	case SQLITE_TRACE_STMT: {
		self->get_logger()->log_message(Log::Debug, L"EXEC: %S", sqlite3_statement_description(P, true).c_str());
	}; break;
	case SQLITE_TRACE_PROFILE: { /* TODO */ }; break;
	default: /* no useful information */;
	}
	
	return 0;
}

IMSSQL::IMSSQL(Syslog* logger)
	: IDBSystem(DBMS::SQLite3, (logger == nullptr) ? make_system_logger(DBMS::SQLite3.ToString()) : logger) {}

IMSSQL::~IMSSQL() {}

int IMSSQL::libversion() {
	return sqlite3_libversion_number();
}

IVirtualSQL* IMSSQL::new_sql_factory(TableColumnInfo* columns, size_t count) {
	return new VirtualSQLite3(columns, count, this->libversion());
}

/*************************************************************************************************/
MSSQL::MSSQL(const wchar_t* dbfile, Syslog* logger, mssql_trace_f xCallback) : IMSSQL(logger) {
	const wchar_t* database = ((dbfile == nullptr) ? L":memory:" : dbfile);

	load_sqlnclient(this->get_logger());

	if (sqlite3_open16(database, &this->db) != SQLITE_OK) {
		this->report_error("failed to connect to [%S]", database);
	} else {
		mssql_trace_f cb = ((xCallback == nullptr) ? mssql_default_trace_callback : xCallback);
		unsigned int uMask = SQLITE_TRACE_STMT | SQLITE_TRACE_PROFILE | SQLITE_TRACE_ROW | SQLITE_TRACE_CLOSE;

		sqlite3_trace_v2(this->db, uMask, cb, this);

		this->get_logger()->log_message(Log::Debug, L"connected to [%s]", database);
	}
}

MSSQL::~MSSQL() {
	std::string filename = this->filename();
	std::string database = ((filename.length() == 0) ? ":memory:" : filename);

	if (sqlite3_close(this->db) != SQLITE_OK) {
		this->report_warning("failed to disconnect [%s]", database.c_str());
	} else {
		this->get_logger()->log_message(Log::Debug, L"disconnected [%S]", database.c_str());
	}

	unload_sqlnclient(this->get_logger());
}

std::string MSSQL::filename(const char* dbname) {
	return make_safe_bytes(sqlite3_db_filename(this->db, dbname));
}

IPreparedStatement* MSSQL::prepare(const std::string& raw) {
	const char* sql = raw.c_str();
	mssql_stmt_t* prepared_stmt;
	MSSQLStatement* stmt = nullptr;

	if (sqlite3_prepare_v2(this->db, sql, strlen(sql), &prepared_stmt, nullptr) == SQLITE_OK) {
		stmt = new MSSQLStatement(this, prepared_stmt);
	} else {
		this->report_error("prepare");
	}

	return stmt;
}

std::list<std::string> MSSQL::list_tables() {
	std::list<std::string> tables;
	std::list<SQLiteMaster> all = select_sqlite_master(this);

	for (auto it = all.begin(); it != all.end(); it++) {
		SQLiteMaster m = (*it);

		if (m.type.compare("table") == 0) {
			tables.push_back(m.name);

			this->get_logger()->log_message(Log::Debug, L"found table '%S'", m.name.c_str());
		}
	}

	return tables;
}

std::list<MSSQLTableInfo> MSSQL::table_info(const char* name) {
	MSSQLStatement* pragma = static_cast<MSSQLStatement*>(IDBSystem::prepare("PRAGMA table_info = %s;", name));
	std::list<MSSQLTableInfo> infos;
	
	if (pragma != nullptr) {
		while (pragma->step()) {
			MSSQLTableInfo info;
			MSSQLDataType type = pragma->column_type(2);

			info.cid = pragma->column_int32(0);
			info.name = pragma->column_text(1);
			info.type = pragma->column_text(2);
			info.notnull = (pragma->column_int32(3) != 0);
			info.dflt_value = pragma->column_text(4);
			info.pk = pragma->column_int32(5);

			this->get_logger()->log_message(Log::Debug, L"Column[%d] %S[%s]: %S, %s, %d; %S",
				info.cid, info.name.c_str(), type.ToString()->Data(), info.type.c_str(),
				info.notnull.ToString()->Data(), info.pk, info.dflt_value.c_str());

			infos.push_back(info);
		}

		delete pragma;
	}

	return infos;
}

int MSSQL::changes(bool total) {
	int changes = 0;
	
	if (total) {
		changes = sqlite3_total_changes(this->db);
	} else {
		changes = sqlite3_changes(this->db);
	}

	return changes;
}

int64 MSSQL::last_insert_rowid() {
	return sqlite3_last_insert_rowid(this->db);
}

int MSSQL::last_errno(int* extended_errno) {
	SET_BOX(extended_errno, sqlite3_extended_errcode(this->db));

	return sqlite3_errcode(this->db);
}

std::string MSSQL::last_error_message() {
	return make_safe_bytes(sqlite3_errmsg(this->db));
}

void MSSQL::set_busy_handler(mssql_busy_handler_f handler, void* args) {
	sqlite3_busy_handler(this->db, handler, args);
}

void MSSQL::set_busy_handler(int timeout_ms) {
	sqlite3_busy_timeout(this->db, timeout_ms);
}

/*************************************************************************************************/
MSSQLStatement::MSSQLStatement(IMSSQL* db, mssql_stmt_t* stmt)
	: IPreparedStatement(DBMS::SQLite3), stmt(stmt), master(db) {}

MSSQLStatement::~MSSQLStatement() {
	// sqlite3_finalize() returns error code of last statement execution, not of the finalization.
	// so there is no need to check the return value;
	sqlite3_finalize(this->stmt);
}

void MSSQLStatement::reset(bool clear) {
	// sqlite3_finalize() returns error code of last statement execution, not of the finalization.
	// so there is no need to check the return value;
	sqlite3_reset(this->stmt);

	if (clear) {
		this->clear_bindings();
	}
}

void MSSQLStatement::clear_bindings() {
	sqlite3_clear_bindings(this->stmt);
}

std::string MSSQLStatement::description(bool expand) {
	return sqlite3_statement_description(this->stmt, expand);
}

bool MSSQLStatement::step(int* data_count, const char* error_src) {
	bool notdone = true;
	
	switch (sqlite3_step(this->stmt)) {
	case SQLITE_DONE: SET_BOX(data_count, 0); notdone = false; break;
	case SQLITE_ROW: SET_BOX(data_count, this->column_data_count()); break;
	default: this->master->report_error(error_src);
	}

	return notdone;
}

int MSSQLStatement::column_data_count() {
	return sqlite3_data_count(this->stmt);
}

bool MSSQLStatement::column_is_null(unsigned int cid) {
	return this->column_type(cid) == MSSQLDataType::Null;
}

std::string MSSQLStatement::column_database_name(unsigned int cid) {
	return make_safe_bytes(sqlite3_column_database_name(this->stmt, cid));
}

std::string MSSQLStatement::column_table_name(unsigned int cid) {
	return make_safe_bytes(sqlite3_column_table_name(this->stmt, cid));
}

std::string MSSQLStatement::column_name(unsigned int cid) {
	return make_safe_bytes(sqlite3_column_origin_name(this->stmt, cid));
}

std::string MSSQLStatement::column_decltype(unsigned int cid) {
	return make_safe_bytes(sqlite3_column_decltype(this->stmt, cid));
}

MSSQLDataType MSSQLStatement::column_type(unsigned int cid) {
	return static_cast<MSSQLDataType>(sqlite3_column_type(this->stmt, cid));
}

std::string MSSQLStatement::column_text(unsigned int cid) {
	return make_safe_bytes(sqlite3_column_text(this->stmt, cid));
}

int32 MSSQLStatement::column_int32(unsigned int cid) {
	return sqlite3_column_int(this->stmt, cid);
}

int64 MSSQLStatement::column_int64(unsigned int cid) {
	return sqlite3_column_int64(this->stmt, cid);
}

double MSSQLStatement::column_double(unsigned int cid) {
	return sqlite3_column_double(this->stmt, cid);
}

unsigned int MSSQLStatement::parameter_count() {
	return sqlite3_bind_parameter_count(this->stmt);
}

void MSSQLStatement::bind_parameter(unsigned int pid) {
	if (sqlite3_bind_null(this->stmt, pid + 1) != SQLITE_OK) {
		this->master->report_error("bind_null");
	}
}

void MSSQLStatement::bind_parameter(unsigned int pid, int32 v) {
	if (sqlite3_bind_int(this->stmt, pid + 1, v) != SQLITE_OK) {
		this->master->report_error("bind_int32");
	}
}

void MSSQLStatement::bind_parameter(unsigned int pid, int64 v) {
	if (sqlite3_bind_int64(this->stmt, pid + 1, v) != SQLITE_OK) {
		this->master->report_error("bind_int64");
	}
}

void MSSQLStatement::bind_parameter(unsigned int pid, double v) {
	if (sqlite3_bind_double(this->stmt, pid + 1, v) != SQLITE_OK) {
		this->master->report_error("bind_double");
	}
}

void MSSQLStatement::bind_parameter(unsigned int pid, const char* v) {
	if (sqlite3_bind_text(this->stmt, pid + 1, v, strlen(v), SQLITE_STATIC) != SQLITE_OK) {
		this->master->report_error("bind_text");
	}
}
