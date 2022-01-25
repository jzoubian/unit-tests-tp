class Range:
	def __init__(self, offset: int, size: int):
		self.offset = offset
		self.size = size

	def __eq__(self, other):
		if isinstance(other, self.__class__):
			return self.offset == other.offset and self.size == self.size
		else:
			return False

	def __repr__(self):
		return f"{self.offset}:{self.size}"

	def end(self) -> int:
		return self.offset + self.size

	def overlap(range1, range2) -> bool:
		return not (range1.end() <= range2.offset or range2.end() <= range1.offset)

	def extrude(range1, range2):
		assert(Range.overlap(range1, range2))
		offset_left = min(range1.offset, range2.offset)
		offset_right = max(range1.offset, range2.offset)
		left = Range(offset_left, offset_right - offset_left)
		end_left = min(range1.end(), range2.end())
		end_right = max(range1.end(), range2.end())
		right = Range(end_left, end_right - end_left)
		return left, right

	def intersect(range1, range2):
		assert(Range.overlap(range1, range2))
		offset = max(range1.offset, range2.offset)
		end = min(range1.end(), range2.end())
		return Range(offset, end - offset)