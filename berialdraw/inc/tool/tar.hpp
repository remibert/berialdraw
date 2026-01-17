#pragma once

namespace berialdraw
{

	#if defined(_MSC_VER)
	#pragma pack(push, 1)
	#endif
			struct TarHeader
			{
				char name[100];
				char mode[8];
				char uid[8];
				char gid[8];
				char size[12];
				char mtime[12];
				char chksum[8];
				char file_type;
				char linkname[100];
				char magic[8];
				char uname[32];
				char gname[32];
				char devmajor[8];
				char devminor[8];
				char pad[167];
			}
	#if defined(__GNUC__) || defined(__clang__)
			__attribute__((packed))
	#endif
			;
	#if defined(_MSC_VER)
	#pragma pack(pop)
	#endif

	/** Tar file reading class */
	class Tar : public Stream
	{
	public:
		/** Result codes for tar operations */
		enum class TarResult
		{
			EndOfFile = -3,
			Failure = -2,
			NotFound = -1,
			Success = 0,
			Found = 1
		};

		/** File type constants */
		static const uint8_t TAR_TYPE_NORMAL_FILE = '0';
		static const uint8_t TAR_TYPE_HARD_LINK = '1';
		static const uint8_t TAR_TYPE_SYMBOLIC_LINK = '2';
		static const uint8_t TAR_TYPE_CHAR_FILE = '3';
		static const uint8_t TAR_TYPE_BLOCK_FILE = '4';
		static const uint8_t TAR_TYPE_DIRECTORY = '5';
		static const uint8_t TAR_TYPE_NAMED_PIPE = '6';
		static const uint8_t TAR_TYPE_CONTIGUOUS = '7';

		static const uint32_t TAR_RECORDSIZE = 512;

		Tar();
		virtual ~Tar();

		/** Open a tar archive
		@param pathname path and filename of the tar file
		@return TarResult::Success if success, TarResult::Failure if failed */
		TarResult open(const char* pathname);

		/** Open a tar archive
		@param pathname path and filename of the tar file
		@return TarResult::Success if success, TarResult::Failure if failed */
		TarResult open(const String& pathname);

		/** Close the tar archive
		@return TarResult::Success if success, TarResult::Failure if failed */
		TarResult close();

		/** Read data from the current file in the archive
		@param ptr pointer on data
		@param size size in bytes of data
		@return the number of bytes read, or -1 if an error occurred */
		virtual int read(void *ptr, uint32_t size);

		/** Writes data into the stream (not supported for tar)
		@return -1 always */
		virtual int write(const void *data, uint32_t size);

		/** Give the position in the current file
		@return file offset */
		virtual long tell();

		/** Repositions the offset in the current file
		@param offset offset according to whence definition
		@param whence SEEK_SET, SEEK_CUR, SEEK_END
		@return resulting offset or -1 if error */
		virtual int seek(long offset, int whence);

		/** Return the size of the current file
		@return size in bytes */
		virtual uint32_t size();

		/** Open directory listing of tar archive
		@param pathname path to tar file
		@return TarResult::Success if success, TarResult::Failure if failed */
		TarResult dir_open(const char* pathname);

		/** Open directory listing of tar archive
		@param pathname path to tar file
		@return TarResult::Success if success, TarResult::Failure if failed */
		TarResult dir_open(const String& pathname);

		/** Close directory listing
		@return TarResult::Success if success, TarResult::Failure if failed */
		TarResult dir_close();

		/** Read next file in archive
		@return 0 if success, -1 if at end of archive */
		int dir_next();

		/** Get the filename of the current entry
		@return filename as String, or empty String if no file selected */
		const String& dir_get_filename() const;

		/** Get the size of the current file
		@return size in bytes, or -1 if no file selected */
		long dir_get_size() const;

		/** Get the offset in the archive of the current file
		@return offset in bytes, or -1 if no file selected */
		long dir_get_offset() const;

		/** Check if current entry is a directory
		@return true if directory, false otherwise */
		bool dir_is_directory() const;

		/** Check if current entry is a file
		@return true if file, false otherwise */
		bool dir_is_file() const;

		/** Index entry for tar files */
		struct TarIndexEntry
		{
			uint32_t fileSize;
			uint64_t fileTime;
			long offset;
		};

		std::unique_ptr<Stream> m_file;    ///< Stream to tar file
		uint32_t m_size;                  ///< Size of current file in archive
		uint32_t m_position;              ///< Current position in file
		long m_begin_tar;                 ///< Start position of current tar entry
		long m_begin_file;                ///< Start position of current file data
		String m_filename;                ///< Current filename
		uint8_t m_file_type;              ///< Type of current entry
		uint64_t m_time;                  ///< Last modified time
		bool m_is_open;                   ///< Archive is open
		bool m_is_dir_mode;               ///< Directory iteration mode
		std::map<String, TarIndexEntry> m_index; ///< Index of files in archive

		/** Read tar header and parse it
		@param header pointer to header structure to fill
		@param search_filename optional filename to search for
		@return TarResult code */
		TarResult read_header(TarHeader& header, const char* search_filename = nullptr);

		/** Align size to TAR record size (512 bytes)
		@param size size to align
		@return aligned size */
		static uint32_t align_size(uint32_t size);

		/** Build the index of files in archive
		@param tar_offset optional offset to start indexing from
		@return TarResult::Success if success, TarResult::Failure if failed */
		TarResult build_index(long tar_offset = 0);

		/** Find a file in the archive
		@param filename filename to search for
		@param file_opened if true, searches for exact file; if false, allows directory mode
		@param begin_tar offset to start search from
		@param out_split_filename filename split position (for nested archives)
		@param out_begin_tar offset where file was found
		@return true if found, false otherwise */
		bool find_file(const char* filename, bool file_opened, long begin_tar,
					  uint32_t& out_split_filename, long& out_begin_tar);

		/** Compare two paths (case-insensitive, handles / and \)
		@param path1 first path
		@param path2 second path
		@return true if paths are equal */
		static bool compare_paths(const char* path1, const char* path2);

		/** Convert octal string to unsigned long
		@param str octal string
		@return converted value */
		static uint32_t octal_to_ulong(const char* str);

#ifdef _DEBUG
		/** Unitary tests */
		static void test();
		static void test1();
		static void test2();
		static void test3();
		static void test4();
		static void test5();
		static void test6();
		static void test7();
		static void test8();
		static void test9();
		static void test10();
		static void test11();
		static void test12();
#endif
	};
}
