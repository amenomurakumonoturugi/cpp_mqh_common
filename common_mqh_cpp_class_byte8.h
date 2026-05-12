#ifndef COMMON_CLASS_FILE_BYTE8

class BYTE8_FILE_MANAGER :public FILE_MANAGER_BASE {

private:

#ifdef COMPILER_FOO_CPP

#pragma pack(1)
	struct BINARY_DATA {

		ushort Value[sizeof(ulong) / sizeof(ushort)];
	};

#endif

#ifdef COMPILER_FOO_MQH5

	struct BINARY_DATA  pack(1) {

		ushort Value[sizeof(ulong) / sizeof(ushort)];
	};

#endif

	struct DATA_TYPE_BIT_MAP {

		ulong Value;
		BINARY_DATA Binary;
	};

	DATA_TYPE_BIT_MAP Data = {};

public:

	~BYTE8_FILE_MANAGER() {

		if (Lock_Mng.Unlock_File(File_Handle.get()) != ERROR_SUCCESS) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_UNLOCK),
				__GetWin32LastError(),
				__LINE__,
				__FILEW__
			);
		}
	}

	inline void Set_File_Handle(const HANDLE& handle) {

		File_Handle.Set(handle);
	}

	inline ulong Get_Read_Data() {

		return Data.Value;
	}

	inline void Set_Write_Data(const ulong data) {

		Data.Value = data;
	}

	virtual ulong File_Initialization(const HANDLE& handle) override {

		LARGE_INTEGER Move_P = {}, Result_P = {};

		if (!__SetFilePointerEx(handle, Move_P, Result_P, FILE_BEGIN)) {

			SYSTEM_ERROR_VALUE::Set_System_Error(
				
				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_INIT),
				__GetWin32LastError(),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_INIT);
		}

		else {

			Data.Value = 0;

			DWORD Write_Size = 0;

			ulong Error_Code = File_Write(handle);

			if (Error_Code != ERROR_SUCCESS) {

				return Error_Code;
			}

			else {

				Error_Code = File_Write_Flash(handle);

				if (Error_Code != ERROR_SUCCESS) {

					return Error_Code;
				}

				return ERROR_SUCCESS;
			}
		}
	}

	virtual ulong Get_Root(const string& file_name, MQH_STRING_VECTOR& result) override {

		int Count = 0;

		if (file_name == ITEM_ERR_MS_ERR_FILE_NAME)
			Count = ERROR_ROOT_COUNT;

		if (file_name == ITEM_ERR_MS_VISIBLE_ERR_NUM_FILE_NAME)
			Count = VIS_ROOT_COUNT;

		if (file_name == ITEM_LANGUAGE_FILE_NAME)
			Count = LANG_ROOT_COUNT;

		for (int i = 0; i < Count; i++) {

			if (file_name == ITEM_ERR_MS_ERR_FILE_NAME)
				result.push_back(FIND_ROOT_STRING_ERROR[i]);

			if (file_name == ITEM_ERR_MS_VISIBLE_ERR_NUM_FILE_NAME)
				result.push_back(FIND_ROOT_STRING_VIS[i]);

			if (file_name == ITEM_LANGUAGE_FILE_NAME)
				result.push_back(FIND_ROOT_STRING_LANG[i]);
		}

		return (uint)result.size();
	}

	virtual ulong Get_Main_Directory(string& result) override {

#ifdef COMPILER_FOO_CPP

		PWSTR path = nullptr;

		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path))) {

			result = path;
			CoTaskMemFree(path);

			return ERROR_SUCCESS;
		}

		else {

			SYSTEM_ERROR_VALUE::Set_System_Error(
				
				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC),
				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC);
		}

#endif

#ifdef COMPILER_FOO_MQH5

		string Data_Path = TerminalInfoString(TERMINAL_DATA_PATH);

		int Result_Num = StringFind(Data_Path, "AppData", 0);

		if (Result_Num == -1) {

			ulong Error = CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC);
			ulong Vis = CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC);
			uint Line = __LINE__;

			string File_Name;

			ulong Error_Code = Get_File_Name(CPP_MQH_FILE, File_Name);

			if (Error_Code != ERROR_SUCCESS)
				return Error_Code;

			Set_System_Error(Error, Vis, Line, File_Name);


			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC);
		}

		else {

			result = StringSubstr(Data_Path, 0, Result_Num);

			if (result == NULL_STRING) {

				ulong Error = CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC);
				ulong Vis = CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC);
				uint Line = __LINE__;

				string File_Name;

				ulong Error_Code = Get_File_Name(CPP_MQH_FILE, File_Name);

				if (Error_Code != ERROR_SUCCESS)
					return Error_Code;

				Set_System_Error(Error, Vis, Line, File_Name);


				return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC);
			}

			else {

				string App_Data_Drc = "AppData\\Local\\";

				if (!StringAdd(result, App_Data_Drc)) {

					ulong Error = CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC);
					ulong Vis = CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC);
					uint Line = __LINE__;

					string File_Name;

					ulong Error_Code = Get_File_Name(CPP_MQH_FILE, File_Name);

					if (Error_Code != ERROR_SUCCESS)
						return Error_Code;

					Set_System_Error(Error, Vis, Line, File_Name);


					return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC);
				}

				else {

					return ERROR_SUCCESS;
				}
			}
		}
#endif
	}

	virtual inline ulong File_Write(const HANDLE& file_handle) override {

		DWORD Write_Size = 0;
		OVERLAPPED  Ov_L = {};

		Data.Binary = std::bit_cast<BINARY_DATA, ulong>(Data.Value);

		if (!__WriteFile(file_handle, Data.Binary, sizeof(Data.Value), Write_Size, Ov_L)) {

			if (__GetWin32LastError() == ERROR_IO_PENDING) {

				return ERROR_SUCCESS;
			}

			else {

				SYSTEM_ERROR_VALUE::Set_System_Error(

					CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_WRITE),
					__GetWin32LastError(),
					__LINE__,
					__FILEW__
				);

				return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_WRITE);
			}
		}

		else {

			if ((uint)Write_Size != sizeof(Data.Value)) {

				SYSTEM_ERROR_VALUE::Set_System_Error(

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

	virtual inline ulong File_Read(const HANDLE& file_handle) override {

		DWORD Read_Size = 0;
		OVERLAPPED Ov_L = {};

		if (!__ReadFile(file_handle, Data.Binary, sizeof(Data.Value), Read_Size, Ov_L)) {

			if (__GetWin32LastError() == ERROR_IO_PENDING) {

				return ERROR_SUCCESS;
			}

			else {

				SYSTEM_ERROR_VALUE::Set_System_Error(

					CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_READ),
					__GetWin32LastError(),
					__LINE__,
					__FILEW__
				);

				return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_READ);
			}
		}

		else {

			if ((uint)Read_Size != sizeof(Data.Value)) {

				SYSTEM_ERROR_VALUE::Set_System_Error(

					CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_WRITE_FILE_BYTE_SIZE),
					CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_WRITE_FILE_BYTE_SIZE),
					__LINE__,
					__FILEW__
				);

				return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_WRITE_FILE_BYTE_SIZE);
			}

			Data.Value = std::bit_cast<ulong, BINARY_DATA>(Data.Binary);

			return ERROR_SUCCESS;
		}
	}

	inline ulong Reset_File_Pointer() {

		LARGE_INTEGER Move_P = {}, result_P = {};

		if (!__SetFilePointerEx(File_Handle.get(), Move_P, result_P, FILE_BEGIN)) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_POINTER_SET),
				__GetWin32LastError(),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_POINTER_SET);
		}

		else {

			return ERROR_SUCCESS;
		}
	}

	inline ulong Get_File_Last_Write_Time(ulong& result) {

		FILETIME Create = {}, Access = {}, Write = {};

		if (!__GetFileTime(File_Handle.get(), Create, Access, Write)) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_FILE_TIME),
				__GetWin32LastError(),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_FILE_TIME);
		}

		result = ((ulong)Write.dwHighDateTime << 32) | Write.dwLowDateTime;

		return ERROR_SUCCESS;
	}
};

#endif
