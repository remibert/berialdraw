#pragma once
#ifndef SWIG
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

		/** Displays memory consumption statistics.
		@param name An optional name for the statistics. */
		static void stat(const char * name = 0);

		/** Suspends memory tracking without stopping it.
		Allocations made while suspended won't be tracked. */
		static void suspend();

		/** Resumes memory tracking after being suspended. */
		static void resume();

	protected:

/// @cond DOXYGEN_IGNORE
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
#endif