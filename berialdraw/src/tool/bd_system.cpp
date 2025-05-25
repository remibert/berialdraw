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
	bd_FILE * result = new bd_FILE;
	if(result->open(pathname, mode) == 0)
	{
		return result;
	}
	else
	{
		delete result;
		return 0;
	}
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
	if(stream)
	{
		int res = stream->read(ptr, (uint32_t)(size*nmemb));
		if (res < 0)
		{
			res = 0;
		}
		return res;
	}
	return 0;
}

size_t bd_fwrite(const void *ptr, size_t size, size_t nmemb, bd_FILE*stream)
{
	if(stream)
	{
		int res = stream->write(ptr, (uint32_t)(size*nmemb));
		if (res < 0)
		{
			res = 0;
		}
		return res;
	}
	return 0;
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

void bd_printf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}






#if defined(WIN32)
	struct bd_DIR
	{
		HANDLE handle;
		WIN32_FIND_DATAA find_file_data;
	};

	/** Opens a directory.
	@param path Path to the directory.
	@return A pointer to the context or NULL if the directory could not be opened. */
	bd_DIR* bd_dir_open(const char* path)
	{
		struct bd_DIR* dir = (struct bd_DIR*)bd_malloc(sizeof(struct bd_DIR));
		if (!dir)
		{
			return NULL;
		}

		char search_path[MAX_PATH];
		snprintf(search_path, MAX_PATH, "%s\\*", path);
		dir->handle = FindFirstFileA(search_path, &dir->find_file_data);
		if (dir->handle == INVALID_HANDLE_VALUE)
		{
			bd_free(dir);
			return NULL;
		}
		return dir;
	}

	/** Closes the currently opened directory. */
	void bd_dir_close(bd_DIR* dir)
	{
		if (dir)
		{
			if (dir->handle != INVALID_HANDLE_VALUE)
			{
				FindClose(dir->handle);
			}
			bd_free(dir);
		}
	}

	/** Gets the first file in the directory.
	@param dir The context of the directory.
	@return The name of the first file, or NULL if none found. */
	const char* bd_dir_first(bd_DIR* dir)
	{
		return (dir && (dir->handle != INVALID_HANDLE_VALUE)) ? dir->find_file_data.cFileName : NULL;
	}

	/** Gets the next file in the directory.
	@param dir The context of the directory.
	@return The name of the next file, or NULL if none found. */
	const char* bd_dir_next(bd_DIR* dir)
	{
		if (dir)
		{
			if (FindNextFileA(dir->handle, &dir->find_file_data))
			{
				return dir->find_file_data.cFileName;
			}
		}
		return NULL;
	}

	/** Gets the size of a file.
	@param file_name Name of the file.
	@return The size of the file in bytes. */
	size_t bd_file_size(const char* file_name)
	{
		WIN32_FILE_ATTRIBUTE_DATA file_info;
		if (GetFileAttributesExA(file_name, GetFileExInfoStandard, &file_info))
		{
			ULONGLONG size = ((ULONGLONG)file_info.nFileSizeHigh << 32) | file_info.nFileSizeLow;
			return (size_t)size;
		}
		return 0;
	}

	/** Indicates if the files exists
	@param file_name Name of the file.
	@return True if file existing. */
	bool bd_file_exists(const char* file_name)
	{
		DWORD attrib = GetFileAttributesA(file_name);
		return (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
	}

	/** Checks if a given file is a directory.
	@param file_name Name of the file.
	@return True if the file is a directory, false otherwise. */
	bool bd_is_directory(const char* file_name)
	{
		DWORD attributes = GetFileAttributesA(file_name);
		return (attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY));
	}


#else
	struct bd_DIR
	{
		DIR* dir;
		struct dirent* entry;
	};

	/** Opens a directory.
	@param path Path to the directory.
	@return A pointer to the context or NULL if the directory could not be opened. */
	bd_DIR* bd_dir_open(const char* path)
	{
		struct bd_DIR* dir = (struct bd_DIR*)bd_malloc(sizeof(struct bd_DIR));
		if (!dir)
		{
			return NULL;
		}
		dir->dir = opendir(path);
		if (!dir->dir)
		{
			bd_free(dir);
			return NULL;
		}
		return dir;
	}

	/** Closes the currently opened directory. */
	void bd_dir_close(bd_DIR* dir)
	{
		if (dir && dir->dir)
		{
			closedir(dir->dir);
		}
		bd_free(dir);
	}

	/** Gets the first file in the directory.
	@param dir The context of the directory. 
	@return The name of the first file, or NULL if none found. */
	const char* bd_dir_first(bd_DIR* dir)
	{
		if (dir)
		{
			dir->entry = readdir(dir->dir);
			return dir->entry ? dir->entry->d_name : NULL;
		}
		return 0;
	}

	/** Gets the next file in the directory.
	@param dir The context of the directory.
	@return The name of the next file, or NULL if none found. */
	const char* bd_dir_next(bd_DIR* dir)
	{
		if (dir)
		{
			dir->entry = readdir(dir->dir);
			return dir->entry ? dir->entry->d_name : NULL;
		}
		return 0;
	}

	/** Gets the size of a file.
	@param file_name Name of the file.
	@return The size of the file in bytes. */
	size_t bd_file_size(const char* file_name)
	{
		struct stat st;
		if (stat(file_name, &st) == 0)
		{
			return st.st_size;
		}
		return 0;
	}

	/** Indicates if the files exists
	@param file_name Name of the file.
	@return True if file existing. */
	bool bd_file_exists(const char* file_name)
	{
		struct stat buffer;
		return (stat(file_name, &buffer) == 0 && S_ISREG(buffer.st_mode));
	}

	/** Checks if a given file is a directory.
	@param file_name Name of the file.
	@return True if the file is a directory, false otherwise. */
	bool bd_is_directory(const char* file_name)
	{
		struct stat st;
		if (stat(file_name, &st) == 0)
		{
			return S_ISDIR(st.st_mode);
		}
		return false;
	}
#endif