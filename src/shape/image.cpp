#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor for image on a canvas */
Image::Image(Canvas * canvas) :
	Shape(canvas, sizeof(Image)),
	m_polygon(0),
	m_parent(canvas)
{
}

/** Constructor for image on a widget */
Image::Image(Widget * widget) :
	Shape(widget, sizeof(Image)),
	m_polygon(0),
	m_parent(widget)
{
}

/** Copy constructor */
Image::Image(const Image & other):
	Shape(other),
	m_polygon(0),
	m_filename(other.m_filename),
	m_fit_mode(other.m_fit_mode),
	m_alpha(other.m_alpha)
{
}

/** Destructor */
Image::~Image()
{
	clear_cache();
}

/** Get the filename value */
const String & Image::filename() const
{
	return m_filename;
}

/** Set filename value with string */
void Image::filename(const String & s)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	m_filename = s;
	m_image_modified = true;
	m_cache_entry = nullptr;
	clear_cache();
}

/** Get the fit mode */
ImageFitMode Image::fit_mode() const
{
	return m_fit_mode;
}

/** Set the fit mode */
void Image::fit_mode(ImageFitMode mode)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	m_fit_mode = mode;
	clear_cache();
}

/** Get the alpha value */
uint8_t Image::alpha() const
{
	return m_alpha;
}

/** Set the alpha value */
void Image::alpha(uint8_t a)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::REDRAW);
	m_alpha = a;
}

/** Get the image width in pixels */
uint32_t Image::image_width() const
{
	uint32_t result = 0;
	if (m_cache_entry)
	{
		result = m_cache_entry->width();
	}
	return result;
}

/** Get the image height in pixels */
uint32_t Image::image_height() const
{
	uint32_t result = 0;
	if (m_cache_entry)
	{
		result = m_cache_entry->height();
	}
	return result;
}

/** Check if the image is loaded */
bool Image::is_loaded() const
{
	return m_cache_entry != nullptr && m_cache_entry->is_valid();
}

/** Load the image from filename */
bool Image::load()
{
	bool result = false;

	if (m_image_modified && m_filename.size() > 0)
	{
		m_cache_entry = UIManager::image_cache()->get(m_filename.c_str());
		m_image_modified = false;
		clear_cache();
	}

	if (m_cache_entry && m_cache_entry->is_valid())
	{
		result = true;
	}

	return result;
}

/** Free cached pixels */
void Image::clear_cache()
{
	if (m_cached_pixels)
	{
		delete[] m_cached_pixels;
		m_cached_pixels = nullptr;
	}
	m_cached_width = 0;
	m_cached_height = 0;
}

/** Rebuild the resized cache if needed */
void Image::rebuild_cache(uint32_t area_width, uint32_t area_height)
{
	if (m_cache_entry && m_cache_entry->pixel_data())
	{
		uint32_t dst_width = 0;
		uint32_t dst_height = 0;

		ImageProcessor::compute_fit_size(
			m_cache_entry->width(),
			m_cache_entry->height(),
			area_width,
			area_height,
			m_fit_mode,
			dst_width,
			dst_height);

		// Rebuild only if size changed
		if (dst_width != m_cached_width || dst_height != m_cached_height)
		{
			clear_cache();

			// If same size as source, just copy
			if (dst_width == m_cache_entry->width() && dst_height == m_cache_entry->height())
			{
				uint32_t count = dst_width * dst_height;
				m_cached_pixels = new uint32_t[count];
				const uint32_t* src = m_cache_entry->pixel_data();
				for (uint32_t i = 0; i < count; i++)
				{
					m_cached_pixels[i] = src[i];
				}
			}
			else
			{
				m_cached_pixels = ImageProcessor::resize_bicubic(
					m_cache_entry->pixel_data(),
					m_cache_entry->width(),
					m_cache_entry->height(),
					dst_width,
					dst_height);
			}
			m_cached_width = dst_width;
			m_cached_height = dst_height;
		}
	}
}

/** Paint the image within a clipping area */
void Image::paint(const Area & foreclip, const Margin & padding, uint8_t alpha)
{
	load();

	if (m_cache_entry && m_cache_entry->pixel_data())
	{
		Dim area_w = foreclip.size().width_() - padding.left_() - padding.right_();
		Dim area_h = foreclip.size().height_() - padding.top_() - padding.bottom_();

		uint32_t area_w_px = (uint32_t)(area_w >> 6);
		uint32_t area_h_px = (uint32_t)(area_h >> 6);

		// Account for renderer scale (HiDPI)
		Dim scale = UIManager::renderer()->scale_();
		uint32_t scale_factor = scale >> 6;
		if (scale_factor == 0)
		{
			scale_factor = 1;
		}

		uint32_t area_w_screen = area_w_px * scale_factor;
		uint32_t area_h_screen = area_h_px * scale_factor;

		if (area_w_screen > 0 && area_h_screen > 0)
		{
			rebuild_cache(area_w_screen, area_h_screen);

			if (m_cached_pixels && m_cached_width > 0 && m_cached_height > 0)
			{
				// Convert cached dimensions back to logical for centering
				uint32_t cached_w_logical = m_cached_width / scale_factor;
				uint32_t cached_h_logical = m_cached_height / scale_factor;

				// Compute offset to center the image in the area
				Coord offset_x = foreclip.position().x_() + padding.left_();
				Coord offset_y = foreclip.position().y_() + padding.top_();

				if (cached_w_logical < area_w_px)
				{
					offset_x += (((Coord)area_w_px - (Coord)cached_w_logical) << 5);
				}
				if (cached_h_logical < area_h_px)
				{
					offset_y += (((Coord)area_h_px - (Coord)cached_h_logical) << 5);
				}

				// Export image to SVG if exporter active
				Exporter * exporter = UIManager::exporter();
				if (exporter)
				{
					exporter->add_image_file(
						m_filename.c_str(),
						(int32_t)(offset_x >> 6),
						(int32_t)(offset_y >> 6),
						cached_w_logical,
						cached_h_logical,
						alpha);
				}

				// Render the image
				Point img_pos;
				img_pos.x_(offset_x);
				img_pos.y_(offset_y);

				UIManager::renderer()->draw_image(
					img_pos,
					foreclip.size(),
					Point(0, 0),
					Margin(),
					0,
					m_cached_pixels,
					m_cached_width,
					m_cached_height,
					alpha);
			}
		}
	}
}

/** Paint the image on the desktop */
void Image::paint(const Point & shift)
{
	Area foreclip;
	foreclip.position(shift);

	// Use explicit size if set, otherwise use image native size
	if (!m_size.is_width_undefined() && !m_size.is_height_undefined())
	{
		foreclip.size(m_size);
	}
	else
	{
		load();
		if (m_cache_entry)
		{
			Size s;
			s.width_(m_cache_entry->width() << 6);
			s.height_(m_cache_entry->height() << 6);
			foreclip.size(s);
		}
	}

	const Margin padding;
	Image::paint(foreclip, padding, m_alpha);
}

/** Get the content size of the shape */
Size Image::content_size()
{
	Size result;

	load();

	if (m_cache_entry)
	{
		if (!m_size.is_width_undefined())
		{
			result.width_(m_size.width_());
		}
		else
		{
			result.width_(m_cache_entry->width() << 6);
		}

		if (!m_size.is_height_undefined())
		{
			result.height_(m_size.height_());
		}
		else
		{
			result.height_(m_cache_entry->height() << 6);
		}
	}

	return result;
}
