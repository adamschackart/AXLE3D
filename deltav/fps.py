from aegame import *
from pyxl import *
from mash3D import *

from math import sin, cos, radians

from pyglet import clock
from pyglet import font

from pyglet.window import mouse
from pyglet.window import key

from . import menu
from . import obj
from . import game

class Character(object):
    MOVEMENT_NAMES = [
        'turn_left',
        'turn_right',
        'forward',
        'left',
        'backward',
        'right',
        'jump',
        'fire']

    center_height = 0
    eye_height = 0
    step_height = 0
    turn_speed = 0
    move_speed = 0
    jump_speed = 0
    push_friction = 0
    collide_radius = 0
    collide_height = 0

    @profile("fps.py", "Character.__init__")
    def __init__(self, position, heading, pitch):
        self.collision = Ellipsoid(position.copy(), Vec3(self.collide_radius,
                                    self.collide_height, self.collide_radius))
        self.heading = heading
        self.pitch = pitch

        self.velocity = Vec3()
        self.freefall = True
        self.pushed = False
        self.movement = {}

        for k in self.MOVEMENT_NAMES:
            self.movement[k] = False

    @profile("fps.py", "Character.get_coord")
    def get_coord(self, eye_offset):
        c = Coord3D()

        c.position = self.collision.c
        if eye_offset: c.y += self.eye_height

        c.pitch = self.pitch
        c.heading = self.heading

        return c

    @profile("fps.py", "Character.apply_camera")
    def apply_camera(self):
        self.get_coord(True).apply_camera()

    @profile("fps.py", "Character.get_facing_ray")
    def get_facing_ray(self):
        return self.get_coord(False).ray

    @profile("fps.py", "Character.get_view_frustum")
    def get_view_frustum(self):
        projection = Mat4x4().row_perspective(70, 1920.0 / 1080.0, 0.1, 1000.0)
        modelview = Mat4x4()

        modelview.imul_mat(Mat4x4().row_rotate_x(radians(-self.pitch)))
        modelview.imul_mat(Mat4x4().row_rotate_y(radians(-self.heading)))

        translate = -self.collision.c
        translate[1] -= self.eye_height

        modelview.imul_mat(Mat4x4().row_translate(translate))
        mvp = projection.mul_mat(modelview)

        return PlaneFrustum().from_model_view_projection_matrix(mvp)

class FPS(game.Scene):
    MOUSE_SENSITIVITY = 0.3
    FOV = 70.
    KEYMAP = {
        'w': 'forward',
        'a': 'left',
        's': 'backward',
        'd': 'right',
        'space': 'jump',
    }

    BUTTONMAP = {
        'left': 'fire',
    }

    @profile("fps.py", "FPS.__init__")
    def __init__(self, level):
        self.level = level
        self.restart()

        self.collision = CollisionMesh()

        for mesh in level.collision_meshes:
            self.collision.add_windings(mesh.polygons)

        self.clock_display = None

        game_font = font.load('Arial', 36, bold=True)
        self.game_label = font.Text(game_font, '', color=(.9, .9, 1, .9))

    @profile("fps.py", "FPS.restart")
    def restart(self):
        self.level.restart()
        self.player = self.level.player # convenience

    @profile("fps.py", "FPS.activate")
    def activate(self, window):
        super(FPS, self).activate(window)
        self.beam_opacity = 0.0

        for k in self.player.movement.keys():
            self.player.movement[k] = False

        Light.default_scene()

    @profile("fps.py", "FPS.update")
    def update(self, dt):
        # This allows the fps display counter to work
        clock.tick()

        # AI, game state
        self.level.update(dt)
        if self.level.has_exit:
            self.has_exit = True
            return

        # Beam fades out
        self.beam_opacity = max(0, self.beam_opacity - dt * 2)

        # Update particle emitters
        for emitter in self.level.emitters:
            emitter.update(dt)

        # Everybody moves, collides with environment
        self.update_character(dt, self.player)
        for baddie in self.level.baddies:
            self.update_character(dt, baddie)

        # Player collides with baddies
        for baddie in self.level.baddies:
            if self.player.collision.intersect_ellipsoid(baddie.collision):
                self.level.on_collide_baddie(baddie)

        # Player collides with triggers
        for polygon, name in self.level.triggers:
            if polygon.intersect_ellipsoid(self.player.collision):
                self.level.on_trigger(dt, name)

        # Trigger cutscene
        if self.level.queued_state:
            self.queued_state = self.level.queued_state
            self.level.queued_state = None

        # Trigger game over
        if self.level.dead:
            self.queued_state = menu.GameMenu(self)

        # Weapon firing
        if self.player.movement['fire'] and self.level.on_fire():
            self.beam_opacity = 1.

            ray = self.player.get_facing_ray()
            ray.p.y += self.player.eye_height

            for baddie in self.level.baddies:
                if baddie.collision.intersect_ray3(ray):
                    self.level.on_hit_baddie(baddie, ray)

    @profile("fps.py", "FPS.update_character")
    def update_character(self, dt, character):
        p = character

        dr = (p.movement['turn_right'] - p.movement['turn_left'])
        p.heading += dr * dt * p.turn_speed

        dz = (p.movement['backward'] - p.movement['forward'])
        dx = (p.movement['right'] - p.movement['left'])

        cs = cos(-radians(p.heading))
        sn = sin(-radians(p.heading))

        dx, dz = dx * cs - dz * sn, dx * sn + dz * cs

        if p.freefall:
            dx /= 2
            dz /= 2

        if p.pushed:
            p.fade_to_death -= dt
            if p.fade_to_death < 0:
                self.level.on_character_fallout(p)
        else:
            p.collision.c.x += dx * dt * p.move_speed
            p.collision.c.z += dz * dt * p.move_speed

        if p.freefall or p.pushed:
            p.velocity.iadd_vec(self.level.gravity.mul_flt(dt))
            p.collision.c.iadd_vec(p.velocity.mul_flt(dt))

            if p.collision.c.y < self.level.fallout_height:
                self.level.on_character_fallout(p); return

        resolution = Vec3()
        collisions = self.collision.intersect_ellipsoid(p.collision)

        for winding, d in collisions:
            n = winding.get_face_normal()
            r = n.mul_flt(d)

            if n.dot(Vec3(0, -1, 0)) > 0.5:
                r.y = min(0, r.y)
                if p.velocity.y > 0: p.velocity.y = 0

            resolution.iadd_vec(r)
        p.collision.c.iadd_vec(resolution)

        if p.freefall or dx or dz:
            seg_length = max(p.step_height, -dt * p.velocity.y)

            segment = LineSegment3(Vec3(p.collision.c.x,
                                        p.collision.c.y - p.center_height,
                                        p.collision.c.z),
                                Vec3(0, seg_length, 0))

            point, plane = self.collision.intersect_floor(segment)
            if plane:
                p.collision.c.y = point.y + p.center_height
                p.velocity.y = 0
                p.freefall = False
            else:
                p.freefall = True

        if not p.pushed and not p.freefall:
            p.velocity = Vec3()
        elif p.pushed and (not p.freefall or resolution):
            p.velocity.imul_flt((1 - dt) * (1 - character.push_friction))
            if p.velocity.magnitude_squared() < 0.1:
                p.velocity = Vec3()
                p.pushed = False

        if p.movement['jump'] and not p.freefall and not p.pushed:
            p.velocity.x = dx * p.move_speed
            p.velocity.y = p.jump_speed
            p.velocity.z = dz * p.move_speed
            p.freefall = True

    @profile("fps.py", "FPS.draw")
    def draw(self, window):
        Texture.draw_skybox(self.level.skybox, self.player.get_coord(False))

        self.player.apply_camera()
        frustum = self.player.get_view_frustum()

        Light.apply_all()

        for mesh in self.level.visual_meshes:
            if frustum.contains_aabbox(*mesh.get_aabbox()):
                mesh.draw()

        for baddie in self.level.baddies:
            c = baddie.get_coord(False)
            c.push_object()
            baddie.draw()
            c.pop_object()

        for emitter in self.level.emitters:
            emitter.draw()

        c = Coord3D()
        c.pos = Vec3(.2, -.4, -.35)
        c.push_object_ex(True)

        for mesh in self.level.weapon_meshes:
            mesh.draw()

        c.pop_object()

        if self.beam_opacity:
            gl.PushMatrix()
            gl.LoadIdentity()

            gl.PushAttrib(gl.ENABLE_BIT | gl.CURRENT_BIT | gl.COLOR_BUFFER_BIT)
            gl.Disable(gl.LIGHTING)

            gl.Enable(gl.BLEND)
            gl.BlendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA)

            self.level.beam_texture.bind()

            gl.Color4f(1, 1, 1, self.beam_opacity)

            w = 0.05
            x = 0.2
            y = -.4
            z = -.35

            gl.Begin(gl.QUADS)
            gl.TexCoord2f(0, 0)
            gl.Vertex3f(x-w, y, z)
            gl.TexCoord2f(1, 0)
            gl.Vertex3f(x+w, y, z)
            gl.TexCoord2f(1, 1000)
            gl.Vertex3f(x+w, 0, -1000)
            gl.TexCoord2f(0, 1000)
            gl.Vertex3f(x-w, 0, -1000)
            gl.End()

            gl.PopAttrib()
            gl.PopMatrix()

        with gl.util.Scene2D(self._width, self._height):
            x = (self._width  - self.level.crosshair.width ) / 2
            y = (self._height - self.level.crosshair.height) / 2

            self.level.crosshair.draw(x, y)

            if self.level.game_text:
                self.game_label.text = self.level.game_text
                self.game_label.x = (self._width - self.game_label.width) / 2
                self.game_label.y = self._height / 3.5
                self.game_label.draw()

            if self.level.fade_to_black:
                gl.util.rect(FloatRect(0, 0, self._width, self._height),
                                Vec4(0, 0, 0, self.level.fade_to_black))

            elif self.level.fade_to_damage:
                gl.util.rect(FloatRect(0, 0, self._width, self._height),
                            Vec4(0.5, 0, 0, self.level.fade_to_damage))

            if self.clock_display:
                self.clock_display.draw()

    def on_mouse_motion(self, mouse, window, buttons, x, y, dx, dy):
        self.player.pitch   += dy * self.MOUSE_SENSITIVITY
        self.player.heading -= dx * self.MOUSE_SENSITIVITY

        self.player.pitch = min(max(self.player.pitch, -80), 80)

    def on_mouse_button(self, mouse, button, pressed):
        if button in self.BUTTONMAP:
            self.player.movement[self.BUTTONMAP[button]] = pressed

    def on_keyboard_key(self, keyboard, mods, key, pressed):
        if pressed:
            if key in self.KEYMAP:
                self.player.movement[self.KEYMAP[key]] = True

            # FIXME: resizing the window somehow leads to a white screen here!!!
            #elif key == 'f11':
            #    super(FPS, self).on_keyboard_key(keyboard, mods, key, pressed)

            elif key == 'escape':
                self.queued_state = menu.GameMenu(self)
            elif key == 't' and 'control' in mods and 'shift' in mods and __debug__:
                self.queued_state = menu.CheatMenu(self)
        else:
            if key in self.KEYMAP:
                self.player.movement[self.KEYMAP[key]] = False
