/*********************************************************/
#include <cassert>
#include <exception>
#include <memory>
#include <cstring>
#include "cache.hpp"

/*********************************************************/
using namespace cache;

/*********************************************************/
Buffer::Buffer(size_t size)
{
	this->size = size;
	if (size == 0)
		this->data = NULL;
	else
		this->data = new char[size];
}

/*********************************************************/
Buffer::Buffer(Buffer && orig)
{
	this->size = orig.size;
	this->data = orig.data;
	orig.size = 0;
	orig.data = NULL;
}


/*********************************************************/
Buffer::Buffer(const char * value)
{
	this->size = strlen(value);
	if (this->size == 0) {
		this->data = NULL;
	} else {
		this->data = new char[this->size];
		memcpy(this->data, value, this->size);
	}
}

/*********************************************************/
Buffer::Buffer(const Buffer & orig)
{
	this->size = orig.size;
	if (this->size == 0) {
		this->data = NULL;
	} else {
		this->data = new char[this->size];
		memcpy(this->data, orig.data, this->size);
	}
}

/*********************************************************/
Buffer::Buffer(const std::string & value)
{
	this->size = value.size();
	if (this->size == 0) {
		this->data = NULL;
	} else {
		this->data = new char[this->size];
		memcpy(this->data, value.c_str(), this->size);
	}
}

/*********************************************************/
Buffer::~Buffer(void)
{
	if (this->data != NULL)
		delete [] this->data;
	this->data = NULL;
	this->size = 0;
}

/*********************************************************/
Buffer & Buffer::operator=(const Buffer & orig)
{
	assert(this->size == orig.size);
	memcpy(this->data, orig.data, this->size);
	return *this;
}

/*********************************************************/
bool Buffer::operator==(const Buffer & orig) const
{
	if (this->size != orig.size)
		return false;
	return strncmp(this->data, orig.data, this->size) == 0;
}

/*********************************************************/
CacheEntry::CacheEntry(Range range)
           :data(range.get_size())
{
	this->state = CacheEntryState::NEW;
	this->range = range;
}

/*********************************************************/
CacheEntry::CacheEntry(CacheEntry && entry)
           :data(std::move(entry.data))
{
	this->state = entry.state;
	this->range = range;
}

/*********************************************************/
bool CacheEntry::operator<(const CacheEntry & other) const
{
	return this->range < other.range;
}

/*********************************************************/
Cache::Cache(StrorageBackend * backend)
{
	assert(backend != NULL);
	this->backend = backend;
}

/*********************************************************/
void Cache::create_entries(Range range)
{
	Range cur_range = range;
	for (auto & entry : this->entries) {
		if (Range::overlap(cur_range, entry.range)) {
			auto res = Range::extrude(cur_range, entry.range);
			Range left = res.first;
			Range right = res.second;
			if (left.get_size() > 0)
				this->entries.emplace_back(left);
			cur_range = right;
		}
		if (cur_range.get_size() == 0)
			break;
	}
	if (cur_range.get_size() != 0)
		this->entries.emplace_back(cur_range);
	this->entries.sort();
}

/*********************************************************/
void Cache::apply_read_on_new_entries(void)
{
	for(auto & entry : this->entries) {
		if (entry.state == CacheEntryState::NEW) {
			Buffer data = this->backend->pread(entry.range.get_offset(), entry.range.get_size());
			if (data.size != entry.range.get_size())
				throw std::exception();
			entry.data = data;
			entry.state = CacheEntryState::CLEAN;
		}
	}
}

/*********************************************************/
Buffer Cache::pread(size_t size, size_t offset)
{
	Range op_range(offset, size);
	this->create_entries(op_range);
	this->apply_read_on_new_entries();
	Buffer res(size);
	for (auto & entry : this->entries) {
		if (Range::overlap(entry.range, op_range)) {
			Range intersect = Range::intersect(op_range, entry.range);
			Range out_data_range(intersect.get_offset() - offset, intersect.get_size());
			Range in_data_range(intersect.get_offset() - entry.range.get_offset(), intersect.get_size());
			memcpy(res.data + out_data_range.get_offset(), entry.data.data + in_data_range.get_offset(), out_data_range.get_size());
		}
	}
	return res;
}

/*********************************************************/
size_t Cache::pwrite(const Buffer & value, size_t offset)
{
	Range op_range(offset, value.size);
	this->create_entries(op_range);
	for (auto & entry : this->entries) {
		if (Range::overlap(entry.range, op_range)) {
			Range intersect = Range::intersect(op_range, entry.range);
			Range out_data_range(intersect.get_offset() - offset, intersect.get_size());
			Range in_data_range(intersect.get_offset() - entry.range.get_offset(), intersect.get_size());
			memcpy(entry.data.data + in_data_range.get_offset(), value.data + out_data_range.get_offset(), out_data_range.get_size());
			entry.state = CacheEntryState::DIRTY;
		}
	}
	return value.size;
}

/*********************************************************/
void Cache::flush(Range range)
{
	for (auto & entry : this->entries) {
		if (entry.state == CacheEntryState::DIRTY && (range.get_size() == 0 || Range::overlap(range, entry.range))) {
			size_t res = this->backend->pwrite(entry.data, entry.range.get_offset());
			if (res != entry.range.get_size())
				throw std::exception();
			entry.state = CacheEntryState::CLEAN;
		}
	}
}
