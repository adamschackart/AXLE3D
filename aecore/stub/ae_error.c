/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void ae_error(const char* fmt, ...)
{
    #if !defined(AE_FATAL_WARNINGS)
        AE_STUB(); // no recursion
    #endif
}

void ae_assert_message( const char* conditional,
                        const char* funcname,
                        const char* filename,
                        const int   lineno,
                        const char* fmt, ...)
{
    AE_STUB();
}

void ae_error_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_error_quit(void)
{
    // AE_STUB();
}
