#include "mz.h"
#include "mz_strm.h"
#include <stdlib.h>
#include <string.h>

// Forward declarations for bd_system functions
// Declared as extern to avoid including C++ headers in C code
typedef void bd_FILE;
typedef void bd_DIR;
typedef void* mz_stream_handle;

extern bd_FILE *bd_fopen(const char *pathname, const char *mode);
extern int bd_fclose(bd_FILE *stream);
extern size_t bd_fread(void *ptr, size_t size, size_t nmemb, bd_FILE *stream);
extern size_t bd_fwrite(const void *ptr, size_t size, size_t nmemb, bd_FILE *stream);
extern long bd_ftell(bd_FILE *stream);
extern int bd_fseek(bd_FILE *stream, long offset, int whence);

// Custom stream OS implementation for berialdraw using bd_system
// This allows minizip to work with berialdraw's filesystem abstraction
// and support micropython with custom filesystems

typedef struct {
	bd_FILE *file;
	const char *filename;
	const char *mode;
} mz_stream_os_berialdraw;

/***************************************************************************/

static mz_stream_handle mz_stream_os_create_impl(void **handle)
{
	mz_stream_os_berialdraw *stream = NULL;

	stream = (mz_stream_os_berialdraw *)malloc(sizeof(mz_stream_os_berialdraw));
	if (stream != NULL)
		memset(stream, 0, sizeof(mz_stream_os_berialdraw));
	if (handle != NULL)
		*handle = stream;

	return (mz_stream_handle)stream;
}

static int32_t mz_stream_os_delete_impl(mz_stream_handle *handle)
{
	mz_stream_os_berialdraw *stream = NULL;

	if (handle == NULL)
		return MZ_PARAM_ERROR;
	stream = (mz_stream_os_berialdraw *)*handle;
	if (stream != NULL)
		free(stream);
	*handle = NULL;
	return MZ_OK;
}

static int32_t mz_stream_os_open_impl(mz_stream_handle handle, const char *path, int32_t mode)
{
	mz_stream_os_berialdraw *stream = (mz_stream_os_berialdraw *)handle;
	const char *file_mode = NULL;

	if (handle == NULL)
		return MZ_PARAM_ERROR;
	if (mode & MZ_OPEN_MODE_WRITE)
		file_mode = "wb";
	else
		file_mode = "rb";

	stream->file = bd_fopen(path, file_mode);
	if (stream->file == NULL)
		return MZ_OPEN_ERROR;

	stream->filename = path;
	stream->mode = file_mode;
	return MZ_OK;
}

static int32_t mz_stream_os_close_impl(mz_stream_handle handle)
{
	mz_stream_os_berialdraw *stream = (mz_stream_os_berialdraw *)handle;

	if (handle == NULL)
		return MZ_PARAM_ERROR;
	if (stream->file != NULL)
	{
		bd_fclose(stream->file);
		stream->file = NULL;
	}
	return MZ_OK;
}

static int32_t mz_stream_os_read_impl(mz_stream_handle handle, void *buf, int32_t size)
{
	mz_stream_os_berialdraw *stream = (mz_stream_os_berialdraw *)handle;
	int32_t bytes_read = 0;

	if (handle == NULL)
		return MZ_PARAM_ERROR;
	if (stream->file == NULL)
		return MZ_PARAM_ERROR;

	bytes_read = (int32_t)bd_fread(buf, 1, size, stream->file);
	if (bytes_read < 0)
		return MZ_READ_ERROR;

	return bytes_read;
}

static int32_t mz_stream_os_write_impl(mz_stream_handle handle, const void *buf, int32_t size)
{
	mz_stream_os_berialdraw *stream = (mz_stream_os_berialdraw *)handle;
	int32_t bytes_written = 0;

	if (handle == NULL)
		return MZ_PARAM_ERROR;
	if (stream->file == NULL)
		return MZ_PARAM_ERROR;

	bytes_written = (int32_t)bd_fwrite(buf, 1, size, stream->file);
	if (bytes_written < 0)
		return MZ_WRITE_ERROR;

	return bytes_written;
}

static int32_t mz_stream_os_tell_impl(mz_stream_handle handle)
{
	mz_stream_os_berialdraw *stream = (mz_stream_os_berialdraw *)handle;

	if (handle == NULL)
		return MZ_PARAM_ERROR;
	if (stream->file == NULL)
		return MZ_PARAM_ERROR;

	return (int32_t)bd_ftell(stream->file);
}

static int32_t mz_stream_os_seek_impl(mz_stream_handle handle, int64_t offset, int32_t whence)
{
	mz_stream_os_berialdraw *stream = (mz_stream_os_berialdraw *)handle;
	int32_t res = 0;

	if (handle == NULL)
		return MZ_PARAM_ERROR;
	if (stream->file == NULL)
		return MZ_PARAM_ERROR;

	res = bd_fseek(stream->file, (long)offset, whence);
	if (res < 0)
		return MZ_SEEK_ERROR;

	return MZ_OK;
}

/***************************************************************************/

// Stubs for OS-specific functions - not used with custom berialdraw filesystem
int32_t mz_os_is_dir_separator(const char *path, int32_t pos)
{
	if (path == NULL)
		return -1;
	if ((path[pos] == '/') || (path[pos] == '\\'))
		return 1;
	return 0;
}

int32_t mz_os_make_dir(const char *path)
{
	// Directory creation not implemented for berialdraw abstraction
	// Would need to add bd_mkdir() if needed
	return MZ_INTERNAL_ERROR;
}

// Public wrapper functions for minizip
mz_stream_handle mz_stream_os_create(void **handle)
{
	return mz_stream_os_create_impl(handle);
}

int32_t mz_stream_os_delete(mz_stream_handle *handle)
{
	return mz_stream_os_delete_impl(handle);
}

int32_t mz_stream_os_open(mz_stream_handle handle, const char *path, int32_t mode)
{
	return mz_stream_os_open_impl(handle, path, mode);
}

int32_t mz_stream_os_read(mz_stream_handle handle, void *buf, int32_t size)
{
	return mz_stream_os_read_impl(handle, buf, size);
}

int32_t mz_stream_os_write(mz_stream_handle handle, const void *buf, int32_t size)
{
	return mz_stream_os_write_impl(handle, buf, size);
}

int32_t mz_stream_os_tell(mz_stream_handle handle)
{
	return mz_stream_os_tell_impl(handle);
}

int32_t mz_stream_os_seek(mz_stream_handle handle, int64_t offset, int32_t whence)
{
	return mz_stream_os_seek_impl(handle, offset, whence);
}

int32_t mz_stream_os_close(mz_stream_handle handle)
{
	return mz_stream_os_close_impl(handle);
}
