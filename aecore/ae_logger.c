/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
TODO: default_log_func should OutputDebugStringA if Win32 and IsDebuggerPresent
--------------------------------------------------------------------------------
XXX the implementation of this system should not use other parts of the system,
--- in order to avoid circular dependencies. for instance, if the flush function
--- emits a fatal error it will recurse forever, as fatal errors flush the log.
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

static void default_log_func(ae_log_category_t category, const char* msg)
{
    printf("%s: %s\n", ae_log_category_name[category] +
                    strlen("AE_LOG_CATEGORY_"), msg);
}

static void default_flush_func(void)
{
    fflush(stdout);
}

static void (*g_log_func)(ae_log_category_t, const char*) = default_log_func;
static void (*g_flush_func)(void) = default_flush_func;

// verbosity permission bools (must map to category enum)
static char g_log_category_enabled[AE_LOG_CATEGORY_COUNT];

/*
================================================================================
 * ~~ [ internal utils ] ~~ *
--------------------------------------------------------------------------------
*/

static ae_log_category_t ae_log_category_from_string(const char* string)
{
    size_t i = 0; // TODO: if this is too slow, sort in header and binary search here?
    for (; i < AE_LOG_CATEGORY_COUNT; i++)
    {
        // we don't have to compare the enum prefix each time, just look at the offset
        if (strcmp(string, ae_log_category_name[i] + strlen("AE_LOG_CATEGORY_")) == 0)
        {
            return (ae_log_category_t)i;
        }
    }

    AE_WARN("%s is not a valid log category", string);
    return AE_LOG_CATEGORY_MISC;
}

#define AE_LOG_IMPL(c)              \
    if (g_log_category_enabled[c])  \
    {                               \
        /* TODO: use vsnprintf!! */ \
        char msg[4096];             \
        va_list v_args;             \
                                    \
        va_start(v_args, fmt);      \
        vsprintf(msg, fmt, v_args); \
        va_end(v_args);             \
                                    \
        if (msg[0])                 \
        {                           \
            g_log_func(c, msg);     \
        }                           \
    }

/*
================================================================================
 * ~~ [ public api ] ~~ *
--------------------------------------------------------------------------------
*/

void ae_log_clear_stdout(void)
{
    #ifdef _MSC_VER
        // This call can fail for things like interactive desktop services that
        // haven't had stdout redirected with SetStdHandle, WINE sessions, etc.
        HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

        if (h_stdout != INVALID_HANDLE_VALUE && h_stdout != NULL)
        {
            COORD coord = { 0, 0 };
            DWORD count;
            CONSOLE_SCREEN_BUFFER_INFO csbi;

            if (GetConsoleScreenBufferInfo(h_stdout, &csbi))
            {
                if (FillConsoleOutputCharacter(h_stdout, ' ',
                    csbi.dwSize.X * csbi.dwSize.Y, coord, &count))
                {
                    if (SetConsoleCursorPosition(h_stdout, coord))
                    {
                        // everything succeeded, queue chariots of fire theme.
                    }
                    else
                    {
                        AE_WARN("SetConsoleCursorPosition failed");
                    }
                }
                else
                {
                    AE_WARN("FillConsoleOutputCharacter failed");
                }
            }
            else
            {
                AE_WARN("GetConsoleScreenBufferInfo failed");
            }
        }
        else
        {
            AE_WARN("GetStdHandle(STD_OUTPUT_HANDLE) failed");
        }
    #else
        printf("\033[2J\033[1;1H");
    #endif
}

void ae_log_ex(ae_log_category_t category, const char* fmt, ...)
{
    AE_LOG_IMPL(category);
}

void ae_log_str(const char* category, const char* fmt, ...)
{
    ae_log_category_t c = ae_log_category_from_string(category);
    AE_LOG_IMPL(c);
}

void ae_log_flush(void)
{
    g_flush_func();
}

void ae_log_enable(ae_log_category_t category, int enabled)
{
    g_log_category_enabled[category] = (char)enabled;
}

void ae_log_enable_all(int enabled)
{
    memset(g_log_category_enabled, enabled, AE_LOG_CATEGORY_COUNT);
}

void ae_log_enable_str(const char* category, int enabled)
{
    ae_log_enable(ae_log_category_from_string(category), enabled);
}

int ae_log_is_enabled(ae_log_category_t category)
{
    return (int)g_log_category_enabled[category];
}

int ae_log_is_enabled_str(const char* category)
{
    return ae_log_is_enabled(ae_log_category_from_string(category));
}

void ae_log_handler(void (*log_func)(ae_log_category_t category,
                    const char* msg), void (*flush_func)(void))
{
    ae_log_flush();

    if (flush_func == NULL || log_func == NULL)
    {
        g_log_func = default_log_func; // reset
        g_flush_func = default_flush_func;
    }
    else
    {
        g_log_func = log_func;
        g_flush_func = flush_func;
    }
}

/*
================================================================================
 * ~~ [ initialization ] ~~ *
--------------------------------------------------------------------------------
*/

/* TODO:
    - get & set all log categories as a byte-wide bitvector
    - set console (tty) text color for specific categories
    - handler that writes to a log file in a writeable path
    - ring buffer handler for displaying most recent items
*/

void ae_logger_init(int argc, char** argv)
{
    int i = 0; // enable or disable some or all logging categories via arguments.
    ae_log_enable(AE_LOG_CATEGORY_WARNING, 1);

    #if defined(AE_LOG_DEFAULT_ENABLE_ALL)
    ae_log_enable_all(1); // puni puni poemy mode
    #endif

    for (; i < argc; i++)
    {
        if (strncmp(argv[i], "-log-", 5) == 0)
        {
            ae_log_enable_str(argv[i] + 5, 1);
        }

        if (strncmp(argv[i], "-nolog-", 7) == 0)
        {
            ae_log_enable_str(argv[i] + 7, 0);
        }

        if (strcmp(argv[i], "--log-all") == 0)
        {
            ae_log_enable_all(1);
        }

        if (strcmp(argv[i], "--log-none") == 0)
        {
            ae_log_enable_all(0);
        }
    }

    // for debugging weird platform filesystem issues...
    if (argc) ae_log(MISC, "argv[0] is %s", argv[0]);
}

void ae_logger_quit(void)
{
    // XXX: can't clean up handler, because we log after this function is called.
    // we'll flush anyways, even if that means some logfile loses a time message -
    // if you're really concerned about it, define AECORE_EXTENDED and hook init.
    #if 0
        ae_log_handler(NULL, NULL);
    #else
        ae_log_flush();
    #endif
}
