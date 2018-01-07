from . import mainmenu
import sys, pyxl

pyxl.Window(

    title='Delta-V',
    fullscreen='-fullscreen' in sys.argv,
    vsync='-vsync' in sys.argv

).run(mainmenu.MainMenu())
