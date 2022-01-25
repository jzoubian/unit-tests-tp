/*********************************************************/
#ifndef RANGE_HPP
#define RANGE_HPP

/*********************************************************/
#include <ostream>
#include <utility>

/*********************************************************/
namespace cache
{

/*********************************************************/
class Range
{
	public:
		Range(size_t offset = 0, size_t size = 0);
		bool operator==(const Range & other) const;
		bool operator<(const Range & other) const;
		size_t end(void) const;
		static bool overlap(const Range & range1, const Range & range2);
		static std::pair<Range, Range> extrude(const Range & range1, const Range & range2);
		static Range intersect(const Range & range1, const Range & range2);
		size_t get_offset(void) const {return this->offset;};
		size_t get_size(void) const {return  this->size;};
	public:
		friend std::ostream & operator<<(std::ostream & out, const Range & range);
	private:	
		size_t offset;
		size_t size;
};

/*********************************************************/
inline std::ostream & operator<<(std::ostream & out, const Range & range)
{
	out << range.offset << ":" << range.size;
	return out;
}

}

#endif //RANGE_HPP
