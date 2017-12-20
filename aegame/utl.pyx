# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from mem cimport ae_conf_path, ae_base_path, ae_base_path_override
import sys, os, platform, functools, atexit

cdef extern from "ae_logger.h":
    void ae_log_clear_stdout() # clear terminal and reset cursor
    void ae_log_str(const char* category, const char* fmt, ...)

    void ae_log_flush() # synchronize log contents with log file

    void ae_log_enable_str(const char* category, int enabled)
    void ae_log_enable_all(int enabled)

    int ae_log_is_enabled_str(const char* category)

cdef extern from "ae_time.h":
    double ae_seconds() # get seconds elapsed since some epoch
    # u64 ae_rdtsc() # get the x86 processor timestamp counter

    double ae_time_wave_ex(double curtime, double rate, double offset)
    double ae_time_wave(double rate) # easy animated cos wave function

    ctypedef void (*ae_frame_callback_t)(const char* name, double dt, void* context)

    # allows alternate old-style func naming convention
    ctypedef ae_frame_callback_t AE_FRAME_CALLBACK_FUNC

    void ae_frame_callback_register(const char* n, ae_frame_callback_t f, void* ctx)
    void ae_frame_callback_unregister(const char* n)
    int  ae_frame_callback_get(const char* name, ae_frame_callback_t* f, void** ctx)

    ctypedef void (*ae_timer_callback_t)(const char* name, double t,
                                        int repeats, void* context)

    # allows alternate old-style func naming convention
    ctypedef ae_timer_callback_t AE_TIMER_CALLBACK_FUNC

    void ae_timer_callback_register(const char* name, ae_timer_callback_t func,
                                    double seconds, int repeat, void* context)

    void ae_timer_callback_unregister(const char* name)

    int ae_timer_callback_get( const char* name, ae_timer_callback_t* function,
                double* current, double* seconds, int* repeat, void** context)

    void ae_timer_callback_set_repeat(const char* name, int repeat)

    double ae_frame_delta()

    # ===== [ profiler ] =======================================================

    void* ae_profile_enter(const char* filename, const char* funcname)
    void  ae_profile_leave(void* ctx) # profile execution of functions

    int ae_profile_enabled()
    void ae_profile_clear()

    # ===== [ profile rendering ] ==============================================

    ctypedef struct ae_profile_node_t:
        char * funcname
        char * filename

        double time_taken
        double first_call
        size_t call_count

        double min_time
        double max_time

    ctypedef void (*AE_PROFILE_RENDER_FUNC)(ae_profile_node_t* node)

    ctypedef enum ae_profile_sort_t:
        AE_PROFILE_SORT_TOTAL_TIME
        AE_PROFILE_SORT_AVERAGE_TIME
        AE_PROFILE_SORT_CALL_COUNT
        AE_PROFILE_SORT_FUNCNAME
        AE_PROFILE_SORT_FILENAME
        AE_PROFILE_SORT_FIRST_CALL
        AE_PROFILE_SORT_MIN_TIME
        AE_PROFILE_SORT_MAX_TIME
        AE_PROFILE_SORT_DIFF_TIME
        AE_PROFILE_SORT_COUNT

    const char* ae_profile_sort_name[] # name of the sort key

    void ae_profile_render( AE_PROFILE_RENDER_FUNC draw, ae_profile_sort_t sort,
                            size_t max_items, double dt) # abstract sort & draw

    void ae_profile_print(ae_profile_sort_t sort, size_t max_items, double dt)

cdef extern from "ae_branch.h":
    # ===== [ global coverage report ] =========================================

    int ae_branch_report_enabled()

    ctypedef struct ae_branch_node_t:
        const char* conditional
        const char* function
        const char* filename
        int line

        double first
        size_t total
        size_t taken

    ctypedef int (*AE_BRANCH_FILTER_FUNC)(ae_branch_node_t* node, void* data)

    void ae_branch_report_clear_filter(AE_BRANCH_FILTER_FUNC filter, void* context)
    void ae_branch_report_clear()

    void ae_branch_report_clear_file(const char* filename)
    void ae_branch_report_clear_function(const char* function)

    void ae_branch_report_clear_extremes(const double epsilon)
    void ae_branch_report_clear_median(const double epsilon)

    ctypedef void (*AE_BRANCH_RENDER_FUNC)(ae_branch_node_t* node)

    ctypedef enum ae_branch_sort_t:
        # sorting by count
        AE_BRANCH_SORT_TOTAL
        AE_BRANCH_SORT_TRUE_COUNT
        AE_BRANCH_SORT_FALSE_COUNT

        # sorting by margins
        AE_BRANCH_SORT_TRUE_RATIO
        AE_BRANCH_SORT_FALSE_RATIO
        AE_BRANCH_SORT_HALFWAYNESS

        # lexical sorting
        AE_BRANCH_SORT_CONDITIONAL
        AE_BRANCH_SORT_FUNCTION
        AE_BRANCH_SORT_FILENAME

        # chronological sorting
        AE_BRANCH_SORT_FIRST_TEST
        AE_BRANCH_SORT_LAST_TEST

    const char* ae_branch_sort_name[]

    # ===== [ abstract iteration ] =============================================

    void ae_branch_report_render_filter(AE_BRANCH_FILTER_FUNC filter, void* context,
                AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)

    void ae_branch_report_render(AE_BRANCH_RENDER_FUNC draw,
                    ae_branch_sort_t sort, size_t max_items)

    void ae_branch_report_render_file(const char* filename, # filter by file
        AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)

    void ae_branch_report_render_function(const char* function, # func filter
        AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)

    void ae_branch_report_render_extremes(const double epsilon, # outer ratios
        AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)

    void ae_branch_report_render_median(const double epsilon, # inner ratios
        AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)

    # ===== [ stdout reporting ] ===============================================

    void ae_branch_report_print_filter(AE_BRANCH_FILTER_FUNC func, void* ctx,
                                    ae_branch_sort_t sort, size_t max_items)

    void ae_branch_report_print(ae_branch_sort_t sort, size_t max_items)

    void ae_branch_report_print_file(const char* filename,
                ae_branch_sort_t sort, size_t max_items)

    void ae_branch_report_print_function(const char* function,
                    ae_branch_sort_t sort, size_t max_items)

    void ae_branch_report_print_extremes(const double epsilon,
                    ae_branch_sort_t sort, size_t max_items)

    void ae_branch_report_print_median(const double epsilon,
                    ae_branch_sort_t sort, size_t max_items)

cdef extern from "ae_switch.h":
    # ===== [ global coverage report ] =========================================

    int ae_switch_report_enabled()

    ctypedef struct ae_switch_node_t:
        const char** names
        const char* type

        const char* function
        const char* filename
        int line

        double first
        size_t total
        size_t count

        size_t* taken

    ctypedef int (*AE_SWITCH_FILTER_FUNC)(ae_switch_node_t* node, void* data)

    void ae_switch_report_clear_filter(AE_SWITCH_FILTER_FUNC filter, void* context)
    void ae_switch_report_clear()

    void ae_switch_report_clear_function(const char* function)
    void ae_switch_report_clear_file(const char* filename)
    void ae_switch_report_clear_type(const char* type_name)

    ctypedef void (*AE_SWITCH_RENDER_FUNC)(ae_switch_node_t* node)

    ctypedef enum ae_switch_sort_t:
        # sorting by count
        AE_SWITCH_SORT_TOTAL

        # sorting by cases covered
        AE_SWITCH_SORT_SPREAD

        # lexical sorting
        AE_SWITCH_SORT_TYPENAME
        AE_SWITCH_SORT_FUNCTION
        AE_SWITCH_SORT_FILENAME

        # chronological sorting
        AE_SWITCH_SORT_FIRST_TEST
        AE_SWITCH_SORT_LAST_TEST

    const char* ae_switch_sort_name[]

    # ===== [ abstract iteration ] =============================================

    void ae_switch_report_render_filter(AE_SWITCH_FILTER_FUNC filter, void* context,
                AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items)

    void ae_switch_report_render(AE_SWITCH_RENDER_FUNC draw,
                    ae_switch_sort_t sort, size_t max_items)

    void ae_switch_report_render_function(const char* function, # func filter
        AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items)

    void ae_switch_report_render_file(const char* filename, # filter by file
        AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items)

    void ae_switch_report_render_type(const char* type_name, # filter by type
        AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items)

    # ===== [ stdout reporting ] ===============================================

    void ae_switch_report_print_filter(AE_SWITCH_FILTER_FUNC func, void* ctx,
                                    ae_switch_sort_t sort, size_t max_items)

    void ae_switch_report_print(ae_switch_sort_t sort, size_t max_items)

    void ae_switch_report_print_function(const char* function_s,
                        ae_switch_sort_t sort, size_t max_items)

    void ae_switch_report_print_file(const char* filename_s,
                    ae_switch_sort_t sort, size_t max_items)

    void ae_switch_report_print_type(const char* typename_s,
                    ae_switch_sort_t sort, size_t max_items)

cdef extern from "ae_core.h":
    void ae_init(int argc, char** argv)
    void ae_quit()

    int ae_is_init()

# ==============================================================================
# ~ [ logging system ]
# ==============================================================================
# NOTE: this is an interface to a global system, so creating these does nothing.
# NOTE: stdout is hooked here so we can easily trace gc info in a separate file.
# ------------------------------------------------------------------------------
# TODO: Install an ae_error exception hook so errors won't be hidden by the log!
#       This will also remove another level of bifurcation between Python and C.
# ------------------------------------------------------------------------------

log = Logger()("PYTHON", "{} {}".format( # init aecore & log python version
        platform.python_implementation(), sys.version).replace(' \n', ' '))

# NOTE: atexit from a DLL or Logger.__del__ aren't guaranteed to be called,
# so we stick our exit func into a list that's called before python exits.
atexit.register(lambda: ae_quit())

cdef class Logger:
    """Displays filtered information about the current state of the program."""
    def __init__(self):
        cdef list bytes_argv = [] # have to keep these resident in memory

        cdef int i # copy sys.argv
        cdef char* sys_argv[64]

        if <size_t>len( sys.argv ) > sizeof( sys_argv ) / sizeof( char* ):
            raise RuntimeError("too many args: {}".format(len(sys.argv)))

        if sys.version_info.major == 2:
            for i in range(len(sys.argv)): bytes_argv.append(<bytes>sys.argv[i])
        else:
            for i in range(len(sys.argv)):
                bytes_argv.append(<bytes>sys.argv[i].encode('utf-8'))

        for i in range(len(sys.argv)):
            sys_argv[i] = <char*>bytes_argv[i]

        # same as AE_LOG_DEFAULT_ENABLE_ALL without changing compiler options
        if hasattr(sys, 'log_enable_all'): self.enable_all(sys.log_enable_all)

        # init before hooking stdout to log (if not in an interactive session)
        ae_init(len(sys.argv), sys_argv)

        if not getattr(sys, 'no_tty_redirect_to_logger', hasattr(sys, "ps1")):
            # sys.stdout = sys.stderr = self # replace std print

            # this is a temp fix for our lack of totally clean error handling.
            # see the comments above re: the log suppressing fatal exceptions.
            sys.stdout = self

    def __call__(self, str cat, str msg):
        cdef bytes cat_b
        cdef bytes msg_b

        # the python print statement writes the newline separately
        if msg == '\n': return self

        # strings could be unicode, so convert them back to ascii
        if sys.version_info.major > 2:
            cat_b = <bytes>cat.encode('utf-8')
            msg_b = <bytes>msg.encode('utf-8')
        else:
            cat_b = <bytes>cat
            msg_b = <bytes>msg

        ae_log_str(<char*>cat_b, "%s", <char*>msg_b); return self

    # this hack enables replacing the standard output stream with our logger.
    # TODO "print A, B" will show A and B on two different lines, so we need
    # to be a little smarter here, and buffer input until a newline is found.
    def write(self, str msg): self("PYTHON", msg)

    # clear the terminal display (equivalent to win32 "cls" or unix "clear").
    def clear_stdout(self): ae_log_clear_stdout()

    # if writing the log to a file to device, synchronize the log's contents.
    # this is automatically called at exit in py3k as sys.stdout is flushed.
    def flush(self): ae_log_flush()

    def enable(self, str cat, bint enabled):
        cdef bytes cat_b

        if sys.version_info.major > 2:
            cat_b = <bytes>cat.encode('utf-8')
        else:
            cat_b = <bytes>cat

        ae_log_enable_str(<char*>cat_b, enabled)

    def enable_all(self, bint enabled):
        ae_log_enable_all(enabled)

    def is_enabled(self, str cat):
        cdef bytes cat_b

        if sys.version_info.major > 2:
            cat_b = <bytes>cat.encode('utf-8')
        else:
            cat_b = <bytes>cat

        return ae_log_is_enabled_str(<char*>cat_b)

# ==============================================================================
# ~ [ filesystem / paths ]
# ==============================================================================

def conf_path(str org, str app):
    """
    Get the only writeable application path, used for config files and savegames.
    """
    cdef bytes b_org, b_app
    cdef const char * path

    if sys.version_info.major > 2:
        # convert unicode python 3 strings into ascii strings for our c library
        b_org = org.encode('utf-8')
        b_app = app.encode('utf-8')

        path = ae_conf_path(<const char*>b_org, <const char*>b_app)
    else:
        path = ae_conf_path(<const char*>org, <const char*>app)

    if not path[0]:
        name = os.path.join(org, app) # python fallback if the C path is stubbed

        if sys.platform in ('cygwin', 'win32'):
            if 'APPDATA' in os.environ:
                name = os.path.join(os.environ['APPDATA'], name)
            else:
                name = os.path.expanduser('~/%s' % name)

        elif sys.platform == 'darwin':
            name = os.path.expanduser('~/Library/Application Support/%s' % name)

        elif sys.platform.startswith('linux'):
            if 'XDG_CONFIG_HOME' in os.environ:
                name = os.path.join(os.environ['XDG_CONFIG_HOME'], name)
            else:
                name = os.path.expanduser('~/.config/%s' % name)
        else:
            name = os.path.expanduser('~/.%s' % name)

        if not os.path.exists(name):
            os.makedirs(name)

        return name.decode() if sys.version_info.major > 2 else name
    else:
        return path.decode() if sys.version_info.major > 2 else path

def _python_base_path():
    frozen = getattr(sys, 'frozen', None) # am i packaged in an exe?

    if frozen in ('windows_exe', 'console_exe'):
        return os.path.dirname(sys.executable)

    elif frozen == 'macosx_app':
        return os.environ['RESOURCEPATH'] # py2app

    else:
        main = sys.modules['__main__']

        if hasattr(main, '__file__'):
            return os.path.dirname(os.path.abspath(main.__file__))
        else:
            if 'python' in os.path.basename(sys.executable):
                #
                # interactive repl session
                #
                return os.getcwd()
            else:
                return os.path.dirname(sys.executable) # cx_Freeze

# set the application path after initialization to the path containing __main__
if sys.version_info.major > 2:
    ae_base_path_override(<bytes>_python_base_path().encode('utf-8'))
else:
    ae_base_path_override(<bytes>_python_base_path())

def base_path():
    """
    Get the read-only path where the main application resides (__main__ path).
    """
    if sys.version_info.major > 2:
        return ae_base_path().decode() # auto-conversion to a unicode string
    else:
        return ae_base_path() # auto-conversion to an oldschool ascii string

# ==============================================================================
# ~ [ timing & profiling ]
# ==============================================================================
# TODO: use ae_profile_render to gather all profile nodes into a list of tuples
# ------------------------------------------------------------------------------

def seconds():
    """The basic equivalent of Python 3.3's time.perf_counter(). This gets the
    number of seconds elapsed since some arbitrary epoch. To compute the time
    elapsed between points (time delta), use aegame.seconds() - start_time."""
    return ae_seconds()

def frame_delta():
    """Tick the global game timer, computing a delta for the current frame."""
    return ae_frame_delta()

def time_wave(double rate=1.0, double offset=0.0):
    """Generate temporal sine waves, useful for flashing color effects etc."""
    return ae_time_wave_ex(ae_seconds(), rate, offset)

# ===== [ profile decorator ] ==================================================

if ae_profile_enabled():
    def profile(filename, funcname):
        def profile_decorator(func):

            @functools.wraps(func) # actually call function with context
            def func_wrapper(*args, **kwargs):
                cdef void* context
                cdef bytes b_file, b_func

                if sys.version_info.major > 2:
                    b_file = <bytes>filename.encode('utf-8')
                else:
                    b_file = <bytes>filename

                if sys.version_info.major > 2:
                    b_func = <bytes>funcname.encode('utf-8')
                else:
                    b_func = <bytes>funcname

                context = ae_profile_enter(<char*>b_file, <char*>b_func)
                f_value = func(*args, **kwargs)

                ae_profile_leave(context); return f_value

            return func_wrapper
        return profile_decorator
else:
    def profile(filename, funcname):
        def profile_decorator(func): return func

        return profile_decorator

# ===== [ profile context manager ] ============================================

cdef class ProfileBlock:
    """Execute timed blocks of code using Python's context manager interface."""
    cdef void* pf_ctx

    cdef bytes b_file
    cdef bytes b_func

    def __init__(self, str filename, str funcname):
        if sys.version_info.major > 2:
            self.b_file = <bytes>filename.encode('utf-8')
            self.b_func = <bytes>funcname.encode('utf-8')
        else:
            self.b_file = <bytes>filename
            self.b_func = <bytes>funcname

    def __enter__(self):
        self.pf_ctx = ae_profile_enter(<char*>self.b_file, <char*>self.b_func)

    def __exit__(self, *a):
        ae_profile_leave(self.pf_ctx)

# ===== [ profile information ] ================================================

def profile_enabled(): return <bint>ae_profile_enabled() # profile status

PROFILE_SORT_TOTAL_TIME = AE_PROFILE_SORT_TOTAL_TIME
PROFILE_SORT_AVERAGE_TIME = AE_PROFILE_SORT_AVERAGE_TIME
PROFILE_SORT_CALL_COUNT = AE_PROFILE_SORT_CALL_COUNT
PROFILE_SORT_FUNCNAME = AE_PROFILE_SORT_FUNCNAME
PROFILE_SORT_FILENAME = AE_PROFILE_SORT_FILENAME
PROFILE_SORT_FIRST_CALL = AE_PROFILE_SORT_FIRST_CALL
PROFILE_SORT_MIN_TIME = AE_PROFILE_SORT_MIN_TIME
PROFILE_SORT_MAX_TIME = AE_PROFILE_SORT_MAX_TIME
PROFILE_SORT_DIFF_TIME = AE_PROFILE_SORT_DIFF_TIME

def profile_print(  ae_profile_sort_t sort, size_t max_items,
                        double dt, bint autoprof_clear=True):
    """
    Print the profile for the current frame, sorted by a given criteria.
    """
    ae_profile_print(sort, max_items, dt)
    if autoprof_clear: ae_profile_clear()

# ==============================================================================
# ~ [ C and C++ branch coverage ]
# ==============================================================================
# NOTE: this is an interface to a global system, so creating these does nothing.
# TODO: use ae_branch_report_render to get all branch nodes in a list of tuples.
# TODO: convert clear and print methods to use `str` for python 3 compatibility.
# ------------------------------------------------------------------------------

cdef class BranchCoverage:
    """
    Tracks information about conditionals evaluated in C code and prints it.
    """
    @staticmethod # get the status of the branch coverage system at runtime.
    def is_enabled(): return ae_branch_report_enabled()

    enabled = is_enabled

    @staticmethod # delete all data currently kept about program branching.
    def clear(): ae_branch_report_clear()

    @staticmethod
    def clear_file(bytes filename):
        ae_branch_report_clear_file(<const char*>filename)

    @staticmethod
    def clear_function(bytes function):
        ae_branch_report_clear_function(<const char*>function)

    @staticmethod
    def clear_extremes(double epsilon):
        ae_branch_report_clear_extremes(epsilon)

    @staticmethod
    def clear_median(double epsilon):
        ae_branch_report_clear_median(epsilon)

    SORT_TOTAL = AE_BRANCH_SORT_TOTAL
    SORT_TRUE_COUNT = AE_BRANCH_SORT_TRUE_COUNT
    SORT_FALSE_COUNT = AE_BRANCH_SORT_FALSE_COUNT
    SORT_TRUE_RATIO = AE_BRANCH_SORT_TRUE_RATIO
    SORT_FALSE_RATIO = AE_BRANCH_SORT_FALSE_RATIO
    SORT_HALFWAYNESS = AE_BRANCH_SORT_HALFWAYNESS
    SORT_CONDITIONAL = AE_BRANCH_SORT_CONDITIONAL
    SORT_FUNCTION = AE_BRANCH_SORT_FUNCTION
    SORT_FILENAME = AE_BRANCH_SORT_FILENAME
    SORT_FIRST_TEST = AE_BRANCH_SORT_FIRST_TEST
    SORT_LAST_TEST = AE_BRANCH_SORT_LAST_TEST

    @staticmethod
    def print_report(ae_branch_sort_t sort, size_t max_items):
        ae_branch_report_print(sort, max_items)

    @staticmethod
    def print_file(bytes filename, ae_branch_sort_t sort, size_t max_items):
        ae_branch_report_print_file(<const char*>filename, sort, max_items)

    @staticmethod
    def print_function(bytes func, ae_branch_sort_t sort, size_t max_items):
        ae_branch_report_print_function(<const char*>func, sort, max_items)

    @staticmethod
    def print_extremes(double epsilon, ae_branch_sort_t sort, size_t max_items):
        ae_branch_report_print_extremes(epsilon, sort, max_items)

    @staticmethod
    def print_median(double epsilon, ae_branch_sort_t sort, size_t max_items):
        ae_branch_report_print_median(epsilon, sort, max_items)

branch_coverage = BranchCoverage()

# ==============================================================================
# ~ [ C and C++ switch coverage ]
# ==============================================================================
# NOTE: this is an interface to a global system, so creating these does nothing.
# TODO: use ae_switch_report_render to get all switch nodes in a list of tuples.
# TODO: convert clear and print methods to use `str` for python 3 compatibility.
# ------------------------------------------------------------------------------

cdef class SwitchCoverage:
    """
    Tracks & prints information about switch statements on enums in C code.
    """
    @staticmethod # get the status of the switch coverage system at runtime.
    def is_enabled(): return ae_switch_report_enabled()

    enabled = is_enabled

    @staticmethod # delete all data currently kept about program dispatch.
    def clear(): ae_switch_report_clear()

    @staticmethod
    def clear_function(bytes function):
        ae_switch_report_clear_function(<const char*>function)

    @staticmethod
    def clear_file(bytes filename):
        ae_switch_report_clear_file(<const char*>filename)

    @staticmethod
    def clear_type(bytes type_name):
        ae_switch_report_clear_type(<const char*>type_name)

    SORT_TOTAL = AE_SWITCH_SORT_TOTAL
    SORT_SPREAD = AE_SWITCH_SORT_SPREAD
    SORT_TYPENAME = AE_SWITCH_SORT_TYPENAME
    SORT_FUNCTION = AE_SWITCH_SORT_FUNCTION
    SORT_FILENAME = AE_SWITCH_SORT_FILENAME
    SORT_FIRST_TEST = AE_SWITCH_SORT_FIRST_TEST
    SORT_LAST_TEST = AE_SWITCH_SORT_LAST_TEST

    @staticmethod
    def print_report(ae_switch_sort_t sort, size_t max_items):
        ae_switch_report_print(sort, max_items)

    @staticmethod
    def print_function(bytes function, ae_switch_sort_t sort, size_t max_items):
        ae_switch_report_print_function(<const char*>function, sort, max_items)

    @staticmethod
    def print_file(bytes filename, ae_switch_sort_t sort, size_t max_items):
        ae_switch_report_print_file(<const char*>filename, sort, max_items)

    @staticmethod
    def print_type(bytes type_name, ae_switch_sort_t sort, size_t max_items):
        ae_switch_report_print_type(<const char*>type_name, sort, max_items)

switch_coverage = SwitchCoverage()
