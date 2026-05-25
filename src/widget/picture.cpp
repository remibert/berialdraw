#include "berialdraw_imp.hpp"

using namespace berialdraw;

Picture::Picture(Widget * parent):
	Widget("picture", parent, sizeof(Picture))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (PictureStyle*)this);
}

Picture::~Picture()
{
	if (m_cached_pixels)
	{
		delete[] m_cached_pixels;
	}
}

/** Load and decode the image file */
void Picture::load_image()
{
	if (m_image_modified)
	{
		// Reset cache entry
		m_cache_entry = nullptr;

		// Free cached pixels
		if (m_cached_pixels)
		{
			delete[] m_cached_pixels;
			m_cached_pixels = nullptr;
			m_cached_width = 0;
			m_cached_height = 0;
		}

		if (m_filename.size() > 0)
		{
			m_cache_entry = UIManager::image_cache()->get(m_filename.c_str());
		}
		m_image_modified = false;
	}
}

/** Rebuild the resized cache if needed */
void Picture::rebuild_cache(uint32_t area_width, uint32_t area_height)
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
			if (m_cached_pixels)
			{
				delete[] m_cached_pixels;
				m_cached_pixels = nullptr;
			}

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

/** Return the size of content without marges */
Size Picture::content_size()
{
	Size result;

	load_image();

	if (m_cache_entry)
	{
		result.width_(m_cache_entry->width() << 6);
		result.height_(m_cache_entry->height() << 6);
	}

	return result;
}

void Picture::place(const Area & area, bool in_layout)
{
	if (!is_absolute())
	{
		in_layout = true;
	}

	// Place background rectangle
	place_in_area(area, in_layout);

	// Place all children (save/restore m_backclip so border area is preserved)
	Area backclip(m_backclip);
	Widget::place(m_foreclip, in_layout);
	m_backclip = backclip;
}

void Picture::paint(const Region & parent_region)
{
	Region region(parent_region);

	// Draw border/background
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);

		// Draw background and border
		Rect::build_focused_polygon(m_foreclip,
			*(CommonStyle*)this,
			*(BorderStyle*)this,
			stated_color(m_color),
			stated_color(m_border_color),
			Color::TRANSPARENT,
			stated_color(m_focus_color),
			m_focused);

		// Load image if needed
		load_image();

		if (m_cache_entry && m_cache_entry->pixel_data())
		{
			// Compute available area in logical pixels (64th pixels >> 6)
			Dim scale = UIManager::renderer()->scale_();
			uint32_t scale_factor = scale >> 6;
			if (scale_factor == 0) scale_factor = 1;

			uint32_t area_w_logical = (uint32_t)(m_foreclip.size().width_() >> 6);
			uint32_t area_h_logical = (uint32_t)(m_foreclip.size().height_() >> 6);

			// Cache at screen resolution for best quality with zoom
			uint32_t area_w_screen = area_w_logical * scale_factor;
			uint32_t area_h_screen = area_h_logical * scale_factor;

			if (area_w_screen > 0 && area_h_screen > 0)
			{
				// Rebuild cache at screen resolution
				rebuild_cache(area_w_screen, area_h_screen);

				if (m_cached_pixels && m_cached_width > 0 && m_cached_height > 0)
				{
					// Convert cached dimensions back to logical for centering and SVG
					uint32_t cached_w_logical = m_cached_width / scale_factor;
					uint32_t cached_h_logical = m_cached_height / scale_factor;

					// Compute offset to center the image in the area (result in 64th pixels)
					Coord offset_x = m_foreclip.position().x_();
					Coord offset_y = m_foreclip.position().y_();

					if (cached_w_logical < area_w_logical)
					{
						offset_x += (((Coord)area_w_logical - (Coord)cached_w_logical) << 5);
					}
					if (cached_h_logical < area_h_logical)
					{
						offset_y += (((Coord)area_h_logical - (Coord)cached_h_logical) << 5);
					}

					// Export image to SVG using original file if exporter active
					Exporter * exporter = UIManager::exporter();
					if (exporter)
					{
						exporter->add_image_file(
							m_filename.c_str(),
							(int32_t)(offset_x >> 6),
							(int32_t)(offset_y >> 6),
							cached_w_logical,
							cached_h_logical,
							m_alpha);
					}

					// Delegate rendering to the Renderer (1:1 since cache is at screen resolution)
					Point img_pos;
					img_pos.x_(offset_x);
					img_pos.y_(offset_y);

					UIManager::renderer()->draw_image(
						img_pos,
						m_foreclip.size(),
						Point(0, 0),
						Margin(),
						0,
						m_cached_pixels,
						m_cached_width,
						m_cached_height,
						m_alpha);
				}
			}
		}

		// Paint children
		Widget::paint(region);
	}
}

/** Get the widget hovered */
Widget * Picture::hovered(const Region & parent_region, const Point & position)
{
	Widget * result = 0;
	Region region(parent_region);
	region.intersect(m_foreclip);

	// If the widget hovered
	if(region.is_inside(position) != Region::Overlap::OUT)
	{
		result = this;
	}

	return result;
}

/** Serialize the content of widget into json */
void Picture::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	BorderStyle::serialize(it);
	PictureStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Picture::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	BorderStyle::unserialize(it);
	PictureStyle::unserialize(it);
	UIManager::invalidator()->dirty(this, Invalidator::ALL);
}

StyleCascadeMode Picture::style_cascade_mode() const
{
	return StyleCascadeMode::NONE;
}
