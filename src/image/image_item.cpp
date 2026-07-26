#include "berialdraw_imp.hpp"
using namespace berialdraw;

// Constructor
ImageItem::ImageItem(const ImageCacheEntry* source, const String & filename)
	: m_source(source), m_filename(filename)
{
}

// Destructor
ImageItem::~ImageItem()
{
	clear();
}

// Clear all cached entries
void ImageItem::clear()
{
	for (uint32_t i = 0; i < m_entries.size(); i++)
	{
		delete m_entries[i];
	}
	m_entries.clear();
}

// Find a cached entry
uint32_t ImageItem::find(Coord angle, const Size& src_size) const
{
	uint32_t idx = m_entries.size();

	for (uint32_t i = 0; i < m_entries.size(); i++)
	{
		RotatedEntry* e = m_entries[i];
		if (e->angle == angle && e->src_size == src_size)
		{
			idx = i;
			break;
		}
	}

	return idx;
}

// Resize pixels to fit dimensions (step 1)
uint32_t* ImageItem::resize_to_fit_size(const Size& fit_size, bool& out_allocated) const
{
	uint32_t* resized_pixels = nullptr;
	out_allocated = false;
	Size src_size = m_source->size();

	// Check if resize needed
	if (fit_size == src_size)
	{
		resized_pixels = const_cast<uint32_t*>(m_source->pixel_data());
	}
	else
	{
		// Apply bicubic interpolation
		resized_pixels = ImageProcessor::resize_bicubic(m_source->pixel_data(), src_size, fit_size);
		out_allocated = true;
	}

	return resized_pixels;
}

// Apply rotation transform if needed (step 2)
uint32_t* ImageItem::apply_rotation_transform(uint32_t* resized_pixels, bool resized_allocated,
	const Size& fit_size, Coord angle,
	Size& out_final_size)
{
	uint32_t* final_pixels = nullptr;
	out_final_size = fit_size;

	// Apply rotation if angle != 0
	if (angle != 0)
	{
		Size rot_size;
		final_pixels = ImageProcessor::rotate_bilinear(resized_pixels, fit_size, angle, rot_size);
		if (resized_allocated)
		{
			delete[] resized_pixels;
		}
		if (final_pixels)
		{
			out_final_size = rot_size;
		}
	}
	else
	{
		// No rotation: either take ownership or copy
		if (resized_allocated)
		{
			final_pixels = resized_pixels;
		}
		else
		{
			uint32_t count = fit_size.width() * fit_size.height();
			final_pixels = new uint32_t[count];
			for (uint32_t i = 0; i < count; i++)
			{
				final_pixels[i] = resized_pixels[i];
			}
		}
	}

	return final_pixels;
}

// Cache final entry and return pixels (step 3)
const uint32_t* ImageItem::cache_final_entry(uint32_t* final_pixels,
	const Size& final_size, Coord angle, const Size& fit_size)
{
	// Evict oldest if cache full
	if (m_entries.size() >= MAX_ENTRIES)
	{
		delete m_entries[0];
		m_entries.remove(0);
	}

	RotatedEntry* entry = new RotatedEntry();
	entry->pixels = final_pixels;
	entry->size = final_size;
	entry->angle = angle;
	entry->src_size = fit_size;
	m_entries.push_back(entry);

	return final_pixels;
}

// Get rotated pixels
const uint32_t* ImageItem::get_pixels(Coord angle, const Size& target_size,
	ImageFitMode fit_mode, Size& out_size)
{
	out_size = Size();
	const uint32_t* result = nullptr;

	// Validate inputs
	if (!m_source || !m_source->is_valid() || target_size.width() == 0 || target_size.height() == 0)
	{
		return result;
	}

	// Compute target dimensions with fit mode
	Size fit_size;
	ImageProcessor::compute_fit_size(m_source->size(), target_size, fit_mode, fit_size);

	if (fit_size.width() == 0 || fit_size.height() == 0)
	{
		return result;
	}

	// Check if cached
	uint32_t idx = find(angle, fit_size);
	if (idx < m_entries.size())
	{
		RotatedEntry* e = m_entries[idx];
		out_size = e->size;
		return e->pixels;
	}

	// Step 1: Resize to fit dimensions
	bool resized_allocated = false;
	uint32_t* resized_pixels = resize_to_fit_size(fit_size, resized_allocated);

	if (!resized_pixels)
	{
		return result;
	}

	// Step 2: Apply rotation if needed
	Size final_size;
	uint32_t* final_pixels = apply_rotation_transform(resized_pixels, resized_allocated,
		fit_size, angle, final_size);

	if (!final_pixels)
	{
		return result;
	}

	// Step 3: Cache and return
	out_size = final_size;
	return cache_final_entry(final_pixels, final_size, angle, fit_size);
}
