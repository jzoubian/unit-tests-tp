/*********************************************************/
#include "cache.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

/*********************************************************/
using namespace cache;
using namespace testing;

/*********************************************************/
class MockedStorageBackend : public StrorageBackend
{
	public:
		MOCK_METHOD(size_t,pwrite,(const Buffer &, size_t), (override));
		MOCK_METHOD(Buffer,pread,(size_t, size_t), (override));
};

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

/*


	def test_pwrite_modify_after_flush(self):
		backend = StorageBackend()
		backend.pwrite = mock.MagicMock(return_value = 5)
		cache = Cache(backend)
		
		cache.pwrite(b"01234", 0)
		cache.pwrite(b"56789", 5)
		backend.pwrite.assert_not_called()

		cache.flush()
		
		backend.pwrite.reset_mock()
		cache.pwrite(b"abcde", 5)
		cache.flush()
		backend.pwrite.assert_called_once_with(b"abcde", 5)

		*/