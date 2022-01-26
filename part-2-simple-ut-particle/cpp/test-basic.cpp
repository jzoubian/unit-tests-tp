/*********************************************************/
#include "particle.hpp"
#include <gtest/gtest.h>

/*********************************************************/
using namespace experiment;

/*********************************************************/
TEST(TestBasic, basic)
{
	int value = 10;
	ASSERT_EQ(10, value);
	//or
	EXPECT_EQ(10, value);
}