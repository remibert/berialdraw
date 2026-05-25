#include "berialdraw_imp.hpp"

using namespace berialdraw;

// ImageCacheEntry

ImageCacheEntry::ImageCacheEntry()
{
}

ImageCacheEntry::~ImageCacheEntry()
{
	if (m_pixels)
	{
		delete[] m_pixels;
		m_pixels = nullptr;
	}
}

// ImageCache

ImageCache::ImageCache(uint32_t max_entries) :
	m_max_entries(max_entries)
{
}

ImageCache::~ImageCache()
{
	clear();
}

/** Find an entry by filename */
uint32_t ImageCache::find(const char * filename) const
{
	uint32_t result = m_entries.size();

	if (filename)
	{
		for (uint32_t i = 0; i < m_entries.size(); i++)
		{
			if (m_entries[i]->m_filename == filename)
			{
				result = i;
				break;
			}
		}
	}

	return result;
}

/** Evict the least recently used entry */
void ImageCache::evict_oldest()
{
	if (m_entries.size() > 0)
	{
		// Find entry with lowest access count
		uint32_t oldest = 0;
		uint32_t oldest_access = m_entries[0]->m_access_count;

		for (uint32_t i = 1; i < m_entries.size(); i++)
		{
			if (m_entries[i]->m_access_count < oldest_access)
			{
				oldest = i;
				oldest_access = m_entries[i]->m_access_count;
			}
		}

		// Free the evicted entry
		m_memory_usage -= m_entries[oldest]->memory_size();
		delete m_entries[oldest];
		m_entries.remove(oldest);
	}
}

/** Set the maximum number of entries in the cache */
void ImageCache::max_entries(uint32_t value)
{
	m_max_entries = value;

	// Evict oldest entries until we fit within the new limit
	while (m_entries.size() > m_max_entries)
	{
		evict_oldest();
	}
}

/** Get a decoded image from cache, or decode and cache it */
const ImageCacheEntry * ImageCache::get(const char * filename)
{
	const ImageCacheEntry * result = nullptr;

	if (filename)
	{
		// Search for existing entry
		uint32_t index = find(filename);

		if (index < m_entries.size())
		{
			// Cache hit: update access counter
			m_access_counter++;
			m_entries[index]->m_access_count = m_access_counter;
			result = m_entries[index];
		}
		else
		{
			// Cache miss: decode the image
			ImageDecoder * decoder = ImageDecoder::create(filename);
			if (decoder)
			{
				if (decoder->decode(filename))
				{
					uint32_t w = decoder->width();
					uint32_t h = decoder->height();
					uint32_t img_size = w * h * 4;

					// Evict until we fit within the entry limit
					while (m_entries.size() >= m_max_entries && m_entries.size() > 0)
					{
						evict_oldest();
					}

					// Copy pixel data
					uint32_t pixel_count = w * h;
					uint32_t * pixels = new uint32_t[pixel_count];
					const uint32_t * src = decoder->pixel_data();
					for (uint32_t i = 0; i < pixel_count; i++)
					{
						pixels[i] = src[i];
					}

					// Create and store new entry
					ImageCacheEntry * entry = new ImageCacheEntry();
					m_access_counter++;
					entry->m_filename = filename;
					entry->m_pixels = pixels;
					entry->m_width = w;
					entry->m_height = h;
					entry->m_access_count = m_access_counter;
					m_memory_usage += img_size;
					m_entries.push_back(entry);
					result = entry;
				}
				delete decoder;
			}
		}
	}

	return result;
}

/** Remove all entries from the cache */
void ImageCache::clear()
{
	for (uint32_t i = 0; i < m_entries.size(); i++)
	{
		delete m_entries[i];
	}
	m_entries.clear();
	m_memory_usage = 0;
	m_access_counter = 0;
}
