/*********************************************************/
#ifndef EXPERIMENT_PHYSICS_HPP
#define EXPERIMENT_PHYSICS_HPP

/*********************************************************/
#include "particle.hpp"

/*********************************************************/
namespace experiment
{

/*********************************************************/
namespace physics
{

/*********************************************************/
bool collide(const Particle & particle1, const Particle & particle2, double dt);
double get_collision_time(const Particle & particle1, const Particle & particle2);
double move_to_collision_point(Particle & particle1, Particle & particle2);
bool elastic_collision(Particle & particle1, Particle & particle2, double dt);

}

}

#endif //EXPERIMENT_PHYSICS_HPP
