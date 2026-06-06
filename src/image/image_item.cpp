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
uint32_t ImageItem::find(Coord angle, uint32_t src_w, uint32_t src_h) const
{
	uint32_t idx = m_entries.size();

	for (uint32_t i = 0; i < m_entries.size(); i++)
	{
		RotatedEntry* e = m_entries[i];
		if (e->angle == angle && e->src_width == src_w && e->src_height == src_h)
		{
			idx = i;
			break;
		}
	}

	return idx;
}

// Resize pixels to fit dimensions (step 1)
uint32_t* ImageItem::resize_to_fit_size(uint32_t fit_width, uint32_t fit_height, bool& out_allocated) const
{
	uint32_t* resized_pixels = nullptr;
	out_allocated = false;

	// Check if resize needed
	if (fit_width == m_source->width() && fit_height == m_source->height())
	{
		resized_pixels = const_cast<uint32_t*>(m_source->pixel_data());
	}
	else
	{
		// Apply bicubic interpolation
		resized_pixels = ImageProcessor::resize_bicubic(m_source->pixel_data(),
			m_source->width(), m_source->height(), fit_width, fit_height);
		out_allocated = true;
	}

	return resized_pixels;
}

// Apply rotation transform if needed (step 2)
uint32_t* ImageItem::apply_rotation_transform(uint32_t* resized_pixels, bool resized_allocated,
	uint32_t fit_width, uint32_t fit_height, Coord angle,
	uint32_t& out_final_width, uint32_t& out_final_height)
{
	uint32_t* final_pixels = nullptr;
	out_final_width = fit_width;
	out_final_height = fit_height;

	// Apply rotation if angle != 0
	if (angle != 0)
	{
		uint32_t rot_w = 0, rot_h = 0;
		final_pixels = ImageProcessor::rotate_bilinear(resized_pixels, fit_width, fit_height, angle, rot_w, rot_h);
		if (resized_allocated)
		{
			delete[] resized_pixels;
		}
		if (final_pixels)
		{
			out_final_width = rot_w;
			out_final_height = rot_h;
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
			uint32_t count = fit_width * fit_height;
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
	uint32_t final_width, uint32_t final_height, Coord angle, uint32_t fit_width, uint32_t fit_height)
{
	// Evict oldest if cache full
	if (m_entries.size() >= MAX_ENTRIES)
	{
		delete m_entries[0];
		m_entries.remove(0);
	}

	RotatedEntry* entry = new RotatedEntry();
	entry->pixels = final_pixels;
	entry->width = final_width;
	entry->height = final_height;
	entry->angle = angle;
	entry->src_width = fit_width;
	entry->src_height = fit_height;
	m_entries.push_back(entry);

	return final_pixels;
}

// Get rotated pixels
const uint32_t* ImageItem::get_pixels(Coord angle, uint32_t target_width, uint32_t target_height,
	ImageFitMode fit_mode, uint32_t& out_width, uint32_t& out_height)
{
	out_width = 0;
	out_height = 0;
	const uint32_t* result = nullptr;

	// Validate inputs
	if (!m_source || !m_source->is_valid() || target_width == 0 || target_height == 0)
	{
		return result;
	}

	// Compute target dimensions with fit mode
	uint32_t fit_width = 0, fit_height = 0;
	ImageProcessor::compute_fit_size(m_source->width(), m_source->height(),
		target_width, target_height, fit_mode, fit_width, fit_height);

	if (fit_width == 0 || fit_height == 0)
	{
		return result;
	}

	// Check if cached
	uint32_t idx = find(angle, fit_width, fit_height);
	if (idx < m_entries.size())
	{
		RotatedEntry* e = m_entries[idx];
		out_width = e->width;
		out_height = e->height;
		return e->pixels;
	}

	// Step 1: Resize to fit dimensions
	bool resized_allocated = false;
	uint32_t* resized_pixels = resize_to_fit_size(fit_width, fit_height, resized_allocated);

	if (!resized_pixels)
	{
		return result;
	}

	// Step 2: Apply rotation if needed
	uint32_t final_width = 0, final_height = 0;
	uint32_t* final_pixels = apply_rotation_transform(resized_pixels, resized_allocated,
		fit_width, fit_height, angle, final_width, final_height);

	if (!final_pixels)
	{
		return result;
	}

	// Step 3: Cache and return
	out_width = final_width;
	out_height = final_height;
	return cache_final_entry(final_pixels, final_width, final_height, angle, fit_width, fit_height);
}
