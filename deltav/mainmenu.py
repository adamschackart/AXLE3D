from aegame import *
from pyxl import *
from mash3D import *

from . import fps
from . import level
from . import survival
from . import game

class MainMenu(game.Scene):
    exclusive_mouse = False
    FOV = 120

    option_height = 30
    options = {
        'mission': (0, 315-37),
        'survival-stage1': (45, 315-134),
        'survival-stage2': (45, 315-180),
        'survival-stage3': (45, 315-226),
        'exit': (0, 315-308),
    }

    def __init__(self):
        self.skybox = level.Level._get_skybox('starfield')
        self.heading = 0
        self.title = level.Level._get_texture('deltav.png')
        self.menu = level.Level._get_texture('menu.png')
        self.cursor = level.Level._get_texture('cursor.png')
        self.one_moment = level.Level._get_texture('onemoment.png')

        self.loading = False
        self.loading_time = 0.0

    def activate(self, window):
        super(MainMenu, self).activate(window)

        self.menu_x = (self._width - self.menu.width) / 2
        self.menu_y = (self._height - self.menu.height) / 2
        self.selected_option = None

        self.loading = False

    @profile("mainmenu.py", "MainMenu.update")
    def update(self, dt):
        self.heading += dt
        if self.loading:
            self.loading_time += dt
            if self.loading_time >= 1.:
                if self.selected_option == 'mission':
                    self.queued_state = fps.FPS(level.Level())
                elif self.selected_option == 'survival-stage1':
                    self.queued_state = fps.FPS(survival.Survival(1))
                elif self.selected_option == 'survival-stage2':
                    self.queued_state = fps.FPS(survival.Survival(2))
                elif self.selected_option == 'survival-stage3':
                    self.queued_state = fps.FPS(survival.Survival(3))

    @profile("mainmenu.py", "MainMenu.draw")
    def draw(self, window):
        #
        # FIXME: why do we need to re-submit this transform? it happens in self.activate!
        #
        with gl.util.Scene3D(self._width, self._height, self.FOV, self.znear, self.zfar):
            c = Coord3D()
            c.heading = self.heading

            Texture.draw_skybox(self.skybox, c)

        with gl.util.Scene2D(self._width, self._height):
            self.menu.blit(self.menu_x, self.menu_y)

            if self.selected_option:
                o_x, o_y = self.options[self.selected_option]
                self.cursor.blit(
                 self.menu_x + o_x - self.cursor.width,
                 self.menu_y + o_y + (self.option_height - self.cursor.height) / 2)

            if self.loading:
                gl.util.rect(FloatRect(0, 0, self._width, self._height), Vec4(0, 0, 0, self.loading_time * 2))

                if self.loading_time > .5:
                    self.one_moment.blit_ex(
                        (self._width  - self.one_moment.width ) / 2,
                        (self._height - self.one_moment.height) / 2,
                        Vec4(1, 1, 1, (self.loading_time - .5) * 2))

            self.title.blit(0, 0)

    @profile("mainmenu.py", "MainMenu.select")
    def select(self, x, y):
        y -= self.menu_y
        self.selected_option = None
        for name, (o_x, o_y) in self.options.items():
            if y >= o_y and y <= o_y + self.option_height:
                self.selected_option = name

    @profile("mainmenu.py", "MainMenu.on_mouse_motion")
    def on_mouse_motion(self, mouse, window, buttons, x, y, dx, dy):
        if not self.loading: self.select(x, y)

    @profile("mainmenu.py", "MainMenu.on_mouse_button")
    def on_mouse_button(self, mouse, button, pressed):
        if pressed:
            self.select(mouse.x, mouse.y)

            if self.selected_option == 'exit':
                raise SystemExit
            elif self.selected_option is not None:
                self.loading = True
                self.loading_time = 0.0
