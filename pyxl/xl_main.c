/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __XL_CORE_H__
#include <xl_core.h>
#endif

// TODO: smarter build config - make sure XL actually uses SDL, or replace this.
#include <SDL2/SDL_main.h>

#if !defined(XL_EXT) // c linkage
    #if defined(__cplusplus)
        #define XL_EXT extern "C"
    #else
        #define XL_EXT extern
    #endif
#endif

// NOTE: if these are in a C++ file, they must be prepended with `extern "C"`.
#if defined(XL_MAIN_INIT)
XL_EXT void XL_MAIN_INIT(int argc, char** argv);
#else
static void XL_MAIN_INIT(int argc, char** argv) {}
#endif

#if defined(XL_MAIN_FUNC)
XL_EXT void XL_MAIN_FUNC(xl_window_t* window);
#else
static void XL_MAIN_FUNC(xl_window_t* window) {}
#endif

int main(int argc, char** argv)
{
    // init hook in case we want to set a log handler or environment variables.
    XL_MAIN_INIT(argc, argv);

    ae_init(argc, argv);
    atexit(ae_quit);

    // takes window argument, in case we want to embed the program into another
    // (like if we make a small game, and put in in a larger one as a minigame).
    XL_MAIN_FUNC(NULL);

    return EXIT_SUCCESS;
}
