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
		//TODO: apply mocking on pread/pwrite
		//virtual size_t pwrite(const Buffer & value, size_t offset) override
		//virtual Buffer pread(size_t size, size_t offset) override
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
TEST(TestCache, pread_one)
{ 
	// 1 - build a storage backend
	// 2 - mock its pread call with a return bytearray as value;
	//     fill it with a string eg. b"Hello"
	// 3 - build a cache
	// 4 - call pread on with offset=0 and size=5 (to match the example string) 
	// 5 - check the result (it should contain your example value)
	// 6 - check the mock has been called (look on assert_called_once_with())
	MockedStorageBackend backend;
	Cache cache(&backend);
	
	EXPECT_CALL(backend, pread(0, 5))
		.WillOnce(Return(Buffer("Hello")));
	Buffer res = cache.pread(5, 0);
	EXPECT_EQ(res, Buffer("Hello"));
}

/*********************************************************/
TEST(TestCache, pread_second_call)
{ 
	// 1 - similar to the previous test
	// 2 - just add a second call to pread before checking
	//     the mock. The cache should have the value and
	//     to not emit a request to the backend.
	//     It can be checked with assert_called_once_with()
	MockedStorageBackend backend;
	Cache cache(&backend);
	
	//first call
	EXPECT_CALL(backend, pread(0, 5))
		.WillOnce(Return(Buffer("Hello")));
	Buffer res = cache.pread(5, 0);
	EXPECT_EQ(res, Buffer("Hello"));

	//second call
	Buffer res2 = cache.pread(5, 0);
	EXPECT_EQ(res2, Buffer("Hello"));
}

/*********************************************************/
TEST(TestCache, pread_advanced_contiguous)
{
	// 1 - build a storage backend
	// 2 - mock its pread call with a return bytearray as value;
	//     fill it with a string eg. b"Hello"
	// 3 - build a cache
	// 4 - call pread on with offset=0 and size=5 (to match the example string) 
	// 5 - check the result (it should contain your example value)
	// 6 - check the mock has been called (look on assert_called_once_with())
	MockedStorageBackend backend;
	Cache cache(&backend);
	
	EXPECT_CALL(backend, pread(0, 5))
		.WillOnce(Return(Buffer("Hello")));
	Buffer res = cache.pread(5, 0);
	EXPECT_EQ(res, Buffer("Hello"));

	EXPECT_CALL(backend, pread(5, 5))
		.WillOnce(Return(Buffer("World")));
	Buffer res2 = cache.pread(10, 0);
	EXPECT_EQ(res2, Buffer("HelloWorld"));
}

/*********************************************************/
TEST(TestCache, pwrite_one)
{
	// 1 - create a backend
	// 2 - mock it with return value 5
	// 3 - call pwrite on cache with b"Hello" and offset 0
	// 4 - check backend has not been called
	// 5 - call flush
	// 6 - check backend has been called
	MockedStorageBackend backend;
	Cache cache(&backend);
	
	Buffer res = cache.pwrite("Hello", 0);

	EXPECT_CALL(backend, pwrite(Buffer("Hello"), 0))
		.WillOnce(Return(5));
	cache.flush();
}

/*********************************************************/
TEST(TestCache, pwrite_modify_after_flush)
{
	// 1 - create a backend
	// 2 - mock it with return value 5
	// 3 - call pwrite on cache with b"Hello" and offset 0
	// 4 - check backend has not been called
	// 5 - call flush
	// 6 - check backend has been called
	MockedStorageBackend backend;
	Cache cache(&backend);
	
	size_t res = cache.pwrite("Hello", 0);
	ASSERT_EQ(5, res);

	EXPECT_CALL(backend, pwrite(Buffer("Hello"), 0))
		.WillOnce(Return(5));
	cache.flush();

	size_t res2 = cache.pwrite("World", 0);
	ASSERT_EQ(5, res2);
	EXPECT_CALL(backend, pwrite(Buffer("World"), 0))
		.WillOnce(Return(5));
	cache.flush();
}

/*********************************************************/
TEST(TestCache, pwrite_advanced)
{
	// Read on 0->5
	// Read on 0->10
	// it should call backend pread once on 0->5
	// and anoder one on 5->5
	MockedStorageBackend backend;
	Cache cache(&backend);
	
	size_t res = cache.pwrite("01234", 0);
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
	//Make a first write
	//flush
	//write again to make dirty again
	//flush again
	//check with mock that we flushed two times
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
