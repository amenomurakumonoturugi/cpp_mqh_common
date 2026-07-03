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