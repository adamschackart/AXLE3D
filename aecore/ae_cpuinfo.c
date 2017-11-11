/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------

Simple DirectMedia Layer
Copyright (C) 1997-2016 Sam Lantinga <slouken@libsdl.org>

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the
use of this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject
to the following restrictions:

1.  The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

2.  Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

3.  This notice may not be removed or altered from any source distribution.

----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

// for file version information (could also link with Advapi32 for GetUserNameA)
#if defined(_WIN32)
    #pragma comment(lib, "version")
#endif

static int ae_cpuinfo_have_cpuid(void)
{
    int has_CPUID = 0;

    #if defined(__GNUC__) && defined(i386)
        __asm__ (
            "        pushfl                      # Get original EFLAGS             \n"
            "        popl    %%eax                                                 \n"
            "        movl    %%eax,%%ecx                                           \n"
            "        xorl    $0x200000,%%eax     # Flip ID bit in EFLAGS           \n"
            "        pushl   %%eax               # Save new EFLAGS value on stack  \n"
            "        popfl                       # Replace current EFLAGS value    \n"
            "        pushfl                      # Get new EFLAGS                  \n"
            "        popl    %%eax               # Store new EFLAGS in EAX         \n"
            "        xorl    %%ecx,%%eax         # Can not toggle ID bit,          \n"
            "        jz      1f                  # Processor=80486                 \n"
            "        movl    $1,%0               # We have CPUID support           \n"
            "1:                                                                    \n"
            : "=m" (has_CPUID)
            :
            : "%eax", "%ecx"
        );

    #elif defined(__GNUC__) && defined(__x86_64__)
        // technically, if this is being compiled under __x86_64__ then it
        // has CPUid by definition. but it's nice to be able to prove it.
        __asm__ (
            "        pushfq                      # Get original EFLAGS             \n"
            "        popq    %%rax                                                 \n"
            "        movq    %%rax,%%rcx                                           \n"
            "        xorl    $0x200000,%%eax     # Flip ID bit in EFLAGS           \n"
            "        pushq   %%rax               # Save new EFLAGS value on stack  \n"
            "        popfq                       # Replace current EFLAGS value    \n"
            "        pushfq                      # Get new EFLAGS                  \n"
            "        popq    %%rax               # Store new EFLAGS in EAX         \n"
            "        xorl    %%ecx,%%eax         # Can not toggle ID bit,          \n"
            "        jz      1f                  # Processor=80486                 \n"
            "        movl    $1,%0               # We have CPUID support           \n"
            "1:                                                                    \n"
            : "=m" (has_CPUID)
            :
            : "%rax", "%rcx"
        );

    #elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
        __asm
        {
            pushfd                      ; Get original EFLAGS
            pop     eax
            mov     ecx, eax
            xor     eax, 200000h        ; Flip ID bit in EFLAGS
            push    eax                 ; Save new EFLAGS value on stack
            popfd                       ; Replace current EFLAGS value
            pushfd                      ; Get new EFLAGS
            pop     eax                 ; Store new EFLAGS in EAX
            xor     eax, ecx            ; Can not toggle ID bit,
            jz      done                ; Processor=80486
            mov     has_CPUID,1         ; We have CPUID support
        done:
        }

    #elif defined(_MSC_VER) && defined(_M_X64)
        has_CPUID = 1;

    #elif defined(__sun) && defined(__i386)
        __asm (
            "       pushfl                 \n"
            "       popl    %eax           \n"
            "       movl    %eax,%ecx      \n"
            "       xorl    $0x200000,%eax \n"
            "       pushl   %eax           \n"
            "       popfl                  \n"
            "       pushfl                 \n"
            "       popl    %eax           \n"
            "       xorl    %ecx,%eax      \n"
            "       jz      1f             \n"
            "       movl    $1,-8(%ebp)    \n"
            "1:                            \n"
        );

    #elif defined(__sun) && defined(__amd64)
        __asm (
            "       pushfq                 \n"
            "       popq    %rax           \n"
            "       movq    %rax,%rcx      \n"
            "       xorl    $0x200000,%eax \n"
            "       pushq   %rax           \n"
            "       popfq                  \n"
            "       pushfq                 \n"
            "       popq    %rax           \n"
            "       xorl    %ecx,%eax      \n"
            "       jz      1f             \n"
            "       movl    $1,-8(%rbp)    \n"
            "1:                            \n"
        );

    #else
        #error TODO
    #endif
    return has_CPUID;
}

/* ===== [ cpuid ] ========================================================== */

#if defined(__GNUC__) && defined(i386)
    #define ae_cpuid(func, a, b, c, d)          \
        __asm__ __volatile__ (                  \
            "        pushl %%ebx        \n"     \
            "        xorl %%ecx,%%ecx   \n"     \
            "        cpuid              \n"     \
            "        movl %%ebx, %%esi  \n"     \
            "        popl %%ebx         \n" :   \
            "=a" (a), "=S" (b), "=c" (c), "=d" (d) : "a" (func))

#elif defined(__GNUC__) && defined(__x86_64__)
    #define ae_cpuid(func, a, b, c, d)          \
        __asm__ __volatile__ (                  \
            "        pushq %%rbx        \n"     \
            "        xorq %%rcx,%%rcx   \n"     \
            "        cpuid              \n"     \
            "        movq %%rbx, %%rsi  \n"     \
            "        popq %%rbx         \n" :   \
            "=a" (a), "=S" (b), "=c" (c), "=d" (d) : "a" (func))

#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
    #define ae_cpuid(func, a, b, c, d)  \
        __asm                           \
        {                               \
            __asm mov eax, func         \
            __asm xor ecx, ecx          \
            __asm cpuid                 \
            __asm mov a, eax            \
            __asm mov b, ebx            \
            __asm mov c, ecx            \
            __asm mov d, edx            \
        }

#elif defined(_MSC_VER) && defined(_M_X64)
    #define ae_cpuid(func, a, b, c, d)  \
    {                                   \
        int CPUInfo[4];                 \
        __cpuid(CPUInfo, func);         \
        a = CPUInfo[0];                 \
        b = CPUInfo[1];                 \
        c = CPUInfo[2];                 \
        d = CPUInfo[3];                 \
    }

#else
    #define ae_cpuid(func, a, b, c, d) a = b = c = d = 0
#endif

static int ae_cpuid_features(void)
{
    int features = 0, a, b, c, d;

    ae_cpuid(0, a, b, c, d);
    if (a >= 1)
    {
        ae_cpuid(1, a, b, c, d);
        features = d;
    }
    return features;
}

static int ae_cpuinfo_saves_ymm(void)
{
    int a, b, c, d;

    // check cpu version info to make sure we can call the xgetbv instruction
    ae_cpuid(0, a, b, c, d);
    if (a < 1) { return 0; }

    ae_cpuid(1, a, b, c, d);
    if (!(c & 0x08000000)) { return 0; }

    a = 0; // call the xgetbv instruction to see if YMM register state is saved

    #if defined(__GNUC__) && (defined(i386) || defined(__x86_64__))
        asm(".byte 0x0f, 0x01, 0xd0" : "=a" (a) : "c" (0) : "%edx");

    #elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64)) && (_MSC_FULL_VER >= 160040219)
        a = (int)_xgetbv(0); // requires Visual Studio 2010 Service Pack 1

    #elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
        __asm
        {
            xor ecx, ecx
            _asm _emit 0x0f _asm _emit 0x01 _asm _emit 0xd0
            mov a, eax
        }
    #endif
    return ((a & 6) == 6) ? 1 : 0;
}

/*
================================================================================
 * ~~ [ compiler name ] ~~ *
--------------------------------------------------------------------------------
*/

#if defined(__GNUC__)

const char* ae_compiler_name(void)
{
    #if defined(i386)
        #define _GCC_SUFFIX "32-bit (Intel)"
    #elif defined(__x86_64__)
        #define _GCC_SUFFIX "64-bit (AMD64)"
    #else
        #define _GCC_SUFFIX "(Unknown Target)"
    #endif

    // clang shows the GCC version it's compatible with at the beginning of
    // __VERSION__ (with clang elsewhere), so this works for both compilers.
    return "GCC " __VERSION__ " " _GCC_SUFFIX;
}

#elif defined(_MSC_VER)

const char* ae_compiler_name(void)
{
    #if _MSC_VER == 1900
        #define _MS_PASTE_VERSION(SUFFIX) ("Visual Studio 2015 " SUFFIX)
    #elif _MSC_VER == 1800
        #define _MS_PASTE_VERSION(SUFFIX) ("Visual Studio 2013 " SUFFIX)
    #elif _MSC_VER == 1700
        #define _MS_PASTE_VERSION(SUFFIX) ("Visual Studio 2012 " SUFFIX)
    #elif _MSC_VER == 1600
        #define _MS_PASTE_VERSION(SUFFIX) ("Visual Studio 2010 " SUFFIX)
    #elif _MSC_VER == 1500
        #define _MS_PASTE_VERSION(SUFFIX) ("Visual Studio 2008 " SUFFIX)
    #elif _MSC_VER == 1400
        #define _MS_PASTE_VERSION(SUFFIX) ("Visual Studio 2005 " SUFFIX)
    #elif _MSC_VER == 1310
        #define _MS_PASTE_VERSION(SUFFIX) ("Visual Studio 2003 " SUFFIX)
    #else
        // paste the actual value of _MSC_VER into the name string.
        // ganked from python's pyconfig.h with some modifications.
        #define _MS_PASTE_VERSION(SUFFIX) \
            ( "MSVC version " _MS_STRINGIZE(_MSC_VER) " " SUFFIX )

        #define _MS_STRINGIZE(X) _MS_STRINGIZE1((X))
        #define _MS_STRINGIZE1(X) _MS_STRINGIZE2 ## X
        #define _MS_STRINGIZE2(X) #X
    #endif

    #if defined(_WIN64)
        #if defined(_M_IA64)
            return _MS_PASTE_VERSION("64-bit (Itanium)");
        #elif defined(_M_X64) || defined(_M_AMD64)
            return _MS_PASTE_VERSION("64-bit (AMD64)");
        #else
            return _MS_PASTE_VERSION("64-bit (Unknown)");
        #endif

    #elif defined(_WIN32)
        #if defined(_M_IX86)
            return _MS_PASTE_VERSION("32-bit (Intel)");
        #else
            return _MS_PASTE_VERSION("32-bit (Unknown)");
        #endif

    #elif defined(__cplusplus)
        return "Unknown Microsoft Windows C++ Compiler";
    #else
        return "Unknown Microsoft Windows C Compiler";
    #endif
}

#else

const char* ae_compiler_name(void)
{
    #if defined(__cplusplus)
        return "Unknown C++ Compiler";
    #else
        return "Unknown C Compiler";
    #endif
}

#endif

const char* ae_language_name(void)
{
    #if defined(__cplusplus)
        return "C++";
    #else
        return "C";
    #endif
}

const char* ae_buildmode_name(void)
{
    #if defined(AE_DEBUG)
        return "debug";
    #else
        return "release";
    #endif
}

const char* ae_linkage_mode(void)
{
    #if defined(AE_COMPILING_DLL)
        return "dynamic";
    #else
        return "static";
    #endif
}

/*
================================================================================
 * ~~ [ operating system name ] ~~ *
--------------------------------------------------------------------------------
TODO: this needs version.dll on win32, open with LoadLibrary instead of linking
--------------------------------------------------------------------------------
*/

#if defined(_WIN32)

const char* ae_platform_name(void)
{
    static char win32platform[256];
    if (!win32platform[0])
    {
        /* NOTE According to MSDN:

            "With the release of Windows 8.1, the behavior of the GetVersionEx
            API has changed in the value it will return for the operating system
            version. The value returned by the GetVersionEx function now depends
            on how the application is manifested. Applications not manifested
            for Windows 8.1 or Windows 10 will return the Windows 8 OS version
            value (6.2). Once an application is manifested for a given operating
            system version, GetVersionEx will always return the version that the
            application is manifested for in future releases."

        This means that in order to properly query the version of Windows we're
        running, we have to find out which version a system DLL is targeted for.
        */
        HMODULE kernel32 = GetModuleHandleA("kernel32.dll");

        if (kernel32)
        {
            char kernel32_path[256]; // GetFileVersionInfo wants the module's path

            const DWORD actual_kernel32_path_length = GetModuleFileNameA(kernel32,
                                            kernel32_path, sizeof(kernel32_path));

            if (actual_kernel32_path_length)
            {
                // XXX: instead of dynamically allocating the memory for the file
                // version info, we just way over-allocate 256k of stack instead.
                // In reality, a single page has more than enough space for this.
                u8 file_version_info[1024 * 256];

                DWORD file_version_size = GetFileVersionInfoSizeA(kernel32_path, NULL);
                assert(actual_kernel32_path_length == strlen(kernel32_path)); // check

                // not needed, but we may as well fault some stack pages anyways >:)
                SecureZeroMemory(file_version_info, sizeof(file_version_info));

                if (file_version_size)
                {
                    assert(file_version_size <= sizeof(file_version_info));

                    if (GetFileVersionInfoA(kernel32_path, 0U, file_version_size,
                        file_version_info)) // load file version info into block
                    {
                        VS_FIXEDFILEINFO* pvi;
                        UINT pvi_size;

                        // VerQueryValue points this to some struct (cast for cpp)
                        LPVOID* pvip = (LPVOID*)&pvi;

                        if (VerQueryValueA(file_version_info, "", pvip, &pvi_size))
                        {
                            const u32 major = pvi->dwProductVersionMS >> 16;
                            const u32 minor = pvi->dwProductVersionMS & 0xFFFF;
                            const u32 build = pvi->dwProductVersionLS >> 16;
                            const u32 exten = pvi->dwProductVersionLS & 0xFFFF;

                            // TODO: what is exten? get marketing name, service pack
                            _snprintf(win32platform, sizeof(win32platform) - 1,
                                "Windows %u.%u.%u.%u", major, minor, build, exten);
                        }
                        else
                        {
                            AE_WARN("VerQueryValueA failed"); // TODO: info?
                            strcpy(win32platform, "Unknown Win32 Platform");
                        }
                    }
                    else
                    {
                        AE_WARN("GetFileVersionInfoA failed"); // TODO: info?
                        strcpy(win32platform, "Unknown Win32 Platform");
                    }
                }
                else
                {
                    AE_WARN("GetFileVersionInfoSizeA failed"); // TODO: info?
                    strcpy(win32platform, "Unknown Win32 Platform");
                }
            }
            else
            {
                AE_WARN("GetModuleFileNameA failed"); // TODO: info?
                strcpy(win32platform, "Unknown Win32 Platform");
            }
        }
        else
        {
            AE_WARN("kernel32.dll not loaded! need to use LoadLibrary instead");
            strcpy(win32platform, "Unknown Win32 Platform"); // TODO: test this
        }
    }
    return win32platform;
}

#elif defined(__unix__)

const char* ae_platform_name(void)
{
    // NOTE: goofy GCC warnings assume huge strings, so we had to bump this
    static char unix_platform[512];

    if (!unix_platform[0])
    {
        struct utsname info;
        if ( !uname(&info) )
        {
            snprintf(unix_platform, sizeof(unix_platform), "%s %s %s on %s",
                    info.sysname, info.release, info.version, info.machine);
        }
        else
        {
            AE_WARN("uname failed"); // TODO: get more info
            strcpy(unix_platform, "Unknown Unix Platform");
        }
    }

    return unix_platform;
}

#else

const char* ae_platform_name(void)
{
    return "Unknown Platform";
}

#endif

/*
================================================================================
 * ~~ [ processor name ] ~~ *
--------------------------------------------------------------------------------
*/

const char* ae_cpuinfo_name(void)
{
    static char ae_cpu_name[48]; // get the longer cpu name string

    if (!ae_cpu_name[0])
    {
        int i = 0, a, b, c, d;

        if (ae_cpuinfo_have_cpuid())
        {
        #if 0
            ae_cpuid(0, a, b, c, d); // short string for reference

            ((int*)ae_cpu_name)[0] = b;
            ((int*)ae_cpu_name)[1] = d;
            ((int*)ae_cpu_name)[2] = c;
        #else
            ae_cpuid(0x80000000, a, b, c, d);
            if (a >= 0x80000004)
            {
                ae_cpuid(0x80000002, a, b, c, d);
                ae_cpu_name[i++] = (char)(a & 0xff); a >>= 8;
                ae_cpu_name[i++] = (char)(a & 0xff); a >>= 8;
                ae_cpu_name[i++] = (char)(a & 0xff); a >>= 8;
                ae_cpu_name[i++] = (char)(a & 0xff); a >>= 8;
                ae_cpu_name[i++] = (char)(b & 0xff); b >>= 8;
                ae_cpu_name[i++] = (char)(b & 0xff); b >>= 8;
                ae_cpu_name[i++] = (char)(b & 0xff); b >>= 8;
                ae_cpu_name[i++] = (char)(b & 0xff); b >>= 8;
                ae_cpu_name[i++] = (char)(c & 0xff); c >>= 8;
                ae_cpu_name[i++] = (char)(c & 0xff); c >>= 8;
                ae_cpu_name[i++] = (char)(c & 0xff); c >>= 8;
                ae_cpu_name[i++] = (char)(c & 0xff); c >>= 8;
                ae_cpu_name[i++] = (char)(d & 0xff); d >>= 8;
                ae_cpu_name[i++] = (char)(d & 0xff); d >>= 8;
                ae_cpu_name[i++] = (char)(d & 0xff); d >>= 8;
                ae_cpu_name[i++] = (char)(d & 0xff); d >>= 8;

                ae_cpuid(0x80000003, a, b, c, d);
                ae_cpu_name[i++] = (char)(a & 0xff); a >>= 8;
                ae_cpu_name[i++] = (char)(a & 0xff); a >>= 8;
                ae_cpu_name[i++] = (char)(a & 0xff); a >>= 8;
                ae_cpu_name[i++] = (char)(a & 0xff); a >>= 8;
                ae_cpu_name[i++] = (char)(b & 0xff); b >>= 8;
                ae_cpu_name[i++] = (char)(b & 0xff); b >>= 8;
                ae_cpu_name[i++] = (char)(b & 0xff); b >>= 8;
                ae_cpu_name[i++] = (char)(b & 0xff); b >>= 8;
                ae_cpu_name[i++] = (char)(c & 0xff); c >>= 8;
                ae_cpu_name[i++] = (char)(c & 0xff); c >>= 8;
                ae_cpu_name[i++] = (char)(c & 0xff); c >>= 8;
                ae_cpu_name[i++] = (char)(c & 0xff); c >>= 8;
                ae_cpu_name[i++] = (char)(d & 0xff); d >>= 8;
                ae_cpu_name[i++] = (char)(d & 0xff); d >>= 8;
                ae_cpu_name[i++] = (char)(d & 0xff); d >>= 8;
                ae_cpu_name[i++] = (char)(d & 0xff); d >>= 8;

                ae_cpuid(0x80000004, a, b, c, d);
                ae_cpu_name[i++] = (char)(a & 0xff); a >>= 8;
                ae_cpu_name[i++] = (char)(a & 0xff); a >>= 8;
                ae_cpu_name[i++] = (char)(a & 0xff); a >>= 8;
                ae_cpu_name[i++] = (char)(a & 0xff); a >>= 8;
                ae_cpu_name[i++] = (char)(b & 0xff); b >>= 8;
                ae_cpu_name[i++] = (char)(b & 0xff); b >>= 8;
                ae_cpu_name[i++] = (char)(b & 0xff); b >>= 8;
                ae_cpu_name[i++] = (char)(b & 0xff); b >>= 8;
                ae_cpu_name[i++] = (char)(c & 0xff); c >>= 8;
                ae_cpu_name[i++] = (char)(c & 0xff); c >>= 8;
                ae_cpu_name[i++] = (char)(c & 0xff); c >>= 8;
                ae_cpu_name[i++] = (char)(c & 0xff); c >>= 8;
                ae_cpu_name[i++] = (char)(d & 0xff); d >>= 8;
                ae_cpu_name[i++] = (char)(d & 0xff); d >>= 8;
                ae_cpu_name[i++] = (char)(d & 0xff); d >>= 8;
                ae_cpu_name[i++] = (char)(d & 0xff); d >>= 8;
            }
        #endif
        }

        if (!ae_cpu_name[0]) { strcpy(ae_cpu_name, "Unknown CPU"); }

        // AJS: added to original SDL version: strip leading whitespace
        for (i = 0; i < sizeof(ae_cpu_name); i++)
        {
            if (!isspace(ae_cpu_name[i])) break;
        }

        if (i)
        {
            char temp_name[sizeof(ae_cpu_name)];
            strcpy(temp_name, ae_cpu_name);
            strcpy(ae_cpu_name, temp_name + i);
        }
    }

    return ae_cpu_name;
}

/*
================================================================================
 * ~~ [ username and hostname ] ~~ *
--------------------------------------------------------------------------------
*/

#if defined(_WIN32)

const char* ae_username(void)
{
    static char username[256];
    if (!username[0])
    {
    //  DWORD length = ( DWORD )sizeof(username);
    //  if (!GetUserNameA(username, &length))
        {
            char* env = getenv("USERNAME");
            if (env) strcpy(username, env); else
            {
                strcpy(username, "Unknown User");
            }
        }
    }
    return username;
}

const char* ae_hostname(void)
{
    static char hostname[256];
    if (!hostname[0])
    {
        DWORD length = ( DWORD )sizeof(hostname);
        if (!GetComputerNameA(hostname, &length))
        {
            char* env = getenv("COMPUTERNAME");
            if (env) strcpy(hostname, env); else
            {
                strcpy(hostname, "Unknown Host");
            }
        }
    }
    return hostname;
}

#elif defined(__unix__)

const char* ae_username(void)
{
    static char username[256];
    if (!username[0])
    {
        char* env = NULL; // TODO: getlogin_r?

        if (!env) env = getenv("USER");
        if (!env) env = getenv("USERNAME");
        if (!env) env = getenv("LOGNAME");

        if (env) strcpy(username, env); else
        {
            strcpy(username, "Unknown User");
        }
    }
    return username;
}

const char* ae_hostname(void)
{
    static char hostname[256];
    if (!hostname[0])
    {
        // NOTE: this exists in windows sockets also
        if (gethostname(hostname, sizeof(hostname)))
        {
            strcpy(hostname, "Unknown Host");
        }
    }
    return hostname;
}

#else

const char* ae_username(void) { return "Unknown User"; }
const char* ae_hostname(void) { return "Unknown Host"; }

#endif

/*
================================================================================
 * ~~ [ cpu feature detection ] ~~ *
--------------------------------------------------------------------------------
*/

static u32 ae_cpu_features; // cpu features bits are set once at initialization

#define AE_CPUINFO_RDTSC    (1 << 0)
#define AE_CPUINFO_SSE      (1 << 1)
#define AE_CPUINFO_SSE2     (1 << 2)
#define AE_CPUINFO_SSE3     (1 << 3)
#define AE_CPUINFO_SSE41    (1 << 4)
#define AE_CPUINFO_SSE42    (1 << 5)
#define AE_CPUINFO_AVX      (1 << 6)
#define AE_CPUINFO_AVX2     (1 << 7)

int ae_cpuinfo_rdtsc(void)
{
    return ae_cpu_features & AE_CPUINFO_RDTSC ? 1 : 0;
}

int ae_cpuinfo_sse(void)
{
    return ae_cpu_features & AE_CPUINFO_SSE ? 1 : 0;
}

int ae_cpuinfo_sse2(void)
{
    return ae_cpu_features & AE_CPUINFO_SSE2 ? 1 : 0;
}

int ae_cpuinfo_sse3(void)
{
    return ae_cpu_features & AE_CPUINFO_SSE3 ? 1 : 0;
}

int ae_cpuinfo_sse41(void)
{
    return ae_cpu_features & AE_CPUINFO_SSE41 ? 1 : 0;
}

int ae_cpuinfo_sse42(void)
{
    return ae_cpu_features & AE_CPUINFO_SSE42 ? 1 : 0;
}

int ae_cpuinfo_avx(void)
{
    return ae_cpu_features & AE_CPUINFO_AVX ? 1 : 0;
}

int ae_cpuinfo_avx2(void)
{
    return ae_cpu_features & AE_CPUINFO_AVX2 ? 1 : 0;
}

/*
================================================================================
 * ~~ [ initialization ] ~~ *
--------------------------------------------------------------------------------
TODO: ae_running_on(MOBILE), ae_running_on_s("pc"), ae_running_on_console() etc.
--------------------------------------------------------------------------------
TODO: check to ensure only a single instance of our process is running anywhere?
use a pidfile on unix, and create a named global mutex or a lock file on windows
--------------------------------------------------------------------------------
*/

/* TODO:
    - cache line size for threads
    - total/free physical memory
    - total/free virtual memory
    - total/free hard drive space
    - the short 12-char cpu name
    - physical/logical core count
*/

void ae_cpuinfo_init(int argc, char** argv)
{
    if (ae_cpuinfo_have_cpuid())
    {
        int a, b, c, d;

        if (ae_cpuid_features() & 0x00000010) ae_cpu_features |= AE_CPUINFO_RDTSC;
        if (ae_cpuid_features() & 0x02000000) ae_cpu_features |= AE_CPUINFO_SSE;
        if (ae_cpuid_features() & 0x04000000) ae_cpu_features |= AE_CPUINFO_SSE2;

        ae_cpuid(0, a, b, c, d);
        if (a >= 1)
        {
            ae_cpuid(1, a, b, c, d);
            if (c & 0x00000001) ae_cpu_features |= AE_CPUINFO_SSE3;
        }

        ae_cpuid(0, a, b, c, d);
        if (a >= 1)
        {
            ae_cpuid(1, a, b, c, d);
            if (c & 0x00080000) ae_cpu_features |= AE_CPUINFO_SSE41;
        }

        ae_cpuid(0, a, b, c, d);
        if (a >= 1)
        {
            ae_cpuid(1, a, b, c, d);
            if (c & 0x00100000) ae_cpu_features |= AE_CPUINFO_SSE42;
        }

        if (ae_cpuinfo_saves_ymm())
        {
            ae_cpuid(0, a, b, c, d);
            if (a >= 1)
            {
                ae_cpuid(1, a, b, c, d);
                if (c & 0x10000000) ae_cpu_features |= AE_CPUINFO_AVX;
            }

            ae_cpuid(0, a, b, c, d);
            if (a >= 7)
            {
                ae_cpuid(7, a, b, c, d);
                if (b & 0x00000020) ae_cpu_features |= AE_CPUINFO_AVX2;
            }
        }
    }

    ae_log(CPUINFO, "running %s %s build with profiling %sabled", // XXX show in time?
        ae_language_name(), ae_buildmode_name(), ae_profile_enabled() ? "en" : "dis");

    ae_log(CPUINFO, "branch coverage %sabled", // XXX move this
                    ae_branch_report_enabled() ? "en" : "dis");

    ae_log(CPUINFO, "switch coverage %sabled", // XXX move this
                    ae_switch_report_enabled() ? "en" : "dis");

    ae_log(CPUINFO, "ae linked %sally", ae_linkage_mode());
    ae_log(CPUINFO, "compiled with %s", ae_compiler_name());
    ae_log(CPUINFO, "cpu string is %s", ae_cpuinfo_name());
    ae_log(CPUINFO, "runtime OS is %s", ae_platform_name());

    ae_log(CPUINFO, "user login is %s @ %s", ae_username(), ae_hostname());
    ae_log(CPUINFO, "endianness is %s", ae_cpuinfo_lil_endian() ? "little" : "big");
    ae_log(CPUINFO, "registers are %u-bit", (u32)sizeof(void*) * CHAR_BIT);

    ae_log(CPUINFO, "has RDTSC   = %s", ae_cpuinfo_rdtsc () ? "true" : "false");
    ae_log(CPUINFO, "has SSE     = %s", ae_cpuinfo_sse   () ? "true" : "false");
    ae_log(CPUINFO, "has SSE 2   = %s", ae_cpuinfo_sse2  () ? "true" : "false");
    ae_log(CPUINFO, "has SSE 3   = %s", ae_cpuinfo_sse3  () ? "true" : "false");
    ae_log(CPUINFO, "has SSE 4.1 = %s", ae_cpuinfo_sse41 () ? "true" : "false");
    ae_log(CPUINFO, "has SSE 4.2 = %s", ae_cpuinfo_sse42 () ? "true" : "false");
    ae_log(CPUINFO, "has AVX     = %s", ae_cpuinfo_avx   () ? "true" : "false");
    ae_log(CPUINFO, "has AVX 2   = %s", ae_cpuinfo_avx2  () ? "true" : "false");
}

void ae_cpuinfo_quit(void)
{
    ae_cpu_features = 0;
}
