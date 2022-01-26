from unittest import TestCase
from particle import Particle
import physics

class TestPysiscs(TestCase):
	def test_collide(self):
		particle1 = Particle(-1.0, 1.0)
		particle2 = Particle(1.0, -1.0)
		self.assertFalse(physics.collide(particle1, particle2, 0.5))
		self.assertFalse(physics.collide(particle1, particle2, 0.9))
		self.assertTrue(physics.collide(particle1, particle2, 1.0))
		self.assertTrue(physics.collide(particle1, particle2, 2.0))
	
	def test_get_collision_time_1_symetric(self):
		particle1 = Particle(-1.0, 1.0)
		particle2 = Particle(1.0, -1.0)
		self.assertEqual(1.0, physics.get_collision_time(particle1, particle2))

	def test_get_collision_time_2_asymetric(self):
		particle1 = Particle(-0.5, 1.0)
		particle2 = Particle(1.0, -1.0)
		self.assertEqual(0.75, physics.get_collision_time(particle1, particle2))

	def test_elastic_collision_1_symetric(self):
		particle1 = Particle(-1.0, 1.0)
		particle2 = Particle(1.0, -1.0)
		ret = physics.elastic_collision(particle1, particle2, 2)
		self.assertTrue(ret)
		self.assertEqual(-1.0, particle1.get_x())
		self.assertEqual(-1.0, particle1.get_vx())
		self.assertEqual(1.0, particle2.get_x())
		self.assertEqual(1.0, particle2.get_vx())