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

#ifndef COMMON_SECRETS_H

#define COMMON_SECRETS_H

#define SYSTEM_VERSION 260.710


#define INPUT_TIME_INT_SIZE                    3
struct INPUT_VALUE
{
	int               Open1[INPUT_TIME_INT_SIZE];
	int               Open2[INPUT_TIME_INT_SIZE];
	int               Close1[INPUT_TIME_INT_SIZE];
	int               Close2[INPUT_TIME_INT_SIZE];
	int               If_Nanpin;
	int               If_Summer;
	int               Loss_Management;
	double            Percentage;
	int               Max_Position;
	double            Fixed;
	double            Commission_USD;
	__int64           Next_Trade;
	int               Time_Ber;
	int               Indicator_Type1;
	int               Indicator_Type2;
	int               Indicator_Type3;
	int               Indicator_Type4;
	int               Indicator_Type5;
	int               Indicator_Value1;
	int               Indicator_Value2;
	int               Indicator_Value3;
	int               Indicator_Value4;
	int               Indicator_Value5;
};

inline void Binary_To_Array(
	
	const ulong& value1,
	const ulong& value2,
	const ulong& value3,
	const ulong& value4,
	const ulong& value5,
	const ulong& value6,
	const ulong& value7,
	const ulong& value8,
	_vector<ulong>& result,
	const int& size) {

	result.resize(size);

	result.Set_at(0, value1);

	if (size >= 2) result.Set_at(1, value2);
	if (size >= 3) result.Set_at(2, value3);
	if (size >= 4) result.Set_at(3, value4);
	if (size >= 5) result.Set_at(4, value5);
	if (size >= 6) result.Set_at(5, value6);
	if (size >= 7) result.Set_at(6, value7);
	if (size >= 8) result.Set_at(7, value8);
}

inline int Get_Byte2_Binary_Array(const ulong value,const int& size, _vector<ushort>& result,const int& start) {

    for (int i = 0; i < size; i++) {

		ulong temp = value;

		for (int i = 0; i < size; i++) {

			result.Set_at(start + i, (ushort)(temp & 0xFF));
			temp >>= 8;
		}
    }

	return size + start;
}

inline void Xor_Decrypt_Byte2_Array(_vector<ushort>& array, const uchar& key) {

    int Size = array.size();

    for (int i = 0; i < Size; i++) {

        // ビットの反転。元のヌル文字(0)も暗号化（0 ^ key）されているため、
        // 途中でBreakせず、配列の末尾まで一気に復号フラット化する
        array.Set_at(i, (ushort)(array.Get_At(i) ^ key));
    }
}

// 公開ヘッダへの記述で文字列の復元だとは分からないようにシンプルな命名
inline string Get_Type_String(
	
	const ulong& value1,
	const int& values_size,
	const ulong value2 = 0,
	const ulong value3 = 0,
	const ulong value4 = 0,
	const ulong value5 = 0,
	const ulong value6 = 0,
	const ulong value7 = 0,
	const ulong value8 = 0,
	const uchar key = 0x5A) {

	int Byte_Size = sizeof(value1);
	_vector<ulong> Sorce;
	_vector<ushort> Dst;

	Dst.resize(values_size * Byte_Size);
	Binary_To_Array(value1, value2, value3, value4, value5, value6, value7, value8, Sorce, values_size);

	int Index = 0;

	for (int i = 0; i < Sorce.size(); i++) {
		Index = Get_Byte2_Binary_Array(Sorce.Get_At(i), Byte_Size, Dst, Index);
	}

	Xor_Decrypt_Byte2_Array(Dst, key);

	return ShortArrayToString(Dst);
}

// Local\\ReturnSafety_CONTROL_System_Log_Mutex
#define MUTEX_LOCAL_NAME_SYSTEM_LOG_PROCESS()  Get_Type_String(0x3F0806363B393516, 6, 0x3F3C3B0934282F2E, 0x080E14151905232E, 0x3F2E292309051615, 0x2F17053D35160537, 0x5A5A5A5A5A223F2E)

// ReturnSafety_CONTROL_Service_Log_Mutex
#define MUTEX_NAME_SERVICE_LOG_PROCESS()  Get_Type_String(0x3B0934282F2E3F08, 5, 0x14151905232E3F3C, 0x283F09051615080E, 0x3D3516053F39332C, 0x5A5A223F2E2F1705)

// Local\\ReturnSafety_CONTROL_Error_Mutex
#define MUTEX_LOCAL_NAME_ERROR_PROCESS()  Get_Type_String(0x3F0806363B393516, 5, 0x3F3C3B0934282F2E, 0x080E14151905232E, 0x283528281F051615, 0x5A5A223F2E2F1705)

// \\ReturnSafety
#define MAKER_FOLDER_DRC()  Get_Type_String(0x0934282F2E3F0806, 2, 0x5A5A5A232E3F3C3B)

// \\CONTROL
#define ITEM_FOLDER_DRC()  Get_Type_String(0x1615080E14151906, 2, 0x5A5A5A5A5A5A5A5A)

// C:\\Program Files
#define EXE_FOLDER_DRC()  Get_Type_String(0x283D35280A066019, 3, 0x293F36331C7A373B, 0x5A5A5A5A5A5A5A5A)

// \\err.bin
#define ITEM_ERR_MS_ERR_FILE_NAME()  Get_Type_String(0x3433387428283F06, 2, 0x5A5A5A5A5A5A5A5A)

// \\err_ms
#define ITEM_ERR_MS_FOLDER_DRC()  Get_Type_String(0x5A29370528283F06, 1)

// \\files
#define ITEM_ERR_MS_FILES_FOLDER_DRC()  Get_Type_String(0x5A5A293F36333C06, 1)

// \\err
#define ITEM_ERR_MS_ERR_FILE_DRC()  Get_Type_String(0x5A5A5A5A28283F06, 1)

// \\vis.bin
#define ITEM_ERR_MS_VISIBLE_ERR_NUM_FILE_NAME()  Get_Type_String(0x3433387429332C06, 2, 0x5A5A5A5A5A5A5A5A)

// \\vis
#define ITEM_ERR_MS_VISIBLE_FILE_DRC()  Get_Type_String(0x5A5A5A5A29332C06, 1)

// \\lang.bin
#define ITEM_LANGUAGE_FILE_NAME()  Get_Type_String(0x3338743D343B3606, 2, 0x5A5A5A5A5A5A5A34)

// \\lang
#define ITEM_LANGUAGE_FOLDER_DRC()  Get_Type_String(0x5A5A5A3D343B3606, 1)

// \\sl.bin
#define ITEM_SL_FILE_NAME()  Get_Type_String(0x5A34333874362906, 1)

// \\sl
#define ITEM_SL_FOLDER_DRC()  Get_Type_String(0x5A5A5A5A5A362906, 1)

// \\err_ms.exe
#define ITEM_ERR_MS_EXE_NAME()  Get_Type_String(0x7429370528283F06, 2, 0x5A5A5A5A5A3F223F)

// \\log_mng.exe
#define ITEM_LOG_MNG_EXE_NAME()  Get_Type_String(0x3D3437053D353606, 2, 0x5A5A5A5A3F223F74)

// \\logs
#define ITEM_LOG_FOLDER_DRC()  Get_Type_String(0x5A5A5A293D353606, 1)

// \\service
#define ITEM_SERVICE_FOLDER_DRC()  Get_Type_String(0x3F39332C283F2906, 2, 0x5A5A5A5A5A5A5A5A)

// \\directory.log
#define MT5_DRC_FILE_NAME()  Get_Type_String(0x352E393F28333E06, 2, 0x5A5A3D3536742328)

// \\MT5\\drc
#define MT5_DRC_FILE_DRC()  Get_Type_String(0x39283E066F0E1706, 2, 0x5A5A5A5A5A5A5A5A)

// \\RuleDefender2.exe
#define APP_FILE_NAME()  Get_Type_String(0x3C3F1E3F362F0806, 3, 0x3F7468283F3E343F, 0x5A5A5A5A5A5A3F22)

// \\r_d
#define APP_FILE_DRC()  Get_Type_String(0x5A5A5A5A3E052806, 1)

// \\CONTROL.ex5
#define EA_FILE_NAME()  Get_Type_String(0x1615080E14151906, 2, 0x5A5A5A5A6F223F74)

// \\CONTROLwatch.exe
#define WATCH_FILE_NAME()  Get_Type_String(0x1615080E14151906, 3, 0x223F7432392E3B2D, 0x5A5A5A5A5A5A5A3F)

// \\watch
#define WATCH_FILE_DRC()  Get_Type_String(0x5A5A32392E3B2D06, 1)

// \\Experts
#define EA_FILE_DRC()  Get_Type_String(0x292E283F2A221F06, 2, 0x5A5A5A5A5A5A5A5A)

// Japanese
#define TERMINAL_LANGUAGE_JP()  Get_Type_String(0x3F293F343B2A3B10, 2, 0x5A5A5A5A5A5A5A5A)

// English
#define TERMINAL_LANGUAGE_EN()  Get_Type_String(0x5A322933363D341F, 1)

// CONTROL system error
#define ERR_MS_WINDOW_TITLE()  Get_Type_String(0x7A1615080E141519, 3, 0x3F7A373F2E292329, 0x5A5A5A5A28352828)

// #32770
#define ERR_MS_WINDOW_CLASS()  Get_Type_String(0x5A5A6A6D6D686979, 1)

// \\system
#define ITEM_SYSTEM_LOG_FOLDER_DRC()  Get_Type_String(0x5A373F2E29232906, 1)

// \\current
#define ITEM_SYSTEM_CURRENT_LOG_FOLDER_DRC()  Get_Type_String(0x2E343F28282F3906, 2, 0x5A5A5A5A5A5A5A5A)

// \\days
#define ITEM_SYSTEM_DAY_LOG_FOLDER_DRC()  Get_Type_String(0x5A5A5A29233B3E06, 1)

#endif