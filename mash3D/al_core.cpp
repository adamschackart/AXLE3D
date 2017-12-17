/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AL_CORE_H__
#include <al_core.h>
#endif

/*
================================================================================
 * ~~ [ init & quit ] ~~ *
--------------------------------------------------------------------------------
*/

static int _al_core_is_initialized;

int al_is_init(void)
{
    return _al_core_is_initialized;
}

const char* al_implementation(void)
{
    return "TODO";
}

void al_init(void)
{
    // TODO
}

void al_quit(void)
{
    // TODO
}
