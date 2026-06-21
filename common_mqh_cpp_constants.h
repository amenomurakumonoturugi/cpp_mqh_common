


#ifndef COMMON_CONSTANTS_H

#define COMMON_CONSTANTS_H

#ifdef __MQL5__

const int INVALID_HANDLE_VALUE = -1;

const uint GENERIC_READ = (uint)0x80000000;
const uint GENERIC_WRITE = 0x40000000;
const uint OPEN_ALWAYS = 4;
const uint FILE_ATTRIBUTE_NORMAL = 0x00000080;
const uint FILE_BEGIN = 0;

const uint LOCKFILE_EXCLUSIVE_LOCK = 0x00000002;
const uint ERROR_FILE_NOT_FOUND = 2;
const uint ERROR_ALREADY_EXISTS = 183;

const uint ERROR_SUCCESS = 0;
const uint ERROR_IO_PENDING = 997L;

const uint DETACHED_PROCESS = 0x00000008;
const uint HIGH_PRIORITY_CLASS = 0x00000080;

const uint  ERROR_NOT_LOCKED = 0x9E;

#endif

#ifdef __CPP__

const uint ERR_USER_ERROR_FIRST = 65536;

const uint GET_MUTEX_TRIAL_MAX_COUNT = 10;
const uint GET_MUTEX_TRIAL_SLEEP_MILLI_SEC = 100;
const uint GET_MUTEX_WAIT_LOCK_MILLI_SEC = 20;

#endif

const string NULL_STRING = _u("");
const string SPACE_STRING = _u(" ");
const string COLON_STRING = _u(":");
const string LINE_BREAK = _u("\n");
const string BACK_SLASH_STRING = _u("\\");
const string COMMAND_LINE_ESCAPE = _u("\"");

const uint MAX_LOAD_STRING = 256;

const uint TIME_SCALE_PREMISE_YEAR = 1900;
const uint TIME_SCALE_MOON_RETOUCH_VALUE = 1;

const uint SLEEP_MILLI_SECOND_VALUE = 10;
const uint TRY_FILE_LOCK_COUNTER = 10;
const int  TRY_FIND_COUNTER = 10;

const uint COMMAND_LINE_ASSIGNMENT_NUMBER_EXE_APP_NAME = 0;
const uint COMMAND_LINE_ASSIGNMENT_NUMBER_CALL_APP_NAME = 1;
const uint COMMAND_LINE_ASSIGNMENT_NUMBER_FILE_NAME = 2;
const uint COMMAND_LINE_ASSIGNMENT_NUMBER_VERSION = 3;
const uint COMMAND_LINE_ASSIGNMENT_NUMBER_LINE_NUMBER = 4;

const uint COMMAND_LINE_ASSIGNMENT_NUMBER_TOTAL = 5;
const uint DUMMY_VALUE = 0;

const ulong ULONG_VALUE_FILE_SIZE = sizeof(ulong);
const ulong MAX_LOG_FILE_SIZE = 2097152;

const uint CPP_MQH_FILE_SHARE_READ = 0x00000001;
const uint CPP_MQH_FILE_SHARE_WRITE = 0x00000002;

#endif