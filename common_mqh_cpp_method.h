

#ifndef COMMON_METHOD_H

#define COMMON_METHOD_H

#ifdef __MQL5__

string Decimal_To_String(const double& value, const uint& digit) {

	return DoubleToString(value, digit);
}

bool StringAssign(string& string_var, const string& assign_substring) {

	string_var = assign_substring;

	return true;
}

inline string ShortArrayToString(_vector<ushort>& array, int start = 0, int count = -1) {

	return ::ShortArrayToString(array.Buf_Ptr, start, count);
}

BOOL _CreateProcessW(

		const string          lpApplicationName,
		string                lpCommandLine,
		SECURITY_ATTRIBUTES&  lpProcessAttributes,
		SECURITY_ATTRIBUTES&  lpThreadAttributes,
		BOOL                  bInheritHandles,
		DWORD                 dwCreationFlags,
		PVOID                 lpEnvironment,
		const string          lpCurrentDirectory,
		STARTUPINFOW&         lpStartupInfo,
		PROCESS_INFORMATION&  lpProcessInformation) {

		return CreateProcessW(

			lpApplicationName,
			lpCommandLine,
			lpProcessAttributes,
			lpThreadAttributes,
			bInheritHandles,
			dwCreationFlags,
			lpEnvironment,
			lpCurrentDirectory,
			lpStartupInfo,
			lpProcessInformation);
}

HANDLE _FindFirstFileW(const string lpFileName, FIND_DATAW& lpFindFileData) {

	return FindFirstFileW(lpFileName, lpFindFileData);
}

BOOL _UnlockFileEx(

	HANDLE       hFile,
	DWORD        dwReserved,
	DWORD        nNumberOfBytesToUnlockLow,
	DWORD        nNumberOfBytesToUnlockHigh,
	OVERLAPPED&  lpOverlapped) {

	dwReserved = 0;

	return UnlockFileEx(

		hFile,
		dwReserved,
		nNumberOfBytesToUnlockLow,
		nNumberOfBytesToUnlockHigh,
		lpOverlapped);
}

BOOL _LockFileEx(

	HANDLE       hFile,
	DWORD        dwFlags,
	DWORD        dwReserved,
	DWORD        nNumberOfBytesToLockLow,
	DWORD        nNumberOfBytesToLockHigh,
	OVERLAPPED&  lpOverlapped) {

	dwReserved = 0;

	return LockFileEx(

		hFile,
		dwFlags,
		dwReserved,
		nNumberOfBytesToLockLow,
		nNumberOfBytesToLockHigh,
		lpOverlapped);
}

BOOL _CreateDirectoryW(const string lpPathName, SECURITY_ATTRIBUTES& lpSecurityAttributes) {

	return CreateDirectoryW(lpPathName, lpSecurityAttributes);
}

BOOL _SetFilePointerEx(

	HANDLE         hFile,
	LARGE_INTEGER& liDistanceToMove,
	LARGE_INTEGER& lpNewFilePointer,
	DWORD          dwMoveMethod) {

	return SetFilePointerEx(

		hFile,
		liDistanceToMove.QuadPart,
		lpNewFilePointer.QuadPart,
		dwMoveMethod);
}

BOOL _GetFileTime(

	HANDLE     hFile,
	FILETIME& lpCreationTime,
	FILETIME& lpLastAccessTime,
	FILETIME& lpLastWriteTime) {

	return GetFileTime(

		hFile,
		lpCreationTime,
		lpLastAccessTime,
		lpLastWriteTime);
}

BOOL _ReadFile(

	HANDLE      hFile,
	ushort&     buffer[],
	DWORD       nNumberOfBytesToRead,
	DWORD&      lpNumberOfBytesRead,
	OVERLAPPED& lpOverlapped) {

	return ReadFile(

		hFile,
		buffer,
		nNumberOfBytesToRead,
		lpNumberOfBytesRead,
		lpOverlapped);
}

BOOL _WriteFile(

	HANDLE        hFile,
	const ushort& buffer[],
	DWORD         nNumberOfBytesToWrite,
	DWORD&        lpNumberOfBytesWritten,
	OVERLAPPED&   lpOverlapped) {

	return WriteFile(

		hFile,
		buffer,
		nNumberOfBytesToWrite,
		lpNumberOfBytesWritten,
		lpOverlapped);
}

BOOL _GetFileSizeEx(HANDLE hFile, LARGE_INTEGER& lpFileSize) {

	return GetFileSizeEx(hFile, lpFileSize.QuadPart);
}

#endif

#ifdef __CPP__



string  ShortArrayToString(_vector<ushort>& array, int start = 0, int count = -1) {

	// 配列が空、または開始位置が配列を超えている場合は即座に空文字を返す
	if (array.empty() || start >= static_cast<int>(array.size())) {

		return NULL_STRING;
	}

	// コピーする文字数を確定させる
	int Size = static_cast<int>(array.size());

	if (Size < start)
		return NULL_STRING;

	int Copy_Count = (count == -1) ? (Size - start) : count;

	// 安全装置：指定された文字数が配列の限界を超えないようにクランプ（防波堤）
	if (start + Copy_Count > Size) {

		Copy_Count = Size - start;
	}

	if (Copy_Count < 0)
		return NULL_STRING;

	string result;

	for (int i = 0; i < Copy_Count; i++) {

		wchar_t Buf = static_cast<wchar_t>(array.Get_At(start + i));

		if (Buf == 0)
			break;
		
		result.append(&Buf, 1);
	}

	return result;
}

bool StringAdd(string& string_var, const string&  add_substring) {

	string_var.append(add_substring);

	return true;
}

bool StringAssign(string& string_var, const string& assign_substring) {

	string_var.assign(assign_substring);

	return true;
}

string Decimal_To_String(const double& value, const uint& digit) {

	wchar_t Buf[256];

	swprintf_s(Buf, _countof(Buf),
		L"%.*f",
		digit,
		value
	);

	return Buf;
}

string IntegerToString(long number) {

	return std::to_wstring(number).c_str();
}

BOOL _CreateProcessW(

	LPCWSTR               lpApplicationName,
	LPWSTR                lpCommandLine,
	SECURITY_ATTRIBUTES   &lpProcessAttributes,
	SECURITY_ATTRIBUTES   &lpThreadAttributes,
	BOOL                  bInheritHandles,
	DWORD                 dwCreationFlags,
	LPVOID                lpEnvironment,
	LPCWSTR               lpCurrentDirectory,
	STARTUPINFOW          &lpStartupInfo,
	PROCESS_INFORMATION   &lpProcessInformation) {

	return CreateProcessW(

		lpApplicationName,
		lpCommandLine,
		&lpProcessAttributes,
		&lpThreadAttributes,
		bInheritHandles,
		dwCreationFlags,
		lpEnvironment,
		lpCurrentDirectory,
		&lpStartupInfo,
		&lpProcessInformation);
}

HANDLE _FindFirstFileW(LPCWSTR lpFileName, FIND_DATAW& lpFindFileData) {

	return FindFirstFileW(lpFileName, &lpFindFileData);
}

BOOL _UnlockFileEx(

	HANDLE       hFile,
	DWORD        dwReserved,
	DWORD        nNumberOfBytesToUnlockLow,
	DWORD        nNumberOfBytesToUnlockHigh,
	OVERLAPPED&  lpOverlapped) {

	dwReserved = 0;

	return UnlockFileEx(

		hFile,
		dwReserved,
		nNumberOfBytesToUnlockLow,
		nNumberOfBytesToUnlockHigh,
		&lpOverlapped);
}

BOOL _LockFileEx(

	HANDLE       hFile,
	DWORD        dwFlags,
	DWORD        dwReserved,
	DWORD        nNumberOfBytesToLockLow,
	DWORD        nNumberOfBytesToLockHigh,
	OVERLAPPED&  lpOverlapped) {

	dwReserved = 0;

	return LockFileEx(

		hFile,
		dwFlags,
		dwReserved,
		nNumberOfBytesToLockLow,
		nNumberOfBytesToLockHigh,
		&lpOverlapped);
}

BOOL _CreateDirectoryW(LPCWSTR lpPathName, SECURITY_ATTRIBUTES& lpSecurityAttributes) {

	return CreateDirectoryW(lpPathName, &lpSecurityAttributes);
}

BOOL _SetFilePointerEx(

	HANDLE         hFile,
	LARGE_INTEGER  liDistanceToMove,
	LARGE_INTEGER& lpNewFilePointer,
	DWORD          dwMoveMethod) {

	return SetFilePointerEx(

		hFile,
		liDistanceToMove,
		&lpNewFilePointer,
		dwMoveMethod);
}

BOOL _GetFileTime(

	HANDLE     hFile,
	FILETIME& lpCreationTime,
	FILETIME& lpLastAccessTime,
	FILETIME& lpLastWriteTime) {

	return GetFileTime(

		hFile,
		&lpCreationTime,
		&lpLastAccessTime,
		&lpLastWriteTime);
}

template<typename T>

BOOL _ReadFile(

	HANDLE        hFile,
	T&            buffer,
	DWORD         nNumberOfBytesToRead,
	DWORD&        lpNumberOfBytesRead,
	OVERLAPPED*   lpOverlapped) {

	return ReadFile(

		hFile,
		&buffer,
		nNumberOfBytesToRead,
		&lpNumberOfBytesRead,
		lpOverlapped);
}

template<typename T>

BOOL _ReadFile(

	HANDLE        hFile,
	T&            buffer,
	DWORD         nNumberOfBytesToRead,
	DWORD&        lpNumberOfBytesRead,
	OVERLAPPED&   lpOverlapped) {

	return ReadFile(

		hFile,
		&buffer,
		nNumberOfBytesToRead,
		&lpNumberOfBytesRead,
		&lpOverlapped);
}

template<typename T>

BOOL _WriteFile(

	HANDLE         hFile,
	const T&       buffer,
	DWORD          nNumberOfBytesToWrite,
	DWORD&         lpNumberOfBytesWritten,
	OVERLAPPED*    lpOverlapped) {

	return WriteFile(

		hFile,
		&buffer,
		nNumberOfBytesToWrite,
		&lpNumberOfBytesWritten,
		lpOverlapped);
}

template<typename T>

BOOL _WriteFile(

	HANDLE         hFile,
	const T&       buffer,
	DWORD          nNumberOfBytesToWrite,
	DWORD&         lpNumberOfBytesWritten,
	OVERLAPPED&    lpOverlapped) {

	return WriteFile(

		hFile,
		&buffer,
		nNumberOfBytesToWrite,
		&lpNumberOfBytesWritten,
		&lpOverlapped);
}

BOOL _WriteFile(
	
	HANDLE        hFile,
	const string& buffer,
	DWORD         nNumberOfBytesToWrite,
	DWORD&        lpNumberOfBytesWritten,
	OVERLAPPED*   lpOverlapped) {
	
	return WriteFile(
		
		hFile,
		buffer.data(),
		nNumberOfBytesToWrite,
		&lpNumberOfBytesWritten,
		lpOverlapped);
}

BOOL _WriteFile(

	HANDLE        hFile,
	const string& buffer,
	DWORD         nNumberOfBytesToWrite,
	DWORD&        lpNumberOfBytesWritten,
	OVERLAPPED&   lpOverlapped) {

	return WriteFile(

		hFile,
		buffer.data(),
		nNumberOfBytesToWrite,
		&lpNumberOfBytesWritten,
		&lpOverlapped);
}

BOOL _GetFileSizeEx(HANDLE hFile, LARGE_INTEGER& lpFileSize) {

	return GetFileSizeEx(hFile, &lpFileSize);
}

#endif

inline ulong _GetWin32LastError() {

#ifdef __CPP__
	return GetLastError();
#endif
#ifdef __MQL5__
	return RtlGetLastWin32Error();
#endif
}

inline ulong CALC_CUSTOM_ERROR_CODE(ulong custom_err) {

	return custom_err + ERR_USER_ERROR_FIRST;
}

#endif