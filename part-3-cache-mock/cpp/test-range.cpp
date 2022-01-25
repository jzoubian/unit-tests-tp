/*********************************************************/
#include "range.hpp"
#include <gtest/gtest.h>

/*********************************************************/
using namespace cache;

/*********************************************************/
TEST(TestRange, constructor)
{
	Range range(10, 20);
	EXPECT_EQ(10, range.get_offset());
	EXPECT_EQ(20, range.get_size());
}

/*********************************************************/
TEST(TestRange, end)
{
	Range range(10, 20);
	EXPECT_EQ(30, range.end());
}

/*********************************************************/
TEST(TestRange, overlap)
{
	Range r1(0, 10);
	Range r2(10, 20);
	Range r3(5, 20);
	Range r4(30, 20);
	EXPECT_FALSE(Range::overlap(r1, r2));
	EXPECT_TRUE(Range::overlap(r1, r3));
	EXPECT_TRUE(Range::overlap(r2, r3));
	EXPECT_FALSE(Range::overlap(r2, r4));
	EXPECT_FALSE(Range::overlap(r3, r4));
	EXPECT_FALSE(Range::overlap(r3, r4));
}

/*********************************************************/
TEST(TestRange, extrude_middle)
{
	auto res = Range::extrude(Range(0, 20), Range(5, 5));
	EXPECT_EQ(Range(0, 5), res.first);
	EXPECT_EQ(Range(10, 10), res.second);
}

/*********************************************************/
TEST(TestRange, extrude_left)
{
	auto res = Range::extrude(Range(0, 20), Range(0, 5));
	EXPECT_EQ(Range(0, 0), res.first);
	EXPECT_EQ(Range(5, 15), res.second);
}

/*********************************************************/
TEST(TestRange, extrude_right)
{
	auto res = Range::extrude(Range(0, 20), Range(15, 5));
	EXPECT_EQ(Range(0, 15), res.first);
	EXPECT_EQ(Range(20, 0), res.second);
}

/*********************************************************/
TEST(TestRange, intersect_left)
{
	Range res = Range::intersect(Range(0,15), Range(10, 10));
	EXPECT_EQ(Range(10, 5), res);
}

/*********************************************************/
TEST(TestRange, intersect_middle)
{
	Range res = Range::intersect(Range(12,5), Range(10, 10));
	EXPECT_EQ(Range(12, 5), res);
}

/*********************************************************/
TEST(TestRange, intersect_right)
{
	Range res = Range::intersect(Range(15,20), Range(10, 10));
	EXPECT_EQ(Range(15, 5), res);
}
