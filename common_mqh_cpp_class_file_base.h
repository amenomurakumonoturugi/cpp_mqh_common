

#ifndef COMMON_CLASS_FILE_BASE_H

#define COMMON_CLASS_FILE_BASE_H

class FILE_MANAGER_BASE {

protected:

	inline ulong Create_Folder(const string& folder_name) {

		SECURITY_ATTRIBUTES Atr = {};

		if (_CreateDirectoryW(folder_name, Atr) == 0) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_ROOT_DRC),
				_GetWin32LastError(),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_ROOT_DRC);
		}

		else {

			return ERROR_SUCCESS;
		}
	}

public:

	LOCK_MANAGER Lock_Mng;

	SAFE_HANDLE File_Handle;

	virtual inline ulong File_Read(const HANDLE file_handle) {

		return 0;
	}

	virtual inline ulong File_Write(const HANDLE file_handle) {

		return 0;
	}

	inline ulong File_Write_Flash(const HANDLE file_handle) {

		if (!FlushFileBuffers(file_handle)) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_WRITE_FLASH),
				_GetWin32LastError(),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_WRITE_FLASH);
		}

		return ERROR_SUCCESS;
	}

	inline ulong Check_File_Size(const ulong& byte_size, const LARGE_INTEGER& file_size) {

		if (byte_size != file_size.QuadPart) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FRAUD_FILE_SIZE),
				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FRAUD_FILE_SIZE),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FRAUD_FILE_SIZE);
		}

		else {

			return ERROR_SUCCESS;
		}
	};

	virtual inline ulong Get_Main_Directory(string& result) {

		return 0;
	}

	inline ulong Find_Drc(const string& drc_name, FIND_DATAW& result) {

		HANDLE Drc_Handle = _FindFirstFileW(drc_name, result);

		if (Drc_Handle == INVALID_HANDLE_VALUE) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_DRC),
				_GetWin32LastError(),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_DRC);
		}

		else {

			FindClose(Drc_Handle);

			return ERROR_SUCCESS;
		}
	}

	virtual ulong Get_Root(const string& file_name, _vector<string>& result) {

		return 0;
	}

	inline ulong Create_Root(_vector<string>& drc_array, string& top_drc, const int& count) {

		FIND_DATAW Data = {};
		ulong Error_Code = ERROR_SUCCESS;

		for (int i = 0; i < count; i++) {

			StringAdd(top_drc, drc_array.Get_At(i));

			Error_Code = Find_Drc(top_drc, Data);

			if (Error_Code != ERROR_SUCCESS) {

				if (_GetWin32LastError() == ERROR_FILE_NOT_FOUND) {

					Error_Code = Create_Folder(top_drc);

					if (Error_Code != ERROR_SUCCESS)
						return Error_Code;
				}
			}
		}

		return ERROR_SUCCESS;
	}

	virtual inline HANDLE Create_File(const string& file_name) {

		return CreateFileW(

			file_name,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	}

	virtual ulong File_Initialization(const HANDLE handle) {

		return 0;
	}

	inline ulong Get_File_Size(const HANDLE file_handle, LARGE_INTEGER& result) {

		BOOL Success = _GetFileSizeEx(file_handle, result);

		if (!Success) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_FILE_SIZE),
				_GetWin32LastError(),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_FILE_SIZE);
		};

		return ERROR_SUCCESS;
	}
};

#endif