/*********************************************************/
#include "particle.hpp"
#include <gtest/gtest.h>

/*********************************************************/
using namespace experiment;

/*********************************************************/
TEST(TestParticle, constructor)
{
	Particle particle(1.0, 1.5);
	EXPECT_EQ(1.0, particle.get_x());
	EXPECT_EQ(1.5, particle.get_vx());
}

/*********************************************************/
TEST(TestParticle, move)
{
	Particle particle(0, 1);
	particle.move(0.5);
	EXPECT_EQ(0.5, particle.get_x());
	EXPECT_EQ(1, particle.get_vx());
}

