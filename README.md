# The AJ Hackman Game Library Collection

This is my own collection of general-purpose reusable game libraries for Python and/or C++. More info and screenshots to come!

Please note that development has been frozen until 2018 while I search for employment and a place to set up my internal Mercurial server. Pull requests and issues will be ignored until this time.

SETUP
-----
Just type

    python setup.py build

and then

    python -m deltav
    
to run the test game.

    python -O setup.py build
    
creates an optimized release build.

Python 3, Mac OSX, package installation, and compiling Cython modules as C++ are currently unsupported.
These libraries were designed to be dropped into the folder containing the game code, as there is a large degree of customization available during the build process.

Windows users must have Cython and cx_Freeze installed via pip, and Linux users must have SDL, DevIL, and OpenGL dev packages installed as well.

AECORE
------
The lowest level of C code, responsible for system-level stuff like timers, math, image codecs and processing, triangle mesh transformation, branch and switch coverage tracking, and much more.

AEGAME
------
A Python wrapper for aecore.

PYXL
----
A simple SNES-style 2D game engine, featuring a unique Smalltalk-esque object system, a hybrid software/hardware renderer, and a really neat game controller interface featuring built-in support for cheat codes and advanced deadzone handling.

MASH3D
------
A fairly basic 3D engine, compatible with PYXL and utilizing the same general structure. More cool particle and animation effects coming soon!

DELTA-V
-------

![Alt text](https://i.imgur.com/zijDoGg.png "Delta-V pause menu")

An example first-person shooter game utilizing Mash3D, originally developed by Alex Holkner for PyWeek. Eventually, Pyglet will be replaced by PYXL as the windowing, input, and 2D font rendering solution.
