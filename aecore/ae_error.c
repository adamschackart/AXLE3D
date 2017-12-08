/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

// link with the core user interface lib on win32 for MessageBoxA error dialogs
#if defined(_WIN32)
    #pragma comment(lib, "user32")
#endif

void ae_error_init(int argc, char** argv) {}
void ae_error_quit(void) {}

// TODO: the user should be able to register an error display callback function
// TODO: log into AE_LOG_CATEGORY_ERROR to view the error message in a log file
void ae_error(const char* fmt, ...)
{
    va_list args;

#if defined(_WIN32)
    char msg[4096]; // TODO: use vsprintf_s here instead

    va_start(args, fmt);
    vsprintf(msg, fmt, args);
    va_end(args);

    MessageBoxA(NULL, msg, NULL, MB_OK | MB_ICONERROR);
#else
    fprintf(stderr, "ERROR: ");

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n");
#endif

    ae_log_flush();
    abort();
}

void ae_assert_message( const char* conditional,
                        const char* funcname,
                        const char* filename,
                        const int   lineno,
                        const char* fmt, ...)
{
    va_list args;

    fprintf(stderr, "assert \"%s\" failed in %s (file \"%s\" line %i): ",
            conditional, funcname, filename, lineno);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");

    ae_log_flush();
}
