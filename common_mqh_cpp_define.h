#ifndef COMMON_DEFINE_H

#define COMMON_DEFINE_H

#ifdef COMPILER_FOO_MQH5

#define BOOL                                                int
#define FALSE                                               0
#define DWORD                                               uint
#define SECURITY_ATTRIBUTES                                 PVOID
#define __int64                                             long
#define __CreateProcessW                                    CreateProcessW
#define __FindFirstFileW                                    FindFirstFileW
#define __UnlockFileEx                                      UnlockFileEx
#define __LockFileEx                                        LockFileEx
#define __CreateDirectoryW                                  CreateDirectoryW
#define __SetFilePointerEx                                  SetFilePointerEx
#define __GetFileTime                                       GetFileTime

#define MQH_ARRAY_ARG(type, name)                           const type& name[]

#define __FILEW__                                           __FILE__

#define _u(obj) obj

#endif

#ifdef COMPILER_FOO_CPP

#define _u(obj) u##obj


#endif

#define CPP_MQH_PRIVATE(obj)                                _u(obj)

#endif