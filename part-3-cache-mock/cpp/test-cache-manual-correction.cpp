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
		ManualMockedStorageBackend(void) : buffer("") {};
		virtual size_t pwrite(const Buffer & value, size_t offset) override {
			this->buffer = value; return this->buffer.size;
		};
		virtual Buffer pread(size_t size, size_t offset) override {
			return this->buffer;
		};
	public:
		Buffer buffer;
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
	ManualMockedStorageBackend backend;
	Cache cache(&backend);

	backend.buffer = Buffer("test");
	Buffer res = cache.pread(4, 10);
	ASSERT_EQ(res, Buffer("test"));
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
	ManualMockedStorageBackend backend;
	Cache cache(&backend);

	backend.buffer = Buffer("----");
	size_t res = cache.pwrite(Buffer("test"), 10);
	ASSERT_EQ(res, 4);
	ASSERT_EQ(backend.buffer, Buffer("----"));
	cache.flush();
	ASSERT_EQ(backend.buffer, Buffer("test"));
}
