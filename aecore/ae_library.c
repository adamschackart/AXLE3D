/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
--- TODO: this is the same thin wrapper everyone makes - better cross-platform
--- filename handling would be useful (like prefixing with "lib" in unix, etc.)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void ae_library_init(int argc, char** argv) {}
void ae_library_quit(void) {}

#if defined(_WIN32)

void* ae_library_open(const char* name)
{
    HMODULE handle = LoadLibraryA(name);

    if (handle)
    {
        TCHAR path[1024];

        GetModuleFileName((HMODULE)handle, path, sizeof(path) / sizeof(TCHAR));
        ae_log(LIBRARY, "successfully opened library \"%s\"", path);
    }
    else
    {
        ae_log(LIBRARY, "failed to open library file \"%s\"", name);
    }

    return handle;
}

void* ae_library_proc(void* library, const char* name)
{
    FARPROC handle = GetProcAddress((HMODULE)library, name);

    #if defined(AE_LIBRARY_TRACE)
        TCHAR path[1024];
        GetModuleFileName((HMODULE)library, path, sizeof(path) / sizeof(TCHAR));

        if (handle)
        {
            ae_log(LIBRARY, "load \"%s\" from \"%s\" success", name, path);
        }
        else
        {
            ae_log(LIBRARY, "load \"%s\" from \"%s\" failure", name, path);
        }
    #endif
    return handle;
}

void ae_library_close(void* library)
{
    TCHAR path[1024];
    GetModuleFileName((HMODULE)library, path, sizeof(path) / sizeof(TCHAR));

    if (FreeLibrary((HMODULE)library))
    {
        ae_log(LIBRARY, "successfully closed library \"%s\"", path);
    }
    else
    {
        ae_log(LIBRARY, "failed to close library file \"%s\"", path);
    }
}

#else

void* ae_library_open(const char* name)
{
    // NOTE: for a massive information dump, set the LD_DEBUG environment variable
    // before running the program. this shows the full dependency chain used by ld.
    void* handle = dlopen(name, RTLD_LAZY | RTLD_LOCAL);

    if (handle)
    {
        // since we don't have a way of getting the library name, show ptr for free
        ae_log(LIBRARY, "successfully opened library \"%s\" at %p", name, handle);
    }
    else
    {
        ae_log(LIBRARY, "%s", dlerror());
    }

    return handle;
}

void* ae_library_proc(void* library, const char* name)
{
    void* handle = dlsym(library, name);

    #if defined(AE_LIBRARY_TRACE)
        if (handle)
        {
            ae_log(LIBRARY, "load \"%s\" from lib %p success", name, library);
        }
        else
        {
            ae_log(LIBRARY, "load \"%s\" from lib %p failure", name, library);
        }
    #endif
    return handle;
}

void ae_library_close(void* library)
{
    int error_code = dlclose(library);

    if (error_code)
    {
        ae_log(LIBRARY, "%s", dlerror()); // TODO: store library name
    }
    else
    {
        ae_log(LIBRARY, "successfully closed library at %p", library);
    }
}

#endif
