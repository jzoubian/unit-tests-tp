class Particle:
	def __init__(self, pos_x, vx):
		self.mass = 1.0
		self.pos_x = pos_x
		self.vx = vx
	
	def get_x(self):
		return self.pos_x
	
	def get_vx(self):
		return self.vx

	def move(self, dt):
		self.pos_x += dt * self.vx

	def set_vx(self, vx):
		self.vx = vx

	def get_mass(self):
		return self.mass