

#ifndef COMMON_CLASS_FILE_FLOW_H

#define COMMON_CLASS_FILE_FLOW_H

class COMPOSITION_FILE_FLOW {

protected:

	ulong Common_Log_Flow(const string& full_path, const ulong& error, const uint& line, const bool& error_type,LOG_MANAGER_BASE& log_mng) {

		LARGE_INTEGER Large = {};

		ulong Error_Code = log_mng.Format_Log_Line(error, DUMMY_VALUE, DUMMY_VALUE);

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		Error_Code = Common_Open_File(log_mng, full_path, Large);

		if (Error_Code != ERROR_SUCCESS) {

			if (Error_Code != CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_CREATE_NEW_FILE)) {

				return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_OPEN_FILE);
			}
		}

		Error_Code = log_mng.Set_File_Pointer_End(log_mng.File_Handle.get());

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		Error_Code = log_mng.File_Write(log_mng.File_Handle.get());

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		Error_Code = log_mng.File_Write_Flash(log_mng.File_Handle.get());

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		Error_Code = log_mng.Lock_Mng.Unlock_File(log_mng.File_Handle.get());

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		return ERROR_SUCCESS;
	}

	ulong Common_Creaate_Root(FILE_MANAGER_BASE& file_mng, const string& file_name, string& result_drc) {

		FIND_DATAW Find_Data = {};
		string Drc = NULL_STRING;

		ulong Error_Code = file_mng.Get_Main_Directory(Drc);

		if (Error_Code != ERROR_SUCCESS) {

			return Error_Code;
		}

		Error_Code = file_mng.Find_Drc(Drc, Find_Data);

		if (Error_Code != ERROR_SUCCESS) {

			return Error_Code;
		}

		MQH_STRING_VECTOR Array;
		int Array_Size = (int)file_mng.Get_Root(file_name, Array);

		Error_Code = file_mng.Create_Root(Array.data, Drc, Array_Size);

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		StringAdd(Drc, file_name);

		Error_Code = file_mng.Find_Drc(Drc, Find_Data);

		if (Error_Code != ERROR_SUCCESS) {

			ulong Error, Vis;
			uint Line;
			string file_name;

			SYSTEM_ERROR_VALUE::Get_System_Error(Error, Vis, Line, file_name);

			if (Vis == ERROR_FILE_NOT_FOUND) {

				file_mng.File_Handle.Set(file_mng.Create_File(Drc));

				if (file_mng.File_Handle.get() == INVALID_HANDLE_VALUE) {

					SYSTEM_ERROR_VALUE::Set_System_Error(

						CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_ROOT_DRC),
						__GetWin32LastError(),
						__LINE__,
						__FILEW__
					);

					return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_ROOT_DRC);
				}

				Error_Code = file_mng.File_Initialization(file_mng.File_Handle.get());

				if (Error_Code != ERROR_SUCCESS) {

					return Error_Code;
				}
			}

			else {

				return Error_Code;
			}
		}

		result_drc = Drc;

		return ERROR_SUCCESS;

	}

	inline ulong Common_Open_File(FILE_MANAGER_BASE& file_mng, const string file_name, LARGE_INTEGER& result_file_size) {

		ulong Error_Code = ERROR_SUCCESS;

		file_mng.File_Handle.Set(file_mng.Create_File(file_name));

		if (file_mng.File_Handle.get() == INVALID_HANDLE_VALUE) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_OPEN_FILE),
				__GetWin32LastError(),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_OPEN_FILE);
		}


		else {

			ulong Open_Attribute = __GetWin32LastError();

			Error_Code = file_mng.Get_File_Size(file_mng.File_Handle.get(), result_file_size);

			if (Error_Code != ERROR_SUCCESS)
				return Error_Code;

			Error_Code = file_mng.Lock_Mng.Try_Lock_File(file_mng.File_Handle.get(), result_file_size.QuadPart);

			if (Error_Code != ERROR_SUCCESS)
				return Error_Code;
				

			if (Open_Attribute == ERROR_SUCCESS)
				return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_CREATE_NEW_FILE);

			else
				return ERROR_SUCCESS;
		}
	}

public:

	inline ulong Byte8_Read(ulong& result, ulong& last_time, const string& file_name, BYTE8_FILE_MANAGER& file_mng) {


		ulong Error_Code = file_mng.Lock_Mng.File_Lock_Only_1();

		if (Error_Code != ERROR_SUCCESS) {

			return Error_Code;
		}

		string File_Drc = NULL_STRING;

		Error_Code = Common_Creaate_Root(file_mng, file_name, File_Drc);

		LARGE_INTEGER P_Large = {};

		MQH_STRING_VECTOR Array;

		Error_Code = Common_Open_File(file_mng, File_Drc, P_Large);

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		Error_Code = file_mng.Check_File_Size(ULONG_VALUE_FILE_SIZE, P_Large);

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		Error_Code = file_mng.Get_File_Last_Write_Time(last_time);

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		Error_Code = file_mng.Reset_File_Pointer();

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		Error_Code = file_mng.File_Read(file_mng.File_Handle.get());

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		result = file_mng.Get_Read_Data();

		return ERROR_SUCCESS;
	};


	inline ulong Byte8_Write(const ulong& data, const string& file_name, BYTE8_FILE_MANAGER& file_mng) {

		ulong Error_Code = file_mng.Lock_Mng.File_Lock_Only_1();

		if (Error_Code != ERROR_SUCCESS) {

			return Error_Code;
		}

		string File_Drc = NULL_STRING;

		Error_Code = Common_Creaate_Root(file_mng, file_name, File_Drc);

		LARGE_INTEGER P_Large = {};

		MQH_STRING_VECTOR Array;

		Error_Code = Common_Open_File(file_mng, File_Drc, P_Large);

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		file_mng.Set_File_Handle(file_mng.File_Handle.get());

		Error_Code = file_mng.Check_File_Size(ULONG_VALUE_FILE_SIZE, P_Large);

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		Error_Code = file_mng.Reset_File_Pointer();

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		file_mng.Set_Write_Data(data);

		Error_Code = file_mng.File_Write(file_mng.File_Handle.get());

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		Error_Code = file_mng.File_Write_Flash(file_mng.File_Handle.get());

		if (Error_Code != ERROR_SUCCESS)
			return Error_Code;

		return ERROR_SUCCESS;
	};
};

#endif