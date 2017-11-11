/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

const char* ae_compiler_name (void) { AE_STUB(); return ""; }
const char* ae_language_name (void) { AE_STUB(); return ""; }
const char* ae_buildmode_name(void) { AE_STUB(); return ""; }
const char* ae_platform_name (void) { AE_STUB(); return ""; }
const char* ae_cpuinfo_name  (void) { AE_STUB(); return ""; }
const char* ae_linkage_mode  (void) { AE_STUB(); return ""; }

const char* ae_username(void) { AE_STUB(); return ""; }
const char* ae_hostname(void) { AE_STUB(); return ""; }

int ae_cpuinfo_rdtsc(void) { AE_STUB(); return 0; }
int ae_cpuinfo_sse  (void) { AE_STUB(); return 0; }
int ae_cpuinfo_sse2 (void) { AE_STUB(); return 0; }
int ae_cpuinfo_sse3 (void) { AE_STUB(); return 0; }
int ae_cpuinfo_sse41(void) { AE_STUB(); return 0; }
int ae_cpuinfo_sse42(void) { AE_STUB(); return 0; }
int ae_cpuinfo_avx  (void) { AE_STUB(); return 0; }
int ae_cpuinfo_avx2 (void) { AE_STUB(); return 0; }

void ae_cpuinfo_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_cpuinfo_quit(void)
{
    // AE_STUB();
}
