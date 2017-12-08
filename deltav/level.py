from . import cutscene
from . import fps
from . import obj
from . import game

from aegame import profile, Vec3
from mash3D import gl

import aegame
import math
import sys

TRIGGER_ACTIVATE_TIME = 3.0
WEAPON_PERIOD = 0.5
MAX_WEAPON_CHARGE = 5.0

class PlayerCharacter(fps.Character):
    center_height = 0.9 # height to center of ellipsoid
    eye_height = 0.6    # above center of ellipsoid
    step_height = 0.3   # allowable step from ground
    move_speed = 7.0
    jump_speed = 20.0
    collide_radius = 0.3
    collide_height = 0.6

    max_health = 3.0

    # AJS: added a backdoor for my impatience during testing
    if __debug__ and '-fastwalk' in sys.argv: move_speed *= 6

    def __init__(self, *args, **kwargs):
        super(PlayerCharacter, self).__init__(*args, **kwargs)
        self.health = self.max_health

class BaddieCharacter(fps.Character):
    center_height = 1.1
    step_height = 0.3
    turn_speed = 90.0
    move_speed = 6.0
    jump_speed = 20.0
    push_friction = 0.01
    collide_radius = 0.5
    collide_height = 0.4

    push_speed = 20 # speed when pushed by player's weapon
    strength = 1.0  # health to take from player
    active = True   # if inactive, baddie is playing death animation

    if __debug__ and ('-nodamage' in sys.argv or '-godmode' in sys.argv):
        strength = 0.0 # AJS: added a backdoor for testing without dying

    fade_to_death = 1.0
    time = 0.0
    attacking = False

    @profile("level.py", "BaddieCharacter.draw")
    def draw(self):
        gl.PushAttrib(gl.ENABLE_BIT | gl.LIGHTING_BIT | gl.CURRENT_BIT)
        gl.DepthMask(False)

        gl.Enable(gl.BLEND)
        gl.BlendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA)

        if self.pushed:
            meshes = self.pushed_meshes
        elif self.attacking:
            meshes = self.attack_meshes
        else:
            meshes = self.default_meshes

        frac = math.pi * 2 / len(meshes)
        for i, mesh in enumerate(meshes):
            a = math.sin(3 * self.time + i * frac) * self.fade_to_death
            if a > 0:
                m = gl.AutoObject(gl.Material(opacity=a).set_all(Vec3()))

                for group in mesh.groups:
                    group.material = m

                mesh.draw()

        gl.DepthMask(True)
        gl.PopAttrib()

class Level(object):
    objs = {}
    textures = {}

    def __init__(self):
        self.beam_texture = self._get_texture('beam.png')

        self.weapon_meshes = [
            self._get_mesh('weapon.obj', 'cube1'),
            self._get_mesh('weapon.obj', 'lights'),
        ]

        self.weapon_materials = [
            self._get_material('weapon.obj', 'level5'),
            self._get_material('weapon.obj', 'level4'),
            self._get_material('weapon.obj', 'level3'),
            self._get_material('weapon.obj', 'level2'),
            self._get_material('weapon.obj', 'level1')
        ]

        self.visual_meshes = [
            self._get_mesh('station.obj', 'visual1'),
            self._get_mesh('station.obj', 'visual2'),
            self._get_mesh('station.obj', 'visual3'),
            self._get_mesh('station.obj', 'recharge1'),
            self._get_mesh('station.obj', 'recharge2'),
            self._get_mesh('station.obj', 'recharge3'),
            self._get_mesh('station.obj', 'panel1'),
            self._get_mesh('station.obj', 'panel2'),
            self._get_mesh('station.obj', 'panel3'),
            self._get_mesh('station.obj', 'panel4'),
            self._get_mesh('station.obj', 'panel5'),
            self._get_mesh('station.obj', 'panel6'),
            self._get_mesh('station.obj', 'panel7'),
            self._get_mesh('station.obj', 'panel8'),
            self._get_mesh('station.obj', 'panel9'),
            self._get_mesh('station.obj', 'panel10'),
            self._get_mesh('station.obj', 'panel11'),
            self._get_mesh('station.obj', 'panel12'),
            self._get_mesh('station.obj', 'panel13'),
        ]

        self.collision_meshes = [
            self._get_mesh('station.obj', 'collide1'),
            self._get_mesh('station.obj', 'collide2'),
            self._get_mesh('station.obj', 'collide3'),
        ]

        self.triggers = [
            (self._get_trigger('station.obj', 'trigger_rocket1'), 'rocket1'),
            (self._get_trigger('station.obj', 'trigger_rocket2'), 'rocket2'),
            (self._get_trigger('station.obj', 'trigger_recharge1'), 'recharge'),
            (self._get_trigger('station.obj', 'trigger_recharge2'), 'recharge'),
            (self._get_trigger('station.obj', 'trigger_recharge3'), 'recharge'),
            (self._get_trigger('station.obj', 'trigger_orbit'), 'orbit'),
            (self._get_trigger('station.obj', 'trigger_powernode1'), 'node1'),
            (self._get_trigger('station.obj', 'trigger_powernode2'), 'node2'),
            (self._get_trigger('station.obj', 'trigger_powernode3'), 'node3'),
            (self._get_trigger('station.obj', 'trigger_powernode4'), 'node4'),
            (self._get_trigger('station.obj', 'trigger_final1'), 'final1'),
            (self._get_trigger('station.obj', 'trigger_final2'), 'final2'),
        ]

        self.player_start = self._get_point('station.obj', 'player_start')

        self.spawn_points_1 = [
            self._get_point('station.obj', 'spawn1_1'),
            self._get_point('station.obj', 'spawn1_2'),
            self._get_point('station.obj', 'spawn1_3'),
            self._get_point('station.obj', 'spawn1_4'),
            self._get_point('station.obj', 'spawn1_5'),
        ]

        self.spawn_points_2 = [
            self._get_point('station.obj', 'spawn2_1'),
            self._get_point('station.obj', 'spawn2_2'),
            self._get_point('station.obj', 'spawn2_3'),
            self._get_point('station.obj', 'spawn2_4'),
            self._get_point('station.obj', 'spawn2_5'),
            self._get_point('station.obj', 'spawn2_6'),
        ]

        self.spawn_points_3 = [
            self._get_point('station.obj', 'spawn3_1'),
            self._get_point('station.obj', 'spawn3_2'),
            self._get_point('station.obj', 'spawn3_3'),
            self._get_point('station.obj', 'spawn3_4'),
            self._get_point('station.obj', 'spawn3_5'),
            self._get_point('station.obj', 'spawn3_6'),
        ]

        BaddieCharacter.default_meshes = [
            self._get_mesh('baddie01.obj', 'cube1'),
            self._get_mesh('baddie02.obj', 'cube1'),
            self._get_mesh('baddie03.obj', 'cube1'),
            self._get_mesh('baddie04.obj', 'cube1'),
        ]

        BaddieCharacter.pushed_meshes = [
            self._get_mesh('baddie05.obj', 'cube1'),
            self._get_mesh('baddie08.obj', 'cube1'),
            self._get_mesh('baddie09.obj', 'cube1'),
        ]

        BaddieCharacter.attack_meshes = [
            self._get_mesh('baddie10.obj', 'cube1'),
            self._get_mesh('baddie11.obj', 'cube1'),
            self._get_mesh('baddie12.obj', 'cube1'),
        ]

        self.crosshair = self._get_texture('crosshair.png')
        self.player = PlayerCharacter(self.player_start, 0, 0)
        self.fallout_height = -100.0

        self.skybox_atmo = self._get_skybox('comawhite')
        self.skybox_space = self._get_skybox('starfield')

        self.queued_state = None

        self.emitters = []
        self.cut_scene_emitters = []

        self.game_text = None
        self.game_text_timeout = 0.

        self.dying = False
        self.dead = False
        self.fade_to_black = 0
        self.fade_to_damage = 0

    def debug(self, msg):
        aegame.log("MISC", msg)

    @profile("level.py", "Level.restart")
    def restart(self):
        self.debug('restart')
        self.stage = 1
        self.has_exit = False
        self.retry_stage()

    @profile("level.py", "Level.retry_stage")
    def retry_stage(self):
        self.dying = False
        self.dead = False
        self.fade_to_black = 0.
        self.weapon_timeout = 0.
        self.weapon_charge = 5.
        self.update_weapon_materials()

        self.player.collision.c = self.player_start.copy()
        self.player.heading = 0
        self.player.pitch = 0
        self.player.velocity = Vec3()
        self.set_health(self.player.max_health)

        self.set_stage(self.stage)

    @profile("level.py", "Level.advance_stage")
    def advance_stage(self):
        if self.stage == 1:
            self.set_stage(2)
            self.add_rockets()
            self.queued_state = cutscene.CutScene1(self)
        elif self.stage == 2:
            self.set_stage(3)
            self.queued_state = cutscene.CutScene2(self)
            self.emitters = [] # remove rockets (not from cutscene)
        elif self.stage == 3:
            self.add_shield()
            self.queued_state = cutscene.CutScene3(self)

    @profile("level.py", "Level.set_stage")
    def set_stage(self, stage):
        self.debug('set stage %d' % stage)
        self.stage = stage
        if stage == 1:
            self.gravity = Vec3(0, -30, 0)
            self.skybox = self.skybox_atmo

            self.baddies = []
            self.max_baddies = 5
            self.spawn_points = self.spawn_points_1
            self.spawn_period = 5.
            self.spawn_timer = 0

            self.rocket1_power = 0.
            self.rocket2_power = 0.

            self.game_text = 'Restore power to the two rockets'
            self.game_text_timeout = 3.

        elif stage == 2:
            self.gravity = Vec3(0, -4, 0)
            self.skybox = self.skybox_space

            self.baddies = []
            self.max_baddies = 10
            self.spawn_points = self.spawn_points_2
            self.spawn_period = 3.
            self.spawn_timer = 0

            self.node1_power = 0.
            self.node2_power = 0.
            self.node3_power = 0.
            self.node4_power = 0.
            self.orbit_power = 0.

            self.game_text = 'Activate the orbital control system'
            self.game_text_timeout = 3.

        elif stage == 3:
            self.gravity = Vec3(0, -1, 0)
            self.skybox = self.skybox_space

            self.baddies = []
            self.max_baddies = 10
            self.spawn_points = self.spawn_points_3
            self.spawn_period = 1.5
            self.spawn_timer = 0

            self.final1_power = 0.
            self.final2_power = 0.

            self.game_text = 'Raise the two shield arms'
            self.game_text_timeout = 3.

        # Set panel textures

        if stage < 2:
            self._replace_texture('station.obj', 'rocket1_panel',
                'rocket_offline.png')
            self._replace_texture('station.obj', 'rocket2_panel',
                'rocket_offline.png')
        else:
            self._replace_texture('station.obj', 'rocket1_panel',
                'rocket_online.png')
            self._replace_texture('station.obj', 'rocket2_panel',
                'rocket_online.png')

        if stage < 3:
            nodes_active = 'node_offline.png'
            self._replace_texture('station.obj', 'orbit_panel',
                'orbit_inactive.png')
        else:
            nodes_active = 'node_online.png'
            self._replace_texture('station.obj', 'orbit_panel',
                'orbit_active.png')
        for panel in ('node1_panel', 'node2_panel', 'node3_panel',
            'node4_panel'):
            self._replace_texture('station.obj', panel, nodes_active)

        self._replace_texture('station.obj', 'final1_panel', 'shield_down.png')
        self._replace_texture('station.obj', 'final2_panel', 'shield_down.png')

    @profile("level.py", "Level.spawn_baddie")
    def spawn_baddie(self):
        spawn_point = self.spawn_points[0]
        baddie = BaddieCharacter(spawn_point, 0, 0)
        baddie.animate_meshes = baddie.default_meshes

        self.baddies.append(baddie)
        self.debug('spawn baddie at %r' % spawn_point)

        # Rotate spawn points
        self.spawn_points.insert(0, self.spawn_points.pop())

    @profile("level.py", "Level.on_trigger")
    def on_trigger(self, dt, name):
        if self.dying:
            return

        if self.stage == 1:
            power_triggers = {
                'rocket1': ('rocket1_power', 'Restoring power to rocket'),
                'rocket2': ('rocket2_power', 'Restoring power to rocket'),
            }
        elif self.stage == 2:
            power_triggers = {
                'node1': ('node1_power', 'Bringing power node 1 online'),
                'node2': ('node2_power', 'Bringing power node 2 online'),
                'node3': ('node3_power', 'Bringing power node 3 online'),
                'node4': ('node4_power', 'Bringing power node 4 online'),
            }
            if self.node1_power == 1. and \
               self.node2_power == 1. and \
               self.node3_power == 1. and \
               self.node4_power == 1.:
                power_triggers['orbit'] = \
                    ('orbit_power', 'Activating orbital control')
        elif self.stage == 3:
            power_triggers = {
                'final1': ('final1_power', 'Raising shield arm'),
                'final2': ('final2_power', 'Raising shield arm'),
            }

        if name == 'orbit' and not (
                self.node1_power == 1. and
                self.node2_power == 1. and
                self.node3_power == 1. and
                self.node4_power == 1.):
            self.game_text = 'Bring the four power nodes online first.'
            self.game_text_timeout = 3.
        elif name in power_triggers:
            attr, message = power_triggers[name]
            power =  getattr(self, attr)
            if power < 1.:
                power = min(1., power + dt / TRIGGER_ACTIVATE_TIME)
                setattr(self, attr, power)
                self.debug('%s = %f' % (power_triggers[name], power))
                self.game_text = '%s... %d%%' % (message, power * 100)
                self.game_text_timeout = .5
                if power == 1.:
                    self.debug('activate %s' % name)
                    self.goal_check()

        elif name == 'recharge':
            self.weapon_charge = min(MAX_WEAPON_CHARGE, self.weapon_charge + dt)
            self.game_text = 'Recharging displacement device... %d%%' % \
                (self.weapon_charge / MAX_WEAPON_CHARGE * 100)
            self.game_text_timeout = .5
            if self.weapon_charge == MAX_WEAPON_CHARGE:
                self.game_text = 'Charged'
            self.update_weapon_materials()

    @profile("level.py", "Level.goal_check")
    def goal_check(self):
        if self.stage == 1:
            if self.rocket1_power == 1.:
                self._replace_texture(
                    'station.obj', 'rocket1_panel', 'rocket_online.png')
            if self.rocket2_power == 1.:
                self._replace_texture(
                    'station.obj', 'rocket2_panel', 'rocket_online.png')

            if self.rocket1_power == 1. and \
               self.rocket2_power == 1.:
                self.debug('completed stage 1')
                self.advance_stage()
        elif self.stage == 2:
            for power, panel in [
                (self.node1_power, 'node1_panel'),
                (self.node2_power, 'node2_panel'),
                (self.node3_power, 'node3_panel'),
                (self.node4_power, 'node4_panel')]:
                 if power == 1:
                     self._replace_texture('station.obj', panel,
                        'node_online.png')
            if self.node1_power == 1. and \
               self.node1_power == 1. and \
               self.node3_power == 1. and \
               self.node4_power == 1. and \
               self.orbit_power == 1.:
                self.debug('completed stage 2')
                self.advance_stage()
        elif self.stage == 3:
            if self.final1_power == 1.:
                self._replace_texture(
                    'station.obj', 'final1_panel', 'shield_up.png')
            if self.final2_power == 1.:
                self._replace_texture(
                    'station.obj', 'final2_panel', 'shield_up.png')
            if self.final1_power == 1. and \
               self.final2_power == 1:
                self.debug('completed stage 3')
                self.advance_stage()

    @profile("level.py", "Level.on_fire")
    def on_fire(self):
        if self.weapon_timeout == 0. and \
           self.weapon_charge > 0 and \
           not self.dying:
            self.weapon_timeout = WEAPON_PERIOD
            self.weapon_charge -= 1
            self.update_weapon_materials()
            return True

    @profile("level.py", "Level.on_hit_baddie")
    def on_hit_baddie(self, baddie, ray):
        push_velocity = ray.v.normalized().mul_flt(baddie.push_speed)
        baddie.velocity.iadd_vec(push_velocity)
        baddie.pushed = True
        baddie.fade_to_death = 1.0

        self.debug('%r was hit' % baddie)

    @profile("level.py", "Level.on_collide_baddie")
    def on_collide_baddie(self, baddie):
        if baddie.active:
            self.fade_to_damage = 1.
            self.set_health(self.player.health - baddie.strength)
            self.debug('player bumped into baddie: health is now %f' % self.player.health)
            baddie.active = False
        else:
            self.debug('player bumped into inactive baddie')

    @profile("level.py", "Level.on_character_fallout")
    def on_character_fallout(self, character):
        if character is self.player:
            self.debug('player fallout')
            self.on_player_die()
        else:
            self.debug('%r fell out' % character)
            self.baddies.remove(character)
            self.spawn_baddie()

    @profile("level.py", "Level.set_health")
    def set_health(self, health):
        self.player.health = min(health, self.player.max_health)
        self.debug('player health now %f' % health)

        if self.player.health <= 0.:
            self.on_player_die()

    @profile("level.py", "Level.on_player_die")
    def on_player_die(self):
        self.debug('player died')
        self.dying = True

    @profile("level.py", "Level.update")
    def update(self, dt):
        if self.dying:
            self.fade_to_black += dt
            if self.fade_to_black >= 1.:
                self.dead = True
            return

        if self.fade_to_damage:
            self.fade_to_damage -= dt * 3
            if self.fade_to_damage < 0:
                self.fade_to_damage = 0

        # The player's health is restored 1 point every 5 secs
        if self.player.health < self.player.max_health:
            self.set_health(self.player.health + dt / 5)

        self.weapon_timeout = max(0., self.weapon_timeout - dt)

        self.baddies = [b for b in self.baddies if b.active]

        # Baddie AI
        for baddie in self.baddies:
            baddie.time += dt

            # Turn to face the player. Dot product of player_dir with plane
            # created by baddie facing dir gives sign of direction to turn.
            player_dir = self.player.collision.c.sub_vec(baddie.collision.c)
            facing_dir = baddie.get_facing_ray().v
            n = facing_dir.cross(Vec3(0, 1, 0))
            d = n.dot(player_dir)
            baddie.movement['turn_left'] = d > -0.1  # hysterisis
            baddie.movement['turn_right'] = d < 0.1

            close = player_dir.magnitude_squared() < 15 ** 2
            very_close = player_dir.magnitude_squared() < 2 ** 2

            player_dir.normalize()
            facing_dir.normalize()

            # Move forward if the player less than 90 degrees away from me
            facing = player_dir.dot(facing_dir)
            baddie.movement['forward'] = facing > 0

            # Match player's left/right movement if facing them and close
            if facing > .5 and close:
                baddie.movement['left'] =  self.player.movement['right']
                baddie.movement['right'] =  self.player.movement['left']

            # If player is much higher, jump
            higher = self.player.collision.c.y > baddie.collision.c.y + 1.
            baddie.movement['jump'] = higher and close and facing > .5

            # Play attack animation
            baddie.attacking = facing > .5 and very_close

        # Spawn another baddie?
        self.spawn_timer += dt
        if len(self.baddies) < self.max_baddies and \
           self.spawn_timer > self.spawn_period:
            self.spawn_baddie()
            self.spawn_timer = 0.

        # Hide game text?
        self.game_text_timeout -= dt
        if self.game_text_timeout <= 0:
            self.game_text = 0.

    @profile("level.py", "Level.update_weapon_materials")
    def update_weapon_materials(self):
        lights = min(max(0, int(self.weapon_charge)), 5)
        for material in self.weapon_materials[:lights]:
            material.diffuse = Vec3(0, .5, 0)
        for material in self.weapon_materials[lights:]:
            material.diffuse = Vec3(0, 0, 0)

        if lights == 1:
            self.weapon_materials[0].diffuse = Vec3(0.5, 0, 0)

    @profile("level.py", "Level.add_rockets")
    def add_rockets(self):
        def add_rocket(p):
            emitter = game.Emitter(
                name="level_smoke",
                position=p,
                position_sigma=Vec3(1, 1, 1),
                velocity=Vec3(0, -1, 0),
                velocity_sigma=Vec3(1, .15, 1),
                texture=self._get_texture('smoke.png'),
                max_point_size=64,
                min_point_size=32,
                period=.001,
                max_particles=200)
            self.emitters.append(emitter)

            emitter = game.Emitter(
                name="cutscene_smoke",
                position=p.add_vec(Vec3(0, -10, 0)),
                position_sigma=Vec3(),
                velocity=Vec3(0, -50, 0),
                velocity_sigma=Vec3(5, 1.5, 5),
                texture=self._get_texture('smoke.png'),
                max_point_size=16,
                min_point_size=8,
                period=.05,
                max_particles=1000)
            self.cut_scene_emitters.append(emitter)

            emitter = game.Emitter(
                name="cutscene_fire",
                position=p,
                position_sigma=Vec3(1, 0, 1),
                velocity=Vec3(0, -10, 0),
                velocity_sigma=Vec3(0.5, 1, 0.5),
                texture=self._get_texture('fire.png'),
                max_point_size=16,
                min_point_size=4,
                period=.01,
                max_particles=128)
            self.cut_scene_emitters.append(emitter)

        add_rocket(self._get_point('station.obj', 'rocket_emitter1'))
        add_rocket(self._get_point('station.obj', 'rocket_emitter2'))

    @profile("level.py", "Level.add_shield")
    def add_shield(self):
        for p in (self._get_point('station.obj', 'shield_emitter1'),
                  self._get_point('station.obj', 'shield_emitter2')):
            emitter = game.Emitter(
                name="cutscene_shield",
                position=p,
                position_sigma = Vec3(),
                velocity=Vec3(0, -50, 0),
                velocity_sigma=Vec3(20, 5, 20),
                texture=self._get_texture('shield.png'),
                max_point_size=16,
                min_point_size=8,
                period=0.01,
                max_particles=500)
            self.cut_scene_emitters.append(emitter)

    @profile("level.py", "Level._get_mesh")
    def _get_mesh(self, obj_name, mesh_name):
        if obj_name not in self.objs:
            self.objs[obj_name] = obj.OBJ(obj_name)

        return self.objs[obj_name].meshes[mesh_name]

    @profile("level.py", "Level._get_material")
    def _get_material(self, obj_name, material_name):
        # assume obj is already loaded
        obj = self.objs[obj_name]
        return obj.materials[material_name]

    @profile("level.py", "Level._get_point")
    def _get_point(self, obj_name, mesh_name):
        mesh = self._get_mesh(obj_name, mesh_name)
        return mesh.polygons[0].get_point()

    @profile("level.py", "Level._get_trigger")
    def _get_trigger(self, obj_name, mesh_name):
        mesh = self._get_mesh(obj_name, mesh_name)

        for polygon in mesh.polygons:
            # prefer up-facing polygon
            if polygon.get_face_normal().y > 0: return polygon

        return mesh.polygons[0]

    @classmethod
    @profile("level.py", "Level._get_texture")
    def _get_texture(cls, filename):
        if filename not in cls.textures:
            cls.textures[filename] = obj.load_texture(filename)

        return cls.textures[filename]

    @profile("level.py", "Level._replace_texture")
    def _replace_texture(self, obj, material, filename):
        self._get_material(obj, material).texture = self._get_texture(filename)

    @classmethod
    @profile("level.py", "Level._get_skybox")
    def _get_skybox(cls, name):
        return [
            cls._get_texture('skybox/%s/%s_ft.jpg' % (name, name)),
            cls._get_texture('skybox/%s/%s_bk.jpg' % (name, name)),
            cls._get_texture('skybox/%s/%s_lf.jpg' % (name, name)),
            cls._get_texture('skybox/%s/%s_rt.jpg' % (name, name)),
            cls._get_texture('skybox/%s/%s_up.jpg' % (name, name)),
            cls._get_texture('skybox/%s/%s_dn.jpg' % (name, name)),
        ]
