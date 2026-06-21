

#ifndef COMMON_TYPE_H

#define COMMON_TYPE_H

#ifdef __MQL5__

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
    };
};

#endif




template<typename T>
class _vector {

private:

#define _VECTOR_BUF_PTR_NAME Buf_Ptr
#define _VECTOR_BUF_NAME Buf

#ifdef __MQL5__

#define _VECTOR_COPY_BUF_TYPE(name) T name[];
#define _VECTOR_MACRO_THIS_CAST(obj) GetPointer(obj)
#define _VECTOR_COPY_BUF_PROCESS(dst,source) source.Get_Array(dst)
#define _VECTOR_SYNC_BACK_PROCESS(source,dst) source.Set_Array(dst)
#define _VECTOR_SET_AT(name,index,value) name.Set_at(index,value)
#define _VECTOR_GET_AT(name,index) name.Get_At(index)

    template<typename U>
    class mql_vector {

    private:

        U Data[];

    public:

        mql_vector() {}
        ~mql_vector() {}

        mql_vector(const mql_vector& other) {

            ArrayResize(Data, ArraySize(other.Data));
            ArrayCopy(Data, other.Data);
        }

        void operator=(const mql_vector& other) {

            ArrayResize(Data, ArraySize(other.Data));
            ArrayCopy(Data, other.Data);
        }

        int size() {

            return ArraySize(Data);
        }

        void push_back(const U& val) {

            ArrayResize(Data, ArraySize(Data) + 1);
            Data[ArraySize(Data) - 1] = val;
        }

        bool empty() const {

            return (ArraySize(Data) == 0);
        }

        void resize(const int new_size) {

            ArrayResize(Data, new_size);
        }

        U Get_At(const int index) const {

            return Data[index];
        }

        void Set_at(const int index, const U& value) {

            Data[index] = value;
        }

        void Get_Array(U& dst[]) {

            ArrayCopy(dst, Data);
        }

        void Set_Array(const U& src[]) {

            ArrayResize(Data, ArraySize(src));
            ArrayCopy(Data, src);
        }
    };

#define _VECTOR_TYPE mql_vector<T> _VECTOR_BUF_NAME

#endif

#ifdef __CPP__

#define _VECTOR_COPY_BUF_TYPE(name) T* name;
#define _VECTOR_MACRO_THIS_CAST(obj) obj
#define _VECTOR_COPY_BUF_PROCESS(dst,source) dst = source.data()
#define _VECTOR_SYNC_BACK_PROCESS(source,dst)
#define _VECTOR_SET_AT(name,index,value) name.at(index)=value
#define _VECTOR_GET_AT(name,index) name.at(index)
#define _VECTOR_TYPE std::vector<T> _VECTOR_BUF_NAME

#endif

    _VECTOR_TYPE;
    _vector* Owner;

    void Copy_Buf() {

#ifdef __MQL5__
        ArrayResize(_VECTOR_BUF_PTR_NAME,size());
#endif

        _VECTOR_COPY_BUF_PROCESS( _VECTOR_BUF_PTR_NAME, _VECTOR_BUF_NAME);
    }

public:

    virtual ~_vector() {}

    _VECTOR_COPY_BUF_TYPE(_VECTOR_BUF_PTR_NAME)

    int size() {

        return (int)_VECTOR_BUF_NAME.size();
    }

    void push_back(const T val) {

        _VECTOR_BUF_NAME.push_back(val);

        Copy_Buf();
    }

    bool empty() const {

        return _VECTOR_BUF_NAME.empty();
    }

    void resize(const int new_size) {

        _VECTOR_BUF_NAME.resize(new_size);

        Copy_Buf();
    }

    void Set_at(const int index, const T value) {

        _VECTOR_SET_AT(_VECTOR_BUF_NAME, index, value);

        Copy_Buf();
    }

    T Get_At(const int index) {

        return _VECTOR_GET_AT(_VECTOR_BUF_NAME, index);
    }

    _vector() {

        Owner = _VECTOR_MACRO_THIS_CAST(this);

        Copy_Buf();
    }

    _vector(const _vector& other) {

        _VECTOR_BUF_NAME = other._VECTOR_BUF_NAME;
        Owner = _VECTOR_MACRO_THIS_CAST(this);
        Copy_Buf();
    }

    void operator=(const _vector& other) {

        if (Owner != _VECTOR_MACRO_THIS_CAST(other)) {

            _VECTOR_BUF_NAME = other._VECTOR_BUF_NAME;
            Owner = _VECTOR_MACRO_THIS_CAST(this);
            Copy_Buf();
        }
    }

    void Update_Internal() {

        _VECTOR_SYNC_BACK_PROCESS(_VECTOR_BUF_NAME, _VECTOR_BUF_PTR_NAME);
    }

#undef _VECTOR_COPY_BUF_TYPE
#undef _VECTOR_MACRO_THIS_CAST
#undef _VECTOR_BUF_PTR_NAME
#undef _VECTOR_BUF_NAME
#undef _VECTOR_COPY_BUF_PROCESS
#undef _VECTOR_SYNC_BACK_PROCESS
#undef _VECTOR_SET_AT
#undef _VECTOR_GET_AT
#undef _VECTOR_TYPE

};


#ifdef __CPP__

class CPP_TYPE_STRING {

#define DEFAULT_COUNT_SIZE -1

private:

    enum DIRTY_TYPE {

        Normal,
        Char_16,
        Win32
    };

    mutable std::u16string Buf;
    mutable std::wstring Use_Win32;
    mutable DIRTY_TYPE Is_Dirty;
    mutable bool Need_Win32_Resize;
    mutable std::recursive_mutex mtx;

    using size_type = std::u16string::size_type;

    void Reserve_Size_Win32(size_t size) {

        Use_Win32.resize(size);
    }

    bool SyncBack() const {

        size_t End_Null_P = Use_Win32.find(L'\0');

        if (End_Null_P != Use_Win32.npos) {

            Use_Win32.resize(End_Null_P );

            Need_Win32_Resize = false;

            return true;
        }

        else {

            // APIが何も書かなかった、あるいはヌルを書き忘れた場合
            // 異常事態だが、これ以上 Buf を「汚染」させないために Need_Win32_Resize を下ろす
            Need_Win32_Resize = false;

            return false;
        }  
    }

    void String_Same() const {

        std::lock_guard<std::recursive_mutex> lock(mtx);

        bool Next_Process = true;

        if (Need_Win32_Resize) {

            Next_Process= SyncBack();
        }
            
        if (Next_Process) {

            if (Is_Dirty == DIRTY_TYPE::Char_16) {

                Buf.assign(Use_Win32.begin(), Use_Win32.end());
            }

            if (Is_Dirty == DIRTY_TYPE::Win32) {

                Use_Win32.assign(Buf.begin(), Buf.end());
            }

            Is_Dirty = DIRTY_TYPE::Normal;
        }
    }

public:

    static inline const size_type npos = std::u16string::npos;

    CPP_TYPE_STRING() {

        Is_Dirty = DIRTY_TYPE::Normal;
        Need_Win32_Resize = false;
    };

    CPP_TYPE_STRING(const CPP_TYPE_STRING& other) {
        
        std::lock_guard<std::recursive_mutex> lock_other(other.mtx);

        other.String_Same();

        this->Buf = other.Buf;
        this->Use_Win32 = other.Use_Win32;
        this->Is_Dirty = other.Is_Dirty;
        this->Need_Win32_Resize = other.Need_Win32_Resize;
    }

    CPP_TYPE_STRING& operator=(const CPP_TYPE_STRING& other) {

        if (this == &other) {

            return *this;
        }

        std::unique_lock<std::recursive_mutex> lock_this(this->mtx, std::defer_lock);
        std::unique_lock<std::recursive_mutex> lock_other(other.mtx, std::defer_lock);
        std::lock(lock_this, lock_other);

        this->String_Same();
        other.String_Same();

        this->Buf = other.Buf;
        this->Use_Win32 = other.Use_Win32;
        this->Is_Dirty = other.Is_Dirty;
        this->Need_Win32_Resize = other.Need_Win32_Resize;

        return *this;
    }

    // _U("") からの入力（安全に初期化）
    CPP_TYPE_STRING(const char16_t* s) {

        Is_Dirty = DIRTY_TYPE::Normal;
        Need_Win32_Resize = false;

        if (s) {

            Buf.assign(s);
            Is_Dirty = DIRTY_TYPE::Win32; // char16_t側が最新
        }
    }

    // WinAPI等、wchar_t* からの安全な取り込み（安全に初期化）
    CPP_TYPE_STRING(const wchar_t* s) {

        Is_Dirty = DIRTY_TYPE::Normal;
        Need_Win32_Resize = false;

        if (s) {

            Use_Win32 = s;
            Is_Dirty = DIRTY_TYPE::Char_16; // Win32側が最新
        }
    }

    operator const char16_t* () const {

        std::lock_guard<std::recursive_mutex> lock(mtx);

        String_Same();

        return { Buf.c_str() };
    }

    // --- const版：読み取り専用 ---
    operator const wchar_t* () const {

        std::lock_guard<std::recursive_mutex> lock(mtx);

        String_Same();

        return { Use_Win32.c_str() };
    }

    // --- 非const版：API書き込み用（要注意！） ---
    operator wchar_t* () {

        std::lock_guard<std::recursive_mutex> lock(mtx);

        String_Same();

        Need_Win32_Resize = true;

        Reserve_Size_Win32(1025 + Buf.size());

        Use_Win32[1024 + Buf.size()] = L'\0';

        Is_Dirty = DIRTY_TYPE::Char_16;

        return { &Use_Win32[0] };
    }

    // 1. クラス同士の比較 (obj1 == obj2)
    bool operator==(const CPP_TYPE_STRING& other) const {

        if (this == &other)
            return true; // 自己比較の最適化

        std::unique_lock<std::recursive_mutex> lock_this(this->mtx, std::defer_lock);
        std::unique_lock<std::recursive_mutex> lock_other(other.mtx, std::defer_lock);
        std::lock(lock_this, lock_other);

        this->String_Same();
        other.String_Same(); // 相手も同期させる

        return this->Buf == other.Buf;
    }

    // 2. 文字列リテラルとの比較 (obj == u"test")
    bool operator==(const char16_t* s) const {

        std::lock_guard<std::recursive_mutex> lock(mtx);

        String_Same();

        if (!s) 
            return Buf.empty();

        return this->Buf == s;
    }

    void append(const char16_t* str, const int count = DEFAULT_COUNT_SIZE) {

        std::lock_guard<std::recursive_mutex> lock(mtx);

        String_Same();

        if (str) {

            if (count == DEFAULT_COUNT_SIZE)
                Buf.append(str);
            else
                Buf.append(str, count);

            Is_Dirty = DIRTY_TYPE::Win32;
        }
    }

    // 自分自身や std::u16string も連結できるようにする
    void append(const CPP_TYPE_STRING& other) {

        if (this == &other) {

            // 自分自身をアペンドする特殊ケースの自己防衛
            std::lock_guard<std::recursive_mutex> lock(mtx);

            String_Same();

            Buf.append(Buf);

            Use_Win32.append(Use_Win32);

            Is_Dirty = DIRTY_TYPE::Normal;
            return;

        }

        std::unique_lock<std::recursive_mutex> lock_this(this->mtx, std::defer_lock);
        std::unique_lock<std::recursive_mutex> lock_other(other.mtx, std::defer_lock);

        std::lock(lock_this, lock_other);

        this->String_Same();
        other.String_Same(); // 相手を叩き起こして同期させる

        this->Buf.append(other.Buf);
        this->Use_Win32.append(other.Use_Win32);
        this->Is_Dirty = DIRTY_TYPE::Normal;
    }

    void append(const wchar_t* str, const int count = DEFAULT_COUNT_SIZE) {

        std::lock_guard<std::recursive_mutex> lock(mtx);

        String_Same();

        if (str) {

            if (count == DEFAULT_COUNT_SIZE)
                Use_Win32.append(str);
            else
                Use_Win32.append(str, count);

            Is_Dirty = DIRTY_TYPE::Char_16;
        }
    }

    void assign(const CPP_TYPE_STRING& other) {

        if (this == &other) {

            // 自分自身
            std::lock_guard<std::recursive_mutex> lock(mtx);

            String_Same();

            Buf.assign(Buf);

            Use_Win32.assign(Use_Win32);

            Is_Dirty = DIRTY_TYPE::Normal;
            return;

        }

        std::unique_lock<std::recursive_mutex> lock_this(this->mtx, std::defer_lock);
        std::unique_lock<std::recursive_mutex> lock_other(other.mtx, std::defer_lock);

        std::lock(lock_this, lock_other);

        this->String_Same();
        other.String_Same(); // 相手を叩き起こして同期させる

        this->Buf.assign(other.Buf);
        this->Use_Win32.assign(other.Use_Win32);
        this->Is_Dirty = DIRTY_TYPE::Normal;
    }

    void assign(const wchar_t* str, const int count = DEFAULT_COUNT_SIZE) {

        std::lock_guard<std::recursive_mutex> lock(mtx);

        String_Same();

        if (str) {

            if (count == DEFAULT_COUNT_SIZE)
                Use_Win32.assign(str);
            else
                Use_Win32.assign(str, count);

            Is_Dirty = DIRTY_TYPE::Char_16;
        }
    }

    void assign(const char16_t* str, const int count = DEFAULT_COUNT_SIZE) {

        std::lock_guard<std::recursive_mutex> lock(mtx);

        String_Same();

        if (str) {

            if (count == DEFAULT_COUNT_SIZE)
                Buf.assign(str);
            else
                Buf.assign(str, count);

            Is_Dirty = DIRTY_TYPE::Win32;
        }
    }

    CPP_TYPE_STRING substr(size_type pos = 0, size_type n = npos) const {

        std::lock_guard<std::recursive_mutex> lock(mtx);

        String_Same();
        
        return CPP_TYPE_STRING(Buf.substr(pos, n).c_str());
    }

    size_t size() const {

        std::lock_guard<std::recursive_mutex> lock(mtx);

        String_Same();

        return Buf.size();
    }

    // 文字列（リテラル）を検索する場合
    size_type rfind(const CPP_TYPE_STRING& s, size_type pos = npos) const {

        if (this == &s) {

            // ★自分自身を検索する特殊ケースの自己防衛
            std::lock_guard<std::recursive_mutex> lock(mtx);
            String_Same();

            return Buf.rfind(Buf, pos);
        }

        // --- 別のオブジェクトを検索する通常ルート ---
        std::unique_lock<std::recursive_mutex> lock_this(this->mtx, std::defer_lock);
        std::unique_lock<std::recursive_mutex> lock_other(s.mtx, std::defer_lock);
        std::lock(lock_this, lock_other);

        String_Same();
        s.String_Same();

        return Buf.rfind(s.Buf, pos);
    }

    // 文字列（リテラル）を検索する場合
    size_type rfind(const wchar_t* s, size_type pos = npos) const {

        std::lock_guard<std::recursive_mutex> lock(mtx);

        String_Same();

        return Use_Win32.rfind(s, pos);
    }

    const wchar_t* data() const {

        std::lock_guard<std::recursive_mutex> lock(mtx);

        String_Same();
        
        return Use_Win32.c_str();
    }

#undef DEFAULT_COUNT_SIZE

};

using ulong = unsigned __int64;
using ushort = unsigned short;
using uint = unsigned int;
using uchar = unsigned char;

using string = CPP_TYPE_STRING;

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