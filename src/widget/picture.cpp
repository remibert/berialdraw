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
	delete m_image;
	delete m_sketch;
}

/** Serialize the content of widget into json */
void Picture::serialize(JsonIterator& it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	BorderStyle::serialize(it);
	PictureStyle::serialize(it);
}

/** Unserialize the content of widget from json */
void Picture::unserialize(JsonIterator& it)
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

/** Load the raster image from the current filename */
void Picture::load_image()
{
	delete m_image;
	m_image = nullptr;

	// Instantiate appropriate decoder based on file extension
	if (FileTools::match_pattern("*.png",  m_filename, true) ||
		FileTools::match_pattern("*.jpg",  m_filename, true) || 
		FileTools::match_pattern("*.jpeg", m_filename, true))
	{
		if (m_image == nullptr)
		{
			m_image = new Image(this);
		}

		if (m_image)
		{
			m_image->filename(m_filename);
			m_image->fit_mode(m_fit_mode);
			m_image->alpha(m_alpha);
			m_image->load();
		}

		delete m_sketch;
		m_sketch = nullptr;
	}
}

/** Load the sketch from the current filename */
void Picture::load_sketch()
{
	// Instantiate appropriate decoder based on file extension
	if (FileTools::match_pattern("*.icn",  m_filename, true))
	{
		if (m_sketch == nullptr)
		{
			m_sketch = new Sketch(this);
		}
		if (m_sketch)
		{
			m_sketch->filename(m_filename);
			m_sketch->load();
		}
		delete m_image;
		m_image = nullptr;
	}
}

/** Load the image or sketch from the current filename */
void Picture::load_picture()
{
	if (m_image_modified)
	{
		m_picture_placed = false;

		if (m_filename.size() > 0)
		{
			load_image();
			load_sketch();
		}
		else
		{
			delete m_image;
			m_image = nullptr;
			delete m_sketch;
			m_sketch = nullptr;
		}
		m_image_modified = false;
	}
}

/** Compute the fit size based on image dimensions, constraints and context */
Size Picture::compute_fit_size(Size & picture_size, const Area & area)
{
	Size new_size;

	bool has_width  = !m_size.is_width_undefined();
	bool has_height = !m_size.is_height_undefined();

	if (has_width && has_height)
	{
		new_size = m_size;
	}
	else if (m_fit_mode == STRETCH)
	{
		new_size = m_foreclip.size();
	}
	else
	{
		Widget * scrollable = scrollable_content();
		ScrollViewStyle * sv_style = dynamic_cast<ScrollViewStyle*>(scrollable);

		if (sv_style)
		{
			ScrollDirection dir = sv_style->scroll_direction();

			if (dir == SCROLL_VERTICAL)
			{
				Dim ref_w = area.size().width_q6();
				if (has_width)
				{
					ref_w = m_size.width_q6();
				}
				uint32_t ref_w_px = (uint32_t)(ref_w >> 6);
				if (ref_w_px > 0)
				{
					uint32_t dst_h_px = (uint32_t)(((uint64_t)picture_size.height() * (uint64_t)ref_w_px) / (uint64_t)picture_size.width());
					new_size.width_q6(ref_w);
					new_size.height_q6(dst_h_px << 6);
				}
			}
			else if (dir == SCROLL_HORIZONTAL)
			{
				Dim ref_h = area.size().height_q6();
				if (has_height)
				{
					ref_h = m_size.height_q6();
				}
				uint32_t ref_h_px = (uint32_t)(ref_h >> 6);
				if (ref_h_px > 0)
				{
					uint32_t dst_w_px = (uint32_t)(((uint64_t)picture_size.width() * (uint64_t)ref_h_px) / (uint64_t)picture_size.height());
					new_size.width_q6(dst_w_px << 6);
					new_size.height_q6(ref_h);
				}
			}
			else
			{
				if (has_width)
				{
					new_size.width_q6(m_size.width_q6());
				}
				else
				{
					new_size.width_q6(picture_size.width() << 6);
				}
				if (has_height)
				{
					new_size.height_q6(m_size.height_q6());
				}
				else
				{
					new_size.height_q6(picture_size.height() << 6);
				}
			}
		}
		else
		{
			Dim area_w = area.size().width_q6();
			Dim area_h = area.size().height_q6();
			if (has_width)
			{
				area_w = m_size.width_q6();
			}
			if (has_height)
			{
				area_h = m_size.height_q6();
			}
			uint32_t area_w_px = (uint32_t)(area_w >> 6);
			uint32_t area_h_px = (uint32_t)(area_h >> 6);

			if (area_w_px > 0 && area_h_px > 0)
			{
				Size dst_size;
				ImageProcessor::compute_fit_size(Size(picture_size.width(), picture_size.height()), Size(area_w_px, area_h_px), FIT, dst_size);
				new_size.width_q6(dst_size.width_q6());
				new_size.height_q6(dst_size.height_q6());
			}
		}
	}

	return new_size;
}

/** Apply max_size constraints to the computed size */
void Picture::apply_max_size_constraints(Size & size)
{
	if (!m_max_size.is_width_undefined() && size.width_q6() > m_max_size.width_q6())
	{
		size.width_q6(m_max_size.width_q6());
	}
	if (!m_max_size.is_height_undefined() && size.height_q6() > m_max_size.height_q6())
	{
		size.height_q6(m_max_size.height_q6());
	}
}

/** Reset scroll position and trigger re-layout */
void Picture::reset_scroll_position()
{
	Widget * scrollable = scrollable_content();
	ScrollableContent * sc = dynamic_cast<ScrollableContent*>(scrollable);
	if (sc)
	{
		sc->scroll_position(0, 0);
	}

	Window * win = dynamic_cast<Window*>(root());
	if (win)
	{
		win->force_flow_replacement();
	}
}

/** Return the size of content without marges */
Size Picture::content_size()
{
	Size result;

	load_picture();

	if (m_picture_placed)
	{
		result = m_fit_content_size;
	}
	else
	{
		if (m_size.is_defined())
		{
			result = m_size;
		}
		else
		{
			if (m_sketch)
			{
				result = m_sketch->size();
			}
			else if (m_image)
			{
				result = m_image->image_size();
			}
			result.increase(m_padding);
			result.increase_q6(m_thickness << 1, m_thickness << 1);
		}
	}
	return result;
}

void Picture::place(const Area & area, bool in_layout)
{
	bool is_loaded = false;
	Size picture_size;

	compute_widget_placement(area, in_layout, m_thickness);

	if (m_image && m_image->is_loaded())
	{
		is_loaded = true;
		picture_size = m_image->image_size();
	}
	else if (m_sketch && m_sketch->resolution().width_q6() > 0)
	{
		is_loaded = true;
		picture_size = m_sketch->resolution();
	}

	if (is_loaded && !m_picture_placed)
	{
		if (picture_size.is_defined())
		{
			Size new_size = compute_fit_size(picture_size, area);
			apply_max_size_constraints(new_size);
			m_fit_content_size = new_size;
			m_picture_placed = true;

			reset_scroll_position();
		}
	}

	// Place all children (save/restore m_backclip so border area is preserved)
	Widget::place_children(m_contentclip, in_layout);
}

void Picture::paint(const Region & parent_region)
{
	Region region(parent_region);

	region.intersect(m_backclip);

	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Overlap::OUT)
	{
		UIManager::renderer()->region(region);

		// Draw rectangle with border
		paint_background(m_foreclip, *(CommonStyle*)this, *(BorderStyle*)this);

		// Clip content
		region.intersect(m_contentclip);
		UIManager::renderer()->region(region);

		if (m_image && m_image->is_loaded())
		{
			m_image->paint(m_contentclip, Margin(), m_alpha);
		}
		else if (m_sketch && m_sketch->resolution().width_q6() > 0)
		{
			uint32_t color = parent_focus_color(stated_color(m_icon_color));
			m_sketch->size(m_contentclip.size());
			m_sketch->paint(m_contentclip, color);
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
	if(region.is_inside(position) != Overlap::OUT)
	{
		result = this;
	}
	return result;
}
