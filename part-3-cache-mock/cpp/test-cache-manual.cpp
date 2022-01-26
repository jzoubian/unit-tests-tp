/*********************************************************/
#include "cache.hpp"
#include <gtest/gtest.h>

/*********************************************************/
using namespace cache;
using namespace testing;

/*********************************************************/
class ManualMockedStorageBackend : public StrorageBackend
{
	public:
		//TODO: implement simple mocked versions:
		//virtual size_t pwrite(const Buffer & value, size_t offset) override
		//virtual Buffer pread(size_t size, size_t offset) override
};

/*********************************************************/
TEST(TestCacheManualMock, constructor)
{
	ManualMockedStorageBackend backend;
	Cache cache(&backend);
}

/*********************************************************/
TEST(TestCacheManualMock, pread)
{
	// 1 - build backend & cache
	// 2 - call pread
	// 3 - check pread result
	// ADVANCED: store a buffer in the mock
	// to be able to set a value to return
}

/*********************************************************/
TEST(TestCacheManualMock, pwrite)
{
	// 1 - build backend & cache
	// 2 - call pread
	// 3 - check pread result
	// ADVANCED: store a buffer in the mock
	// to be able tocapture the write parameter and
	// post check it
}
