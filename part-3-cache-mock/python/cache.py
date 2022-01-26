from code import interact
from enum import Enum
from range import Range

class StorageBackend:
	"""
	Define the storage backend to be plugged under the cache
	"""
	def pwrite(self, data, offset) -> int:
		return -1
	def pread(self, offset, size) -> int:
		return bytearray()

class CacheEntryState(Enum):
	"""
	Define the state (dirty of not) of a cache entry.
	"""
	NEW = 1
	CLEAN = 2
	DIRTY = 3

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
		# build range
		cur_range = Range(offset, size)

		# loop on all entries to find the holes we need to fill by allocating a new entry
		for i in range(len(self.entries)):
			# extract the entry
			entry = self.entries[i]

			# check if overlap
			if Range.overlap(cur_range, entry.range):
				# get the left and right segment by extruding the incomming one with the pre-existing one
				left, right = Range.exclude(cur_range, entry.range)

				# if the left residut is not NULL
				if left.size > 0:
					self.entries.append(CacheEntry(left.offset, left.size))
				
				# replace current by the right residut
				cur_range = right
			
			# the current become NULL, we can stop we reached the enf of the overlapping region
			if cur_range.size == 0:
				break

		# There is a last one to insert after the last pre-existing one
		if cur_range.size != 0:
			self.entries.append(CacheEntry(cur_range.offset, cur_range.size))

		# sort again
		self.entries.sort(key = Cache.get_sort_key)

	def apply_read_on_new_entries(self):
		# loop on all entries
		for entry in self.entries:
			# check if not loaded or dirty
			if entry.state == CacheEntryState.NEW:
				# load data
				data = self.backend.pread(entry.range.offset, entry.range.size)

				# failed to load
				if len(data) != entry.range.size:
					raise Exception("Not enougth data to read")

				# fill the new entry
				entry.data = data
				entry.state = CacheEntryState.CLEAN

	def pwrite(self, data, offset):
		# build range object to ease implementation
		op_range = Range(offset, len(data))

		# create all required new entries
		self.create_entries(op_range.offset, op_range.size)

		# loop on all
		for entry in self.entries:
			# if overlap
			if Range.overlap(entry.range, op_range):
				# calc intersect to get the copy ranges
				intersect = Range.intersect(op_range, entry.range)

				# build shifted sub-ranges in the in/out data segment.
				in_data_range = intersect.shift(-offset)
				out_data_range = intersect.shift(-entry.range.offset)

				# cpoy the data
				entry.data[out_data_range.offset:out_data_range.end()] = data[in_data_range.offset:in_data_range.end()]

				#mark dirty
				entry.state = CacheEntryState.DIRTY
	
	def pread(self, offset, size) -> bytearray:
		# build range object to ease implementation
		op_range = Range(offset, size)

		# create all required new entries
		self.create_entries(op_range.offset, op_range.size)

		# perform read operation to fetch data from the storage
		self.apply_read_on_new_entries()

		# allocate memory to load data in
		out_data = bytearray(size)

		# loop on all entries
		for entry in self.entries:
			# if overlap we need to copy
			if Range.overlap(entry.range, op_range):
				# calc intersect to find what to copy
				intersect = Range.intersect(op_range, entry.range)

				# calc buffer in & out copy ranges
				out_data_range = intersect.shift(-offset)
				in_data_range = intersect.shift(-entry.range.offset)

				# copy data
				out_data[out_data_range.offset:out_data_range.end()] = entry.data[in_data_range.offset:in_data_range.end()]
		
		# ok return the segment
		return out_data

	def flush(self, range: Range = Range(0,0)):
		# loop on all entries
		for entry in self.entries:
			# if is dirty and overlap the requested range
			if entry.state == CacheEntryState.DIRTY and (range.size == 0 or Range.collide(range, entry.range)):
				# do pwrite
				res = self.backend.pwrite(entry.data, entry.range.offset)

				# check has been done
				if res != entry.range.size:
					raise Exception("Fail to fully write data into storage backend")

				# mark clean
				entry.state = CacheEntryState.CLEAN