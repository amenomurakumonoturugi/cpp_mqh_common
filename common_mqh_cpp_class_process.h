//+------------------------------------------------------------------+
//|                                                                  |
//| Copyright 2026 amenomurakumonoturugi                             |
//|                                                                  |
//| Licensed under the Apache License, Version 2.0 (the "License");  |
//| you may not use this file except in compliance with the License. |
//| You may obtain a copy of the License at                          |
//|                                                                  |
//|     http://www.apache.org/licenses/LICENSE-2.0                   |
//|                                                                  |
//| Unless required by applicable law or agreed to in writing,       |
//| software distributed under the License is distributed on an      |
//| "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,     |
//| either express or implied. See the License for the specific      |
//| language governing permissions and limitations under the         |
//| License.                                                         |
//+------------------------------------------------------------------+

#ifndef COMMON_CLASS_PROCESS_H

#define COMMON_CLASS_PROCESS_H

class PROCESS_MANAGER {

private:

#ifdef __CPP__

	bool Get_Mutex(const string& mutex_name, DWORD milli_second) {

		Handle.Set(CreateMutexW(NULL, FALSE, mutex_name));

		if (Handle.get() == NULL)
			return false;

		DWORD result = WaitForSingleObject(Handle.get(), milli_second);

		switch (result) {

		case WAIT_OBJECT_0:
			return true;
		case WAIT_ABANDONED:
			return true;
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
		default:
			return false;
		}
	}

#endif

	void Get_Command_Line_Norm(const string& source,string& result) {

		StringAssign(result, COMMAND_LINE_ESCAPE);

		StringAdd(result, source);
		StringAdd(result, COMMAND_LINE_ESCAPE);
	}

public:

#ifdef __MQL5__

	SAFE_HANDLE Handle;

#endif

#ifdef __CPP__

	SAFE_MUTEX_HANDLE Handle;

	ulong Get_Command_Line(_vector<string>& result) {


		LPWSTR Full_Cmd_Line = GetCommandLineW();

#ifdef _DEBUG

		LPWSTR Debug_Array[COMMAND_LINE_ASSIGNMENT_NUMBER_TOTAL] = { (LPWSTR)L"may_app",(LPWSTR)L"call_app", (LPWSTR)L"file_name", (LPWSTR)L"1.00",(LPWSTR)L"1234", };

		int Argc = COMMAND_LINE_ASSIGNMENT_NUMBER_TOTAL;
		LPWSTR* Argv = Debug_Array;

#else

		int Argc;
		LPWSTR* Argv = CommandLineToArgvW(Full_Cmd_Line, &Argc);

#endif

		if (Argv != NULL && Argc == COMMAND_LINE_ASSIGNMENT_NUMBER_TOTAL) {

			for (size_t i = 0; i < Argc; i++) {

				result.push_back(Argv[i]);
			}

			LocalFree(Argv);

			return ERROR_SUCCESS;
		}

		else {

			if (Argv != NULL) {

				LocalFree(Argv);
			}

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FRAUD_COMMAND_LINE_VALUE),
				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FRAUD_COMMAND_LINE_VALUE),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FRAUD_COMMAND_LINE_VALUE);
		}
	}

	bool Trial_Get_Mutex(const string& mutex_name) {

		for (size_t i = 0; i < GET_MUTEX_TRIAL_MAX_COUNT; i++) {

			if (Get_Mutex(mutex_name, GET_MUTEX_WAIT_LOCK_MILLI_SEC)) {

				return true;
			}

			else {

				Sleep(GET_MUTEX_TRIAL_SLEEP_MILLI_SEC);
			}
		}

		return false;
	}

#endif

	void Get_File_Drc(_vector<string>& drc_array, const uint count, const string file_name, string& result) {

		StringAssign(result, NULL_STRING);

		for (uint i = 0; i < count; i++)
			StringAdd(result, drc_array.Get_At(i));

		StringAdd(result, file_name);
	}

	ulong Proccess_Boot_File_Exist(const string& drc) {

		FIND_DATAW Data = {};
		HANDLE Drc_Handle = _FindFirstFileW(drc, Data);

		if (Drc_Handle == INVALID_HANDLE_VALUE) {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_APP_EXIST),
				_GetWin32LastError(),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_APP_EXIST);
		}

		else {

			FindClose(Drc_Handle);

			return ERROR_SUCCESS;
		}
	}

	void Create_Command_Line(const string& exe_app, const string& call_app, const string& file_name, const double& version, const uint& line,string& result) {

		StringAssign(result, NULL_STRING);

		string Buf;

		Get_Command_Line_Norm(exe_app, Buf);

		StringAdd(result, Buf);
		StringAdd(result, SPACE_STRING);

		Get_Command_Line_Norm(call_app, Buf);

		StringAdd(result, Buf);
		StringAdd(result, SPACE_STRING);

		Get_Command_Line_Norm(file_name, Buf);

		StringAdd(result, Buf);
		StringAdd(result, SPACE_STRING);

		uint Digit = 3;
		string Decimal = Decimal_To_String(version, Digit);

		Get_Command_Line_Norm(Decimal, Buf);

		StringAdd(result, Buf);
		StringAdd(result, SPACE_STRING);

		string Integer = IntegerToString(line);

		Get_Command_Line_Norm(Integer, Buf);

		StringAdd(result, Buf);
	}

	ulong Create_Proccess(

		const string& app_name,
		string& command_line,
		const uint32_t& creation_flags,
		const string& current_directory) {

		SECURITY_ATTRIBUTES Proccess = {}, Thread = {};
		PVOID Environment = NULL;


		STARTUPINFOW Startup_Info = {};
		PROCESS_INFORMATION Process_Information = {};

		Startup_Info.cb = sizeof(Startup_Info);

		if (_CreateProcessW(

			app_name,
			command_line,
			Proccess,
			Thread,
			FALSE,
			creation_flags,
			Environment,
			current_directory,
			Startup_Info,
			Process_Information)) {

			SAFE_HANDLE hProcess, hThread;

			hProcess.Set(Process_Information.hProcess);
			hThread.Set(Process_Information.hThread);

			return ERROR_SUCCESS;
		}

		else {

			SYSTEM_ERROR_VALUE::Set_System_Error(

				CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_APP_BOOT),
				_GetWin32LastError(),
				__LINE__,
				__FILEW__
			);

			return CALC_CUSTOM_ERROR_CODE(CUSTOM_ERROR_CODE_FAILED_APP_BOOT);
		}
	}
};

#endif