import pyglet # disable fake gl context
pyglet.options['shadow_window'] = False

import sys # import everything else after pyglet hinting
from . import game, mainmenu

game.GameWindow(width=1920 / 2, height=1080 / 2, # main
    vsync='-vsync' in sys.argv).run(mainmenu.MainMenu())
