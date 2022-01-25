/*********************************************************/
#ifndef CACHE_HPP
#define CACHE_HPP

/*********************************************************/
#include "range.hpp"
#include <list>

/*********************************************************/
namespace cache
{

/*********************************************************/
struct Buffer
{
	Buffer(size_t size);
	Buffer(const Buffer & orig);
	Buffer(Buffer && orig);
	Buffer(const std::string & value);
	Buffer(const char * value);
	~Buffer(void);
	Buffer & operator=(const Buffer & orig);
	bool operator==(const Buffer & orig) const;
	char * data;
	size_t size;
};

/*********************************************************/
class StrorageBackend
{
	public:
		virtual ~StrorageBackend(void) {};
		virtual size_t pwrite(const Buffer & value, size_t offset) = 0;
		virtual Buffer pread(size_t size, size_t offset) = 0;
};

/*********************************************************/
enum CacheEntryState
{
	NEW,
	CLEAN,
	DIRTY
};

/*********************************************************/
struct CacheEntry
{
	CacheEntry(Range range);
	CacheEntry(CacheEntry && entry);
	bool operator<(const CacheEntry & other) const;
	Range range;
	CacheEntryState state;
	Buffer data;
};

/*********************************************************/
class Cache
{
	public:
		Cache(StrorageBackend * backend);
		Buffer pread(size_t size, size_t offset);
		size_t pwrite(const Buffer & value, size_t offset);
		void flush(Range range = Range(0,0));
	private:
		void create_entries(Range range);
		void apply_read_on_new_entries(void);
	private:
		StrorageBackend * backend;
		std::list<CacheEntry> entries;
};

}

#endif //CACHE_HPP
