/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void ae_log_clear_stdout(void)
{
    AE_STUB();
}

void ae_log_ex(ae_log_category_t category, const char* fmt, ...)
{
    #if defined(AE_FATAL_WARNINGS)
        AE_STUB(); // no recursion
    #endif
}

void ae_log_str(const char* category, const char* fmt, ...)
{
    AE_STUB();
}

void ae_log_flush(void)
{
    AE_STUB();
}

void ae_log_enable(ae_log_category_t category, int enabled)
{
    AE_STUB();
}

void ae_log_enable_all(int enabled)
{
    AE_STUB();
}

void ae_log_enable_str(const char* category, int enabled)
{
    AE_STUB();
}

int ae_log_is_enabled(ae_log_category_t category)
{
    AE_STUB(); return 0;
}

int ae_log_is_enabled_str(const char* category)
{
    AE_STUB(); return 0;
}

void ae_log_handler(void (*log_func)(ae_log_category_t category,
                    const char* msg), void (*flush_func)(void))
{
    AE_STUB();
}

void ae_logger_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_logger_quit(void)
{
    // AE_STUB();
}
