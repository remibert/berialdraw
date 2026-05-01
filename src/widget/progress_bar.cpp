#include "berialdraw_imp.hpp"

using namespace berialdraw;

ProgressBar::ProgressBar(Widget * parent):
	Widget("progress_bar", parent, sizeof(ProgressBar))
{
	UIManager::styles()->apply(this, (CommonStyle*)this);
	UIManager::styles()->apply(this, (WidgetStyle*)this);
	UIManager::styles()->apply(this, (BorderStyle*)this);
	UIManager::styles()->apply(this, (ProgressBarStyle*)this);
}

ProgressBar::~ProgressBar()
{
}

/** Copy all styles of the progress_bar */
void ProgressBar::copy(const ProgressBar & progress_bar)
{
	*((CommonStyle*)this) = *(CommonStyle*)(&progress_bar);
	*((WidgetStyle*)this) = *(WidgetStyle*)(&progress_bar);
	*((BorderStyle*)this) = *(BorderStyle*)(&progress_bar);
	*((ProgressBarStyle*)this) = *(ProgressBarStyle*)(&progress_bar);
}

/** Copy all styles of the progress_bar */
void ProgressBar::copy(const ProgressBar * progress_bar)
{
	if(progress_bar)
	{
		copy(*progress_bar);
	}
}
/** Return the size of content without marges */
Size ProgressBar::content_size()
{
	Size size;
	if (m_fill_size > m_track_size)
	{
		size.set_(m_fill_size,m_fill_size);
	}
	else
	{
		size.set_(m_track_size,m_track_size);
	}

	if (m_extend == Extend::EXTEND_HEIGHT)
	{
		if (size.width() %2)
		{
			size.width(size.width()+1);
		}
	}
	else 
	{
		if (size.height() %2)
		{
			size.height(size.height()+1);
		}
	}
	
	return size;
}

void ProgressBar::place(const Area & area, bool in_layout)
{
	if (!is_absolute())
	{
		in_layout = true;
	}
	place_in_area(area, in_layout);

	// If absolute place
	if (in_layout == false)
	{
		Area backclip = m_foreclip;
		Margin marg;
		Size size(content_size());

		if (m_extend == Extend::EXTEND_HEIGHT)
		{
			size.height_(m_foreclip.height_());
		}
		else
		{
			size.width_(m_foreclip.width_());
		}
		place_in_layout(backclip, size, marg, EXTEND_NONE, m_foreclip, (Align)m_align);
	}
}

Dim ProgressBar::get_bar_length(Dim length)
{
	Dim result = 0;
	check_progress_bar();
	Dim value = m_value;

	if ((m_max_value - m_min_value) > 0)
	{
		if (m_step_value > 0)
		{
			Dim delta = value % m_step_value;

			if (delta > 0)
			{
				if (delta > m_step_value>>1)
				{
					value += (m_step_value - delta);
				}
				else
				{
					value -= delta;
				}
			}
		}
		result = (value * length) / (m_max_value - m_min_value);
	}
	return result;
}

void ProgressBar::draw_track()
{
	Coord x;
	Coord y;
	Dim width;
	Dim height;

	Dim      track_size = (m_track_size>>6) % 2 ? m_track_size + 64 : m_track_size;
	Dim      track_radius = m_radius;
	uint32_t track_border_color;
	Dim      track_thickness;
	Area     track_area(m_foreclip);

	Dim      fill_size  = (m_fill_size >>6) % 2 ? m_fill_size  + 64 : m_fill_size;
	Dim      fill_radius = m_radius;
	Dim      fill_thickness;
	uint32_t fill_border_color;
	Area     fill_area(m_foreclip);

	Dim half_thickness = ((m_thickness>>7)<<6);

	// If the track is thicker than fill
	if (track_size >= fill_size)
	{
		Dim radius_dif = (track_size-fill_size) >> 1;
		
		track_thickness = m_thickness;
		fill_thickness = 0;
		
		if (radius_dif <= m_radius)
		{
			fill_radius -= radius_dif;
		}
		fill_border_color = Color::TRANSPARENT;
		track_border_color = stated_color(m_border_color);
		
		// Vertical progress bar
		if (m_extend == Extend::EXTEND_HEIGHT)
		{
			width  = substract(fill_size,half_thickness+half_thickness);
			height = get_bar_length(substract(m_foreclip.height_(), radius_dif + radius_dif + half_thickness + half_thickness));
			x      = m_foreclip.x_() + radius_dif + half_thickness;

			// Align the fill in track
			if ((m_align & Align::ALIGN_VERTICAL) == Align::ALIGN_TOP)
			{
				y = m_foreclip.y_() + radius_dif + half_thickness;
			}
			else
			{
				y = m_foreclip.y_() + substract(m_foreclip.height_(), (height+radius_dif+half_thickness));
			}
		}
		// Horizontal progress bar
		else
		{
			height  = substract(fill_size,half_thickness+half_thickness);
			width  = get_bar_length(substract(m_foreclip.width_(), radius_dif + radius_dif + half_thickness + half_thickness));
			y      = m_foreclip.y_() + radius_dif + half_thickness;

			// Align the fill in track
			if ((m_align & Align::ALIGN_HORIZONTAL) == Align::ALIGN_LEFT)
			{
				x = m_foreclip.x_() + radius_dif + half_thickness;
			}
			else
			{
				x = m_foreclip.x_() + substract(m_foreclip.width_(), (width+radius_dif+half_thickness));
			}
		}
	}
	// If the track is thinner than fill
	else if (track_size < fill_size)
	{
		Dim radius_dif = (fill_size-track_size)>>1;

		fill_thickness = m_thickness;
		track_thickness = 0;

		if (radius_dif <= m_radius)
		{
			track_radius -= radius_dif;
		}
		track_border_color = Color::TRANSPARENT;
		fill_border_color = stated_color(m_border_color);

		// Vertical progress bar
		if (m_extend == Extend::EXTEND_HEIGHT)
		{
			track_area.width_(track_size);
			track_area.x_(m_foreclip.x_()+radius_dif);

			width = fill_size;
			height = get_bar_length(m_foreclip.height_());
			x      = m_foreclip.x_();

			// Align the fill in track
			if ((m_align & Align::ALIGN_VERTICAL) == Align::ALIGN_TOP)
			{
				y = m_foreclip.y_();
			}
			else
			{
				y = m_foreclip.y_()+ substract(m_foreclip.height_(),height);
			}
		}
		// Horizontal progress bar
		else
		{
			track_area.height_(track_size);
			track_area.y_(m_foreclip.y_()+radius_dif);
			
			width = get_bar_length(m_foreclip.width_());
			height = fill_size;
			y     = m_foreclip.y_();

			// Align the fill in track
			if ((m_align & Align::ALIGN_HORIZONTAL) == Align::ALIGN_LEFT)
			{
				x = m_foreclip.x_();
			}
			else
			{
				x = m_foreclip.x_() + substract(m_foreclip.width_(),width);
			}
		}
	}
	fill_area.position().set_(x,y);
	fill_area.size().set_(width,height);

	// Draw track
	track_area.nearest_pixel();
	Rect::build_polygon(track_area, track_radius, track_thickness, 0, m_borders, stated_color(m_track_color), track_border_color);

	// Draw fill
	fill_area.nearest_pixel();
	Rect::build_polygon(fill_area, fill_radius, fill_thickness, 0, m_borders, stated_color(m_fill_color), fill_border_color);
}

void ProgressBar::paint(const Region & parent_region)
{
	Region region(parent_region);
	region.intersect(m_backclip);

	// If widget visible
	if (region.is_inside(m_backclip.position(), m_backclip.size()) != Region::OUT)
	{
		UIManager::renderer()->region(region);
		check_progress_bar();
		draw_track();
	}
}

/** Get the widget hovered */
Widget * ProgressBar::hovered(const Region & parent_region, const Point & position)
{
	Region region(parent_region);
	region.intersect(m_foreclip);

	// If the widget hovered
	if(region.is_inside(position) != Region::Overlap::OUT)
	{
		return this;
	}
	return 0;
}

/** Serialize the content of widget into json */
void ProgressBar::serialize(JsonIterator & it)
{
	it["type"] = m_classname;
	CommonStyle::serialize(it);
	WidgetStyle::serialize(it);
	BorderStyle::serialize(it);
	ProgressBarStyle::serialize(it);

}

/** Unserialize the content of widget from json */
void ProgressBar::unserialize(JsonIterator & it)
{
	CommonStyle::unserialize(it);
	WidgetStyle::unserialize(it);
	BorderStyle::unserialize(it);
	ProgressBarStyle::unserialize(it);
}

