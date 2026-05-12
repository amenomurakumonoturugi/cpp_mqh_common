

#ifndef COMMON_CLASS_LANGUAGE_H

#define COMMON_CLASS_LANGUAGE_H

class LANGUAGE_MANAGER {

private:

	MT5_LANGUAGE_NUMBER Lang_Num;

public:

	inline void Set_Lang_Num(const string& Lang) {

		if (Lang == TERMINAL_LANGUAGE_JP)
			Lang_Num = JP;

		else if (Lang == TERMINAL_LANGUAGE_EN)
			Lang_Num = EN;

		else
			Lang_Num = OTHERS;
	}

	inline ulong Get_Lang_Num() {

		return (ulong)Lang_Num;
	}
};

#endif