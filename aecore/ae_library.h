/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_LIBRARY_H__
#define __AE_LIBRARY_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_library_init(int argc, char** argv);
void ae_library_quit(void); // private init

AE_DECL void* AE_CALL ae_library_open (const char* name);
AE_DECL void* AE_CALL ae_library_proc (void* library, const char* name);
AE_DECL void  AE_CALL ae_library_close(void* library);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_LIBRARY_H__ */
