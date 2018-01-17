from aegame import *
from pyxl import *
from mash3D import *

class Scene(object):
    FOV = 70.0
    znear = 0.1
    zfar = 1000.0
    exclusive_mouse = True

    def on_enter(self, window):
        Mouse.get_primary().relative = self.exclusive_mouse

        self._width = window.width
        self._height = window.height

        gl.util.enter3D(self._width, self._height, self.FOV, self.znear, self.zfar)

    def on_leave(self, window):
        Mouse.get_primary().relative = False
        gl.util.leave3D()

    def on_quit(self):
        raise SystemExit

    def on_window_resize(self, window, width, height):
        window.render_size = (width, height)

        self.on_leave(window)
        self.on_enter(window)

    def on_keyboard_key(self, keyboard, mods, key, pressed):
        if pressed:
            if key == 'f11':
                w = Window.get_primary()
                w.fullscreen = not w.fullscreen

            elif key == 'escape':
                raise SystemExit
