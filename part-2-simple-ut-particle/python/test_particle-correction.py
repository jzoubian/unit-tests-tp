from unittest import TestCase
from particle import Particle

class TestParticle(TestCase):
	def test_constructor(self):
		particle = Particle(0, 1)
		self.assertEqual(0, particle.get_x())
		self.assertEqual(1, particle.get_vx())

	def test_move(self):
		particle = Particle(0, 1)
		particle.move(0.5)
		self.assertEqual(0.5, particle.get_x())
		self.assertEqual(1, particle.get_vx())
