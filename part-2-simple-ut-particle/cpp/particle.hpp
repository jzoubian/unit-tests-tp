/*********************************************************/
#ifndef EXPERIMENT_PARTICLE_HPP
#define EXPERIMENT_PARTICLE_HPP

/*********************************************************/
namespace experiment
{

/*********************************************************/
class Particle
{
	public:
		Particle(double pos_x, double vx);
		double get_x(void) const;
		double get_vx(void) const;
		double get_mass(void) const;
		void set_vx(double vx);
		void move(double td);
	private:
		double pos_x;
		double vx;
		double mass;
};

}

#endif //EXPERIMENT_PARTICLE_HPP
