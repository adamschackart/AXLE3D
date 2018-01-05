from aegame import *
from pyxl import *
from mash3D import *

from pyglet import clock
from pyglet import font

from . import game

class Menu(game.Scene):
    options = []
    text_color = (0, 0, 0, 1)

    def __init__(self):
        self.selected_option = None
        self.selected_hot = False

        self.label_font = font.load('Arial', 24.0, bold=True)
        self.labels = []

        width = height = 0
        for name in self.options[::-1]:
            if name is None:
                height += 30
                continue

            label = font.Text(self.label_font, name, color=self.text_color)
            label.y = height + 5
            width = max(width, label.width)
            height += label.height
            self.labels.append(label)
            height += 10

        self.width = width
        self.height = height
        self.x = 0
        self.y = 0

        self.has_exit = False

    def activate(self, window):
        super(Menu, self).activate(window)

        self.x = (window.width - self.width) / 2
        self.y = (window.height - self.height) / 2

        Mouse.get_primary().relative = False

    @profile("menu.py", "Menu.draw")
    def draw(self, window):
        with gl.util.Scene2D(self._width, self._height):
            self.draw_background()

            gl.Translatef(self.x, self.y, 0)
            for label in self.labels:
                label.draw()

    def draw_background(self):
        pass

    def on_keyboard_key(self, keyboard, mods, key, pressed):
        if pressed:
            if key == 'f11':
               w = Window.get_primary()
               w.fullscreen = not w.fullscreen
            else:
                self.has_exit = True

    def get_selected(self, x, y):
        if x > self.x and x < self.x + self.width:
            y -= self.y
            for label in self.labels:
                if y > label.y + label.font.descent and \
                   y < label.y + label.height + label.font.descent:
                    return label
        return None

    def on_mouse_motion(self, mouse, window, buttons, x, y, dx, dy):
        self.selected_option = self.get_selected(x, y)

    def on_mouse_button(self, mouse, button, pressed):
        if pressed:
            if self.selected_option:
                self.selected_hot = True
        else:
            selected = self.get_selected(mouse.x, mouse.y)

            if selected is not None:
                self.on_select(selected)

            self.selected_hot = False

class PopupMenu(Menu):
    def __init__(self, fps):
        super(PopupMenu, self).__init__()
        self.fps = fps

    @profile("menu.py", "PopupMenu.draw_background")
    def draw_background(self):
        gl.util.rect(FloatRect(0, 0, self._width, self._height), Vec4(0, 0, 0, 0.5))

        pad = 10
        y = self.y + self.label_font.descent - pad
        x = self.x - pad

        gl.util.rect(FloatRect( x, y, self.width + pad * 2, self.height + pad * 2 ),
                                Vec4(1, 1, 1, 0.7))

        if self.selected_option:
            if self.selected_hot:
                c = Vec4(1.0, 1.0, 1.0, 0.8)
            else:
                c = Vec4(0.8, 0.8, 1.0, 0.8)

            label = self.selected_option

            gl.util.rect(FloatRect( self.x + label.x - pad,
                                    self.y + label.y + label.font.descent - 5,
                                    self.width + pad * 2,
                                    label.height + 10), c)

    @profile("menu.py", "PopupMenu.draw")
    def draw(self, window):
        #
        # FIXME: why do we need to re-submit this transform? it happens in self.activate!
        #
        with gl.util.Scene3D(self._width, self._height, self.FOV, self.znear, self.zfar):
            self.fps.draw(window)

        super(PopupMenu, self).draw(window)

class GameMenu(PopupMenu):
    options = [
        'Resume Game',
        'Retry Stage',
        'Main Menu',
        'Exit',
    ]

    def on_select(self, label):
        if label.text == 'Resume Game':
            pass
        elif label.text == 'Retry Stage':
            self.fps.level.retry_stage()
        elif label.text == 'Main Menu':
            self.fps.has_exit = True
        elif label.text == 'Exit':
            raise SystemExit

        self.has_exit = True

class CheatMenu(PopupMenu):
    options = [
        'Resume Game',
        'Advance Stage',
        'Unlimited Weapon Charge',
        'Unlimited Health',
        'FPS Counter'
    ]

    def on_select(self, label):
        if label.text == 'Resume Game':
            pass
        elif label.text == 'Advance Stage':
            self.fps.level.advance_stage()
        elif label.text == 'Unlimited Weapon Charge':
            self.fps.level.weapon_charge = 10000000.
            self.fps.level.update_weapon_materials()
        elif label.text == 'Unlimited Health':
            self.fps.level.set_health(1000000.)
        elif label.text == 'FPS Counter':
            self.fps.clock_display = clock.ClockDisplay()

        self.has_exit = True
