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

#ifndef __MQL5__
#define __CPP__
#endif

#ifdef __CPP__

#include <Windows.h>
#include <Shlobj.h>
#include <filesystem>
#include <mutex>

#include "common_mqh_cpp_define.h"
#include "common_mqh_cpp_types.h"
#include "common_mqh_cpp_constants.h"
#include "common_mqh_cpp_enums.h"
#include "common_mqh_cpp_method.h"
#include "common_mqh_cpp_secrets.h"
#include "common_mqh_cpp_class_error.h"
#include "common_mqh_cpp_lock.h"
#include "common_mqh_cpp_class_file_base.h"
#include "common_mqh_cpp_class_byte8.h"
#include "common_mqh_cpp_class_language.h"
#include "common_mqh_cpp_class_log_base.h"
#include "common_mqh_cpp_class_process.h"
#include "common_mqh_cpp_class_pipe.h"
#include "common_mqh_cpp_class_file_flow.h"

#endif

#ifdef __MQL5__

#include <WinAPI\winapi.mqh>

#import "ntdll.dll"
uint RtlGetLastWin32Error(void);
#import

#include "common_mqh_cpp_define.mqh"
#include "common_mqh_cpp_types.mqh"
#include "common_mqh_cpp_constants.mqh"
#include "common_mqh_cpp_method.mqh"
#include "common_mqh_cpp_enums.mqh"
#include "common_mqh_cpp_secrets.mqh"
#include "common_mqh_cpp_class_error.mqh"
#include "common_mqh_cpp_lock.mqh"
#include "common_mqh_cpp_class_file_base.mqh"
#include "common_mqh_cpp_class_byte8.mqh"
#include "common_mqh_cpp_class_language.mqh"
#include "common_mqh_cpp_class_log_base.mqh"
#include "common_mqh_cpp_class_process.mqh"
#include "common_mqh_cpp_class_pipe.mqh"
#include "common_mqh_cpp_class_file_flow.mqh"

#endif