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

		// Invalidate the fit size cache (image or fit_mode changed)
		m_picture_placed = false;

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

	if (m_picture_placed)
	{
		// After place() computed the correct size, return it
		result = m_fit_content_size;
	}
	// else: first pass returns (0,0) minimal size

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

	// Compute image content size only once (or after image change)
	if (m_cache_entry && !m_picture_placed)
	{
		uint32_t img_w = m_cache_entry->width();
		uint32_t img_h = m_cache_entry->height();

		if (img_w > 0 && img_h > 0)
		{
			Size new_size;

			// Check if explicit size constraints are set
			bool has_width  = !m_size.is_width_undefined();
			bool has_height = !m_size.is_height_undefined();

			if (has_width && has_height)
			{
				// Both dimensions explicitly set
				new_size.width_(m_size.width_());
				new_size.height_(m_size.height_());
			}
			else if (m_fit_mode == STRETCH)
			{
				// Stretch: fill the available area
				new_size.width_(m_foreclip.size().width_());
				new_size.height_(m_foreclip.size().height_());
			}
			else
			{
				// FIT: preserve aspect ratio
				// Walk up parents to find the first ScrollView/ScrollableContent
				Widget * scrollable = scrollable_content();
				ScrollViewStyle * sv_style = dynamic_cast<ScrollViewStyle*>(scrollable);

				if (sv_style)
				{
					ScrollDirection dir = sv_style->scroll_direction();

					if (dir == SCROLL_VERTICAL)
					{
						// Vertical scroll: use width as reference, compute height keeping ratio
						Dim ref_w = area.size().width_();
						if (has_width)
						{
							ref_w = m_size.width_();
						}
						uint32_t ref_w_px = (uint32_t)(ref_w >> 6);
						if (ref_w_px > 0)
						{
							uint32_t dst_h_px = (uint32_t)(((uint64_t)img_h * (uint64_t)ref_w_px) / (uint64_t)img_w);
							new_size.width_(ref_w);
							new_size.height_(dst_h_px << 6);
						}
					}
					else if (dir == SCROLL_HORIZONTAL)
					{
						// Horizontal scroll: use height as reference, compute width keeping ratio
						Dim ref_h = area.size().height_();
						if (has_height)
						{
							ref_h = m_size.height_();
						}
						uint32_t ref_h_px = (uint32_t)(ref_h >> 6);
						if (ref_h_px > 0)
						{
							uint32_t dst_w_px = (uint32_t)(((uint64_t)img_w * (uint64_t)ref_h_px) / (uint64_t)img_h);
							new_size.width_(dst_w_px << 6);
							new_size.height_(ref_h);
						}
					}
					else
					{
						// SCROLL_ALL_DIRECTIONS: use real image dimensions unless constrained
						if (has_width)
						{
							new_size.width_(m_size.width_());
						}
						else
						{
							new_size.width_(img_w << 6);
						}
						if (has_height)
						{
							new_size.height_(m_size.height_());
						}
						else
						{
							new_size.height_(img_h << 6);
						}
					}
				}
				else
				{
					// No ScrollView: adapt to available area keeping ratio
					Dim area_w = area.size().width_();
					Dim area_h = area.size().height_();
					if (has_width)
					{
						area_w = m_size.width_();
					}
					if (has_height)
					{
						area_h = m_size.height_();
					}
					uint32_t area_w_px = (uint32_t)(area_w >> 6);
					uint32_t area_h_px = (uint32_t)(area_h >> 6);

					if (area_w_px > 0 && area_h_px > 0)
					{
						uint32_t dst_w = 0, dst_h = 0;
						ImageProcessor::compute_fit_size(img_w, img_h, area_w_px, area_h_px, FIT, dst_w, dst_h);
						new_size.width_(dst_w << 6);
						new_size.height_(dst_h << 6);
					}
				}
			}

			// Apply max_size constraints
			if (!m_max_size.is_width_undefined() && new_size.width_() > m_max_size.width_())
			{
				new_size.width_(m_max_size.width_());
			}
			if (!m_max_size.is_height_undefined() && new_size.height_() > m_max_size.height_())
			{
				new_size.height_(m_max_size.height_());
			}

			m_fit_content_size = new_size;
			m_picture_placed = true;

			// Reset parent ScrollView scroll position to top-left
			// (first pass had wrong content size, scroll was centered)
			Widget * scrollable = scrollable_content();
			ScrollableContent * sc = dynamic_cast<ScrollableContent*>(scrollable);
			if (sc)
			{
				sc->scroll_position(0, 0);
			}

			// Trigger a second layout pass so content_size() returns the computed value
			Window * win = dynamic_cast<Window*>(root());
			if (win)
			{
				win->force_flow_replacement();
			}
		}
	}

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
