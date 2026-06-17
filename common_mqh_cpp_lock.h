

#ifndef COMMON_CLASS_FILE_LOCK_H

#define COMMON_CLASS_FILE_LOCK_H

class LOCK_MANAGER {

private:

	bool If_Lock;
	ulong Byte_Size;

	inline uint32_t FILE_LOCK_MASK_LOW(const ulong& file_data) {

		return (uint32_t)(file_data & 0xFFFFFFFF);
	}

	inline uint32_t FILE_LOCK_MASK_HIGH(const ulong& file_data) {

		return (uint32_t)(file_data >> 32);
	}

	bool Get_Now_File_Lock() const {

		return If_Lock;
	}

	void Set_Now_File_Lock(const bool& value) {

		If_Lock = value;
	}

	inline ulong Lock_File(const HANDLE& file_handle, const ulong& byte_size) {

		OVERLAPPED Ov_R = {};

		uint LockLow = FILE_LOCK_MASK_LOW(byte_size);
		uint LockHigh = FILE_LOCK_MASK_HIGH(byte_size);

		if (!_LockFileEx(file_handle, LOCKFILE_EXCLUSIVE_LOCK, 0, LockLow, LockHigh, Ov_R)) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_LOCK),
				_GetWin32LastError(),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_LOCK);
		}

		else {

			Set_Now_File_Lock(true);
			Byte_Size = byte_size;

			return ERROR_SUCCESS;
		}
	}

public:

	LOCK_MANAGER() {

		If_Lock = false;
		Byte_Size = 0;
	}

#ifdef __MQL5__
#define LOCK_MNG_FILE_SIZE_TYPE long
#endif

#ifdef __CPP__
#define LOCK_MNG_FILE_SIZE_TYPE ulong
#endif

	ulong Exchange_File_Size(const LOCK_MNG_FILE_SIZE_TYPE& value, ulong& result) {

		if (value < 0) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_LOCK),
				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_LOCK),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_LOCK);
		}

		else {

			result = (ulong)value;

			return ERROR_SUCCESS;
		}
	}

#undef LOCK_MNG_FILE_SIZE_TYPE

	ulong File_Lock_Only_1() {

		if (Get_Now_File_Lock()) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_LOCK_ONE),
				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_LOCK_ONE),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_LOCK_ONE);
		}

		else {

			return ERROR_SUCCESS;
		}
	}

	inline ulong Unlock_File(const HANDLE file_handle) {

		OVERLAPPED Ov_R = {};
		uint LockLow = FILE_LOCK_MASK_LOW(Byte_Size);
		uint LockHigh = FILE_LOCK_MASK_HIGH(Byte_Size);

		if (Get_Now_File_Lock()) {

			if (!_UnlockFileEx(file_handle, 0, LockLow, LockHigh, Ov_R)) {

				if (_GetWin32LastError() == ERROR_NOT_LOCKED)
					return ERROR_SUCCESS;

				else {

					SYSTEM_ERROR_VALUE::Set_System_Error(

						CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_UNLOCK),
						_GetWin32LastError(),
						__LINE__,
						__FILEW__
					);

					return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_FILE_UNLOCK);
				}
			}

			else {

				Set_Now_File_Lock(false);

				return ERROR_SUCCESS;
			}
		}

		else
			return ERROR_SUCCESS;
	}

	inline ulong Try_Lock_File(const HANDLE file_handle, const ulong& byte_size) {

		ulong result = 0;

		for (uint i = 0; i < TRY_FILE_LOCK_COUNTER; i++) {

			result = Lock_File(file_handle, byte_size);

			if (result == ERROR_SUCCESS) {

				return ERROR_SUCCESS;
			}

			Sleep(SLEEP_MILLI_SECOND_VALUE);
		}

		return result;
	}
};

#endif