/*********************************************************/
#include "physics.hpp"
#include <gtest/gtest.h>

/*********************************************************/
using namespace experiment;

/*********************************************************/
TEST(TestPhysics, collide)
{
	// 1 - build two particles going to collide
	// 2 - test Physics.collide() with a dt too short (no collision)
	// 3 - test Physics.collide() with a dt just OK
	// 4 - test Physics.collide() with a dt larger than OK
	ASSERT_TRUE(false) << "Not implemented";
}

/*********************************************************/
TEST(TestPhysics, get_collision_time_1_symetric)
{
	// 1 - build two particules in a symmetric configuration
	// 2 - check get_collision_time() time
	ASSERT_TRUE(false) << "Not implemented";
}

/*********************************************************/
TEST(TestPhysics, get_collision_time_2_asymetric)
{
	// 1 - build two particules in an asymetric configuratoin
	// 2 - check get_collision_time() time
	ASSERT_TRUE(false) << "Not implemented";
}

/*********************************************************/
TEST(TestPhysics, test_elastic_collision_1_symetric)
{
	// 1 - build two particules in a symmetric configuration
	// 2 - call elastic_collision() with a valid dt
	// 3 - check status of the function
	// 4 - check new properties of both particules
	ASSERT_TRUE(false) << "Not implemented";
}
