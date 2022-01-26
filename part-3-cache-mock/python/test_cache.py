from unittest import TestCase, mock
from cache import *

class ManualMockBackend(StorageBackend):
    # TODO: build a manual mock
    # send hard coded value. A bytearray for pread (eg. b"Hello")
    # A fixed size for pwrite (we will use "Hello" when writing)
    #
    # def pwrite(self, data: bytearray, offset) -> int:
    # def pread(self,offset, size) -> bytearray:
    pass

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
        #raise NotImplementedError("error")
        pass

    def test_pwrite(self):
        # 1 - build a mocked backend
        # 2 - build a cache
        # 3 - pre fill the data field in the manual mock (same size)
        # 4 - call write with a different pattern
        # 5 - check that is has been updated
        #raise NotImplementedError("error")
        pass

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
        #raise NotImplementedError("error")
        pass

    def test_pread_second_call(self):
        # 1 - similar to the previous test
        # 2 - just add a second call to pread before checking
        #     the mock. The cache should have the value and
        #     to not emit a request to the backend.
        #     It can be checked with assert_called_once_with()
        #raise NotImplementedError("error")
        pass

    def test_pread_advanced(self):
        # Read on 0->5
        # Read on 0->10
        # it should call backend pread once on 0->5
        # and anoder one on 5->5
        #raise NotImplementedError("error")
        pass

    def test_pwrite_one(self):
        # 1 - create a backend
        # 2 - mock it with return value 5
        # 3 - call pwrite on cache with b"Hello" and offset 0
        # 4 - check backend has not been called
        # 5 - call flush
        # 6 - check backend has been called
        #raise NotImplementedError("error")
        pass

    def test_pwrite_modify_after_flush(self):
        # 1 - create a backend
        # 2 - mock it with return value 5
        # 3 - call pwrite on cache with b"Hello" and offset 0
        # 4 - check backend has not been called
        # 5 - call flush
        # 6 - check backend has been called
        #raise NotImplementedError("error")
        pass

    def test_pwrite_advanced(self):
        # Read on 0->5
        # Read on 0->10
        # it should call backend pread once on 0->5
        # and anoder one on 5->5
        #raise NotImplementedError("error")
        pass
