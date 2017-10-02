# HackmanGames

This is my own collection of general-purpose reusable game libraries for Python and/or C++. More info and screenshots to come!

AECORE
------
The lowest level of C code, responsible for system-level stuff like timers, math, image codecs and processing, triangle mesh transformation, branch and switch coverage tracking, and much more.

AEGAME
------
A Python wrapper for aecore.

PYXL
----
A simple SNES-style 2D game engine, featuring a unique Smalltalk-esque object system and a really neat game controller interface.

MASH3D
------
A fairly basic 3D engine, compatible with PYXL and utilizing the same general structure. More cool particle and animation effects coming soon!

DELTA-V
-------
An example game utilizing Mash3D, originally developed by Alex Holkner for PyWeek. Eventually, Pyglet will be replaced by PYXL as the windowing, input, and 2D font rendering solution.
