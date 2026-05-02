#include "berialdraw_imp.hpp"

#ifdef _WIN32

using namespace berialdraw;

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
	bd_DIR* result = NULL;
	struct bd_DIR* dir = (struct bd_DIR*)bd_malloc(sizeof(struct bd_DIR));

	if (dir != NULL)
	{
		char search_path[MAX_PATH];
		snprintf(search_path, MAX_PATH, "%s\\*", path);
		dir->handle = FindFirstFileA(search_path, &dir->find_file_data);
		if (dir->handle != INVALID_HANDLE_VALUE)
		{
			result = dir;
		}
		else
		{
			bd_free(dir);
		}
	}

	return result;
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
	const char* result = NULL;

	if (dir != NULL)
	{
		if (FindNextFileA(dir->handle, &dir->find_file_data))
		{
			result = dir->find_file_data.cFileName;
		}
	}

	return result;
}

/** Gets the size of a file.
@param file_name Name of the file.
@return The size of the file in bytes. */
size_t bd_file_size(const char* file_name)
{
	size_t result = 0;
	WIN32_FILE_ATTRIBUTE_DATA file_info;

	if (GetFileAttributesExA(file_name, GetFileExInfoStandard, &file_info))
	{
		ULONGLONG size = ((ULONGLONG)file_info.nFileSizeHigh << 32) | file_info.nFileSizeLow;
		result = (size_t)size;
	}

	return result;
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

int bd_is_symlink(const char* path)
{
	int result = 1; // Non-zero means "not a symlink"

	if (path == NULL)
	{
		result = -1; // Error: null path
	}

	return result;
}

int bd_read_symlink(const char* path, char* target_path, int max_target_path)
{
	int result = -1; // Default: error

	if (path == NULL || target_path == NULL || max_target_path <= 0)
	{
		result = -1; // Error: invalid parameters
	}
	else
	{
		result = -2; // Error: not supported on Windows
	}

	return result;
}

#endif
