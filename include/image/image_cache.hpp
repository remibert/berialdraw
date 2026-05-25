#pragma once
namespace berialdraw
{
	/** Cache entry storing decoded image data */
	class ImageCacheEntry
	{
	public:
		/** Constructor */
		ImageCacheEntry();

		/** Destructor */
		~ImageCacheEntry();

		/** Get the filename associated with this entry */
		const String & filename() const { return m_filename; }

		/** Get the decoded pixel data */
		const uint32_t * pixel_data() const { return m_pixels; }

		/** Get the image width in pixels */
		uint32_t width() const { return m_width; }

		/** Get the image height in pixels */
		uint32_t height() const { return m_height; }

		/** Check if this entry contains valid data */
		bool is_valid() const { return m_pixels != nullptr; }

		/** Get the memory size of this entry in bytes */
		uint32_t memory_size() const { return m_width * m_height * 4; }

		/** Get the access counter (for LRU eviction) */
		uint32_t access_count() const { return m_access_count; }

	protected:
	/// @cond DOXYGEN_IGNORE
		friend class ImageCache;
		String    m_filename;
		uint32_t* m_pixels = nullptr;
		uint32_t  m_width = 0;
		uint32_t  m_height = 0;
		uint32_t  m_access_count = 0;
	/// @endcond
	};

	/** LRU image cache that stores decoded images to avoid re-decoding.
	When the cache exceeds its maximum memory limit, the least recently used
	entry is evicted to make room for new images. */
	class ImageCache
	{
	public:
		/** Constructor
		@param max_entries Maximum number of entries in the cache (default 64) */
		ImageCache(uint32_t max_entries = 64);

		/** Destructor */
		~ImageCache();

		/** Get a decoded image from cache, or decode and cache it
		@param filename The image file path
		@return Pointer to the cache entry, or nullptr if decoding failed */
		const ImageCacheEntry * get(const char * filename);

		/** Remove all entries from the cache */
		void clear();

		/** Get the current memory usage in bytes */
		uint32_t memory_usage() const { return m_memory_usage; }

		/** Get the maximum number of entries in the cache */
		uint32_t max_entries() const { return m_max_entries; }

		/** Set the maximum number of entries in the cache.
		If the new limit is smaller, the oldest entries are evicted. 
		@param value New maximum number of entries */
		void max_entries(uint32_t value);

		/** Get the number of entries in the cache */
		uint32_t count() const { return m_entries.size(); }

	protected:
	/// @cond DOXYGEN_IGNORE
		/** Remove operator = */
		ImageCache& operator=(const ImageCache& other) = delete;

		/** Evict the least recently used entry */
		void evict_oldest();

		/** Find an entry by filename
		@param filename The image file path
		@return Index of the entry, or size if not found */
		uint32_t find(const char * filename) const;

		Vector<ImageCacheEntry*> m_entries;
		uint32_t  m_memory_usage = 0;
		uint32_t  m_max_entries;
		uint32_t  m_access_counter = 0;
	/// @endcond
	};
}
