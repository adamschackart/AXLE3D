# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------

cdef extern from "al_core.h":
    # ==========================================================================
    # ~ [ macros & init ]
    # ==========================================================================

    int al_is_init() # safe hacking
    const char* al_implementation()

    void al_init()
    void al_quit()

def early_init():
    """
    Take the initialization lag hit early, before issuing any audio API calls.
    """
    al_init()

def implementation():
    """
    Identify the underlying platform layer, for 'safer' game-specific hacks.
    """
    return al_implementation()
