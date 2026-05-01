#pragma once
namespace berialdraw
{
	class MemHeader;

	/** Trace all memory allocations and check for memory leaks. */
	class MemoryLeakTracer
	{
	public:
		/** Starts memory analysis. */
		static void start();

		/** Stop memory analysis */
		static void stop();

		/** Sets a break point to stop when the specified allocation ID is reached.
		@param break_id The allocation ID at which to break. */
		static void break_at(size_t break_id);

		/** Displays all currently allocated memory blocks. */
		static void show();

		/** Allocates memory.
		@param size The size in bytes to allocate.
		@return A pointer to the allocated memory block. */
		static void * malloc(std::size_t size);

		/** Allocates memory for an array and initializes it to zero.
		@param count The number of elements to allocate.
		@param size The size in bytes of each element.
		@return A pointer to the allocated memory block. */
		static void * calloc(std::size_t count, std::size_t size);

		/** Reallocates a memory block to a new size.
		@param block The pointer to the current memory block.
		@param size The new size in bytes.
		@return A pointer to the reallocated memory block. */
		static void * realloc(void * block, std::size_t size);

		/** Frees the allocated memory block.
		@param ptr The pointer to the memory block to free. */
		static void free(void* ptr) noexcept;

		/** Returns the count of currently allocated blocks.
		@return The number of allocated blocks. */
		static size_t count();

		/** Returns the total size of currently allocated memory.
		@return The size in bytes of allocated memory. */
		static size_t size();

		/** Returns the base ID number for memory allocation.
		@return The base ID. */
		static size_t base_id();

		/** Returns the previous memory statistics size.
		@return The previous statistics size. */
		static size_t stat_size();

		/** Returns the previous memory statistics count.
		@return The previous statistics count. */
		static size_t stat_count();

		/** Returns whether memory analysis has started.
		@return True if memory analysis is started, false otherwise. */
		static bool started();

		/** Returns whether memory tracking is currently suspended.
		@return True if memory tracking is suspended, false otherwise. */
		static bool suspended();

		/** Displays memory consumption statistics.
		@param name An optional name for the statistics. */
		static void stat(const char * name = 0);

		/** Log test function entry with function name and current base_id.
		@param filename The name of the source file.
		@param line The line number in the source file.
		@param func_name The function name to log. */
		static void log(const char * filename, int line, const char * func_name);

		/** Suspends memory tracking without stopping it.
		Allocations made while suspended won't be tracked. */
		static void suspend();

		/** Resumes memory tracking after being suspended. */
		static void resume();

	protected:
/// @cond DOXYGEN_IGNORE
		/** Remove all log entries from the allocated list. */
		static void clean_logs();

		/** Allocates memory.
		@param size The size in bytes to allocate.
		@return A pointer to the allocated memory block. */
		static void * alloc(std::size_t size);

		/** Frees the allocated memory block.
		@param ptr The pointer to the memory block to free. */
		static void unalloc(void* ptr) noexcept;

		/** The total size of allocated memory. */
		static size_t m_size;

		/** The previous memory statistics size. */
		static size_t m_stat_size;

		/** The previous memory statistics count. */
		static size_t m_stat_count;

		/** The count of currently allocated memory blocks. */
		static size_t m_count;

		/** The base ID number for memory allocation. */
		static size_t m_base_id;

		/** The break point ID for memory analysis. */
		static size_t m_break_at;

		/** Indicates whether memory analysis has started. */
		static bool m_started;

		/** Indicates whether memory tracking is currently suspended. */
		static bool m_suspended;

		/** List of currently allocated memory blocks. */
		static MemHeader * m_allocated;
/// @endcond
	};

	#define MemoryLeakLog MemoryLeakTracer::log(__FILE__, __LINE__, __FUNCTION__);
/** Software break point definition */
#if defined(_MSC_VER)
	#include <intrin.h>
	#define breakpoint() __debugbreak()
#elif defined(__APPLE__)
	#include <signal.h>
	#include <unistd.h>
	#define breakpoint() raise(SIGSTOP); kill(getpid(), SIGCONT);
#elif defined(__GNUC__) || defined(__clang__)
	#include <signal.h>
	#define breakpoint() raise(SIGTRAP)
#else
	#include <cassert>
	#define breakpoint() assert(false && "Debug break")
#endif

}
