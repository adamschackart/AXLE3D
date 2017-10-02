# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from setup_utils import *

# ==============================================================================
# ~ [ aecore & aegame ]
# ==============================================================================

shared_libs = [
    SharedLibrary('aecore', ['aecore/ae_core.c',
            # 'aecore/ae_branch.c',
            # 'aecore/ae_cpuinfo.c',
            # 'aecore/ae_easing.c',
            # 'aecore/ae_error.c',
            # 'aecore/ae_file.c',
            # 'aecore/ae_hash.c',
            # 'aecore/ae_image.c',
            # 'aecore/ae_index.c',
            # 'aecore/ae_library.c',
            # 'aecore/ae_logger.c',
            # 'aecore/ae_math.c',
            # 'aecore/ae_memory.c',
            # 'aecore/ae_minifont.c',
            # 'aecore/ae_polygon.c',
            # 'aecore/ae_random.c',
            # 'aecore/ae_search.c',
            # 'aecore/ae_sort.c',
            # 'aecore/ae_switch.c',
            # 'aecore/ae_time.c',
            # 'aecore/ae_triangle.c',
            # 'aecore/ae_vertex.c',
        ],

        include_dirs = [
            'aecore', 'external',
        ],

        define_macros = [
            ('AE_COMPILING_DLL', '1'),
            ('AE_NOQUIT_ATEXIT', '1'),

            # ('AE_NOBUILD_BRANCH', '1'),
            # ('AE_NOBUILD_CPUINFO', '1'),
            # ('AE_NOBUILD_EASING', '1'),
            # ('AE_NOBUILD_ERROR', '1'),
            # ('AE_NOBUILD_FILE', '1'),
            # ('AE_NOBUILD_HASH', '1'),
            # ('AE_NOBUILD_IMAGE', '1'),
            # ('AE_NOBUILD_INDEX', '1'),
            # ('AE_NOBUILD_LIBRARY', '1'),
            # ('AE_NOBUILD_LOGGER', '1'),
            # ('AE_NOBUILD_MATH', '1'),
            # ('AE_NOBUILD_MEMORY', '1'),
            # ('AE_NOBUILD_MINIFONT', '1'),
            # ('AE_NOBUILD_POLYGON', '1'),
            # ('AE_NOBUILD_RANDOM', '1'),
            # ('AE_NOBUILD_SEARCH', '1'),
            # ('AE_NOBUILD_SORT', '1'),
            # ('AE_NOBUILD_SWITCH', '1'),
            # ('AE_NOBUILD_TIME', '1'),
            # ('AE_NOBUILD_TRIANGLE', '1'),
            # ('AE_NOBUILD_VERTEX', '1'),

            # ('AE_STUB_BRANCH', '1'),
            # ('AE_STUB_CPUINFO', '1'),
            # ('AE_STUB_EASING', '1'),
            # ('AE_STUB_ERROR', '1'),
            # ('AE_STUB_FILE', '1'),
            # ('AE_STUB_HASH', '1'),
            # ('AE_STUB_IMAGE', '1'),
            # ('AE_STUB_INDEX', '1'),
            # ('AE_STUB_LIBRARY', '1'),
            # ('AE_STUB_LOGGER', '1'),
            # ('AE_STUB_MATH', '1'),
            # ('AE_STUB_MEMORY', '1'),
            # ('AE_STUB_MINIFONT', '1'),
            # ('AE_STUB_POLYGON', '1'),
            # ('AE_STUB_RANDOM', '1'),
            # ('AE_STUB_SEARCH', '1'),
            # ('AE_STUB_SORT', '1'),
            # ('AE_STUB_SWITCH', '1'),
            # ('AE_STUB_TIME', '1'),
            # ('AE_STUB_TRIANGLE', '1'),
            # ('AE_STUB_VERTEX', '1'),

            # ('AE_GLOBAL_HEAP_RESERVE', '0'),
            # ('AE_GLOBAL_STACK_RESERVE', '0'),
            # ('AE_SMALL_STRING_BLOCK_SIZE', '4'),
            # ('AE_SMALL_STRING_NUM_BLOCKS', '0'),
            # ('AE_IMAGE_HEAP_RESERVE', '0'),
            # ('AE_QUAD_EDGES_NUM_BLOCKS', '0'),
            # ('AE_TRI_EDGES_NUM_BLOCKS', '0'),

            # ('AE_LIBRARY_TRACE', '1'),
            # ('AE_FILE_TRACE', '1'),
            # ('AE_STACK_TRACE', '1'),
            # ('AE_HEAP_TRACE', '1'),
            # ('AE_BLOCK_TRACE', '1'),

            # ('AE_RANDOM_USE_XORSHIFT128', '1'),
            # ('AE_RANDOM_USE_XORSHIFT32', '1'),

            # ('AE_BRANCH_COVERAGE', '1'),
            # ('AE_SWITCH_COVERAGE', '1'),
            # ('AE_PROFILE_ENABLE', '1'),
            # ('AE_NO_INLINING', '1'),
            # ('AE_LOG_DEFAULT_ENABLE_ALL', '1'),
            # ('AE_FATAL_WARNINGS', '1'),
        ],
    ),
]

ext_modules = [
    Extension('aegame.utl', ['aegame/utl.pyx'], libraries=['aecore'], include_dirs=['aecore']),
    Extension('aegame.rng', ['aegame/rng.pyx'], libraries=['aecore'], include_dirs=['aecore']),
    Extension('aegame.vec', ['aegame/vec.pyx'], libraries=['aecore'], include_dirs=['aecore']),
    Extension('aegame.lin', ['aegame/lin.pyx'], libraries=['aecore'], include_dirs=['aecore']),
    Extension('aegame.mat', ['aegame/mat.pyx'], libraries=['aecore'], include_dirs=['aecore']),
    Extension('aegame.cvx', ['aegame/cvx.pyx'], libraries=['aecore'], include_dirs=['aecore']),
    Extension('aegame.box', ['aegame/box.pyx'], libraries=['aecore'], include_dirs=['aecore']),
    Extension('aegame.exm', ['aegame/exm.pyx'], libraries=['aecore'], include_dirs=['aecore']),
    Extension('aegame.mem', ['aegame/mem.pyx'], libraries=['aecore'], include_dirs=['aecore']),
    Extension('aegame.idx', ['aegame/idx.pyx'], libraries=['aecore'], include_dirs=['aecore']),
    Extension('aegame.vtx', ['aegame/vtx.pyx'], libraries=['aecore'], include_dirs=['aecore']),
    Extension('aegame.tri', ['aegame/tri.pyx'], libraries=['aecore'], include_dirs=['aecore']),
    Extension('aegame.img', ['aegame/img.pyx'], libraries=['aecore'], include_dirs=['aecore']),
    Extension('aegame.fnt', ['aegame/fnt.pyx'], libraries=['aecore'], include_dirs=['aecore']),
]

if sys.platform == "win32":
    if __debug__: # windows needs to link with different libs in debug
        if sys.maxsize == 0x7FFFFFFF:
            pydll_path = 'x86'

        if sys.maxsize == 0x7FFFFFFFFFFFFFFF:
            pydll_path = 'x64'

        pydll_path = 'external/Py{}kDebugDLL/{}/'.format( \
                        sys.version_info.major, pydll_path)

        for name in os.listdir(pydll_path):
            if 'clean' in sys.argv:
                # cleanup the main dir, ignoring file not found errors
                try: os.remove(name)
                except WindowsError: pass
            else:
                try: shutil.copy2(os.path.join(pydll_path, name), name)
                except IOError: pass # throws if building with dbg lib

        # copy the sizer utility, for reporting windows pdb sizes
        if 'clean' in sys.argv:
            try: os.remove('Sizer.exe')
            except WindowsError: pass
        else:
            shutil.copy2('external/sizer/Sizer.exe', 'Sizer.exe')

# ==============================================================================
# ~ [ pyxl ]
# ==============================================================================

# NOTE we're just putting this code here, for easy access from both c and python.
# it could also go inside the pyx module itself, or into its own shared library.
shared_libs[0].include_dirs.extend(
[
    'pyxl',
])

shared_libs[0].sources.extend(
[
    'pyxl/xl_core.c',
])

shared_libs[0].define_macros.extend(
[
    ('XL_COMPILING_DLL', '1'),
])

shared_libs[0].libraries.extend(
[
    'opengl32' if sys.platform == 'win32' else 'GL',
    'SDL2', 'SDL2_ttf', 'SDL2_mixer',
])

ext_modules.insert(0,
    Extension(
        'pyxl.xl',

        sources = [
            'pyxl/xl.pyx',
        ],

        libraries = [
            'aecore',
        ],

        include_dirs = [
            'aecore',
        ],
    )
)

# XXX threw this new name in here to either reduce or increase confusion (aecore + XL)
if 1:
    shared_libs[0].name = 'axle'

    for module in ext_modules:
        module.libraries = ['axle' if L == 'aecore' else L for L in module.libraries]

if sys.platform == "win32":
    # copy all SDL libraries to the main directory in windows
    if sys.maxsize == 0x7FFFFFFF:
        sdl_path = 'x86'

    if sys.maxsize == 0x7FFFFFFFFFFFFFFF:
        sdl_path = 'x64'

    sdl_path = 'external/SDL2/{}/'.format(sdl_path)

    for name in os.listdir(sdl_path):
        if 'clean' in sys.argv:
            try: os.remove(name)
            except WindowsError: pass
        else:
            shutil.copy2(os.path.join(sdl_path, name), name)

if 'clean' in sys.argv:
    """
    try: os.remove('external/SDL2/GameControllerData/mapping.inl')
    except Exception as e: print e
    """
else:
    headerize('external/SDL2/GameControllerData', 'gamecontrollerdb.txt', 'mapping.inl')

# ==============================================================================
# ~ [ mash3D ]
# ==============================================================================

shared_libs[0].include_dirs.extend(
[
    'mash3D',
])

shared_libs[0].sources.extend(
[
    'mash3D/gl_core.cpp',
    'mash3D/al_core.cpp',
])

shared_libs[0].define_macros.extend(
[
    ('GL_COMPILING_DLL', '1'),
    ('AL_COMPILING_DLL', '1'),
])

ext_modules.insert(0,
    Extension(
        'mash3D.al',

        sources = [
            'mash3D/al.pyx',
        ],

        libraries = [
            'axle',
        ],

        include_dirs = [
            'aecore',
        ],
    )
)

ext_modules.insert(0,
    Extension(
        'mash3D.gl',

        sources = [
            'mash3D/gl.pyx',
        ],

        libraries = [
            'axle',
        ],

        include_dirs = [
            'aecore',
        ],
    )
)

# re-patch the name of the module to indicate it's not "vanilla" aecore or axle
if 1:
    shared_libs[0].name = 'axle3D'

    for mod in ext_modules:
        mod.libraries = ['axle3D' if L == 'axle' else L for L in mod.libraries]

if sys.platform == "win32":
    # we currently need DevIL for loading DDS files in Delta-V
    if sys.maxsize == 0x7FFFFFFF:
        devil_path = 'x86'

    if sys.maxsize == 0x7FFFFFFFFFFFFFFF:
        devil_path = 'x64'

    devil_path = 'external/DevIL/{}/'.format(devil_path)

    for name in os.listdir(devil_path):
        if 'clean' in sys.argv:
            try: os.remove(name)
            except WindowsError: pass
        else:
            shutil.copy2(os.path.join(devil_path, name), name)

# ==============================================================================
# ~ [ setup ]
# ==============================================================================

setup(
    shared_libs = shared_libs,
    ext_modules = ext_modules,
)
