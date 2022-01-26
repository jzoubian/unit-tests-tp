from unittest import TestCase
from particle import Particle

class TestBasic(TestCase):
	def test_basic(self):
		value = 10
		self.assertEqual(value, 10)
