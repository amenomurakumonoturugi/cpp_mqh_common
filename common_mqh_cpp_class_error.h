


#ifndef COMMON_CLASS_ERROR_H

#define COMMON_CLASS_ERROR_H

class SYSTEM_ERROR_VALUE {

private:

	struct ERROR_BOX {

		ulong Error;
		ulong Visible;
		uint  Line;
		string File_Name;
	};

	static ERROR_BOX Error_Box;

	static ulong Get_File_Name(const string& file_drc, string& result) {

#ifdef COMPILER_FOO_CPP

		size_t Pos = file_drc.rfind((const char16_t*)BACK_SLASH_STRING);

		if (Pos != file_drc.npos) {

			result = file_drc.substr(Pos + 1);

			return ERROR_SUCCESS;
		}
#endif

#ifdef COMPILER_FOO_MQH5

		int Index = -1;
		int Found = -1;

		while ((Found = StringFind(file_drc, BACK_SLASH_STRING, Found + 1)) != -1) {

			Index = Found;
		}

		if (Index != -1) {

			result = StringSubstr(file_drc, Index + 1, -1);

			return ERROR_SUCCESS;
		}
#endif

		result = NULL_STRING;

		return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_FILE_NAME);
	}

public:

	static inline void Reset_System_Error() {

		Error_Box.Error = ERROR_SUCCESS;
		Error_Box.Visible = ERROR_SUCCESS;
		Error_Box.Line = 0;
		Error_Box.File_Name = NULL_STRING;
	}

	static inline void Get_System_Error(ulong& result_error, ulong& result_visible, uint& result_line, string& result_string) {

		result_error = Error_Box.Error;
		result_visible = Error_Box.Visible;
		result_line = Error_Box.Line;
		result_string = Error_Box.File_Name;
	}

	static inline void Set_System_Error(const ulong error, const ulong visiable, const uint line, const string file_drc) {

		Error_Box.Error = error;
		Error_Box.Visible = visiable;
		Error_Box.Line = line;

		string File_Name = NULL_STRING;
		Get_File_Name(file_drc, File_Name);

		Error_Box.File_Name = File_Name;
	}
};

SYSTEM_ERROR_VALUE::ERROR_BOX SYSTEM_ERROR_VALUE::Error_Box = { ERROR_SUCCESS, ERROR_SUCCESS, 0, NULL_STRING };

#ifdef COMPILER_FOO_CPP

class MUTEX_SYSTEM_ERROR_VALUE {

private:

	struct ERROR_BOX {

		ulong Error;
		ulong Visible;
		uint  Line;
		string File_Name;
	};

	static ERROR_BOX Error_Box;
	static std::mutex Mutex;

	static ulong Get_File_Name(const string& file_drc, string& result) {
		
        size_t Pos = file_drc.rfind((const char16_t*)BACK_SLASH_STRING);
		
		if (Pos != file_drc.npos) {

			result = file_drc.substr(Pos + 1);

			return ERROR_SUCCESS;
		}

		result = NULL_STRING;

		return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_GET_FILE_NAME);
	}

public:

	static inline void Reset_System_Error() {

		std::lock_guard<std::mutex> lock(Mutex);

		Error_Box.Error = ERROR_SUCCESS;
		Error_Box.Visible = ERROR_SUCCESS;
		Error_Box.Line = 0;
		Error_Box.File_Name = NULL_STRING;
	}

	static inline void Get_System_Error(ulong& result_error, ulong& result_visible, uint& result_line, string& result_string) {

		std::lock_guard<std::mutex> lock(Mutex);

		result_error = Error_Box.Error;
		result_visible = Error_Box.Visible;
		result_line = Error_Box.Line;
		result_string = Error_Box.File_Name;
	}

	static inline void Set_System_Error(const ulong error, const ulong visiable, const uint line, const string file_drc) {

		std::lock_guard<std::mutex> lock(Mutex);

		Error_Box.Error = error;
		Error_Box.Visible = visiable;
		Error_Box.Line = line;

		string File_Name = NULL_STRING;
		Get_File_Name(file_drc, File_Name);

		Error_Box.File_Name = File_Name;
	}
};

MUTEX_SYSTEM_ERROR_VALUE::ERROR_BOX MUTEX_SYSTEM_ERROR_VALUE::Error_Box = { ERROR_SUCCESS, ERROR_SUCCESS, 0, NULL_STRING };
std::mutex MUTEX_SYSTEM_ERROR_VALUE::Mutex;

#endif

#endif