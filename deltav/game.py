from pyglet.window import Window
from pyglet import clock

from mash3D import *
from aegame import *

class GameWindow(Window):
    def __init__(self, **kwargs):
        super(GameWindow, self).__init__(**kwargs)
        self.states = []

    def push_state(self, state):
        if self.states:
            if hasattr(self.states[-1], 'deactivate'):
                self.states[-1].deactivate(self)

            self.states[-1].queued_state = None

        self.states.append(state)
        self.push_handlers(state)

        if hasattr(state, 'activate'):
            state.activate(self)

    def pop_state(self):
        if hasattr(self.states[-1], 'deactivate'):
            self.states[-1].deactivate(self)

        self.states.pop()
        self.pop_handlers()

        if self.states:
            if hasattr(self.states[-1], 'activate'):
                self.states[-1].activate(self)
        else:
            self.exit_handler.has_exit = True

    def on_resize(self, width, height):
        pass # prevent pyglet from setting up an orthographic projection matrix

    def run(self, state=None):
        if state:
            self.push_state(state)

        while not getattr(self, 'has_exit', False):
            while self.states and getattr(self.states[-1], 'has_exit', False):
                self.pop_state()

            dt = clock.tick()

            # Filter out insane spikes (e.g. due to another process, or loading)
            if dt > 0.1:
                log("TIME", "long frame ({} seconds)".format(dt))
                dt = 0.01

            self.dispatch_events()

            if self.states:
                if hasattr(self.states[-1], 'update'):
                    self.states[-1].update(dt)

                if hasattr(self.states[-1], 'draw'):
                    self.states[-1].draw()

            self.flip()

            if self.states and getattr(self.states[-1], 'queued_state', None):
                self.push_state(self.states[-1].queued_state)

class ThreeD(object):
    FOV = 70.
    znear = 0.1
    zfar = 1000.0
    exclusive_mouse = True

    def activate(self, window):
        if self.exclusive_mouse:
            window.set_exclusive_mouse(True)

        self._width = window.width
        self._height = window.height

        gl.util.enter3D(self._width, self._height, self.FOV, self.znear, self.zfar)

    def deactivate(self, window):
        if self.exclusive_mouse:
            window.set_exclusive_mouse(False)

        gl.util.leave3D()

    @profile("threed.py", "ThreeD.begin2d")
    def begin2d(self):
        gl.util.enter2D(self._width, self._height)

    @profile("threed.py", "ThreeD.end2d")
    def end2d(self):
        gl.util.leave2D()

    @profile("threed.py", "ThreeD.draw_fade")
    def draw_fade(self, r, g, b, a):
        gl.util.rect(FloatRect(0, 0, self._width, self._height), Vec4(r, g, b, a))
