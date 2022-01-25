from code import interact
from enum import Enum
from range import Range

class StorageBackend:
	"""
	Define the storage backend to be plugged under the cache
	"""
	def pwrite(data, offset) -> int:
		return -1
	def pread(offset, size) -> int:
		return bytearray()

class CacheEntryState(Enum):
	"""
	Define the state (dirty of not) of a cache entry.
	"""
	NEW = 1
	CLEAN = 2
	DIRTY = 2

class CacheEntry:
	"""
	Define a cache entry which is a data segment mapped on a specific range.
	It has a clean/dirty/new state.
	"""
	def __init__(self, offset, size):
		self.range = Range(offset, size)
		self.state = CacheEntryState.NEW
		self.data = bytearray(size)

class Cache:
	"""
	Implement a cache which can store data an flush them to a storage backend
	On a read operation it automatically fetch the data if not already present
	in the cache
	"""
	def __init__(self, backend: StorageBackend):
		self.backend = backend
		self.entries = []

	def get_sort_key(entry: CacheEntry):
		return entry.range.offset

	def create_entries(self, offset, size):
		cur_range = Range(offset, size)
		for i in range(len(self.entries)):
			entry = self.entries[i]
			if Range.overlap(cur_range, entry.range):
				left, right = Range.extrude(cur_range, entry.range)
				if left.size > 0:
					self.entries.append(CacheEntry(left.offset, left.size))
				cur_range = right
			if cur_range.size == 0:
				break
		if cur_range.size != 0:
			self.entries.append(CacheEntry(cur_range.offset, cur_range.size))
		self.entries.sort(key = Cache.get_sort_key)

	def apply_read_on_new_entries(self):
		for entry in self.entries:
			if entry.state == CacheEntryState.NEW:
				data = self.backend.pread(entry.range.offset, entry.range.size)
				if len(data) != entry.range.size:
					raise Exception("Not enougth data to read")
				entry.data = data
				entry.state = CacheEntryState.CLEAN

	def pwrite(self, data, offset):
		op_range = Range(offset, len(data))
		self.create_entries(op_range.offset, op_range.size)
		for entry in self.entries:
			if Range.overlap(entry.range, op_range):
				intersect = Range.intersect(op_range, entry.range)
				in_data_range = Range(intersect.offset - offset, intersect.size)
				out_data_range = Range(intersect.offset - entry.range.offset, intersect.size)
				entry.data[out_data_range.offset:out_data_range.end()] = data[in_data_range.offset:in_data_range.end()]
				entry.state = CacheEntryState.DIRTY
	
	def pread(self, offset, size) -> bytearray:
		op_range = Range(offset, size)
		self.create_entries(op_range.offset, op_range.size)
		self.apply_read_on_new_entries()
		out_data = bytearray(size)
		for entry in self.entries:
			if Range.overlap(entry.range, op_range):
				intersect = Range.intersect(op_range, entry.range)
				out_data_range = Range(intersect.offset - offset, intersect.size)
				in_data_range = Range(intersect.offset - entry.range.offset, intersect.size)
				#print("{}:{} = {}:{}".format(out_data_range.offset,out_data_range.end(),in_data_range.offset,in_data_range.end()))
				out_data[out_data_range.offset:out_data_range.end()] = entry.data[in_data_range.offset:in_data_range.end()]
		return out_data

	def flush(self, range: Range = Range(0,0)):
		for entry in self.entries:
			if entry.state == CacheEntryState.DIRTY and (range.size == 0 or Range.collide(range, entry.range)):
				res = self.backend.pwrite(entry.data, entry.range.offset)
				if res != entry.range.size:
					raise Exception("Fail to fully write data into storage backend")
				entry.state = CacheEntryState.CLEAN