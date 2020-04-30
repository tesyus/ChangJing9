#pragma once

#include <ppltasks.h>
#include <mutex>
#include <map>

#include "datum/string.hpp"
#include "datum/path.hpp"

#include "syslog.hpp"

namespace WarGrey::SCADA {
	template<class FileType>
	private class IMsAppdata abstract {
	public:
		virtual ~IMsAppdata() noexcept {
			this->shared_task.cancel();
		}

	protected:
		virtual void on_appdata(Windows::Foundation::Uri^ ms_appdata, FileType^ ftobject) = 0;
		virtual void on_appdata_notify(Windows::Foundation::Uri^ ms_appdata, FileType^ ftobject) {}

		virtual FileType^ on_appdata_not_found(Windows::Foundation::Uri^ ms_appdata) {
			this->log_message(WarGrey::GYDM::Log::Error,
				make_wstring(L"failed to load %s: file does not exist",
					ms_appdata->ToString()->Data()));

			return nullptr;
		}

	protected:
		virtual void log_message(WarGrey::GYDM::Log level, Platform::String^ message) {
			syslog(level, message);
		}

		virtual Windows::Foundation::IAsyncOperation<FileType^>^ read(Platform::String^ path) {
			return Concurrency::create_async([=] {
				return FileType::load(path);
			});
		}

		virtual Windows::Foundation::IAsyncOperation<bool>^ write(FileType^ ftobject, Platform::String^ path) {
			return Concurrency::create_async([=] {
				return FileType::save(ftobject, path);
			});
		}

	protected:
		void load(Windows::Foundation::Uri^ ms_appdata, Platform::String^ file_type = "configuration data") {
			auto uuid = ms_appdata->ToString()->GetHashCode();

			IMsAppdata<FileType>::critical_sections[uuid].lock();
			auto item = IMsAppdata<FileType>::databases.find(uuid);

			IMsAppdata<FileType>::lists[uuid].insert(std::pair<IMsAppdata<FileType>*, bool>(this, true));

			if (item == IMsAppdata<FileType>::databases.end()) {
				IMsAppdata<FileType>::databases.insert(std::pair<int, bool>(uuid, false));
				this->load_async(uuid, ms_appdata, file_type);
			} else if (item->second == true) {
				this->do_notify(ms_appdata, IMsAppdata<FileType>::filesystem[uuid]);

				this->log_message(WarGrey::GYDM::Log::Debug,
					make_wstring(L"reused the %s: %s with reference count %d",
						file_type->Data(), ms_appdata->ToString()->Data(),
						IMsAppdata<FileType>::lists[uuid].size()));
			} else {
				this->log_message(WarGrey::GYDM::Log::Debug,
					make_wstring(L"waiting for the %s: %s",
						file_type->Data(), ms_appdata->ToString()->Data()));
			}
			IMsAppdata<FileType>::critical_sections[uuid].unlock();
		}

		void store(Windows::Foundation::Uri^ ms_appdata, FileType^ ftobject, Platform::String^ file_type = "configuration data") {
			auto uuid = ms_appdata->ToString()->GetHashCode();

			IMsAppdata<FileType>::critical_sections[uuid].lock();
			auto item = IMsAppdata<FileType>::databases.find(uuid);

			this->store_async(Windows::Storage::ApplicationData::Current->LocalFolder,
				ms_appdata->Path, ftobject, file_type, 7 /* skip "/local/" */);

			if (item == IMsAppdata<FileType>::databases.end()) {
				IMsAppdata<FileType>::databases.insert(std::pair<int, bool>(uuid, true));
			} else {
				IMsAppdata<FileType>::databases[uuid] = true;
			}

			if (IMsAppdata<FileType>::filesystem.find(uuid) == IMsAppdata<FileType>::filesystem.end()) {
				IMsAppdata<FileType>::filesystem.insert(std::pair<int, FileType^>(uuid, ftobject));
			} else {
				IMsAppdata<FileType>::filesystem[uuid]->refresh(ftobject);
			}

			IMsAppdata<FileType>::lists[uuid][this] = true;
			
			{ // do refreshing
				this->broadcast(ms_appdata, uuid, ftobject);
				
				this->log_message(WarGrey::GYDM::Log::Debug,
					make_wstring(L"refreshed the %s: %s with reference count %d",
						file_type->Data(), ms_appdata->ToString()->Data(),
						IMsAppdata<FileType>::lists[uuid].size()));
			}

			IMsAppdata<FileType>::critical_sections[uuid].unlock();
		}

		void unload(Windows::Foundation::Uri^ ms_appdata) {
			auto uuid = ms_appdata->ToString()->GetHashCode();

			IMsAppdata<FileType>::critical_sections[uuid].lock();

			auto item = IMsAppdata<FileType>::lists.find(uuid);
			
			if (item != IMsAppdata<FileType>::lists.end()) {
				auto ls = item->second;
				auto self = ls.find(this);

				if (self != ls.end()) {
					ls.erase(self);
				}

				if (ls.size() < 1) {
					IMsAppdata<FileType>::databases.erase(uuid);
					IMsAppdata<FileType>::filesystem.erase(uuid);
					IMsAppdata<FileType>::lists.erase(uuid);
				}
			}

			IMsAppdata<FileType>::critical_sections[uuid].unlock();
		}

	private:
		void load_async(int uuid, Windows::Foundation::Uri^ ms_appdata, Platform::String^ file_type) {
			auto token = this->shared_task.get_token();
			auto get_file = Concurrency::create_task(Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(ms_appdata), token);

			get_file.then([=](Concurrency::task<Windows::Storage::StorageFile^> sfile) {
				Windows::Storage::StorageFile^ file = sfile.get(); // Stupid Microsoft: `sfile.get()` seems lost itself if the file does not exist.

				this->log_message(WarGrey::GYDM::Log::Debug,
					make_wstring(L"loading the %s: %s",
						file_type->Data(), ms_appdata->ToString()->Data()));

				return Concurrency::create_task(this->read(file->Path), token);
			}).then([=](Concurrency::task<FileType^> doc) {
				IMsAppdata<FileType>::critical_sections[uuid].lock();
				
				try {
					FileType^ ftobject = doc.get();

					if (ftobject != nullptr) {
						IMsAppdata<FileType>::filesystem[uuid] = ftobject;
						IMsAppdata<FileType>::databases[uuid] = true;

						this->broadcast(ms_appdata, uuid, ftobject);

						this->log_message(WarGrey::GYDM::Log::Debug,
							make_wstring(L"loaded the %s: %s with reference count %d",
								file_type->Data(), ms_appdata->ToString()->Data(),
								IMsAppdata<FileType>::lists[uuid].size()));
					} else {
						this->try_create(ms_appdata, uuid, file_type);
					}
				} catch (Platform::Exception^ e) {
					switch (e->HResult) {
					case HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND): {
						this->try_create(ms_appdata, uuid, file_type);
					}; break;
					default: {
						IMsAppdata<FileType>::clear(uuid);

						this->log_message(WarGrey::GYDM::Log::Error,
							make_wstring(L"failed to load %s: %s",
								ms_appdata->ToString()->Data(), e->Message->Data()));
					}
					}
				} catch (Concurrency::task_canceled&) {
					IMsAppdata<FileType>::clear(uuid);

					this->log_message(WarGrey::GYDM::Log::Debug,
						make_wstring(L"cancelled loading %s", ms_appdata->ToString()->Data()));
				} catch (std::exception& e) {
					IMsAppdata<FileType>::clear(uuid);
					this->log_message(WarGrey::GYDM::Log::Debug,
						make_wstring(L"unexcepted exception: %s", e.what()));
				}

				IMsAppdata<FileType>::critical_sections[uuid].unlock();
			});
		}

		void store_async(Windows::Storage::StorageFolder^ rootdir, Platform::String^ ms_appdata, FileType^ ftobject, Platform::String^ file_type, int search_start = 0) {
			int slash_pos = path_next_slash_position(ms_appdata, search_start);
			Platform::String^ subdir = substring(ms_appdata, search_start, slash_pos);
			unsigned int subsize = subdir->Length();
			unsigned int next_search_pos = search_start + subdir->Length() + 1;

			if (next_search_pos < ms_appdata->Length()) {
				if ((subsize == 0) || (subdir->Equals("."))) { // TODO: deal with '..'
					this->store_async(rootdir, ms_appdata, ftobject, file_type, next_search_pos);
				} else {
					auto token = this->shared_task.get_token();
					auto cd = Concurrency::create_task(rootdir->CreateFolderAsync(subdir, Windows::Storage::CreationCollisionOption::OpenIfExists), token);

					cd.then([=](Concurrency::task<Windows::Storage::StorageFolder^> sdir) {
						this->store_async(sdir.get(), ms_appdata, ftobject, file_type, next_search_pos);
					}).then([=](Concurrency::task<void> maybe_exn) {
						try {
							maybe_exn.get();
						} catch (Platform::Exception^ e) {
							this->log_message(WarGrey::GYDM::Log::Error, make_wstring(L"failed to save %s: %s", ms_appdata->Data(), e->Message->Data()));
						} catch (Concurrency::task_canceled&) {
							this->log_message(WarGrey::GYDM::Log::Debug, make_wstring(L"cancelled saving %s", ms_appdata->Data()));
						} catch (std::exception& e) {
							this->log_message(WarGrey::GYDM::Log::Debug, make_wstring(L"store: unexcepted exception: %s", e.what()));
						}
					});
				}
			} else {
				auto token = this->shared_task.get_token();

				Concurrency::create_task(this->write(ftobject, rootdir->Path + "\\" + subdir), token).then([=](Concurrency::task<bool> saved) {
					try {
						if (saved.get()) {
							this->log_message(WarGrey::GYDM::Log::Debug, make_wstring(L"saved the %s: %s", file_type->Data(), ms_appdata->Data()));
						}
					} catch (Platform::Exception^ e) {
						this->log_message(WarGrey::GYDM::Log::Error, make_wstring(L"failed to save %s: %s", ms_appdata->Data(), e->Message->Data()));
					} catch (Concurrency::task_canceled&) {
						this->log_message(WarGrey::GYDM::Log::Debug, make_wstring(L"cancelled saving %s", ms_appdata->Data()));
					} catch (std::exception& e) {
						this->log_message(WarGrey::GYDM::Log::Debug, make_wstring(L"unexcepted exception: %s", e.what()));
					}
				});
			}
		}

	private:
		static void clear(int uuid) {
			auto q = IMsAppdata<FileType>::lists[uuid];

			q.clear();
			IMsAppdata<FileType>::lists.erase(uuid);
			IMsAppdata<FileType>::databases.erase(uuid);
		}

	private:
		void broadcast(Windows::Foundation::Uri^ ms_appdata, int uuid, FileType^ ftobject) {
			auto q = IMsAppdata<FileType>::lists[uuid];

			for (auto self = q.begin(); self != q.end(); self++) {
				self->first->do_notify(ms_appdata, ftobject);
			}
		}

		void do_notify(Windows::Foundation::Uri^ ms_appdata, FileType^ ftobject) {
			this->on_appdata(ms_appdata, ftobject);
			this->on_appdata_notify(ms_appdata, ftobject);
		}

		void try_create(Windows::Foundation::Uri^ ms_appdata, int uuid, Platform::String^ file_type) {
			FileType^ maybe_ftobject = this->on_appdata_not_found(ms_appdata);

			if (maybe_ftobject != nullptr) {
				IMsAppdata<FileType>::critical_sections[uuid].unlock();
				this->store(ms_appdata, maybe_ftobject, file_type);
				IMsAppdata<FileType>::critical_sections[uuid].lock();
			} else {
				IMsAppdata<FileType>::clear(uuid);
			}
		}

	private:
		Concurrency::cancellation_token_source shared_task;
		
	private: // Stupid C++/CX, `FileType^` does not want to work with std::optional which employs `C union`
		static std::map<int, bool> databases;
		static std::map<int, FileType^> filesystem;
		static std::map<int, std::map<IMsAppdata<FileType>*, bool>> lists;
		static std::map<int, std::mutex> critical_sections;
	};

	template<class FileType> std::map<int, bool> IMsAppdata<FileType>::databases;
	template<class FileType> std::map<int, FileType^> IMsAppdata<FileType>::filesystem;
	template<class FileType> std::map<int, std::map<IMsAppdata<FileType>*, bool>> IMsAppdata<FileType>::lists;
	template<class FileType> std::map<int, std::mutex> IMsAppdata<FileType>::critical_sections;
}
