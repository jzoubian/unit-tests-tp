/*********************************************************/
#include "cache.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

/*********************************************************/
using namespace cache;
using namespace testing;

/*********************************************************/
class ToBeMocked
{
	public:
		virtual int run(int value) = 0;
};

/*********************************************************/
class MockObj : public ToBeMocked
{
	public:
		MOCK_METHOD(int, run, (int), (override));
};

/*********************************************************/
TEST(TestCache, run)
{
	//build the mock
	MockObj obj;

	//register expectation & assign return value
	EXPECT_CALL(obj, run(10)).WillOnce(Return(6));

	//call
	int res = obj.run(10);

	//check
	EXPECT_EQ(6, res);
}
