from turtle import back
from unittest import TestCase, mock
import unittest
from cache import *

class TestCache(TestCase):
	def test_init_1(self):
		backend = StorageBackend()
		cache = Cache(backend)

	def test_pread(self):
		backend = StorageBackend()
		backend.pread = mock.MagicMock(return_value = b"0123456789")
		cache = Cache(backend)
		
		res = cache.pread(10,10)
		self.assertEqual(res, b"0123456789")
		backend.pread.assert_called_once_with(10,10)
		backend.pread.reset_mock()

		res = cache.pread(10,20)
		self.assertEqual(res, b"01234567890123456789")
		backend.pread.assert_called_once_with(20,10)
		backend.pread.reset_mock()

	def test_pwrite_one(self):
		backend = StorageBackend()
		backend.pwrite = mock.MagicMock(return_value = 5)
		cache = Cache(backend)
		
		cache.pwrite(b"01234", 0)
		backend.pwrite.assert_not_called()

		cache.flush()
		backend.pwrite.assert_called_once_with(b"01234", 0)

	def test_pwrite_many(self):
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

		