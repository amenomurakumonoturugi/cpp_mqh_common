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

#ifndef COMMON_CLASS_LANGUAGE_H

#define COMMON_CLASS_LANGUAGE_H

class LANGUAGE_MANAGER {

private:

	MT5_LANGUAGE_NUMBER Lang_Num;

public:

	inline void Set_Lang_Num(const string& Lang) {

		if (Lang == TERMINAL_LANGUAGE_JP())
			Lang_Num = JP;

		else if (Lang == TERMINAL_LANGUAGE_EN())
			Lang_Num = EN;

		else
			Lang_Num = OTHERS;
	}

	inline ulong Get_Lang_Num() const {

		return (ulong)Lang_Num;
	}
};

#endif