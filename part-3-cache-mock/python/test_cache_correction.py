from turtle import back
from unittest import TestCase, mock
from cache import *

class ManualMockBackend(StorageBackend):
	# TODO: build a manual mock
	# send hard coded value. A bytearray for pread (eg. b"Hello")
	# A fixed size for pwrite (we will use "Hello" when writing)
	#
	# def pwrite(self, data: bytearray, offset) -> int:
	# def pread(self,offset, size) -> bytearray:
	def __init__(self):
		self.data = bytearray(0)
	def pwrite(self,data, offset) -> int:
		self.data = data
		return len(data)
	def pread(self,offset, size) -> int:
		return self.data

class ManualTestCache(TestCase):
	def test_init(self):
		backend = ManualMockBackend()
		cache = Cache(backend)

	def test_pread(self):
		# 1 - build a mocked backend
		# 2 - build a cache
		# 3 - pre-fill the data field in the manual mock
		# 4 - call pread
		# 5 - check if you obtained the same data than pushed to the backend
		backend = ManualMockBackend()
		cache = Cache(backend)

		backend.data = b"Hello"
		res = cache.pread(0, 5)
		self.assertEqual(res, b"Hello")

	def test_pwrite(self):
		# 1 - build a mocked backend
		# 2 - build a cache
		# 3 - pre fill the data field in the manual mock (same size)
		# 4 - call write with a different pattern
		# 5 - check that is has been updated
		backend = ManualMockBackend()
		cache = Cache(backend)

		backend.data = b"-----"
		res = cache.pwrite(b"Hello", 0)
		self.assertEqual(backend.data, b"-----")
		cache.flush()
		self.assertEqual(backend.data, b"Hello")

class TestCache(TestCase):
	def test_init_1(self):
		backend = StorageBackend()
		cache = Cache(backend)

	def test_pread_one(self):
		# 1 - build a storage backend
		# 2 - mock its pread call with a return bytearray as value;
		#     fill it with a string eg. b"Hello"
		# 3 - build a cache
		# 4 - call pread on with offset=0 and size=5 (to match the example string) 
		# 5 - check the result (it should contain your example value)
		# 6 - check the mock has been called (look on assert_called_once_with())
		backend = StorageBackend()
		backend.pread = mock.MagicMock(return_value = b"Hello")
		cache = Cache(backend)
		
		res = cache.pread(0,5)
		self.assertEqual(res, b"Hello")
		backend.pread.assert_called_once_with(0, 5)
	
	def test_pread_second_call(self):
		# 1 - similar to the previous test
		# 2 - just add a second call to pread before checking
		#     the mock. The cache should have the value and
		#     to not emit a request to the backend.
		#     It can be checked with assert_called_once_with()
		backend = StorageBackend()
		backend.pread = mock.MagicMock(return_value = b"Hello")
		cache = Cache(backend)
		
		res = cache.pread(0,5)
		self.assertEqual(res, b"Hello")
		res = cache.pread(0,5)
		self.assertEqual(res, b"Hello")

		backend.pread.assert_called_once_with(0,5)

	def test_pread_advanced(self):
		# Read on 0->5
		# Read on 0->10
		# it should call backend pread once on 0->5
		# and anoder one on 5->5
		backend = StorageBackend()
		backend.pread = mock.MagicMock(return_value = b"Hello")
		cache = Cache(backend)
		
		res = cache.pread(0,5)
		self.assertEqual(res, b"Hello")
		backend.pread.assert_called_once_with(0,5)
		backend.pread.reset_mock()

		res = cache.pread(0,10)
		self.assertEqual(res, b"HelloHello")
		backend.pread.assert_called_once_with(5,5)
		backend.pread.reset_mock()

	def test_pwrite_one(self):
		# 1 - create a backend
		# 2 - mock it with return value 5
		# 3 - call pwrite on cache with b"Hello" and offset 0
		# 4 - check backend has not been called
		# 5 - call flush
		# 6 - check backend has been called
		backend = StorageBackend()
		backend.pwrite = mock.MagicMock(return_value = 5)
		cache = Cache(backend)
		
		cache.pwrite(b"Hello", 0)
		backend.pwrite.assert_not_called()

		cache.flush()
		backend.pwrite.assert_called_once_with(b"Hello", 0)

	def test_pwrite_modify_after_flush(self):
		# 1 - create a backend
		# 2 - mock it with return value 5
		# 3 - call pwrite on cache with b"Hello" and offset 0
		# 4 - check backend has not been called
		# 5 - call flush
		# 6 - check backend has been called
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

	def test_pwrite_advanced(self):
		# Read on 0->5
		# Read on 0->10
		# it should call backend pread once on 0->5
		# and anoder one on 5->5
		backend = StorageBackend()
		backend.pwrite = mock.MagicMock(return_value = 5)
		cache = Cache(backend)
		
		cache.pwrite(b"01234", 0)
		cache.pwrite(b"56789", 5)
		backend.pwrite.assert_not_called()

		cache.flush()
		self.assertEqual(backend.pwrite.mock_calls, [
			mock.call(b"01234", 0), 
			mock.call(b"56789", 5)]
		)