#pragma once

#include <Windows.h>
#include <roapi.h>


extern "C" {
    /* From C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\ucrt\corecrt_terminate.h */
    typedef void (__CRTDECL* terminate_handler )(void);


    /* From C:\Program Files (x86)\Windows Kits\10\Source\10.0.20348.0\ucrt\inc\corecrt_internal.h */
    // Maximum lengths for the language name, country name, and full locale name.
    #define MAX_LANG_LEN 64
    #define MAX_CTRY_LEN 64
    #define MAX_LC_LEN   (MAX_LANG_LEN + MAX_CTRY_LEN + 3)

    typedef struct __crt_ctype_compatibility_data
    {
        unsigned long id;
        int           is_clike;
    } __crt_ctype_compatibility_data;

    typedef struct __crt_qualified_locale_data
    {
        // Static data for qualified locale code
        wchar_t const* pchLanguage;
        wchar_t const* pchCountry;
        int            iLocState;
        int            iPrimaryLen;
        BOOL           bAbbrevLanguage;
        BOOL           bAbbrevCountry;
        UINT           _cachecp;
        wchar_t        _cachein [MAX_LC_LEN];
        wchar_t        _cacheout[MAX_LC_LEN];

        // Static data for LC_CTYPE
        __crt_ctype_compatibility_data _Loc_c[5];
        wchar_t                        _cacheLocaleName[LOCALE_NAME_MAX_LENGTH];
    } __crt_qualified_locale_data;

    typedef struct __crt_qualified_locale_data_downlevel
    {
        // Static data for the downlevel qualified locale code
        int iLcidState;
        LCID lcidLanguage;
        LCID lcidCountry;
    } __crt_qualified_locale_data_downlevel;

    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //
    // AppCRT Threading
    //
    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    typedef struct __acrt_thread_parameter
    {
        // The thread procedure and context argument
        void*   _procedure;
        void*   _context;

        // The handle for the newly created thread.  This is initialized only from
        // _beginthread (not _beginthreadex).  When a thread created via _beginthread
        // exits, it frees this handle.
        HANDLE _thread_handle;

        // The handle for the module in which the user's thread procedure is defined.
        // This may be null if the handle could not be obtained.  This handle enables
        // us to bump the reference count of the user's module, to ensure that the
        // module will not be unloaded while the thread is executing.  When the thread
        // exits, it frees this handle.
        HMODULE _module_handle;

        // This flag is true if RoInitialized was called on the thread to initialize
        // it into the MTA.
        bool    _initialized_apartment;
    } __acrt_thread_parameter;


    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //
    // AppCRT Per-Thread Data
    //
    //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    typedef struct __acrt_ptd
    {
        // These three data members support signal handling and runtime errors
        struct __crt_signal_action_t* _pxcptacttab;     // Pointer to the exception-action table
        EXCEPTION_POINTERS*           _tpxcptinfoptrs;  // Pointer to the exception info pointers
        int                           _tfpecode;        // Last floating point exception code

        terminate_handler  _terminate;  // terminate() routine

        int                  _terrno;          // errno value
        unsigned long        _tdoserrno;       // _doserrno value

        unsigned int         _rand_state;      // Previous value of rand()

        // Per-thread strtok(), wcstok(), and mbstok() data:
        char*                _strtok_token;
        unsigned char*       _mbstok_token;
        wchar_t*             _wcstok_token;

        // Per-thread tmpnam() data:
        char*                _tmpnam_narrow_buffer;
        wchar_t*             _tmpnam_wide_buffer;

        // Per-thread time library data:
        char*                _asctime_buffer;  // Pointer to asctime() buffer
        wchar_t*             _wasctime_buffer; // Pointer to _wasctime() buffer
        struct tm*           _gmtime_buffer;   // Pointer to gmtime() structure

        char*                _cvtbuf;          // Pointer to the buffer used by ecvt() and fcvt().

        // Per-thread error message data:
        char*      _strerror_buffer;            // Pointer to strerror()  / _strerror()  buffer
        wchar_t*   _wcserror_buffer;            // Pointer to _wcserror() / __wcserror() buffer

        // Locale data:
        __crt_multibyte_data*                  _multibyte_info;
        __crt_locale_data*                     _locale_info;
        __crt_qualified_locale_data            _setloc_data;
        __crt_qualified_locale_data_downlevel* _setloc_downlevel_data;
        int                                    _own_locale;   // See _configthreadlocale() and __acrt_should_sync_with_global_locale()

        // The buffer used by _putch(), and the flag indicating whether the buffer
        // is currently in use or not.
        unsigned char  _putch_buffer[MB_LEN_MAX];
        unsigned short _putch_buffer_used;

        // The thread-local invalid parameter handler
        _invalid_parameter_handler _thread_local_iph;

        // If this thread was started by the CRT (_beginthread or _beginthreadex),
        // then this points to the context with which the thread was created.  If
        // this thread was not started by the CRT, this pointer is null.
        __acrt_thread_parameter* _beginthread_context;

    } __acrt_ptd;

    __acrt_ptd* __cdecl __acrt_getptd(void);
    __acrt_ptd* __cdecl __acrt_getptd_noexit(void);
    void        __cdecl __acrt_freeptd(void);

    void __cdecl __acrt_errno_map_os_error (unsigned long);

    HRESULT WINAPI __acrt_RoInitialize(
        _In_ RO_INIT_TYPE init_type
        );

    void WINAPI __acrt_RoUninitialize(void);

    // Specifies whether RoInitialize() should be called when creating a thread
    typedef enum
    {
        begin_thread_init_policy_unknown,
        begin_thread_init_policy_none,
        begin_thread_init_policy_ro_initialize
    } begin_thread_init_policy;

    begin_thread_init_policy __cdecl __acrt_get_begin_thread_init_policy(void);


    /* From C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\ucrt\corecrt_startup.h */
    _ACRTIMP int __cdecl _seh_filter_exe(
        _In_ unsigned long               _ExceptionNum,
        _In_ struct _EXCEPTION_POINTERS* _ExceptionPtr
        );


    /* From C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\ucrt\corecrt_malloc.h */
    _Check_return_ _Ret_maybenull_ _Post_writable_byte_size_(_Count * _Size)
    _ACRTIMP _CRTALLOCATOR _CRTRESTRICT
    void* __cdecl _calloc_base(
        _In_ size_t _Count,
        _In_ size_t _Size
        );

    _ACRTIMP
    void __cdecl _free_base(
        _Pre_maybenull_ _Post_invalid_ void* _Block
        );


    /* From C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\ucrt\process.h */
    typedef void     (__cdecl*   _beginthread_proc_type  )(void*);
    typedef unsigned (__stdcall* _beginthreadex_proc_type)(void*);


    /* From C:\Program Files (x86)\Windows Kits\10\Include\10.0.20348.0\ucrt\crtdbg.h */
    #define _ASSERT_EXPR(expr, msg) ((void)0)


    /* From C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\crt\src\vcruntime\internal_shared.h */
    #define _calloc_crt   _calloc_base
    #define _free_crt     _free_base

    #define _INVALID_PARAMETER(expr) _invalid_parameter_noinfo()

    #define _VALIDATE_RETURN(expr, errorcode, retexpr)                             \
        {                                                                          \
            int _Expr_val=!!(expr);                                                \
            _ASSERT_EXPR((_Expr_val), _CRT_WIDE(#expr));                           \
            if (!(_Expr_val))                                                      \
            {                                                                      \
                errno = errorcode;                                                 \
                _INVALID_PARAMETER(_CRT_WIDE(#expr));                              \
                return retexpr;                                                    \
            }                                                                      \
        }
}


extern "C++" {
    /* From C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\crt\src\vcruntime\internal_shared.h */
    struct __crt_internal_free_policy
    {
        template <typename T>
        void operator()(T const* const p) const noexcept
        {
            _free_crt(const_cast<T*>(p));
        }
    };

    template <typename T, typename Free = __crt_internal_free_policy>
    class __crt_unique_heap_ptr
    {
    public:

        explicit __crt_unique_heap_ptr(T* const p = nullptr) noexcept
            : _p(p)
        {
        }

        __crt_unique_heap_ptr(__crt_unique_heap_ptr const&) = delete;
        __crt_unique_heap_ptr& operator=(__crt_unique_heap_ptr const&) = delete;

        __crt_unique_heap_ptr(__crt_unique_heap_ptr&& other) noexcept
            : _p(other._p)
        {
            other._p = nullptr;
        }

        ~__crt_unique_heap_ptr() noexcept
        {
            release();
        }

        __crt_unique_heap_ptr& operator=(__crt_unique_heap_ptr&& other) noexcept
        {
            release();
            _p = other._p;
            other._p = nullptr;
            return *this;
        }

        T* detach() noexcept
        {
            T* const local_p{_p};
            _p = nullptr;
            return local_p;
        }

        void attach(T* const p) noexcept
        {
            release();
            _p = p;
        }

        void release() noexcept
        {
            Free()(_p);
            _p = nullptr;
        }

        bool is_valid() const noexcept
        {
            return _p != nullptr;
        }

        explicit operator bool() const noexcept
        {
            return is_valid();
        }

        T* get() const noexcept
        {
            return _p;
        }

        T** get_address_of() noexcept
        {
            return &_p;
        }

        T** release_and_get_address_of() noexcept
        {
            release();
            return &_p;
        }

    private:
        T* _p;
    };

	#define _calloc_crt_t(t, n)      (__crt_unique_heap_ptr     <t>(static_cast<t*>(_calloc_crt  (     (n),  sizeof(t)))))
}
