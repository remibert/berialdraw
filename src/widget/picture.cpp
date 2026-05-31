#include "berialdraw_imp.hpp"

using namespace berialdraw;

Picture::Picture(Widget * parent):
	Widget("picture", parent, sizeof(Picture))
{
	m_illustration = new Illustration(this);
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (PictureStyle*)this);
}

Picture::~Picture()
{
	if (m_illustration)
	{
		delete m_illustration;
	}
}

/** Load the illustration from the current filename */
void Picture::load_illustration()
{
	if (m_image_modified)
	{
		m_picture_placed = false;

		if (m_filename.size() > 0)
		{
			m_illustration->filename(m_filename);
			m_illustration->fit_mode(m_fit_mode);
			m_illustration->alpha(m_alpha);
			m_illustration->load();
		}
		m_image_modified = false;
	}
}

/** Compute the fit size based on image dimensions, constraints and context */
Size Picture::compute_fit_size(uint32_t img_w, uint32_t img_h, const Area & area)
{
	Size new_size;

	bool has_width  = !m_size.is_width_undefined();
	bool has_height = !m_size.is_height_undefined();

	if (has_width && has_height)
	{
		new_size.width_(m_size.width_());
		new_size.height_(m_size.height_());
	}
	else if (m_fit_mode == STRETCH)
	{
		new_size.width_(m_foreclip.size().width_());
		new_size.height_(m_foreclip.size().height_());
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

	return new_size;
}

/** Apply max_size constraints to the computed size */
void Picture::apply_max_size_constraints(Size & size)
{
	if (!m_max_size.is_width_undefined() && size.width_() > m_max_size.width_())
	{
		size.width_(m_max_size.width_());
	}
	if (!m_max_size.is_height_undefined() && size.height_() > m_max_size.height_())
	{
		size.height_(m_max_size.height_());
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

	load_illustration();

	if (m_picture_placed)
	{
		result = m_fit_content_size;
	}

	return result;
}

void Picture::place(const Area & area, bool in_layout)
{
	if (!is_absolute())
	{
		in_layout = true;
	}

	place_in_area(area, in_layout);

	if (m_illustration && m_illustration->is_loaded() && !m_picture_placed)
	{
		uint32_t img_w = m_illustration->image_width();
		uint32_t img_h = m_illustration->image_height();

		if (img_w > 0 && img_h > 0)
		{
			Size new_size = compute_fit_size(img_w, img_h, area);
			apply_max_size_constraints(new_size);

			m_fit_content_size = new_size;
			m_picture_placed = true;

			reset_scroll_position();
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

	region.intersect(m_backclip);

	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);

		Rect::build_focused_polygon(m_foreclip,
			*(CommonStyle*)this,
			*(BorderStyle*)this,
			stated_color(m_color),
			stated_color(m_border_color),
			Color::TRANSPARENT,
			stated_color(m_focus_color),
			m_focused);

		load_illustration();

		if (m_illustration && m_illustration->is_loaded())
		{
			m_illustration->paint(m_foreclip, Margin(), m_alpha, stated_color(m_color));
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
