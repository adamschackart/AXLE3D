/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void* ae_library_open(const char* name)
{
    AE_STUB(); return NULL;
}

void* ae_library_proc(void* library, const char* name)
{
    AE_STUB(); return NULL;
}

void ae_library_close(void* library)
{
    AE_STUB();
}

void ae_library_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_library_quit(void)
{
    // AE_STUB();
}
