#include "berialdraw_imp.hpp"

namespace berialdraw
{
	/** Memory tag constant for header validation */
	static constexpr size_t MEMORY_TAG     = 0xCAFEFADE;
	/** Memory tag constant for log entries */
	static constexpr size_t MEMORY_LOG_TAG = 0xCAFEFADF;
	/** Mask to match both MEMORY_TAG and MEMORY_LOG_TAG */
	static constexpr size_t MEMORY_TAG_MASK = 0xFFFFFFFE;
	/** Number of callstack frames stored per allocation */
	static constexpr int CALLSTACK_DEPTH = 3;
	/** Number of internal frames to skip when capturing callstack */
	static constexpr int CALLSTACK_SKIP  = 4;

	/** Memory header */
	class MemHeader
	{
	public:
		size_t tag = MEMORY_TAG;
		size_t size = 0;
		size_t id = 0;
		MemHeader * next = 0;
		MemHeader * prev = 0;
		void* callstack[3] = {0, 0, 0};  // 64 bytes total, 16-byte aligned for ARM64 SIMD
	};

	size_t MemoryLeakTracer::m_size  = 0;
	size_t MemoryLeakTracer::m_count = 0;
	size_t MemoryLeakTracer::m_base_id    = 0;
	size_t MemoryLeakTracer::m_break_at = -1;
	size_t MemoryLeakTracer::m_stat_size = 0;
	size_t MemoryLeakTracer::m_stat_count = 0;

	std::mutex m_mutex;
	bool MemoryLeakTracer::m_started = false;
	bool MemoryLeakTracer::m_suspended = false;
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

/** Remove all log entries from the allocated list */
void MemoryLeakTracer::clean_logs()
{
	MemHeader * current = MemoryLeakTracer::m_allocated;
	while (current)
	{
		MemHeader * next = current->next;
		if (current->tag == MEMORY_LOG_TAG)
		{
			// Remove from doubly-linked list
			if (MemoryLeakTracer::m_allocated == current)
			{
				MemoryLeakTracer::m_allocated = current->next;
				if (MemoryLeakTracer::m_allocated)
				{
					MemoryLeakTracer::m_allocated->prev = 0;
				}
			}
			else
			{
				if (current->prev)
				{
					current->prev->next = current->next;
				}
				if (current->next)
				{
					current->next->prev = current->prev;
				}
			}
			std::free(current);
		}
		current = next;
	}
}

/** Stop memory analysis */
void MemoryLeakTracer::stop()
{
	clean_logs();
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
	if (MemoryLeakTracer::m_count > 0)
	{
		MemHeader * current = MemoryLeakTracer::m_allocated;
		bd_printf("\n------------------------------------------------------------\n");
		bd_printf("  MEMORY LEAK DETECTED\n");
		bd_printf("  Not freed count:%lu, size:%lu, last_id:%lu\n",(long)MemoryLeakTracer::m_count, (long) MemoryLeakTracer::m_size, (long)MemoryLeakTracer::m_base_id);
		bd_printf("------------------------------------------------------------\n");
		while (current)
		{
			if (current->tag == MEMORY_LOG_TAG)
			{
				// Display log entry
				const char* log_text = (const char*)(&(current[1]));
				bd_printf("Id:%08lu, %s\n", (long)current->id, log_text);
			}
			else
			{
				uint8_t* data = (uint8_t*)(&(current[1]));
				size_t hex_len = (current->size >= 16) ? 16 : current->size;
				
				bd_printf("Id:%08lu, size:%-6lu, ptr:0x%08llX dump:| ", (long)current->id, (long)current->size, (long long)(&(current[1])));
				
				for (size_t i = 0; i < hex_len; i++)
				{
					bd_printf("%02X ", data[i]);
				}

				for (size_t i = hex_len; i < 16; i++)
				{
					bd_printf("   ");
				}
				
				bd_printf("|  |");
				for (size_t i = 0; i < hex_len; i++) {
					char c = data[i];
					bd_printf("%c", (c >= 32 && c < 127) ? c : '.');
				}
				bd_printf("|\n");

				// Display callstack for leaked blocks
#if defined(__APPLE__) || defined(__linux__)
				for (int i = 0; i < CALLSTACK_DEPTH && current->callstack[i]; i++)
				{
					Dl_info info;
					if (dladdr(current->callstack[i], &info) && info.dli_sname)
					{
						bd_printf("  at: %s (%s)\n", info.dli_sname, info.dli_fname ? info.dli_fname : "?");
					}
					else
					{
						bd_printf("  at: %p\n", current->callstack[i]);
					}
				}
#elif defined(_MSC_VER)
				for (int i = 0; i < CALLSTACK_DEPTH && current->callstack[i]; i++)
				{
					bd_printf("  at: 0x%llX\n", (unsigned long long)current->callstack[i]);
				}
#endif
			}
			current = current->next;
		}
		bd_printf("Not freed count:%lu, size:%lu, last_id:%lu\n",(long)MemoryLeakTracer::m_count, (long) MemoryLeakTracer::m_size, (long)MemoryLeakTracer::m_base_id);
		bd_printf("------------------------------------------------------------\n");
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

/** Suspend memory tracking without stopping it */
void MemoryLeakTracer::suspend()
{
	MemoryLeakTracer::m_suspended = true;
}

/** Resume memory tracking after being suspended */
void MemoryLeakTracer::resume()
{
	MemoryLeakTracer::m_suspended = false;
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

/** Base ID number for memory allocation */
size_t MemoryLeakTracer::base_id()
{
	return MemoryLeakTracer::m_base_id;
}

/** Previous memory statistics size */
size_t MemoryLeakTracer::stat_size()
{
	return MemoryLeakTracer::m_stat_size;
}

/** Previous memory statistics count */
size_t MemoryLeakTracer::stat_count()
{
	return MemoryLeakTracer::m_stat_count;
}

/** Indicates whether memory analysis has started */
bool MemoryLeakTracer::started()
{
	return MemoryLeakTracer::m_started;
}

/** Indicates whether memory tracking is currently suspended */
bool MemoryLeakTracer::suspended()
{
	return MemoryLeakTracer::m_suspended;
}

/** Log test function entry with function name and current base_id */
void MemoryLeakTracer::log(const char * filename, int line, const char * func_name)
{
	if (!MemoryLeakTracer::m_started || MemoryLeakTracer::m_suspended)
	{
		return;
	}

	const char * fn = filename ? filename : "unknown";
	const char * fnc = func_name ? func_name : "unknown";

	// Format the log string
	char buffer[512];
	snprintf(buffer, sizeof(buffer), "%s:%d:%s", fn, line, fnc);
	size_t str_len = strlen(buffer) + 1;

	// Allocate a log block via std::malloc (not tracked)
	m_mutex.lock();
	size_t alloc_size = sizeof(MemHeader) + str_len;
	MemHeader * header = (MemHeader*)std::malloc(alloc_size);
	if (header)
	{
		header->tag  = MEMORY_LOG_TAG;
		header->size = str_len;
		header->id   = MemoryLeakTracer::m_base_id;
		header->next = MemoryLeakTracer::m_allocated;
		header->prev = 0;

		if (MemoryLeakTracer::m_allocated)
		{
			MemoryLeakTracer::m_allocated->prev = header;
		}
		MemoryLeakTracer::m_allocated = header;

		// Copy log string after header
		memcpy(&(header[1]), buffer, str_len);
	}
	m_mutex.unlock();

	//bd_printf("Id:%08lu, %s:%d:%s\n", (long)MemoryLeakTracer::m_base_id, fn, line, fnc);
}

/** Memory calloc */
void * MemoryLeakTracer::calloc(std::size_t count, std::size_t size)
{
	void * result;
	result = MemoryLeakTracer::malloc(count * size);
	memset(result, 0, count * size);
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
			
			if ((header->tag & MEMORY_TAG_MASK) != MEMORY_TAG)
			{
				return std::realloc(ptr, size);
			}
			
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
		size_t alloc_size = size + sizeof(MemHeader);
		header = (MemHeader*)std::malloc(alloc_size + 4);

		if (!header)
		{
			m_mutex.unlock();
			throw std::bad_alloc();
		}
		else
		{
			// Mark allocated block has to check overflow
			((char*)header)[alloc_size+0] = '[';
			((char*)header)[alloc_size+1] = '#';
			((char*)header)[alloc_size+2] = '#';
			((char*)header)[alloc_size+3] = ']';

			// Set default information
			header->size = size;
			header->id   = 0;
			header->next = 0;
			header->prev = 0;
			header->tag  = MEMORY_TAG;
			memset(header->callstack, 0, sizeof(header->callstack));

			// If analysis started
			if (MemoryLeakTracer::m_started)
			{
				 // Only track if not suspended
				if (!MemoryLeakTracer::m_suspended)
				{
					// Set header
					header->id   = ++MemoryLeakTracer::m_base_id;
					header->size = size;
					header->next = MemoryLeakTracer::m_allocated;
					header->prev = 0;
					header->tag  = MEMORY_TAG;

					// Capture callstack
#if defined(__APPLE__) || defined(__linux__)
					{
						void* temp[CALLSTACK_SKIP + CALLSTACK_DEPTH];
						int depth = backtrace(temp, CALLSTACK_SKIP + CALLSTACK_DEPTH);
						for (int i = 0; i < CALLSTACK_DEPTH && (i + CALLSTACK_SKIP) < depth; i++)
						{
							header->callstack[i] = temp[i + CALLSTACK_SKIP];
						}
					}
#elif defined(_MSC_VER)
					CaptureStackBackTrace(CALLSTACK_SKIP, CALLSTACK_DEPTH, header->callstack, NULL);
#endif

					// Update prev pointer of old root (doubly-linked list)
					if (MemoryLeakTracer::m_allocated)
					{
						MemoryLeakTracer::m_allocated->prev = header;
					}

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

		if ((header->tag & MEMORY_TAG_MASK) != MEMORY_TAG)
		{
			if (MemoryLeakTracer::m_started) m_mutex.unlock();
			std::free(ptr);
			return;
		}

		size_t alloc_size = header->size + sizeof(MemHeader);

		// Checks if the allocated block has not overflowed in memory
		if (((char*)header)[alloc_size+0] != '[' ||
		    ((char*)header)[alloc_size+1] != '#' ||
		    ((char*)header)[alloc_size+2] != '#' ||
		    ((char*)header)[alloc_size+3] != ']')
		{
			// Memory corrupted
			breakpoint();
		}

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

				// Remove from doubly-linked list in O(1) time
				if (MemoryLeakTracer::m_allocated == header)
				{
					// Removing head node
					MemoryLeakTracer::m_allocated = header->next;
					if (MemoryLeakTracer::m_allocated)
					{
						MemoryLeakTracer::m_allocated->prev = 0;
					}
				}
				else
				{
					// Removing non-head node using prev pointer
					if (header->prev)
					{
						header->prev->next = header->next;
					}
					if (header->next)
					{
						header->next->prev = header->prev;
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
