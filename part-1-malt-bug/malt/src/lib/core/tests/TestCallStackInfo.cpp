/*****************************************************
             PROJECT  : MALT
             VERSION  : 1.2.2
             DATE     : 10/2020
             AUTHOR   : Valat Sébastien
             LICENSE  : CeCILL-C
*****************************************************/

/********************  HEADERS  *********************/
#include <gtest/gtest.h>
#include <core/CallStackInfo.hpp>

/***************** USING NAMESPACE ******************/
using namespace MALT;

/*******************  FUNCTION  *********************/
TEST(SimpleQuantityHistory,constructor)
{
	//build
	SimpleQuantityHistory history;

	//check
	EXPECT_EQ(0, history.count);
	EXPECT_EQ(0, history.min);
	EXPECT_EQ(0, history.max);
	EXPECT_EQ(0, history.sum);
}

/*******************  FUNCTION  *********************/
TEST(SimpleQuantityHistory,addEvent)
{
	//build
	SimpleQuantityHistory history;

	//call
	history.addEvent(32);
	history.addEvent(32);
	history.addEvent(64);

	//check
	EXPECT_EQ(3, history.count);
	EXPECT_EQ(32, history.min);
	EXPECT_EQ(64, history.max);
	EXPECT_EQ(128, history.sum);
}

/*******************  FUNCTION  *********************/
TEST(SimpleQuantityHistory,push)
{
	//build
	SimpleQuantityHistory history;
	SimpleQuantityHistory subHistory;

	//call
	subHistory.addEvent(32);
	subHistory.addEvent(32);
	subHistory.addEvent(64);

	//merge two times
	history.push(subHistory);
	history.push(subHistory);

	//check
	EXPECT_EQ(6, history.count);
	EXPECT_EQ(32, history.min);
	EXPECT_EQ(64, history.max);
	EXPECT_EQ(256, history.sum);
}
