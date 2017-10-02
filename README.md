# HackmanGames

This is my own collection of general-purpose reusable game libraries. More info and screenshots to come!

AECORE
------
The lowest level of C code, responsible for system-level stuff like timers, math, image codecs and processing, triangle mesh transformation, etc.

AEGAME
------
A Python wrapper for aecore.

PYXL
----
A simple SNES-style 2D game engine, featuring a unique Smalltalk-esque object system.

MASH3D
------
A fairly basic 3D engine, compatible with PYXL and utilizing the same general structure.

DELTA-V
-------
An example game utilizing Mash3D, originally developed by Alex Holkner for PyWeek. Eventually, Pyglet will be replaced by PYXL as the windowing and 2D font rendering solution.
