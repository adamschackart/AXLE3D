from aegame import Vec3
import level
import random

SAMPLE_PERIOD = 0.5
MAX_SAMPLES = 5

class Survival(level.Level):
    def __init__(self, stage):
        super(Survival, self).__init__()
        self.survival_stage = stage

    def restart(self):
        self.stage = self.survival_stage
        self.has_exit = False
        self.retry_stage()

        self.sample_timeout = SAMPLE_PERIOD + 2.0
        self.samples = []

        self.max_baddies = 20
        self.spawn_period = 3.0

    def retry_stage(self):
        super(Survival, self).retry_stage()
        self.survival_points = 0

    def update(self, dt):
        super(Survival, self).update(dt)

        self.sample_timeout -= dt
        if self.sample_timeout <= 0:
            self.samples.append(self.player.collision.c.copy())
            self.sample_timeout = SAMPLE_PERIOD
            if len(self.samples) > MAX_SAMPLES:
                self.samples.pop(0)

        self.survival_points += dt
        self.game_text = 'Score: %d' % self.survival_points

    def on_trigger(self, dt, name):
        if name == 'recharge':
            super(Survival, self).on_trigger(dt, name)

    def spawn_baddie(self):
        spawn_point = random.choice(self.samples[:3]).add_vec(Vec3(0, 0.5, 0))
        heading = random.random() * 360.0
        baddie = level.BaddieCharacter(spawn_point, heading, 0)
        baddie.animate_meshes = baddie.default_meshes

        self.baddies.append(baddie)
        self.debug('spawn baddie at %r' % spawn_point)
