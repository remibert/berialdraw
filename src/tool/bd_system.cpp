#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Memory allocator */
void * bd_malloc(std::size_t size)
{
	return MemoryLeakTracer::malloc(size);
}

/** Memory calloc */
void * bd_calloc(std::size_t count, std::size_t size)
{
	return MemoryLeakTracer::calloc(count, size);
}

/** Memory reallocation */
void * bd_realloc(void * block, std::size_t size)
{
	return MemoryLeakTracer::realloc(block, size);
}

/** Memory deallocator */
void bd_free(void* ptr)
{
	MemoryLeakTracer::free(ptr);
}

bd_FILE * bd_fopen(const char *pathname, const char *mode)
{
	bd_FILE * result = NULL;
	bd_FILE * file = new bd_FILE;

	if (file->open(pathname, mode) == 0)
	{
		result = file;
	}
	else
	{
		delete file;
	}

	return result;
}

int bd_fclose(bd_FILE *stream)
{
	int result = -1;
	if(stream)
	{
		result = stream->close();
		delete stream;
	}
	return result;
}

size_t bd_fread(void *ptr, size_t size, size_t nmemb, bd_FILE *stream)
{
	size_t result = 0;

	if (stream != NULL)
	{
		int res = stream->read(ptr, (uint32_t)(size*nmemb));
		if (res >= 0)
		{
			result = res;
		}
	}

	return result;
}

size_t bd_fwrite(const void *ptr, size_t size, size_t nmemb, bd_FILE*stream)
{
	size_t result = 0;

	if (stream != NULL)
	{
		int res = stream->write(ptr, (uint32_t)(size*nmemb));
		if (res >= 0)
		{
			result = res;
		}
	}

	return result;
}

long bd_ftell(bd_FILE *stream)
{
	int result = -1;
	if(stream)
	{
		result = stream->tell();
	}
	return result;
}

int bd_fseek(bd_FILE *stream, long offset, int whence)
{
	int result = -1;
	if(stream)
	{
		result = stream->seek(offset, whence);
	}
	return result;
}

// Default implementation (standard vprintf)
static void bd_default_printf(const char* format, va_list args)
{
	vprintf(format, args);
	fflush(stdout);
}

// Typedef for printf function pointer
typedef void (*PrintfRedirection)(const char* format, va_list args);

// Global function pointer
static PrintfRedirection bd_current_printf = bd_default_printf;

// Function to initialize callback
void bd_redirect_printf(void (*callback)(const char* format, va_list args))
{
	bd_current_printf = callback;
}

void bd_printf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	
	if (bd_current_printf)
	{
		bd_current_printf(format, args);
	}
	va_end(args);
}
