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

/** Free rotated image cache */
void Image::clear_cache()
{
	if (m_rotated_cache)
	{
		delete m_rotated_cache;
		m_rotated_cache = nullptr;
	}
}

/** Ensure the rotated cache exists */
void Image::ensure_cache()
{
	if (m_cache_entry && !m_rotated_cache)
	{
		m_rotated_cache = new ImageItem(m_cache_entry, m_filename);
	}
}

/** Paint the image within a clipping area */
void Image::paint(const Area & foreclip, const Margin & padding, uint8_t alpha)
{
	load();
	ensure_cache();

	if (m_rotated_cache && m_rotated_cache->is_valid())
	{
		// Compute target area
		Size target_size;
		target_size.width_(foreclip.size().width_() - padding.left_() - padding.right_());
		target_size.height_(foreclip.size().height_() - padding.top_() - padding.bottom_());

		// Base position
		Point pos;
		pos.x_(foreclip.position().x_() + padding.left_());
		pos.y_(foreclip.position().y_() + padding.top_());

		// Let the renderer handle everything: resize, rotate, center offset, and SVG export
		UIManager::renderer()->draw_image(
			pos,
			target_size,
			m_center,
			Margin(),
			m_angle,
			m_rotated_cache,
			m_fit_mode,
			alpha);
	}
}

/** Paint the image on the desktop */
void Image::paint(const Point & shift)
{
	Area foreclip;
	
	// Combine shift with image position
	Point pos(shift);
	pos.move_(m_position.x_(), m_position.y_());
	foreclip.position(pos);

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
