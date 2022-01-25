/*********************************************************/
#include "physics.hpp"

/*********************************************************/
using namespace experiment;

/*********************************************************/
bool experiment::physics::collide(const Particle & particle1, const Particle & particle2, double dt)
{
	// calc distance before
	double delta_before = particle1.get_x() - particle2.get_x();
	// calc distance after dt
	double delta_after = (particle1.get_x() + particle1.get_vx() * dt) - (particle2.get_x() + particle2.get_vx() * dt);
	// special case, strict equal
	if (delta_before == 0 or delta_after == 0) {
		return true;
	} else {
		// as we use the sign in delta_* if one is positiv and the other is negative, the particles collided
		return delta_after / delta_before < 0;
	}
}

/*********************************************************/
double experiment::physics::get_collision_time(const Particle & particle1, const Particle & particle2)
{
	// 0 = x + vx * dt - x2 + vx2 * dt
	// x2 - x = vx * dt + vx2 * dt
	// (x2 - x)/dt = vx - vx2
	// 1/dt = (vx- vx2) / (x2-x)
	// dt = (x2 - x) / (vx1 - vx2)
	double dt = (particle2.get_x() - particle1.get_x()) / (particle1.get_vx() - particle2.get_vx());
	return dt;
}

/*********************************************************/
double experiment::physics::move_to_collision_point(Particle & particle1, Particle & particle2)
{
	// cal collision time
	double dt_collision = physics::get_collision_time(particle1, particle2);

	// move to particules to the collision point
	particle1.move(dt_collision);
	particle2.move(dt_collision);

	// return td
	return dt_collision;
}

/*********************************************************/
bool experiment::physics::elastic_collision(Particle & particle1, Particle & particle2, double dt)
{
	// check has collision of exit
	if (physics::collide(particle1, particle2, dt) == false)
		return false;

	// move to collision point
	double dt_collision = move_to_collision_point(particle1, particle2);

	// extract infos
	double u1 = particle1.get_vx();
	double m1 = particle1.get_mass();
	double u2 = particle2.get_vx();
	double m2 = particle2.get_mass();

	// apply elastic collision
	double v1 = (u1* (m1 - m2) + 2 * m2 * u2) / (m1 + m2);
	double v2 = (u2* (m2 - m1) + 2 * m1 * u1) / (m2 + m1);

	// apply back
	particle1.set_vx(v1);
	particle2.set_vx(v2);

	// move the rest of dt
	double dt_rest = dt - dt_collision;
	particle1.move(dt_rest);
	particle2.move(dt_rest);

	// return collided
	return true;

}
