/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AL_CORE_H__
#define __AL_CORE_H__

#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

/*
================================================================================
 * ~~ [ macros & init ] ~~ *
--------------------------------------------------------------------------------
*/

#if defined(AL_COMPILING_EXE)
    #define AL_DECL
    #define AL_CALL
#endif

#if !defined(AL_DECL)
    #if defined(AL_COMPILING_DLL)
        #define AL_DECL AE_EXPORT
    #else
        #define AL_DECL AE_IMPORT
    #endif
#endif

#if !defined(AL_CALL)
    #define AL_CALL AE_CALL
#endif

AL_DECL int AL_CALL al_is_init(void); // safer hacks
AL_DECL const char* AL_CALL al_implementation(void);

AL_DECL void AL_CALL al_init(void);
AL_DECL void AL_CALL al_quit(void);

// NOTE avoiding linker issues when changing build configurations,
// in case somebody accidentally types this into a header file...
#undef AL_DECL

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AL_CORE_H__ */
