/*********************************************************/
#include <cassert>
#include <algorithm>
#include "range.hpp"

/*********************************************************/
using namespace cache;

/*********************************************************/
Range::Range(size_t offset, size_t size)
      :offset(offset)
      ,size(size)
{
}

/*********************************************************/
bool Range::operator==(const Range & other) const
{
	return this->offset == other.offset && this->size == other.size;
}

/*********************************************************/
bool Range::operator<(const Range & other) const
{
	return this->offset < other.offset;
}

/*********************************************************/
size_t Range::end(void) const
{
	return this->offset + this->size;
}

/*********************************************************/
bool Range::overlap(const Range & range1, const Range & range2)
{
	return ! (range1.end() <= range2.offset || range2.end() <= range1.offset);
}

/*********************************************************/
std::pair<Range, Range> Range::extrude(const Range & range1, const Range & range2)
{
	assert(Range::overlap(range1, range2));
	size_t offset_left = std::min(range1.offset, range2.offset);
	size_t offset_right = std::max(range1.offset, range2.offset);
	Range left(offset_left, offset_right - offset_left);
	size_t end_left = std::min(range1.end(), range2.end());
	size_t end_right = std::max(range1.end(), range2.end());
	Range right(end_left, end_right - end_left);
	std::pair<Range, Range> res;
	res.first = left;
	res.second = right;
	return res;
}

/*********************************************************/
Range Range::intersect(const Range & range1, const Range & range2)
{
	assert(Range::overlap(range1, range2));
	size_t offset = std::max(range1.offset, range2.offset);
	size_t end = std::min(range1.end(), range2.end());
	return Range(offset, end - offset);
}
