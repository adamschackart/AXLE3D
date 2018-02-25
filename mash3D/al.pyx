# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
import sys # version info

cdef extern from "al_core.h":
    # ==========================================================================
    # ~ [ macros & init ]
    # ==========================================================================

    int al_is_init() # safe hacking
    const char* al_implementation()

    void al_init()
    void al_quit()

    # ==========================================================================
    # ~ [ OpenAL wrapper ]
    # ==========================================================================

    # TODO

    # ===== [ alc ] ============================================================

    # TODO

    # ===== [ alext ] ==========================================================

    # TODO

    # ===== [ efx ] ============================================================

    # TODO

def early_init():
    """
    Take the initialization lag hit early, before issuing any audio API calls.
    """
    al_init()

def implementation():
    """
    Identify the underlying platform layer, for 'safer' game-specific hacks.
    """
    if sys.version_info.major > 2:
        return al_implementation().decode() # auto-convert to unicode string
    else:
        return al_implementation() # auto-convert to oldschool ascii string

# ==============================================================================
# ~ [ OpenAL wrapper ]
# ==============================================================================

# TODO

# ===== [ alc ] ================================================================

# TODO

# ===== [ alext ] ==============================================================

# TODO

# ===== [ efx ] ================================================================

# TODO
