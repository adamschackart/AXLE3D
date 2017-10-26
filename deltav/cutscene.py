from mash3D import Coord3D, Texture, gl
from aegame import Vec3
from pyglet import font

from . import game

class CutScene(game.ThreeD):
    znear = 0.5
    zfar = 100000.0

    def __init__(self, level):
        self.level = level

    def draw(self):
        gl.Clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT)

        c = Coord3D()
        c.heading = self.camera_heading
        c.pitch = self.camera_pitch
        c.position = self.camera_pos.sub_vec(self.station_pos)

        Texture.draw_skybox(self.skybox, c)
        c.apply_camera()

        gl.Light.apply_all()

        for mesh in self.level.visual_meshes:
            mesh.draw()

        for emitter in self.level.cut_scene_emitters:
            emitter.draw()

    def on_key_press(self, symbol, modifiers):
        if self.time > 1: self.has_exit = True
        return True

class CutScene1(CutScene):
    def __init__(self, level):
        super(CutScene1, self).__init__(level)

        self.skybox = level.skybox_atmo

        self.camera_pos = Vec3()
        self.camera_pitch = 53
        self.camera_heading = 235
        self.camera_heading_speed = 0
        self.station_pos = Vec3(500, 800, 1600)
        self.station_vel = Vec3()
        self.station_accel = Vec3()
        self.station_accel_accel = Vec3()

        self.time = 0

    def update(self, dt):
        self.station_accel.iadd_vec(self.station_accel_accel.mul_flt(dt))
        self.station_vel.iadd_vec(self.station_accel.mul_flt(dt))
        self.station_pos.iadd_vec(self.station_vel.mul_flt(dt))

        self.camera_heading += self.camera_heading_speed * dt
        self.time += dt

        if self.time > 3:
            self.camera_heading_speed = 1
            self.station_accel_accel = Vec3(0, 8, 0)
            for emitter in self.level.cut_scene_emitters:
                emitter.update(dt)

        if self.time > 20.:
            self.has_exit = True

class CutScene2(CutScene):
    def __init__(self, level):
        super(CutScene2, self).__init__(level)

        self.skybox = level.skybox_space

        self.camera_pos = Vec3()
        self.camera_pitch = 53
        self.camera_heading = 235
        self.camera_heading_speed = 0
        self.station_pos = Vec3(500, 0, 1600)
        self.station_vel = Vec3(0, 300, 0)
        self.station_accel = Vec3(0, -15, 0)
        self.station_accel_accel = Vec3()

        self.time = 0

    def update(self, dt):
        self.station_accel.iadd_vec(self.station_accel_accel.mul_flt(dt))
        self.station_vel.iadd_vec(self.station_accel.mul_flt(dt))
        self.station_pos.iadd_vec(self.station_vel.mul_flt(dt))

        self.camera_heading += self.camera_heading_speed * dt
        self.time += dt

        for emitter in self.level.cut_scene_emitters:
            emitter.update(dt)
        if self.time > 2:
            # teleport the emitter out of sight for a "thrust failure" effect
            for emitter in self.level.cut_scene_emitters:
                emitter.position = Vec3(0, -50000, 0)

        if self.time > 10.:
            self.level.cut_scene_emitters = []
            self.has_exit = True

class CutScene3(CutScene):
    credits = [
        (3, 'delta-v', 1, ['']),
        (4, 'Code and art', 2, ['', 'Alex Holkner']),
        (8, 'Additional art', 2,
          ['', 'amethyst7.gotdoofed.com', 'nasa.gov', 'developer.nvidia.com']),
        (1, 'Thanks for playing', 1, [''])
    ]

    def __init__(self, level):
        super(CutScene3, self).__init__(level)

        self.skybox = level.skybox_space

        self.camera_pos = Vec3(100, 600, 1400)
        self.camera_pitch = -25
        self.camera_heading = 30
        self.camera_heading_speed = -1
        self.station_pos = Vec3()
        self.station_vel = Vec3()
        self.station_accel = Vec3()
        self.station_accel_accel = Vec3()

        self.time = 0

        self.credit_line = -1
        self.subcredit_line = -1
        self.credit_time = 2.
        self.subcredit_time = 2.
        self.max_credit_time = 2.
        self.max_subcredit_time = 2.
        self.subcredits = []

        label1_font = font.load('Arial', 36., bold=True)
        self.label1 = font.Text(label1_font, '', color=(1, 1, 1, 1))
        label2_font = font.load('Arial', 24., bold=True)
        self.label2 = font.Text(label2_font, '', color=(1, 1, 1, 1))

    def update(self, dt):
        self.level.has_exit = True

        self.time += dt
        self.camera_heading += self.camera_heading_speed * dt

        if self.time > 2:
            for emitter in self.level.cut_scene_emitters:
                emitter.update(dt)

        self.credit_time -= dt
        if self.credit_time <= 0. and self.credit_line < len(self.credits) - 1:
            self.credit_line = min(self.credit_line + 1, len(self.credits) - 1)
            self.max_credit_time, self.label1.text, self.max_subcredit_time, \
                            self.subcredits = self.credits[self.credit_line]
            self.credit_time = self.max_credit_time
            self.subcredit_time = 0
            self.subcredit_line = -1

            self.label1.x = (self._width - self.label1.width) / 2
            self.label1.y = self._height * .6

        self.subcredit_time -= dt
        if self.subcredit_time <= 0 and self.subcredit_line < len(self.subcredits) - 1:
            self.subcredit_line = min(self.subcredit_line + 1, len(self.subcredits) - 1)
            self.label2.text = self.subcredits[self.subcredit_line]
            self.subcredit_time = self.max_subcredit_time

            self.label2.x = (self._width - self.label2.width) / 2
            self.label2.y = self.label1.y - self.label2.height - 20

        if self.credit_time >= self.max_credit_time - .5:
            c = self.max_credit_time - .5
            self.label1.color = (1, 1, 1, 1 - (self.credit_time - c) / .5)
        elif self.credit_time < 0.5 and self.credit_line < len(self.credits)-1:
            self.label1.color = (1, 1, 1, self.credit_time / .5)

        if self.subcredit_time >= self.max_subcredit_time - .5:
            c = self.max_subcredit_time - .5
            self.label2.color = (1, 1, 1, 1 - (self.subcredit_time - c) / .5)
        elif self.subcredit_time < 0.5:
            #self.subcredit_line < len(self.subcredits)-1:
            self.label2.color = (1, 1, 1, self.subcredit_time / .5)

    def draw(self):
        super(CutScene3, self).draw()

        self.begin2d()
        self.label1.draw()
        self.label2.draw()
        self.end2d()
