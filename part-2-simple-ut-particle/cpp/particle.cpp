/*********************************************************/
#include "particle.hpp"

/*********************************************************/
using namespace experiment;

/*********************************************************/
Particle::Particle(double pos_x, double vx)
         :pos_x(pos_x)
         ,vx(vx)
         ,mass(1.0)
{
}

/*********************************************************/
double Particle::get_x(void) const
{
	return this->pos_x;
}

/*********************************************************/
double Particle::get_vx(void) const
{
	return this->vx;
}

/*********************************************************/
double Particle::get_mass(void) const
{
	return this->mass;
}

/*********************************************************/
void Particle::set_vx(double vx)
{
	this->vx = vx;
}

/*********************************************************/
void Particle::move(double dt)
{
	this->pos_x += this->vx * dt;
}
