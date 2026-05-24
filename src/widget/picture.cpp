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
	if (m_decoder)
	{
		delete m_decoder;
	}
}

/** Load and decode the image file */
void Picture::load_image()
{
	if (m_image_modified)
	{
		// Free previous decoder
		if (m_decoder)
		{
			delete m_decoder;
			m_decoder = nullptr;
		}

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
			m_decoder = ImageDecoder::create(m_filename.c_str());
			if (m_decoder)
			{
				if (m_decoder->decode(m_filename.c_str()) == false)
				{
					delete m_decoder;
					m_decoder = nullptr;
				}
			}
		}
		m_image_modified = false;
	}
}

/** Rebuild the resized cache if needed */
void Picture::rebuild_cache(uint32_t area_width, uint32_t area_height)
{
	if (m_decoder && m_decoder->pixel_data())
	{
		uint32_t dst_width = 0;
		uint32_t dst_height = 0;

		ImageProcessor::compute_fit_size(
			m_decoder->width(),
			m_decoder->height(),
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
			if (dst_width == m_decoder->width() && dst_height == m_decoder->height())
			{
				uint32_t count = dst_width * dst_height;
				m_cached_pixels = new uint32_t[count];
				const uint32_t* src = m_decoder->pixel_data();
				for (uint32_t i = 0; i < count; i++)
				{
					m_cached_pixels[i] = src[i];
				}
			}
			else
			{
				m_cached_pixels = ImageProcessor::resize_bicubic(
					m_decoder->pixel_data(),
					m_decoder->width(),
					m_decoder->height(),
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

	if (m_decoder)
	{
		result.width_(m_decoder->width());
		result.height_(m_decoder->height());
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

	// Place all children
	Widget::place(m_foreclip, in_layout);
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

		if (m_decoder && m_decoder->pixel_data())
		{
			// Compute available area (foreclip in pixels)
			Dim scale = UIManager::renderer()->scale_();
			uint32_t area_w = (uint32_t)(m_foreclip.size().width_() / scale);
			uint32_t area_h = (uint32_t)(m_foreclip.size().height_() / scale);

			if (area_w > 0 && area_h > 0)
			{
				// Rebuild cache if area size changed
				rebuild_cache(area_w, area_h);

				if (m_cached_pixels && m_cached_width > 0 && m_cached_height > 0)
				{
					// Compute offset to center the image in the area
					Coord offset_x = m_foreclip.position().x_();
					Coord offset_y = m_foreclip.position().y_();

					if (m_cached_width < area_w)
					{
						offset_x += (((Coord)area_w - (Coord)m_cached_width) * scale) / 2;
					}
					if (m_cached_height < area_h)
					{
						offset_y += (((Coord)area_h - (Coord)m_cached_height) * scale) / 2;
					}

					// Delegate rendering to the Renderer (handles framebuf abstraction, SVG export, rotation)
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
