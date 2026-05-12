

#ifndef COMMON_METHOD_H

#define COMMON_METHOD_H

#ifdef COMPILER_FOO_MQH5

string Decimal_To_String(const double& value, const uint& digit) {

	return DoubleToString(value, digit);
}

#endif

#ifdef COMPILER_FOO_CPP

bool StringAdd(string& string_var, string  add_substring) {

	string_var.append(add_substring);

	return true;
}

string Decimal_To_String(const double& value, const uint& digit) {

	wchar_t Buf[256];

	swprintf_s(Buf, _countof(Buf),
		L"%.*f",
		digit,
		value
	);

	return string(Buf);
}

string IntegerToString(long number) {

	return string(std::to_wstring(number).c_str());
}

BOOL __CreateProcessW(

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

HANDLE __FindFirstFileW(LPCWSTR lpFileName, WIN32_FIND_DATAW& lpFindFileData) {

	return FindFirstFileW(lpFileName, &lpFindFileData);
}

BOOL __UnlockFileEx(

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

BOOL __LockFileEx(

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

BOOL __CreateDirectoryW(LPCWSTR lpPathName, SECURITY_ATTRIBUTES lpSecurityAttributes) {

	return CreateDirectoryW(lpPathName, &lpSecurityAttributes);
}

BOOL __SetFilePointerEx(

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

BOOL __GetFileTime(

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

BOOL __ReadFile(

	HANDLE        hFile,
	T&            buffer,
	DWORD         nNumberOfBytesToRead,
	DWORD& lpNumberOfBytesRead,
	OVERLAPPED& lpOverlapped) {

	return ReadFile(

		hFile,
		&buffer,
		nNumberOfBytesToRead,
		&lpNumberOfBytesRead,
		&lpOverlapped);
}

template<typename T>

BOOL __WriteFile(

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

BOOL __WriteFile(
	
	HANDLE        hFile,
	const string& buffer,
	DWORD         nNumberOfBytesToWrite,
	DWORD&        lpNumberOfBytesWritten,
	OVERLAPPED&   lpOverlapped) {

	lpOverlapped.Offset = 0xFFFFFFFF;
	lpOverlapped.OffsetHigh = 0xFFFFFFFF;
	
	return WriteFile(
		
		hFile,
		buffer.data(),
		nNumberOfBytesToWrite,
		&lpNumberOfBytesWritten,
		&lpOverlapped);
}

#endif

inline ulong __GetWin32LastError() {

#ifdef COMPILER_FOO_CPP
	return GetLastError();
#endif
#ifdef COMPILER_FOO_MQH5
	return RtlGetLastWin32Error();
#endif
}

inline ulong CALC_CUSTOM_ERROR_CODE(ulong custom_err) {

	return custom_err + ERR_USER_ERROR_FIRST;
}


#endif