#ifndef COMMON_DEFINE_H

#define COMMON_DEFINE_H

#ifdef __MQL5__

#define BOOL                                                int
#define FALSE                                               0
#define DWORD                                               uint
#define SECURITY_ATTRIBUTES                                 PVOID
#define __int64                                             long


#define MQH_ARRAY_ARG(type, name)                           const type& name[]

#define __FILEW__                                           __FILE__

#define _u(obj) obj

#endif

#ifdef __CPP__

#define _u(obj)                                             u##obj

#endif

#endif