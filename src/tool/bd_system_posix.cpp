#include "berialdraw_imp.hpp"

#ifndef _WIN32

using namespace berialdraw;

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
	bd_DIR* result = NULL;
	struct bd_DIR* dir = (struct bd_DIR*)bd_malloc(sizeof(struct bd_DIR));

	if (dir != NULL)
	{
		dir->dir = opendir(path);
		if (dir->dir != NULL)
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
	const char* result = NULL;

	if (dir != NULL)
	{
		dir->entry = readdir(dir->dir);
		if (dir->entry != NULL)
		{
			result = dir->entry->d_name;
		}
	}

	return result;
}

/** Gets the next file in the directory.
@param dir The context of the directory.
@return The name of the next file, or NULL if none found. */
const char* bd_dir_next(bd_DIR* dir)
{
	const char* result = NULL;

	if (dir != NULL)
	{
		dir->entry = readdir(dir->dir);
		if (dir->entry != NULL)
		{
			result = dir->entry->d_name;
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
	struct stat st;

	if (stat(file_name, &st) == 0)
	{
		result = st.st_size;
	}

	return result;
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
	bool result = false;
	struct stat st;

	if (stat(file_name, &st) == 0)
	{
		result = S_ISDIR(st.st_mode);
	}

	return result;
}

int bd_is_symlink(const char* path)
{
	int result = 1; // Non-zero means "not a symlink"
	struct stat path_stat;

	if (path != NULL)
	{
		memset(&path_stat, 0, sizeof(path_stat));
		if (lstat(path, &path_stat) != -1)
		{
			if (S_ISLNK(path_stat.st_mode))
				result = 0; // Success: is a symlink
		}
	}
	else
	{
		result = -1; // Error: null path
	}

	return result;
}

int bd_read_symlink(const char* path, char* target_path, int max_target_path)
{
	int result = -1; // Default: error
	ssize_t length = 0;

	if (path != NULL && target_path != NULL && max_target_path > 0)
	{
		length = readlink(path, target_path, max_target_path - 1);
		if (length == -1)
		{
			result = -2; // Error reading symlink
		}
		else if (length >= max_target_path - 1)
		{
			result = -3; // Buffer too small
		}
		else
		{
			target_path[length] = 0;
			result = 0; // Success
		}
	}

	return result;
}

#endif // !_WIN32
