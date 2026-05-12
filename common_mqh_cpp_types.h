

#ifndef COMMON_TYPE_H

#define COMMON_TYPE_H

#ifdef COMPILER_FOO_MQH5

namespace std {

	template<typename To, typename From>
	To bit_cast(const From& from) {

		// 【安全性ガード】サイズが一致しない場合、ここでコンパイルエラーを誘発させる
		// MQL5にはstatic_assertがないため、負のサイズの配列定義という古典的ハックを使用
		int static_assertion[sizeof(To) == sizeof(From) ? 1 : -1];

		union Converter pack(1) {

			From f;
			To   t;
		} conv;

		conv.f = from;
		return conv.t;
	}
};

#endif

#ifdef COMPILER_FOO_CPP

template<typename T>
class MQH_VECTOR_INTERNAL {

private:

    std::vector<T> internal_vector;

public:

    struct DataProxy {

        MQH_VECTOR_INTERNAL<T>* owner;
        operator T* () { return owner->internal_vector.data(); }
        operator const T* () const { return owner->internal_vector.data(); }
    };

    DataProxy data;

    MQH_VECTOR_INTERNAL() {

        data.owner = this;
    }

    // コピーコンストラクタ
    // 別の Vector からコピーして新しく作る時に呼ばれる
    MQH_VECTOR_INTERNAL(const MQH_VECTOR_INTERNAL& other)
        : internal_vector(other.internal_vector) // 中身の vector は普通にコピー
    {
        // 住所をセットし直す
        data.owner = this;
    }

    // 3. 代入演算子
    // すでに存在する Vector に別の Vector を代入する時に呼ばれる
    MQH_VECTOR_INTERNAL& operator=(const MQH_VECTOR_INTERNAL& other) {

        if (this != &other) { // 自己代入チェック
            internal_vector = other.internal_vector;
            // ここでも自分の住所を再確認
            data.owner = this;
        }
        return *this;
    }

    // 1. 通常のアクセス用（非const）
    T& operator[](const size_t index) {

        return this->internal_vector[index]; // 内部配列へのアクセス
    }

    // 2. 読み取り専用アクセス用（const）
    const T& operator[](const size_t index) const {
       
        return this->internal_vector[index];
    }

    void push_back(const T& val) { internal_vector.push_back(val); }
    size_t size() const { return internal_vector.size(); }
};

template<typename T>
struct MQH_ArrayView_Internal {

	const T* m_ptr;

	// 既存の：ポインタから作る
	MQH_ArrayView_Internal(const T* p) : m_ptr(p) {}

	// 【追加】：ポインタに化けられるもの（DataProxyなど）から直接作る
	// これにより「2段階変換」を「1段階」に短縮します
	template<typename U>
	MQH_ArrayView_Internal(const U& proxy) : m_ptr((const T*)proxy) {}

	// 既存の：Vector本体から作る
	MQH_ArrayView_Internal(const MQH_VECTOR_INTERNAL<T>& v) : m_ptr(v.data) {}

	const T& operator[](int i) const { return m_ptr[i]; }
};

#define MQH_ARRAY_ARG(type, name) const MQH_ArrayView_Internal<type> name


class CPP_TYPE_STRING {

private
    :
    std::u16string Buf;
    mutable std::wstring Use_Win32;
    mutable bool Is_Dirty = true;

    using size_type = std::u16string::size_type;

public:

    static inline const size_type npos = std::u16string::npos;

    CPP_TYPE_STRING() : Is_Dirty(true) {}

    // _U("") からの入力
    CPP_TYPE_STRING(const char16_t* s) {

        if (s) 
            Buf.assign(s);

        Is_Dirty = true;
    }

    // WinAPI等、wchar_t* からの安全な取り込み
    CPP_TYPE_STRING(const wchar_t* s) {

        if (s) {

            Use_Win32 = s;
            Buf.assign(Use_Win32.begin(), Use_Win32.end());
        }

        Is_Dirty = false; // 同期直後なので false
    }

    operator const char16_t* () const {

        return Buf.c_str();
    }

    // --- const版：読み取り専用 ---
    operator const wchar_t* () const {

        if (Is_Dirty) {

            Use_Win32.assign(Buf.begin(), Buf.end());
            Is_Dirty = false;
        }

        return Use_Win32.c_str();
    }

    // --- 非const版：API書き込み用（要注意！） ---
    operator wchar_t* () {

        Is_Dirty = true;

        Use_Win32.assign(Buf.begin(), Buf.end());

        // もし空なら、最低限ヌル文字1つ分は確保してクラッシュを防ぐ
        if (Use_Win32.empty())
            Use_Win32.resize(1);

        return &Use_Win32[0];
    }

    // 1. クラス同士の比較 (obj1 == obj2)
    bool operator==(const CPP_TYPE_STRING& other) const {

        return this->Buf == other.Buf;
    }

    // 2. 文字列リテラルとの比較 (obj == u"test")
    bool operator==(const char16_t* s) const {

        if (!s) 
            return Buf.empty();

        return this->Buf == s;
    }

    void SyncBack() {

        // 物理的な最大サイズを取得
        size_t max_limit = Use_Win32.size();

        // wcsnlen は「\0 を探すが、max_limit を超えたらそこで止まる」安全な関数
        size_t real_len = wcsnlen_s(Use_Win32.c_str(), max_limit);

        // 特定された「本当の長さ」だけを Buf に連れ戻す
        Buf.assign(Use_Win32.begin(), Use_Win32.begin() + real_len);

        Is_Dirty = false;
    }

    // 外部から「API書き込み用に1024文字分確保する」といった指示を出す
    void ReserveForApi(size_t size) {

        Use_Win32.resize(size);
        Is_Dirty = true; // 内部が空でも API が書き込むので Dirty にしておく
    }

    void append(const char16_t* str) {

        if (str) Buf.append(str);
        Is_Dirty = true;
    }

    // 自分自身や std::u16string も連結できるようにする
    void append(const CPP_TYPE_STRING& other) {

        Buf.append(other.Buf);
        Is_Dirty = true;
    }

    void append(const wchar_t* str) {

        if (str) {

            // 安全な変換ルート（一度 Use_Win32 に入れてからイテレータで Buf へ）
            Use_Win32 = str;
            Buf.append(Use_Win32.begin(), Use_Win32.end());
        }

        Is_Dirty = true;
    }

    void assign(const wchar_t* str) {

        if (str) {

            // 安全な変換ルート（一度 Use_Win32 に入れてからイテレータで Buf へ）
            Use_Win32 = str;
            Buf.assign(Use_Win32.begin(), Use_Win32.end());
        }

        Is_Dirty = true;
    }

    void assign(const char16_t* str) {

        if (str) 
            Buf.assign(str);

        Is_Dirty = true;
    }

    CPP_TYPE_STRING substr(size_type pos = 0, size_type n = npos) const {
        
        return CPP_TYPE_STRING(Buf.substr(pos, n).c_str());
    }

    size_t size() {

        return Buf.size();
    }

    // 文字列（リテラル）を検索する場合
    size_type rfind(const char16_t* s, size_type pos = npos) const {

        return Buf.rfind(s, pos);
    }

    // 文字列（リテラル）を検索する場合
    size_type rfind(const wchar_t* s, size_type pos = npos) const {

        return Use_Win32.rfind(s, pos);
    }

    const wchar_t* data() const { return (const wchar_t*)*this; }
};

using string = CPP_TYPE_STRING;
using MQH_STRING_VECTOR = MQH_VECTOR_INTERNAL<string>;


using ulong = unsigned __int64;
using ushort = unsigned short;
using uint = unsigned int;

using FIND_DATAW = WIN32_FIND_DATAW;

class SAFE_MUTEX_HANDLE {

private:

    HANDLE            Handle;

public:
    // 1. デフォルトコンストラクターを「明示的」に一番上に書く
    SAFE_MUTEX_HANDLE()
    {
        Handle = INVALID_HANDLE_VALUE;
    }
    // デストラクタで確実に閉じる
    ~SAFE_MUTEX_HANDLE()
    {
        if (is_valid()) {

            ReleaseMutex(Handle);

            CloseHandle(Handle);

            Handle = INVALID_HANDLE_VALUE;
        }
    }

    // 値をセットする際、古いのがあれば閉じる
    void              Set(HANDLE new_h)
    {
        if (is_valid())
            CloseHandle(Handle);

        Handle = new_h;
    }

    // 2. コピーと代入を明示的に「禁止」する
    void              operator=(const SAFE_MUTEX_HANDLE&) = delete;
    SAFE_MUTEX_HANDLE(const SAFE_MUTEX_HANDLE&) = delete;

    bool              is_valid() const {

        return Handle != INVALID_HANDLE_VALUE && Handle != NULL;
    }

    HANDLE            get() const {

        return Handle;
    }
};

#endif

class SAFE_HANDLE {

private:

    HANDLE            Handle;

public:
    // 1. デフォルトコンストラクターを「明示的」に一番上に書く
    SAFE_HANDLE()
    {
        Handle = INVALID_HANDLE_VALUE;
    }
    // デストラクタで確実に閉じる
    ~SAFE_HANDLE()
    {
        if (is_valid()) {

            CloseHandle(Handle);
            Handle = INVALID_HANDLE_VALUE;
        }
            
    }

    // 値をセットする際、古いのがあれば閉じる
    void              Set(HANDLE new_h)
    {
        if (is_valid())
            CloseHandle(Handle);

        Handle = new_h;
    }

    // 2. コピーと代入を明示的に「禁止」する
    void              operator=(const SAFE_HANDLE&) = delete;
    SAFE_HANDLE(const SAFE_HANDLE&) = delete;

    bool              is_valid() const { 
        
        return Handle != INVALID_HANDLE_VALUE && Handle != NULL;
    }

    HANDLE            get() const {
        
        return Handle;
    }
};

#endif