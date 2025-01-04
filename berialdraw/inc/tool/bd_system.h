#ifndef BdSytem_h_INCLUDED
#define BdSytem_h_INCLUDED

#if defined(__cplusplus)
/** This file provides a redefined system interface for file operations, 
adaptable to the hardware platform it is used on.*/

extern "C"
{
#endif
	/** Allocates memory of the specified size.
	@param size The size in bytes to allocate.
	@return A pointer to the allocated memory block, or NULL if the allocation fails. */
	void * bd_malloc(std::size_t size);

	/** Allocates memory for an array of elements, initializing all bytes to zero.
	@param count The number of elements to allocate.
	@param size The size in bytes of each element.
	@return A pointer to the allocated memory block, or NULL if the allocation fails. */
	void * bd_calloc(std::size_t count, std::size_t size);

	/** Reallocates a memory block to a new size.
	@param block A pointer to the currently allocated memory block.
	@param size The new size in bytes for the memory block.
	@return A pointer to the reallocated memory block, or NULL if the reallocation fails. */
	void * bd_realloc(void * block, std::size_t size);

	/** Frees the allocated memory block.
	@param ptr A pointer to the memory block to free. If NULL, no action is performed. */
	void bd_free(void* ptr);

	#if defined(__cplusplus)
		/** @typedef bd_FILE
		Defines bd_FILE as an alias for berialdraw::File in C++. */
		typedef berialdraw::File bd_FILE;
	#else
		/** @typedef bd_FILE
		Defines bd_FILE as a struct in C. */
		typedef struct bd_FILE bd_FILE;
	#endif

	/** Opens a file with the specified path and mode.
	@param pathname Path to the file.
	@param mode Mode in which to open the file.
	@return A pointer to bd_FILE representing the opened file, or NULL if the file couldn't be opened. */
	bd_FILE * bd_fopen(const char * pathname, const char * mode);

	/** Closes the given file.
	@param stream Pointer to the bd_FILE structure representing the file to close.
	@return Zero on success, or EOF on error.*/
	int bd_fclose(bd_FILE * stream);

	/** Reads data from a file into a buffer.
	@param ptr Pointer to the buffer where the read data will be stored.
	@param size Size of each element to be read.
	@param nmemb Number of elements to be read.
	@param stream Pointer to the bd_FILE structure representing the file.
	@return The total number of elements successfully read. */
	size_t bd_fread(void * ptr, size_t size, size_t nmemb, bd_FILE * stream);

	/** Writes data from a buffer to a file.
	@param ptr Pointer to the buffer containing the data to be written.
	@param size Size of each element to be written.
	@param nmemb Number of elements to be written.
	@param stream Pointer to the bd_FILE structure representing the file.
	@return The total number of elements successfully written. */
	size_t bd_fwrite(const void * ptr, size_t size, size_t nmemb, bd_FILE * stream);

	/** Returns the current file position of the given file stream.
	@param stream Pointer to the bd_FILE structure representing the file.
	@return The current file position, or -1L on error. */
	long bd_ftell(bd_FILE * stream);

	/** Sets the file position for the given file stream.
	@param stream Pointer to the bd_FILE structure representing the file.
	@param offset The offset to move the file pointer.
	@param whence The reference point for the offset (SEEK_SET, SEEK_CUR, or SEEK_END).
	@return Zero on success, or non-zero on error. */
	int bd_fseek(bd_FILE * stream, long offset, int whence);


	/** @typedef bd_DIR
	Defines bd_DIR as an alias for berialdraw::Dir in C++. */
	typedef struct bd_DIR bd_DIR;

	/** Opens a directory.
	@param path Path to the directory.
	@return A pointer to the context or NULL if the directory could not be opened. */
	bd_DIR* bd_dir_open(const char* path);

	/** Closes the currently opened directory. */
	void bd_dir_close(bd_DIR* dir);

	/** Gets the first file in the directory.
	@param dir The context of the directory.
	@return The name of the first file, or NULL if none found. */
	const char* bd_dir_first(bd_DIR* dir);

	/** Gets the next file in the directory.
	@param dir The context of the directory.
	@return The name of the next file, or NULL if none found. */
	const char* bd_dir_next(bd_DIR* dir);

	/** Gets the size of a file.
	@param dir The context of the directory.
	@param file_name Name of the file.
	@return The size of the file in bytes. */
	size_t bd_file_size(const char* file_name);

	/** Indicates if the files exists
	@param file_name Name of the file.
	@return True if file existing. */
	bool bd_file_exists(const char* file_name);

	/** Checks if a given file is a directory.
	@param dir The context of the directory.
	@param file_name Name of the file.
	@return True if the file is a directory, false otherwise. */
	bool bd_is_directory(const char* file_name);

	/** Prints a formatted string to the standard output.
	@param format The format string (as in printf).
	@param ... Additional arguments based on the format string. */
	void bd_printf(const char * format, ...);

	#define _ bd_printf("%s:%d\n",__FILE__,__LINE__);
#if defined(__cplusplus)
}
#endif

#endif
