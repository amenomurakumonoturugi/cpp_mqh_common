

#ifndef COMMON_CLASS_LOG_BASE_H

#define COMMON_CLASS_LOG_BASE_H

#ifdef __CPP__

class LOG_MANAGER_BASE : public FILE_MANAGER_BASE {

protected:

	struct LOG_FILE_ENTRY {

		wchar_t* Full_Path;

		FIND_DATAW Find_Data;
	};

	string Log_Data;

	LOG_MANAGER_BASE() {

		StringAssign(Log_Data, NULL_STRING);
	}

	ulong Get_Current_Time(std::tm& result, WORD& milli_sec) {

		SYSTEMTIME System_Milli_Sec;

		GetLocalTime(&System_Milli_Sec);

		milli_sec = System_Milli_Sec.wMilliseconds;

		std::time_t Current_Sec = std::time(nullptr);

		errno_t err = localtime_s(&result, &Current_Sec);

		if (err != 0)
			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_LOG_NONE_GET_UNKNOWN_TIME);

		return ERROR_SUCCESS;
	}

	ulong Get_Directory_Total_Size(const string& dir_path, ulong& total_result) {

		FIND_DATAW Find_Data = {};

		string Search_Path;
		StringAssign(Search_Path, dir_path);

		Search_Path.append(u"\\*");

		HANDLE h_Find = _FindFirstFileW(Search_Path, Find_Data);

		if (h_Find == INVALID_HANDLE_VALUE)
			return ERROR_SUCCESS;

		do {

			string Name = Find_Data.cFileName;

			if (Name == u"." || Name == u"..")
				continue;

			string Full_Path;
			StringAssign(Full_Path, dir_path);

			Full_Path.append(u"\\");
			Full_Path.append(Find_Data.cFileName);

			if (Find_Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

				Get_Directory_Total_Size(Full_Path, total_result);
			}

			else {

				ULARGE_INTEGER Ularge = {};

				Ularge.LowPart = Find_Data.nFileSizeLow;
				Ularge.HighPart = Find_Data.nFileSizeHigh;
				total_result += Ularge.QuadPart;
			}

		} while (FindNextFileW(h_Find, &Find_Data));

		FindClose(h_Find);

		return ERROR_SUCCESS;
	}

	ulong Delete_File(const LOG_FILE_ENTRY& file, ulong& result_file_size) {

		if (DeleteFileW(file.Full_Path)) {

			ULARGE_INTEGER Ularge = {};

			Ularge.LowPart = file.Find_Data.nFileSizeLow; Ularge.HighPart = file.Find_Data.nFileSizeHigh;

			result_file_size += Ularge.QuadPart;

			return ERROR_SUCCESS;
		}

		else {

			MUTEX_SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_DELETE),
				_GetWin32LastError(),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_DELETE);
		}
	}


	ulong Collect_Old_Logs(const string& drc, std::vector<LOG_FILE_ENTRY>& list) {

		FIND_DATAW Find_Data = {};

		string Search_Path;
		StringAssign(Search_Path, drc);

		Search_Path.append(u"\\*");

		HANDLE h_Find = _FindFirstFileW(Search_Path, Find_Data);

		if (h_Find == INVALID_HANDLE_VALUE)
			return ERROR_SUCCESS;

		do {

			std::wstring_view name = Find_Data.cFileName;

			if (name == L"." || name == L"..")
				continue;

			string Full_Path;
			StringAssign(Full_Path, drc);

			Full_Path.append(u"\\");
			Full_Path.append(Find_Data.cFileName);

			if (Find_Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

				Collect_Old_Logs(Full_Path, list);
			}
			else {

				list.push_back({ Full_Path, Find_Data });
			}

		} while (FindNextFileW(h_Find, &Find_Data));

		FindClose(h_Find);

		return ERROR_SUCCESS;
	}

public:

	virtual inline HANDLE Create_File(const string& file_name) override {

		return CreateFileW(

			file_name,
			FILE_APPEND_DATA | FILE_GENERIC_READ | FILE_GENERIC_WRITE | SYNCHRONIZE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	}

	virtual ulong Format_Log_Line(const ulong& error, const uint& line, const bool& error_type) {

		return 0;
	}

	ulong Maintenance_Log_Capacity(const string& root) {

		ulong Current_Use = 0;

		Get_Directory_Total_Size(root, Current_Use);

		// 400MB threshold
		if (Current_Use > 419430400) {

			std::vector<LOG_FILE_ENTRY> Files;

			Collect_Old_Logs(root, Files);

			// 更新日時でソート（古い順）
			std::sort(Files.begin(), Files.end(), [](const LOG_FILE_ENTRY& a, const LOG_FILE_ENTRY& b) {

				return CompareFileTime(&a.Find_Data.ftLastWriteTime, &b.Find_Data.ftLastWriteTime) == -1; });

			ulong Target_To_Delete = Current_Use - 419430400;
			ulong Deleted_Sum = 0;

			for (const LOG_FILE_ENTRY& Entry : Files) {

				if (Deleted_Sum >= Target_To_Delete)
					break;

				Delete_File(Entry, Deleted_Sum);
			}
		}

		return ERROR_SUCCESS;
	}

	ulong Get_Log_File_Name(string& result) {

		std::tm Current_Time;

		WORD Milli_Sec;

		ulong Error_Code = Get_Current_Time(Current_Time, Milli_Sec);

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		wchar_t Buf[1024];

		swprintf_s(Buf, _countof(Buf), L"\\%04d%02d%02d.log",
			Current_Time.tm_year + 1900,
			Current_Time.tm_mon + 1,
			Current_Time.tm_mday);

		StringAssign(result, Buf);

		return ERROR_SUCCESS;
	}

	virtual inline ulong File_Write(const HANDLE file_handle) override {

		DWORD Write_Size = 0;

		if (!_WriteFile(file_handle, Log_Data, (DWORD)Log_Data.size() * sizeof(wchar_t), Write_Size, NULL)) {

			if (_GetWin32LastError() == ERROR_IO_PENDING) {

				return ERROR_SUCCESS;
			}

			else {

				MUTEX_SYSTEM_ERROR_VALUE::Set_System_Error(

					CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_WRITE),
					_GetWin32LastError(),
					__LINE__,
					__FILEW__
				);

				return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_WRITE);
			}
		}

		else {

			if ((uint)Write_Size != (DWORD)Log_Data.size() * sizeof(wchar_t)) {

				MUTEX_SYSTEM_ERROR_VALUE::Set_System_Error(

					CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_WRITE_FILE_BYTE_SIZE),
					CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_WRITE_FILE_BYTE_SIZE),
					__LINE__,
					__FILEW__
				);

				return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_WRITE_FILE_BYTE_SIZE);
			}

			return ERROR_SUCCESS;
		}
	}

	ulong Set_File_Pointer_End(const HANDLE& file_handle) {

		LARGE_INTEGER Move_P = {}, result_P = {};

		if (_SetFilePointerEx(file_handle, Move_P, result_P, FILE_END) == INVALID_SET_FILE_POINTER) {

			MUTEX_SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_POINTER_SET),
				_GetWin32LastError(),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_POINTER_SET);
		}

		return ERROR_SUCCESS;
	}

	void Reset_Log_Data() {

		StringAssign(Log_Data, NULL_STRING);
	}
};

#endif

#endif