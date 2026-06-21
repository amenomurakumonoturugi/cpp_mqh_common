#ifndef COMMON_CLASS_FILE_BYTE8

class BYTE8_FILE_MANAGER :public FILE_MANAGER_BASE {

private:

#ifdef __CPP__

#pragma pack(1)
	struct BINARY_DATA {

		ushort Value[sizeof(ulong) / sizeof(ushort)];
	};

#endif

#ifdef __MQL5__

	struct BINARY_DATA  pack(1) {

		ushort Value[sizeof(ulong) / sizeof(ushort)];
	};

#endif

	struct DATA_TYPE_BIT_MAP {

		ulong Value;
		BINARY_DATA Binary;
	};

	DATA_TYPE_BIT_MAP Data;

public:

	BYTE8_FILE_MANAGER() {

#ifdef __CPP__

		Data = {};
#endif

	}

	~BYTE8_FILE_MANAGER() {

		if (Lock_Mng.Unlock_File(File_Handle.get()) != ERROR_SUCCESS) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_UNLOCK),
				_GetWin32LastError(),
				__LINE__,
				__FILEW__
			);
		}
	}

	inline void Set_File_Handle(const HANDLE handle) {

		File_Handle.Set(handle);
	}

	inline ulong Get_Read_Data() const {

		return Data.Value;
	}

	inline void Set_Write_Data(const ulong data) {

		Data.Value = data;
	}

	virtual ulong File_Initialization(const HANDLE handle) override {

		LARGE_INTEGER Move_P = {}, Result_P = {};

		if (!_SetFilePointerEx(handle, Move_P, Result_P, FILE_BEGIN)) {

			SYSTEM_ERROR_VALUE::Set_System_Error(
				
				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_INIT),
				_GetWin32LastError(),
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

	virtual ulong Get_Root(const string& file_name, _vector<string>& result) override {

		if (file_name == ITEM_ERR_MS_ERR_FILE_NAME()) {

			result.push_back(MAKER_FOLDER_DRC());
			result.push_back(ITEM_FOLDER_DRC());
			result.push_back(ITEM_ERR_MS_FOLDER_DRC());
			result.push_back(ITEM_ERR_MS_FILES_FOLDER_DRC());
			result.push_back(ITEM_ERR_MS_ERR_FILE_DRC());
		}

		if (file_name == ITEM_ERR_MS_VISIBLE_ERR_NUM_FILE_NAME()) {

			result.push_back(MAKER_FOLDER_DRC());
			result.push_back(ITEM_FOLDER_DRC());
			result.push_back(ITEM_ERR_MS_FOLDER_DRC());
			result.push_back(ITEM_ERR_MS_FILES_FOLDER_DRC());
			result.push_back(ITEM_ERR_MS_VISIBLE_FILE_DRC());
		}

		if (file_name == ITEM_LANGUAGE_FILE_NAME()) {

			result.push_back(MAKER_FOLDER_DRC());
			result.push_back(ITEM_FOLDER_DRC());
			result.push_back(ITEM_LANGUAGE_FOLDER_DRC());
		};
		
		return (uint)result.size();
	}

	virtual ulong Get_Main_Directory(string& result) override {

#ifdef __CPP__

		PWSTR path = nullptr;

		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path))) {

			StringAssign(result, path);
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

#ifdef __MQL5__

		string Data_Path = TerminalInfoString(TERMINAL_DATA_PATH);

		int Result_Num = StringFind(Data_Path, "AppData", 0);

		if (Result_Num == -1) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC),
				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC),
				__LINE__,
				__FILEW__
			);


			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC);
		}

		else {

			result = StringSubstr(Data_Path, 0, Result_Num);

			if (result == NULL_STRING) {

				SYSTEM_ERROR_VALUE::Set_System_Error(

					CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC),
					CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC),
					__LINE__,
					__FILEW__
				);

				return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC);
			}

			else {

				string App_Data_Drc = "AppData\\Local";

				if (!StringAdd(result, App_Data_Drc)) {

					SYSTEM_ERROR_VALUE::Set_System_Error(

						CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC),
						CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC),
						__LINE__,
						__FILEW__
					);

					return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_LOCAL_APP_DRC);
				}

				else {

					return ERROR_SUCCESS;
				}
			}
		}
#endif
	}

	virtual inline ulong File_Write(const HANDLE file_handle) override {

		DWORD Write_Size = 0;
		OVERLAPPED Ov_L = {};

		Data.Binary = std::bit_cast<BINARY_DATA, ulong>(Data.Value);

		if (!_WriteFile(file_handle, Data.Binary.Value, sizeof(Data.Value), Write_Size, Ov_L)) {

			if (_GetWin32LastError() == ERROR_IO_PENDING) {

				return ERROR_SUCCESS;
			}

			else {

				SYSTEM_ERROR_VALUE::Set_System_Error(

					CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_WRITE),
					_GetWin32LastError(),
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

	virtual inline ulong File_Read(const HANDLE file_handle) override {

		DWORD Read_Size = 0;
		OVERLAPPED Ov_L = {};

		if (!_ReadFile(file_handle, Data.Binary.Value, sizeof(Data.Value), Read_Size, Ov_L)) {

			if (_GetWin32LastError() == ERROR_IO_PENDING) {

				return ERROR_SUCCESS;
			}

			else {

				SYSTEM_ERROR_VALUE::Set_System_Error(

					CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_READ),
					_GetWin32LastError(),
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

		if (!_SetFilePointerEx(File_Handle.get(), Move_P, result_P, FILE_BEGIN)) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_POINTER_SET),
				_GetWin32LastError(),
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

		if (!_GetFileTime(File_Handle.get(), Create, Access, Write)) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_FILE_TIME),
				_GetWin32LastError(),
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
