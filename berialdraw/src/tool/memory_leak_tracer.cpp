#include "berialdraw_imp.hpp"

namespace berialdraw
{
	/** Memory header */
	class MemHeader
	{
	public:
		size_t tag = 0xCAFEFADE;
		size_t size = 0;
		size_t id = 0;
		MemHeader * next = 0;
	};

	size_t MemoryLeakTracer::m_size  = 0;
	size_t MemoryLeakTracer::m_count = 0;
	size_t MemoryLeakTracer::m_base_id    = 0;
	size_t MemoryLeakTracer::m_break_at = -1;
	size_t MemoryLeakTracer::m_stat_size = 0;
	size_t MemoryLeakTracer::m_stat_count = 0;

	std::mutex m_mutex;
	bool MemoryLeakTracer::m_started = false;
	MemHeader *  MemoryLeakTracer::m_allocated = 0;
}
using namespace berialdraw;

extern "C" void MemTracerAtExit()
{
	MemoryLeakTracer::stop();
}

/** Start memory analysis */
void MemoryLeakTracer::start()
{
	atexit(MemTracerAtExit);
	MemoryLeakTracer::m_started = true;
}

/** Stop memory analysis */
void MemoryLeakTracer::stop()
{
	MemoryLeakTracer::m_started = false;
}

/** Set break point and break when id allocated is reached */
void MemoryLeakTracer::break_at(size_t break_id)
{
	MemoryLeakTracer::m_break_at = break_id;
}

/** Show all blocks yet allocated */
void MemoryLeakTracer::show()
{
	MemHeader * current = MemoryLeakTracer::m_allocated;
	bd_printf("Not freed count:%lu, size:%lu, last_id:%lu\n",(long)MemoryLeakTracer::m_count, (long) MemoryLeakTracer::m_size, (long)MemoryLeakTracer::m_base_id);
	while (current)
	{
		bd_printf("Id:%lu, size:%lu, ptr:0x%08llX\n",(long)current->id, (long)current->size, (long long)(&(current[1])));
		current = current->next;
	}
}

void size_to_string(int32_t size, size_t width = 7)
{
	if (abs(size) > 1073741824)
	{
		bd_printf("%*.2fG",(int)width, (float)size / 1073741824.);
	}
	else if (abs(size) > 1048576)
	{
		bd_printf("%*.2fM", (int)width, (float)size / 1048576.);
	}
	else if (abs(size) > 1024)
	{
		bd_printf("%*.2fK", (int)width, (float)size / 1024.);
	}
	else
	{
		bd_printf("%*d_b",(int)width, size);
	}
}

/** Show statistic information of memory consumption */
void MemoryLeakTracer::stat(const char * name)
{
	if (name)
	{
		size_to_string((int32_t)m_size - (int32_t)m_stat_size);
		bd_printf("/");
		size_to_string((int32_t)m_size);
		bd_printf(" [%4d/%4d] %s\n",(int)(m_count - m_stat_count), (int)(m_count), name);
	}
	m_stat_size = m_size;
	m_stat_count = m_count;
}

/** Count of blocks yet allocated */
size_t MemoryLeakTracer::count()
{
	return MemoryLeakTracer::m_count;
}

/** Size of blocks yet allocated */
size_t MemoryLeakTracer::size()
{
	return MemoryLeakTracer::m_size;
}

/** Memory calloc */
void * MemoryLeakTracer::calloc(std::size_t count, std::size_t size)
{
	void * result;
	result = MemoryLeakTracer::malloc(count * size);
	std::memset(result, 0, count * size);
	return result;
}

/** Memory reallocation */
void * MemoryLeakTracer::realloc(void * ptr, std::size_t size)
{
	void * result = ptr;
	size_t previous_size = 0;

	if (size == 0)
	{
		MemoryLeakTracer::unalloc(ptr);
		result = 0;
	}
	else
	{
		if (ptr)
		{
			MemHeader * header = (MemHeader*)ptr;
			header--;
			previous_size = header->size;
		}

		if (size != previous_size)
		{
			result = MemoryLeakTracer::alloc(size);

			if (ptr && result)
			{
				if (size < previous_size)
				{
					memcpy(result, ptr, size);
				}
				else
				{
					memcpy(result, ptr, previous_size);
				}
			}
			MemoryLeakTracer::unalloc(ptr);
		}
		else
		{
			result = ptr;
		}
	}
	return result;
}

/** Memory allocator */
void * MemoryLeakTracer::malloc(std::size_t size)
{
	void* result = 0;
	if (size > 0)
	{
		result = MemoryLeakTracer::alloc(size);
	}
	return result;
}

/** Memory deallocator */
void MemoryLeakTracer::free(void* ptr) noexcept
{
	if (ptr)
	{
		MemoryLeakTracer::unalloc(ptr);
	}
}


/** Memory allocator */
void * MemoryLeakTracer::alloc(std::size_t size)
{
	MemHeader* header = 0;
	if (size > 0)
	{
		if (MemoryLeakTracer::m_started) m_mutex.lock();
		header = (MemHeader*)std::malloc(size + sizeof(MemHeader));
		if (!header)
		{
			m_mutex.unlock();
			throw std::bad_alloc();
		}
		else
		{
			// Set default information
			header->size = size;
			header->id   = 0;
			header->next = 0;
			header->tag  = 0xCAFEFADE;

			// If analysis started
			if (MemoryLeakTracer::m_started)
			{
				// Set header
				header->id   = ++MemoryLeakTracer::m_base_id;
				header->size = size;
				header->next = MemoryLeakTracer::m_allocated;
				header->tag  = 0xCAFEFADE;

				// Increase counters
				MemoryLeakTracer::m_count ++;
				MemoryLeakTracer::m_size += size;
				MemoryLeakTracer::m_allocated = header;
				
				// If block id is searched
				if (header->id == MemoryLeakTracer::m_break_at)
				{
					breakpoint();
				}
			}
		}
		header++;
		if (MemoryLeakTracer::m_started) m_mutex.unlock();
	}
	return header;
}

/** Memory deallocator */
void MemoryLeakTracer::unalloc(void* ptr) noexcept
{
	if (ptr)
	{
		if (MemoryLeakTracer::m_started) m_mutex.lock();
		MemHeader * header = (MemHeader*)ptr;
		header--;

		// If analysis started
		if (MemoryLeakTracer::m_started)
		{
			// If header is setted
			if (header->id > 0)
			{
				// If block id is searched
				if (header->id == MemoryLeakTracer::m_break_at)
				{
					breakpoint();
				}

				// Decrease counters
				MemoryLeakTracer::m_count --;
				MemoryLeakTracer::m_size -= header->size;

				// If root allocated setted
				if(MemoryLeakTracer::m_allocated)
				{
					// If the current block is the root
					if (MemoryLeakTracer::m_allocated == header)
					{
						// Change the root with next block
						MemoryLeakTracer::m_allocated = MemoryLeakTracer::m_allocated->next;
					}
					else
					{
						MemHeader * next    = MemoryLeakTracer::m_allocated->next;
						MemHeader * current = MemoryLeakTracer::m_allocated;
				
						// Search the block in list
						while (current)
						{
							// If next block is the block searched
							if (next == header)
							{
								// Remove the block from the list
								current->next = next->next;
								break;
							}

							// If next existing
							if (next)
							{
								// Select next block
								current = next;
								next = next->next;
							}
							else
							{
								// Exit
								break;
							}
						}
					}
				}
			}
		}
		std::free(header);
		if (MemoryLeakTracer::m_started) m_mutex.unlock();
	}
}

/** Redefinition of the global new operator */
void* operator new(std::size_t size)
{
	return MemoryLeakTracer::malloc(size);
}

/** Redefinition of the global delete operator */
void operator delete(void* ptr) noexcept
{
	MemoryLeakTracer::free(ptr);
}

/** Redefinition of the new[] operator for arrays */
void* operator new[](std::size_t size)
{
	return MemoryLeakTracer::malloc(size);
}

/** Redefinition of the delete[] operator for arrays */
void operator delete[](void* ptr) noexcept
{
	MemoryLeakTracer::free(ptr);
}
