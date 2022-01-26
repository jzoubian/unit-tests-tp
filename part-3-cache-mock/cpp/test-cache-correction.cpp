/*********************************************************/
#include "cache.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

/*********************************************************/
using namespace cache;
using namespace testing;

/*********************************************************/
class ManualMockedStorageBackend : public StrorageBackend
{
	public:
		ManualMockedStorageBackend(void) : buffer("") {};
		virtual size_t pwrite(const Buffer & value, size_t offset) override {this->buffer = value; return this->buffer.size;};
		virtual Buffer pread(size_t size, size_t offset) override {return this->buffer;};
	public:
		Buffer buffer;
};

/*********************************************************/
class MockedStorageBackend : public StrorageBackend
{
	public:
		MOCK_METHOD(size_t,pwrite,(const Buffer &, size_t), (override));
		MOCK_METHOD(Buffer,pread,(size_t, size_t), (override));
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
	ManualMockedStorageBackend backend;
	Cache cache(&backend);

	backend.buffer = Buffer("test");
	Buffer res = cache.pread(4, 10);
	ASSERT_EQ(res, Buffer("test"));
}

/*********************************************************/
TEST(TestCacheManualMock, pwrite)
{
	ManualMockedStorageBackend backend;
	Cache cache(&backend);

	backend.buffer = Buffer("----");
	size_t res = cache.pwrite(Buffer("test"), 10);
	ASSERT_EQ(res, 4);
	ASSERT_EQ(backend.buffer, Buffer("----"));
	cache.flush();
	ASSERT_EQ(backend.buffer, Buffer("test"));
}

/*********************************************************/
TEST(TestCache, constructor)
{
	MockedStorageBackend backend;
	Cache cache(&backend);
}

/*********************************************************/
TEST(TestCache, pread)
{
	MockedStorageBackend backend;
	Cache cache(&backend);
	
	EXPECT_CALL(backend, pread(10, 10))
		.WillOnce(Return(Buffer("0123456789")));
	Buffer res = cache.pread(10, 10);
	EXPECT_EQ(0, strncmp(res.data, "0123456789", 10));

	EXPECT_CALL(backend, pread(20, 10))
		.WillOnce(Return(Buffer("0123456789")));
	Buffer res2 = cache.pread(10, 20);
	EXPECT_EQ(0, strncmp(res2.data, "01234567890123456789", 10));
}

/*********************************************************/
TEST(TestCache, pwrite_one)
{
	MockedStorageBackend backend;
	Cache cache(&backend);
	
	Buffer res = cache.pwrite(Buffer("01234"), 0);

	EXPECT_CALL(backend, pwrite(Buffer("01234"), 0))
		.WillOnce(Return(5));
	cache.flush();
}

/*********************************************************/
TEST(TestCache, pwrite_two)
{
	MockedStorageBackend backend;
	Cache cache(&backend);
	
	size_t res = cache.pwrite(Buffer("01234"), 0);
	ASSERT_EQ(res, 5);
	res = cache.pwrite(Buffer("56789"), 5);
	ASSERT_EQ(res, 5);

	EXPECT_CALL(backend, pwrite(Buffer("01234"), 0))
		.WillOnce(Return(5));
	EXPECT_CALL(backend, pwrite(Buffer("56789"), 5))
		.WillOnce(Return(5));
	cache.flush();
}

/*********************************************************/
TEST(TestCache, pwrite_flush_pwrite_flush_dirty)
{
	MockedStorageBackend backend;
	Cache cache(&backend);
	
	size_t res = cache.pwrite(Buffer("01234"), 0);
	ASSERT_EQ(res, 5);
	res = cache.pwrite(Buffer("56789"), 5);
	ASSERT_EQ(res, 5);

	EXPECT_CALL(backend, pwrite(Buffer("01234"), 0))
		.WillOnce(Return(5));
	EXPECT_CALL(backend, pwrite(Buffer("56789"), 5))
		.WillOnce(Return(5));
	cache.flush();

	res = cache.pwrite(Buffer("abcde"), 5);
	ASSERT_EQ(res, 5);
	EXPECT_CALL(backend, pwrite(Buffer("abcde"), 5))
		.WillOnce(Return(5));
	cache.flush();
}
