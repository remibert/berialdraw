#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Paint a simple filled rectangle
void RectRenderer::paint_rect(const Area& area, uint32_t color)
{
	Rect rect(0);
	rect.size(area.size());
	rect.color(color);
	rect.paint(area.position(), true);
}

// Paint a rectangle with border and optional rounded corners
void RectRenderer::paint_border_round_rect(const Area& area, Dim radius,
	Dim thickness, Dim gap, uint8_t borders, uint32_t backcolor, uint32_t bordercolor,
	Dim focus_thickness)
{
	// If back color defined
	if (backcolor || (bordercolor && thickness > 0) || (focus_thickness > 0))
	{
		Size  siz = area.size();
		Point pos = area.position();
		Rect rect(0);
		rect.radius_q6(radius);

		// Reduce the thickness to avoid to exceed the size of rectangle
		thickness = min(area.size().width_q6(), min(area.size().height_q6(), thickness));

		if (focus_thickness)
		{
			rect.thickness_q6(focus_thickness);
			// Offset to place focus outside the area with gap distance
			// (compensates for the automatic t offset in paint())
			pos.move_q6(0 - focus_thickness - gap, 0 - focus_thickness - gap);

			// Increase size to cover area plus gap on both sides plus focus thickness
			siz.increase_q6((focus_thickness << 1) + gap + gap, (focus_thickness << 1) + gap + gap);

			// Adjust radius: border outer radius + gap + focus mid-radius
			rect.radius_q6(radius + (thickness >> 1) + (focus_thickness >> 1) + gap);
		}
		else
		{
			// Set the thickness - geometry is now sized to outer edge
			rect.thickness_q6(thickness);
		}

		// Set the size of rectangle
		rect.size(siz);

		// If back color defined
		if (backcolor)
		{
			rect.color(backcolor);
			rect.borders(borders | Borders::INNER_AREA);
			rect.paint(pos, true);
		}
		
		// If border defined
		if (bordercolor && (thickness || focus_thickness))
		{
			rect.color(bordercolor);
			rect.borders(borders);
			rect.paint(pos, true);
		}
	}
}

// Build a clip mask from the inner area of a rounded rectangle
void RectRenderer::build_clip_mask_rounded_rect(const Area& area, Dim radius, Dim thickness, Dim gap, uint8_t borders, ClipMask& mask)
{
	thickness = (Dim)min((Coord)area.size().width_q6(), min((Coord)area.size().height_q6(), (Coord)thickness));

	Size  siz = area.size();
	Point pos = area.position();

	Rect rect(0);

	if (thickness > 0)
	{
		if (thickness >> 1 < radius)
		{
			radius -= thickness >> 1;
			rect.radius_q6(radius);
		}
		else
		{
			rect.radius_q6(0);
		}
	}

	// If the thickness is odd, shift by half a pixel
	if ((thickness >> 6) % 2)
	{
		pos.move_q6(-32, -32);
	}

	rect.size(siz);
	rect.borders((uint8_t)(borders | Borders::INNER_AREA));
	RectRenderer::create_part(rect.m_polygon, siz, rect.m_radius, rect.m_thickness, rect.m_borders);

	UIManager::renderer()->build_clip_from_shape(rect, pos, mask);
}

// Paint a focused rounded rectangle
void RectRenderer::paint_focused_round_rect(const Area& area,
	Borders borders,
	uint32_t color,
	uint32_t border_color,
	Dim radius_q6,
	Dim thickness_q6,
	Dim focus_thickness,
	Dim focus_gap_q6,
	uint32_t focus_border_color,
	bool focused)
{
	if (color || border_color || focus_border_color)
	{
		// If focus present
		if (focused)
		{
			RectRenderer::paint_focus(area, borders, radius_q6, thickness_q6, focus_thickness, focus_gap_q6, focus_border_color);
		}
		// Draw background with border
		RectRenderer::paint_border_round_rect(area, radius_q6, thickness_q6, 0, borders, color, border_color, 0);
	}
}

// Paint a rounded rectangle
void RectRenderer::paint_round_rect(const Area& area, Dim radius_q6, uint32_t color)
{
	Rect rect(0);
	rect.size(area.size());
	rect.radius_q6(radius_q6);
	rect.color(color);
	rect.paint(area.position(), true);
}

// Paint a focus indicator around a rectangle
void RectRenderer::paint_focus(const Area& area,
	Borders borders,
	Dim radius_q6,
	Dim thickness_q6,
	Dim focus_thickness,
	Dim focus_gap_q6,
	uint32_t focus_border_color)
{
	// If focus present
	if (focus_border_color && focus_thickness)
	{
		Area focus_area(area);
		Dim reduce = focus_gap_q6 + (focus_thickness << 6);

		// According to the kind of borders
		switch ((uint8_t)borders & ALL_BORDERS)
		{
		case BOTTOM_BORDER | LEFT_BORDER | RIGHT_BORDER:
			focus_area.size().decrease_q6(0, reduce);
			focus_area.position().move_q6(0, reduce);
			break;

		case TOP_BORDER | LEFT_BORDER | RIGHT_BORDER:
			focus_area.size().decrease_q6(0, reduce);
			break;

		case LEFT_BORDER | BOTTOM_BORDER | TOP_BORDER:
			focus_area.size().decrease_q6(reduce, 0);
			break;

		case RIGHT_BORDER | BOTTOM_BORDER | TOP_BORDER:
			focus_area.size().decrease_q6(reduce, 0);
			focus_area.position().move_q6(reduce, 0);
			break;

		case BOTTOM_BORDER | LEFT_BORDER:
			focus_area.position().move_q6(0, reduce);
			focus_area.size().decrease_q6(reduce, reduce);
			break;

		case TOP_BORDER | LEFT_BORDER:
			focus_area.size().decrease_q6(reduce, reduce);
			break;

		case RIGHT_BORDER | TOP_BORDER:
			focus_area.position().move_q6(reduce, 0);
			focus_area.size().decrease_q6(reduce, reduce);
			break;

		case RIGHT_BORDER | BOTTOM_BORDER:
			focus_area.position().move_q6(reduce, reduce);
			focus_area.size().decrease_q6(reduce, reduce);
			break;

		case BOTTOM_BORDER:
			focus_area.size().decrease_q6(reduce << 1, reduce);
			focus_area.position().move_q6(reduce, reduce);
			break;

		case TOP_BORDER:
			focus_area.size().decrease_q6(reduce << 1, reduce);
			focus_area.position().move_q6(reduce, 0);
			break;

		case RIGHT_BORDER:
			focus_area.size().decrease_q6(reduce, reduce << 1);
			focus_area.position().move_q6(reduce, reduce);
			break;

		case LEFT_BORDER:
			focus_area.size().decrease_q6(reduce, reduce << 1);
			focus_area.position().move_q6(0, reduce);
			break;
		}

		// Draw focus
		RectRenderer::paint_border_round_rect(
			focus_area,           // area
			radius_q6,            // radius
			thickness_q6,         // thickness
			focus_gap_q6,         // gap
			borders,              // borders
			Color::TRANSPARENT,   // backcolor
			focus_border_color,   // bordercolor
			focus_thickness << 6  // focus_thickness
		);
	}
}

// Adapt radius for the rectangle size
void RectRenderer::adapt_radius(Coord& radius, const Size& size, uint8_t borders)
{
	bool radius_standard = true;
	// If the end have a right angle
	if (borders & Borders::RIGHT_ANGLE_END)
	{
		Borders b = (Borders)(borders & Borders::ALL_BORDERS);
		
		// According to the borders present adapt the radius
		switch((uint8_t)b)
		{
		case Borders::BOTTOM_BORDER | Borders::LEFT_BORDER | Borders::RIGHT_BORDER: 
		case Borders::TOP_BORDER    | Borders::LEFT_BORDER | Borders::RIGHT_BORDER:
			if(radius > (Coord)size.width_q6()/2)
			{
				radius = size.width_q6()/2;
			}
			if(radius > (Coord)size.height_q6())
			{
				radius = size.height_q6();
			}
			radius_standard = false;
			break;
	
		case Borders::LEFT_BORDER  | Borders::BOTTOM_BORDER | Borders::TOP_BORDER:
		case Borders::RIGHT_BORDER | Borders::BOTTOM_BORDER | Borders::TOP_BORDER:
			if(radius > (Coord)size.width_q6())
			{
				radius = size.width_q6();
			}
			if(radius > (Coord)size.height_q6()/2)
			{
				radius = size.height_q6()/2;
			}
			radius_standard = false;
			break;

		case Borders::BOTTOM_BORDER | Borders::LEFT_BORDER: 
		case Borders::TOP_BORDER    | Borders::LEFT_BORDER:
		case Borders::RIGHT_BORDER  | Borders::TOP_BORDER:
		case Borders::RIGHT_BORDER  | Borders::BOTTOM_BORDER:
			if(radius > (Coord)size.width_q6())
			{
				radius = size.width_q6();
			}
			if(radius > (Coord)size.height_q6())
			{
				radius = size.height_q6();
			}
			radius_standard = false;
			break;
		}
	}

	// If radius not adaptated previously
	if (radius_standard)
	{
		if(radius > (Coord)size.width_q6()>>1)
		{
			radius = size.width_q6()>>1;
		}
		if(radius > (Coord)size.height_q6()>>1)
		{
			radius = size.height_q6()>>1;
		}
	}
}

// Adapt thickness for the rectangle size
void RectRenderer::adapt_thickness(Coord& thickness, const Size& size)
{
	// Adapt the thickness according to the size
	if(thickness > (Coord)size.width_q6()>>1)
	{
		thickness = size.width_q6()>>1;
	}
	if(thickness > (Coord)size.height_q6()>>1)
	{
		thickness = size.height_q6()>>1;
	}
}

// Add corner to polygon
void RectRenderer::add_corner(Polygon& polygon, Coord x, Coord y, Coord radius, Dim thickness, uint32_t flags)
{
	uint32_t adding_flags = 0;

	// If the end have a right angle
	if (0)  // borders not accessible here - this is handled at call site
	{
		adding_flags = Polygon::FLAG_RIGHT_ANGLE_END;
	}
	else if ((0) == 0)  // placeholder for border check
	{
		adding_flags = Polygon::FLAG_STRAIGHT;
	}

	// This is a placeholder - the actual add_corner needs borders parameter
	polygon.arc_(x,y,radius,thickness,flags|adding_flags);
}

// Create part with inner-based coordinates
void RectRenderer::create_part(Polygon& polygon, const Size& size, Coord radius, Coord thickness, uint8_t borders)
{
	Coord adapted_radius = radius;
	Coord adapted_thickness = thickness;

	RectRenderer::adapt_radius(adapted_radius, size, borders);
	RectRenderer::adapt_thickness(adapted_thickness, size);

	Coord t = adapted_thickness>>1;
	Coord R = adapted_radius +t;
	Coord r = adapted_radius -t;
	// size is the outer size of the rectangle
	// Reduce w and h by thickness so that the geometry fits inside size
	Coord w = size.width_q6() -adapted_thickness;
	Coord h = size.height_q6() -adapted_thickness;

	polygon.clear();

	// If radius is negativ
	if (adapted_radius <= 0)
	{
		// Remove internal round and external round
		r = -adapted_thickness;
		R = 0;
	}

	// No border thickness is null : fill completly
	if (adapted_thickness == 0)
	{
		if ((borders & Borders::ALL_BORDERS) == Borders::ALL_BORDERS)
		{
			RectRenderer::no_border_rectangle(polygon, w, h, R, t, borders);
		}
		else
		{
			RectRenderer::rounded_border_rectangle(polygon, w, h, R, r, t, borders);
		}
	}
	// The ends of borders are not rounded
	else if ((borders & Borders::ROUNDED_END) == 0)
	{
		RectRenderer::right_angle_border_rectangle(polygon, w, h, R, r, t, borders);
	}
	else // The end of borders are rounded
	{
		RectRenderer::rounded_border_rectangle(polygon, w, h, R, r, t, borders);
	}
}

// Helper to add a corner to polygon (needs access to borders)
static void add_corner_with_borders(Polygon& polygon, Coord x, Coord y, Coord radius, Dim thickness, uint32_t flags, uint8_t borders)
{
	uint32_t adding_flags = 0;

	// If the end have a right angle
	if (borders & Borders::RIGHT_ANGLE_END)
	{
		adding_flags = Polygon::FLAG_RIGHT_ANGLE_END;
	}
	else if ((borders & Borders::ROUNDED_END) == 0)
	{
		adding_flags = Polygon::FLAG_STRAIGHT;
	}

	if (borders & Borders::INNER_AREA)
	{
		adding_flags |= Polygon::FLAG_INNER;
	}

	polygon.arc_(x,y,radius,thickness,flags|adding_flags);
}

// No border filled rectangle (inner-based)
void RectRenderer::no_border_rectangle(Polygon& polygon, Coord w, Coord h, Coord R, Coord t, uint8_t borders)
{
	add_corner_with_borders(polygon, -t   ,  R-t,R,R,Polygon::LEFT_TO_TOP, borders);
	add_corner_with_borders(polygon, w+t-R,   -t,R,R,Polygon::TOP_TO_RIGHT, borders);
	add_corner_with_borders(polygon, w+t  ,h+t-R,R,R,Polygon::RIGHT_TO_BOTTOM, borders);
	add_corner_with_borders(polygon, -t+R ,  h+t,R,R,Polygon::BOTTOM_TO_LEFT, borders);
	polygon.next_contour();
}

// Rounded border rectangle (inner-based)
void RectRenderer::rounded_border_rectangle(Polygon& polygon, Coord w, Coord h, Coord R, Coord r, Coord t, uint8_t borders)
{
	// If borders present (not an inner rectangle)
	if ((borders & Borders::INNER_AREA) == 0 && t > 0)
	{
		RectRenderer::no_border_rectangle(polygon, w, h, R, t, borders);
	}

	Coord delta_x_left = 0;
	Coord delta_x_right = 0;
	Coord delta_y_bottom = 0;
	Coord delta_y_top = 0;

	Coord round_top_left = 0;
	Coord round_left_bottom = 0;
	Coord round_bottom_right = 0;
	Coord round_right_top = 0;

	Coord move_top_left = 0;
	Coord move_left_bottom = 0;
	Coord move_bottom_right = 0;
	Coord move_right_top = 0;

	if ((borders & Borders::TOP_BORDER)    == 0) delta_y_top = -(t<<1);
	if ((borders & Borders::BOTTOM_BORDER) == 0) delta_y_bottom =  (t<<1);
	if ((borders & Borders::LEFT_BORDER)   == 0) delta_x_left = -(t<<1);
	if ((borders & Borders::RIGHT_BORDER)  == 0) delta_x_right =  (t<<1);

	if ((borders & (Borders::TOP_BORDER|Borders::RIGHT_BORDER)   ) == 0) 
	{
		if ((borders & Borders::INNER_AREA) && (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER))
		{
			round_right_top = -r;
			move_right_top = -r;
		}
		else
		{
			round_right_top = t+t;
			move_right_top = +t+t;
		}
	}
	if ((borders & (Borders::TOP_BORDER|Borders::LEFT_BORDER)    ) == 0)
	{
		if ((borders & Borders::INNER_AREA) && (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER))
		{
			round_top_left = -r;
			move_top_left = -r;
		}
		else
		{
			round_top_left = t+t;
			move_top_left = +t+t;
		}
	}
	if ((borders & (Borders::BOTTOM_BORDER|Borders::RIGHT_BORDER)) == 0)
	{
		if ((borders & Borders::INNER_AREA) && (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER))
		{
			round_bottom_right = -r;
			move_bottom_right =r;
		}
		else
		{
			round_bottom_right = t+t; 
			move_bottom_right = -t-t;
		}
	}
	if ((borders & (Borders::BOTTOM_BORDER|Borders::LEFT_BORDER )) == 0)
	{
		if ((borders & Borders::INNER_AREA) && (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER))
		{
			round_left_bottom = -r;
			move_left_bottom = r;
		}
		else
		{
			round_left_bottom = t+t; 
			move_left_bottom = -t-t;
		}
	}

	// Top left
	add_corner_with_borders(polygon, R-t  +delta_x_left+move_top_left, t+delta_y_top        , r+round_top_left, r+round_top_left, Polygon::TOP_TO_LEFT, borders);

	// Top right
	add_corner_with_borders(polygon, t    +delta_x_left    , h+t-R+delta_y_bottom+move_left_bottom, r+round_left_bottom, r+round_left_bottom, Polygon::LEFT_TO_BOTTOM, borders);

	// Bottom right
	add_corner_with_borders(polygon, w+t-R+delta_x_right+move_bottom_right, h-t+delta_y_bottom      , r+round_bottom_right, r+round_bottom_right, Polygon::BOTTOM_TO_RIGHT, borders);

	// Bottom left
	add_corner_with_borders(polygon, w-t  +delta_x_right    , R-t+delta_y_top+move_right_top  , r+round_right_top, r+round_right_top, Polygon::RIGHT_TO_TOP, borders);

	polygon.next_contour();
}

// Right angle border rectangle (inner-based)
void RectRenderer::right_angle_border_rectangle(Polygon& polygon, Coord w, Coord h, Coord R, Coord r, Coord t, uint8_t borders)
{
	Coord thickness = t << 1;

	// According to the borders style (remove all other flags)
	switch(borders & Borders::ALL_BORDERS)
	{
	case Borders::NO_BORDER               : 
		if ((borders & Borders::INNER_AREA) != 0)
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, -t   , r-t    , r, r, Polygon::LEFT_TO_TOP, borders);
			add_corner_with_borders(polygon, w+t-r, -t     , r, r, Polygon::TOP_TO_RIGHT, borders);
			add_corner_with_borders(polygon, w+t  , h+t-r  , r, r, Polygon::RIGHT_TO_BOTTOM, borders);
			add_corner_with_borders(polygon, -t+r , h+t    , r, r, Polygon::BOTTOM_TO_LEFT, borders);
			polygon.next_contour();
		}
		break;

	// One border
	case Borders::TOP_BORDER|Borders::BOTTOM_BORDER  : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, R-t  , -t, R, thickness, Polygon::TOP_TO_LEFT  | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, w+t-R, -t, R, thickness, Polygon::TOP_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
			add_corner_with_borders(polygon, R-t  , h+t, R, thickness, Polygon::BOTTOM_TO_LEFT  | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, w+t-R, h+t, R, thickness, Polygon::BOTTOM_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			polygon.add_point_q6(-t,t);
			polygon.add_point_q6(w+t,t);
			polygon.add_point_q6(w+t,h-t);
			polygon.add_point_q6(-t,h-t);
			polygon.next_contour();
		}
		break;

	case Borders::TOP_BORDER              : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, R-t  , -t, R, thickness, Polygon::TOP_TO_LEFT  | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, w+t-R, -t, R, thickness, Polygon::TOP_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, w+t  , h+t-r  , r, r, Polygon::RIGHT_TO_BOTTOM, borders);
			add_corner_with_borders(polygon, -t+r , h+t    , r, r, Polygon::BOTTOM_TO_LEFT, borders);
			polygon.add_point_q6(-t,t);
			polygon.add_point_q6(w+t,t);
			polygon.next_contour();
		}
		break;

	case Borders::BOTTOM_BORDER           : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, R-t  , h+t, R, thickness, Polygon::BOTTOM_TO_LEFT  | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, w+t-R, h+t, R, thickness, Polygon::BOTTOM_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, -t   , r-t    , r, r, Polygon::LEFT_TO_TOP, borders);
			add_corner_with_borders(polygon, w+t-r, -t     , r, r, Polygon::TOP_TO_RIGHT, borders);
			polygon.add_point_q6(w+t,h-t);
			polygon.add_point_q6(-t,h-t);
			polygon.next_contour();
		}
		break;

	case Borders::LEFT_BORDER|Borders::RIGHT_BORDER  :
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, w+t, R-t  , R, thickness, Polygon::RIGHT_TO_TOP    | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, w+t, h+t-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();

			add_corner_with_borders(polygon, -t, R-t  , R, thickness, Polygon::LEFT_TO_TOP    | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, -t, h+t-R, R, thickness, Polygon::LEFT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			polygon.add_point_q6(t,-t);
			polygon.add_point_q6(w-t,-t);
			polygon.add_point_q6(w-t,h+t);
			polygon.add_point_q6(t,h+t);
			polygon.next_contour();
		}
		break;

	case Borders::LEFT_BORDER             : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, -t, R-t  , R, thickness, Polygon::LEFT_TO_TOP    | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, -t, h+t-R, R, thickness, Polygon::LEFT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, w+t-r, -t     , r, r, Polygon::TOP_TO_RIGHT, borders);
			add_corner_with_borders(polygon, w+t  , h+t-r  , r, r, Polygon::RIGHT_TO_BOTTOM, borders);
			polygon.add_point_q6(t,h+t);
			polygon.add_point_q6(t,-t);
			polygon.next_contour();
		}
		break;

	case Borders::RIGHT_BORDER            : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, w+t, R-t  , R, thickness, Polygon::RIGHT_TO_TOP    | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, w+t, h+t-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, -t   , r-t    , r, r, Polygon::LEFT_TO_TOP, borders);
			polygon.add_point_q6(w-t,-t);
			polygon.add_point_q6(w-t,h+t);
			add_corner_with_borders(polygon, -t+r , h+t    , r, r, Polygon::BOTTOM_TO_LEFT, borders);
			polygon.next_contour();
		}
		break;

	// Two borders corner
	case Borders::TOP_BORDER|Borders::RIGHT_BORDER        : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, w+t-R, t   , r, r        , Polygon::TOP_TO_RIGHT    | Polygon::FLAG_INTERNAL, borders);
			add_corner_with_borders(polygon, w+t  ,h+t-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE| Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, w+t  , R-t , R, R        , Polygon::RIGHT_TO_TOP, borders);
			add_corner_with_borders(polygon, R-t  , -t  , R, thickness, Polygon::TOP_TO_LEFT     | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			polygon.add_point_q6(-t,t);
			add_corner_with_borders(polygon, w+t-R, t   , r, r        , Polygon::TOP_TO_RIGHT    | Polygon::FLAG_INTERNAL, borders);
			polygon.add_point_q6(w-t,h+t);
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, -t    , h+t-r  , r, r, Polygon::LEFT_TO_BOTTOM|Polygon::FLAG_REVERSE, borders);
			polygon.next_contour();
		}
		break;

	case Borders::TOP_BORDER|Borders::LEFT_BORDER         : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, -t   , h+t-R, R, thickness, Polygon::LEFT_TO_BOTTOM | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, t    , R-t  , r, r        , Polygon::LEFT_TO_TOP    | Polygon::FLAG_INTERNAL, borders);
			add_corner_with_borders(polygon, w+t-R, -t   , R, thickness, Polygon::TOP_TO_RIGHT   | Polygon::FLAG_REVERSE| Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, R-t  , -t   , R, R        , Polygon::TOP_TO_LEFT, borders);
			polygon.next_contour();
		}
		else
		{
			add_corner_with_borders(polygon, t    , R-t  , r, r        , Polygon::LEFT_TO_TOP    | Polygon::FLAG_INTERNAL, borders);
			polygon.add_point_q6(w+t,t);
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, w+t-r, h+t    , r, r, Polygon::BOTTOM_TO_RIGHT|Polygon::FLAG_REVERSE, borders);
			polygon.add_point_q6(t,h+t);
			polygon.next_contour();
		}
		break;

	case Borders::BOTTOM_BORDER|Borders::RIGHT_BORDER     : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, w+t  , R-t  , R, thickness, Polygon::RIGHT_TO_TOP    | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, w-t  , h+t-R, r, r        , Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_INTERNAL, borders);
			add_corner_with_borders(polygon, R-t  , h+t  , R, thickness, Polygon::BOTTOM_TO_LEFT  | Polygon::FLAG_REVERSE| Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, w+t-R, h+t  , R, R        , Polygon::BOTTOM_TO_RIGHT, borders);
			polygon.next_contour();
		}
		else
		{
			add_corner_with_borders(polygon, w-t  , h+t-R, r, r        , Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_INTERNAL, borders);
			polygon.add_point_q6(-t,h-t);
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, r-t  , -t      , r, r, Polygon::TOP_TO_LEFT|Polygon::FLAG_REVERSE, borders);
			polygon.add_point_q6(w-t,-t);
			polygon.next_contour();
		}
		break;

	case Borders::BOTTOM_BORDER|Borders::LEFT_BORDER      :
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, w+t-R, h+t  , R, thickness, Polygon::BOTTOM_TO_RIGHT| Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, R-t  , h-t  , r, r        , Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_INTERNAL, borders);
			add_corner_with_borders(polygon, -t   , R-t  , R, thickness, Polygon::LEFT_TO_TOP    | Polygon::FLAG_REVERSE| Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, -t   , h+t-R, R, R        , Polygon::LEFT_TO_BOTTOM, borders);
			polygon.next_contour();
		}
		else
		{
			add_corner_with_borders(polygon, R-t  , h-t  , r, r        , Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_INTERNAL, borders);
			polygon.add_point_q6(t,-t);
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, w+t  , r-t    , r, r, Polygon::RIGHT_TO_TOP|Polygon::FLAG_REVERSE, borders);
			polygon.add_point_q6(w+t,h-t);
			polygon.next_contour();
		}
		break;
		
	// Three borders U
	case Borders::TOP_BORDER|Borders::RIGHT_BORDER|Borders::BOTTOM_BORDER : 
		add_corner_with_borders(polygon, R-t  , -t   , R, thickness, Polygon::TOP_TO_LEFT    | Polygon::FLAG_END, borders);
		add_corner_with_borders(polygon, w+t-R, t    , r, r        , Polygon::TOP_TO_RIGHT   | Polygon::FLAG_INTERNAL, borders);
		add_corner_with_borders(polygon, w-t  , h+t-R, r, r        , Polygon::RIGHT_TO_BOTTOM| Polygon::FLAG_INTERNAL, borders);
		add_corner_with_borders(polygon, R-t  , h+t  , R, thickness, Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);

		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, w+t-R, h+t  , R, R    , Polygon::BOTTOM_TO_RIGHT, borders);
			add_corner_with_borders(polygon, w+t  , R-t  , R, R    , Polygon::RIGHT_TO_TOP, borders);
		}

		polygon.next_contour();
		break;

	case Borders::RIGHT_BORDER|Borders::BOTTOM_BORDER|Borders::LEFT_BORDER: 
		add_corner_with_borders(polygon, -t   , R-t  , R, thickness, Polygon::LEFT_TO_TOP   | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);

		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, -t   , h+t-R, R, R    , Polygon::LEFT_TO_BOTTOM, borders);
			add_corner_with_borders(polygon, w+t-R, h+t  , R, R    , Polygon::BOTTOM_TO_RIGHT, borders);
		}

		add_corner_with_borders(polygon, w+t  , R-t  , R, thickness, Polygon::RIGHT_TO_TOP   | Polygon::FLAG_END, borders);
		add_corner_with_borders(polygon, w-t  , h+t-R, r, r        , Polygon::RIGHT_TO_BOTTOM| Polygon::FLAG_INTERNAL, borders);
		add_corner_with_borders(polygon, R-t  , h-t  , r, r        , Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_INTERNAL, borders);
		polygon.next_contour();
		break;

	case Borders::BOTTOM_BORDER|Borders::LEFT_BORDER|Borders::TOP_BORDER  : 
		add_corner_with_borders(polygon, w+t-R, -t   , R, thickness, Polygon::TOP_TO_RIGHT    | Polygon::FLAG_END, borders);
		add_corner_with_borders(polygon, R-t  , t    , r, r        , Polygon::TOP_TO_LEFT, borders);
		add_corner_with_borders(polygon, t    , h+t-R, r, r        , Polygon::LEFT_TO_BOTTOM, borders);
		add_corner_with_borders(polygon, w+t-R, h+t  , R, thickness, Polygon::BOTTOM_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);

		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, R-t  , h+t  , R, R    , Polygon::BOTTOM_TO_LEFT, borders);
			add_corner_with_borders(polygon, -t   , R-t  , R, R    , Polygon::LEFT_TO_TOP, borders);
		}

		polygon.next_contour();
		break;

	case Borders::LEFT_BORDER|Borders::TOP_BORDER|Borders::RIGHT_BORDER   : 
		add_corner_with_borders(polygon, -t   , h+t-R, R, thickness, Polygon::LEFT_TO_BOTTOM  | Polygon::FLAG_END, borders); 
		add_corner_with_borders(polygon, t    , R-t  , r, r        , Polygon::LEFT_TO_TOP     | Polygon::FLAG_INTERNAL, borders);
		add_corner_with_borders(polygon, w+t-R, t    , r, r        , Polygon::TOP_TO_RIGHT    | Polygon::FLAG_INTERNAL, borders);
		add_corner_with_borders(polygon, w+t  , h+t-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE| Polygon::FLAG_END, borders);

		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, w+t  , R-t  , R, R        , Polygon::RIGHT_TO_TOP, borders);
			add_corner_with_borders(polygon, R-t  , -t   , R, R        , Polygon::TOP_TO_LEFT, borders);
		}

		polygon.next_contour();
		break;

	// All borders
	case Borders::ALL_BORDERS:
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, -t   , R-t    , R, R, Polygon::LEFT_TO_TOP, borders);
			add_corner_with_borders(polygon, w+t-R, -t     , R, R, Polygon::TOP_TO_RIGHT, borders);
			add_corner_with_borders(polygon, w+t  , h+t-R  , R, R, Polygon::RIGHT_TO_BOTTOM, borders);
			add_corner_with_borders(polygon, -t+R , h+t    , R, R, Polygon::BOTTOM_TO_LEFT, borders);
			polygon.next_contour();
		}

		add_corner_with_borders(polygon, R-t  , t      , r, r, Polygon::TOP_TO_LEFT, borders);
		add_corner_with_borders(polygon, t    , h+t-R  , r, r, Polygon::LEFT_TO_BOTTOM, borders);
		add_corner_with_borders(polygon, w+t-R, h-t    , r, r, Polygon::BOTTOM_TO_RIGHT, borders);
		add_corner_with_borders(polygon, w-t  , R-t    , r, r, Polygon::RIGHT_TO_TOP, borders);
		polygon.next_contour();
		break;
	}
}

// No border filled rectangle (outer-based)
void RectRenderer::no_border_rectangle_outer(Polygon& polygon, Coord W, Coord H, Coord R, uint8_t borders)
{
	add_corner_with_borders(polygon, 0  , R  , R, R, Polygon::LEFT_TO_TOP, borders);
	add_corner_with_borders(polygon, W-R, 0  , R, R, Polygon::TOP_TO_RIGHT, borders);
	add_corner_with_borders(polygon, W  , H-R, R, R, Polygon::RIGHT_TO_BOTTOM, borders);
	add_corner_with_borders(polygon, R  , H  , R, R, Polygon::BOTTOM_TO_LEFT, borders);
	polygon.next_contour();
}

// Rounded border rectangle (outer-based)
void RectRenderer::rounded_border_rectangle_outer(Polygon& polygon, Coord W, Coord H, Coord R, Coord r, Coord thickness, uint8_t borders)
{
	// If borders present (not an inner rectangle)
	if ((borders & Borders::INNER_AREA) == 0 && thickness > 0)
	{
		RectRenderer::no_border_rectangle_outer(polygon, W, H, R, borders);
	}

	Coord delta_x_left = 0;
	Coord delta_x_right = 0;
	Coord delta_y_bottom = 0;
	Coord delta_y_top = 0;

	Coord round_top_left = 0;
	Coord round_left_bottom = 0;
	Coord round_bottom_right = 0;
	Coord round_right_top = 0;

	Coord move_top_left = 0;
	Coord move_left_bottom = 0;
	Coord move_bottom_right = 0;
	Coord move_right_top = 0;

	if ((borders & Borders::TOP_BORDER)    == 0) delta_y_top    = -thickness;
	if ((borders & Borders::BOTTOM_BORDER) == 0) delta_y_bottom =  thickness;
	if ((borders & Borders::LEFT_BORDER)   == 0) delta_x_left   = -thickness;
	if ((borders & Borders::RIGHT_BORDER)  == 0) delta_x_right  =  thickness;

	if ((borders & (Borders::TOP_BORDER|Borders::RIGHT_BORDER)   ) == 0) 
	{
		if ((borders & Borders::INNER_AREA) && (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER))
		{
			round_right_top = -r;
			move_right_top = -r;
		}
		else
		{
			round_right_top = thickness;
			move_right_top = thickness;
		}
	}
	if ((borders & (Borders::TOP_BORDER|Borders::LEFT_BORDER)    ) == 0)
	{
		if ((borders & Borders::INNER_AREA) && (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER))
		{
			round_top_left = -r;
			move_top_left = -r;
		}
		else
		{
			round_top_left = thickness;
			move_top_left = thickness;
		}
	}
	if ((borders & (Borders::BOTTOM_BORDER|Borders::RIGHT_BORDER)) == 0)
	{
		if ((borders & Borders::INNER_AREA) && (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER))
		{
			round_bottom_right = -r;
			move_bottom_right = r;
		}
		else
		{
			round_bottom_right = thickness; 
			move_bottom_right = -thickness;
		}
	}
	if ((borders & (Borders::BOTTOM_BORDER|Borders::LEFT_BORDER )) == 0)
	{
		if ((borders & Borders::INNER_AREA) && (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER))
		{
			round_left_bottom = -r;
			move_left_bottom = r;
		}
		else
		{
			round_left_bottom = thickness; 
			move_left_bottom = -thickness;
		}
	}

	// Top left
	add_corner_with_borders(polygon, R          +delta_x_left+move_top_left,     thickness+delta_y_top,                      r+round_top_left,     r+round_top_left,     Polygon::TOP_TO_LEFT, borders);

	// Left bottom
	add_corner_with_borders(polygon, thickness  +delta_x_left,                   H-R+delta_y_bottom+move_left_bottom,        r+round_left_bottom,  r+round_left_bottom,  Polygon::LEFT_TO_BOTTOM, borders);

	// Bottom right
	add_corner_with_borders(polygon, W-R        +delta_x_right+move_bottom_right, H-thickness+delta_y_bottom,                r+round_bottom_right, r+round_bottom_right, Polygon::BOTTOM_TO_RIGHT, borders);

	// Right top
	add_corner_with_borders(polygon, W-thickness+delta_x_right,                  R+delta_y_top+move_right_top,               r+round_right_top,    r+round_right_top,    Polygon::RIGHT_TO_TOP, borders);

	polygon.next_contour();
}

// Right angle border rectangle (outer-based)
void RectRenderer::right_angle_border_rectangle_outer(Polygon& polygon, Coord W, Coord H, Coord R, Coord r, Coord thickness, uint8_t borders)
{
	// According to the borders style (remove all other flags)
	switch(borders & Borders::ALL_BORDERS)
	{
	case Borders::NO_BORDER               : 
		if ((borders & Borders::INNER_AREA) != 0)
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, 0  , r  , r, r, Polygon::LEFT_TO_TOP, borders);
			add_corner_with_borders(polygon, W-r, 0  , r, r, Polygon::TOP_TO_RIGHT, borders);
			add_corner_with_borders(polygon, W  , H-r, r, r, Polygon::RIGHT_TO_BOTTOM, borders);
			add_corner_with_borders(polygon, r  , H  , r, r, Polygon::BOTTOM_TO_LEFT, borders);
			polygon.next_contour();
		}
		break;

	// Two parallel borders
	case Borders::TOP_BORDER|Borders::BOTTOM_BORDER  : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, R  , 0, R, thickness, Polygon::TOP_TO_LEFT  | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, W-R, 0, R, thickness, Polygon::TOP_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
			add_corner_with_borders(polygon, R  , H, R, thickness, Polygon::BOTTOM_TO_LEFT  | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, W-R, H, R, thickness, Polygon::BOTTOM_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			polygon.add_point_q6(0, thickness);
			polygon.add_point_q6(W, thickness);
			polygon.add_point_q6(W, H-thickness);
			polygon.add_point_q6(0, H-thickness);
			polygon.next_contour();
		}
		break;

	case Borders::TOP_BORDER              : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, R  , 0, R, thickness, Polygon::TOP_TO_LEFT  | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, W-R, 0, R, thickness, Polygon::TOP_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, W, H-r, r, r, Polygon::RIGHT_TO_BOTTOM, borders);
			add_corner_with_borders(polygon, r, H  , r, r, Polygon::BOTTOM_TO_LEFT, borders);
			polygon.add_point_q6(0, thickness);
			polygon.add_point_q6(W, thickness);
			polygon.next_contour();
		}
		break;

	case Borders::BOTTOM_BORDER           : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, R  , H, R, thickness, Polygon::BOTTOM_TO_LEFT  | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, W-R, H, R, thickness, Polygon::BOTTOM_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, 0  , r  , r, r, Polygon::LEFT_TO_TOP, borders);
			add_corner_with_borders(polygon, W-r, 0  , r, r, Polygon::TOP_TO_RIGHT, borders);
			polygon.add_point_q6(W, H-thickness);
			polygon.add_point_q6(0, H-thickness);
			polygon.next_contour();
		}
		break;

	case Borders::LEFT_BORDER|Borders::RIGHT_BORDER  :
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, W, R  , R, thickness, Polygon::RIGHT_TO_TOP    | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, W, H-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();

			add_corner_with_borders(polygon, 0, R  , R, thickness, Polygon::LEFT_TO_TOP    | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, 0, H-R, R, thickness, Polygon::LEFT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			polygon.add_point_q6(thickness, 0);
			polygon.add_point_q6(W-thickness, 0);
			polygon.add_point_q6(W-thickness, H);
			polygon.add_point_q6(thickness, H);
			polygon.next_contour();
		}
		break;

	case Borders::LEFT_BORDER             : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, 0, R  , R, thickness, Polygon::LEFT_TO_TOP    | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, 0, H-R, R, thickness, Polygon::LEFT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, W-r, 0  , r, r, Polygon::TOP_TO_RIGHT, borders);
			add_corner_with_borders(polygon, W  , H-r, r, r, Polygon::RIGHT_TO_BOTTOM, borders);
			polygon.add_point_q6(thickness, H);
			polygon.add_point_q6(thickness, 0);
			polygon.next_contour();
		}
		break;

	case Borders::RIGHT_BORDER            : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, W, R  , R, thickness, Polygon::RIGHT_TO_TOP    | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, W, H-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, 0, r, r, r, Polygon::LEFT_TO_TOP, borders);
			polygon.add_point_q6(W-thickness, 0);
			polygon.add_point_q6(W-thickness, H);
			add_corner_with_borders(polygon, r, H, r, r, Polygon::BOTTOM_TO_LEFT, borders);
			polygon.next_contour();
		}
		break;

	// Two borders corner
	case Borders::TOP_BORDER|Borders::RIGHT_BORDER        : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, W-R       , thickness, r, r        , Polygon::TOP_TO_RIGHT    | Polygon::FLAG_INTERNAL, borders);
			add_corner_with_borders(polygon, W         , H-R      , R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE| Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, W         , R        , R, R        , Polygon::RIGHT_TO_TOP, borders);
			add_corner_with_borders(polygon, R         , 0        , R, thickness, Polygon::TOP_TO_LEFT     | Polygon::FLAG_END, borders);
			polygon.next_contour();
		}
		else
		{
			polygon.add_point_q6(0, thickness);
			add_corner_with_borders(polygon, W-R       , thickness, r, r        , Polygon::TOP_TO_RIGHT    | Polygon::FLAG_INTERNAL, borders);
			polygon.add_point_q6(W-thickness, H);
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, 0         , H-r      , r, r, Polygon::LEFT_TO_BOTTOM|Polygon::FLAG_REVERSE, borders);
			polygon.next_contour();
		}
		break;

	case Borders::TOP_BORDER|Borders::LEFT_BORDER         : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, 0         , H-R      , R, thickness, Polygon::LEFT_TO_BOTTOM | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, thickness , R        , r, r        , Polygon::LEFT_TO_TOP    | Polygon::FLAG_INTERNAL, borders);
			add_corner_with_borders(polygon, W-R       , 0        , R, thickness, Polygon::TOP_TO_RIGHT   | Polygon::FLAG_REVERSE| Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, R         , 0        , R, R        , Polygon::TOP_TO_LEFT, borders);
			polygon.next_contour();
		}
		else
		{
			add_corner_with_borders(polygon, thickness , R        , r, r        , Polygon::LEFT_TO_TOP    | Polygon::FLAG_INTERNAL, borders);
			polygon.add_point_q6(W, thickness);
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, W-r       , H        , r, r, Polygon::BOTTOM_TO_RIGHT|Polygon::FLAG_REVERSE, borders);
			polygon.add_point_q6(thickness, H);
			polygon.next_contour();
		}
		break;

	case Borders::BOTTOM_BORDER|Borders::RIGHT_BORDER     : 
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, W          , R        , R, thickness, Polygon::RIGHT_TO_TOP    | Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, W-thickness, H-R      , r, r        , Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_INTERNAL, borders);
			add_corner_with_borders(polygon, R          , H        , R, thickness, Polygon::BOTTOM_TO_LEFT  | Polygon::FLAG_REVERSE| Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, W-R        , H        , R, R        , Polygon::BOTTOM_TO_RIGHT, borders);
			polygon.next_contour();
		}
		else
		{
			add_corner_with_borders(polygon, W-thickness, H-R      , r, r        , Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_INTERNAL, borders);
			polygon.add_point_q6(0, H-thickness);
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, r          , 0        , r, r, Polygon::TOP_TO_LEFT|Polygon::FLAG_REVERSE, borders);
			polygon.add_point_q6(W-thickness, 0);
			polygon.next_contour();
		}
		break;

	case Borders::BOTTOM_BORDER|Borders::LEFT_BORDER      :
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, W-R       , H          , R, thickness, Polygon::BOTTOM_TO_RIGHT| Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, R         , H-thickness, r, r        , Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_INTERNAL, borders);
			add_corner_with_borders(polygon, 0         , R          , R, thickness, Polygon::LEFT_TO_TOP    | Polygon::FLAG_REVERSE| Polygon::FLAG_END, borders);
			add_corner_with_borders(polygon, 0         , H-R        , R, R        , Polygon::LEFT_TO_BOTTOM, borders);
			polygon.next_contour();
		}
		else
		{
			add_corner_with_borders(polygon, R         , H-thickness, r, r        , Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_INTERNAL, borders);
			polygon.add_point_q6(thickness, 0);
			if (r < 0 || (borders & Borders::RIGHT_ANGLE_END) || (borders & Borders::RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner_with_borders(polygon, W         , r          , r, r, Polygon::RIGHT_TO_TOP|Polygon::FLAG_REVERSE, borders);
			polygon.add_point_q6(W, H-thickness);
			polygon.next_contour();
		}
		break;
		
	// Three borders U
	case Borders::TOP_BORDER|Borders::RIGHT_BORDER|Borders::BOTTOM_BORDER : 
		add_corner_with_borders(polygon, R          , 0          , R, thickness, Polygon::TOP_TO_LEFT    | Polygon::FLAG_END, borders);
		add_corner_with_borders(polygon, W-R        , thickness  , r, r        , Polygon::TOP_TO_RIGHT   | Polygon::FLAG_INTERNAL, borders);
		add_corner_with_borders(polygon, W-thickness, H-R        , r, r        , Polygon::RIGHT_TO_BOTTOM| Polygon::FLAG_INTERNAL, borders);
		add_corner_with_borders(polygon, R          , H          , R, thickness, Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);

		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, W-R, H, R, R, Polygon::BOTTOM_TO_RIGHT, borders);
			add_corner_with_borders(polygon, W  , R, R, R, Polygon::RIGHT_TO_TOP, borders);
		}

		polygon.next_contour();
		break;

	case Borders::RIGHT_BORDER|Borders::BOTTOM_BORDER|Borders::LEFT_BORDER: 
		add_corner_with_borders(polygon, 0          , R          , R, thickness, Polygon::LEFT_TO_TOP   | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);

		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, 0  , H-R, R, R, Polygon::LEFT_TO_BOTTOM, borders);
			add_corner_with_borders(polygon, W-R, H  , R, R, Polygon::BOTTOM_TO_RIGHT, borders);
		}

		add_corner_with_borders(polygon, W          , R          , R, thickness, Polygon::RIGHT_TO_TOP   | Polygon::FLAG_END, borders);
		add_corner_with_borders(polygon, W-thickness, H-R        , r, r        , Polygon::RIGHT_TO_BOTTOM| Polygon::FLAG_INTERNAL, borders);
		add_corner_with_borders(polygon, R          , H-thickness, r, r        , Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_INTERNAL, borders);
		polygon.next_contour();
		break;

	case Borders::BOTTOM_BORDER|Borders::LEFT_BORDER|Borders::TOP_BORDER  : 
		add_corner_with_borders(polygon, W-R       , 0          , R, thickness, Polygon::TOP_TO_RIGHT    | Polygon::FLAG_END, borders);
		add_corner_with_borders(polygon, R         , thickness  , r, r        , Polygon::TOP_TO_LEFT, borders);
		add_corner_with_borders(polygon, thickness , H-R        , r, r        , Polygon::LEFT_TO_BOTTOM, borders);
		add_corner_with_borders(polygon, W-R       , H          , R, thickness, Polygon::BOTTOM_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END, borders);

		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, R, H, R, R, Polygon::BOTTOM_TO_LEFT, borders);
			add_corner_with_borders(polygon, 0, R, R, R, Polygon::LEFT_TO_TOP, borders);
		}

		polygon.next_contour();
		break;

	case Borders::LEFT_BORDER|Borders::TOP_BORDER|Borders::RIGHT_BORDER   : 
		add_corner_with_borders(polygon, 0          , H-R        , R, thickness, Polygon::LEFT_TO_BOTTOM  | Polygon::FLAG_END, borders); 
		add_corner_with_borders(polygon, thickness  , R          , r, r        , Polygon::LEFT_TO_TOP     | Polygon::FLAG_INTERNAL, borders);
		add_corner_with_borders(polygon, W-R        , thickness  , r, r        , Polygon::TOP_TO_RIGHT    | Polygon::FLAG_INTERNAL, borders);
		add_corner_with_borders(polygon, W          , H-R        , R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE| Polygon::FLAG_END, borders);

		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, W, R, R, R, Polygon::RIGHT_TO_TOP, borders);
			add_corner_with_borders(polygon, R, 0, R, R, Polygon::TOP_TO_LEFT, borders);
		}

		polygon.next_contour();
		break;

	// All borders
	case Borders::ALL_BORDERS:
		// If borders present (not an inner rectangle)
		if ((borders & Borders::INNER_AREA) == 0)
		{
			add_corner_with_borders(polygon, 0  , R  , R, R, Polygon::LEFT_TO_TOP, borders);
			add_corner_with_borders(polygon, W-R, 0  , R, R, Polygon::TOP_TO_RIGHT, borders);
			add_corner_with_borders(polygon, W  , H-R, R, R, Polygon::RIGHT_TO_BOTTOM, borders);
			add_corner_with_borders(polygon, R  , H  , R, R, Polygon::BOTTOM_TO_LEFT, borders);
			polygon.next_contour();
		}

		add_corner_with_borders(polygon, R          , thickness  , r, r, Polygon::TOP_TO_LEFT, borders);
		add_corner_with_borders(polygon, thickness  , H-R        , r, r, Polygon::LEFT_TO_BOTTOM, borders);
		add_corner_with_borders(polygon, W-R        , H-thickness, r, r, Polygon::BOTTOM_TO_RIGHT, borders);
		add_corner_with_borders(polygon, W-thickness, R          , r, r, Polygon::RIGHT_TO_TOP, borders);
		polygon.next_contour();
		break;
	}
}

// Create part with outer-based coordinates
void RectRenderer::create_part_outer(Polygon& polygon, const Size& size, Coord radius, Coord thickness, uint8_t borders)
{
	Coord adapted_radius = radius;
	Coord adapted_thickness = thickness;

	RectRenderer::adapt_radius(adapted_radius, size, borders);
	RectRenderer::adapt_thickness(adapted_thickness, size);

	Coord t = adapted_thickness >> 1;
	Coord R = adapted_radius + t;
	Coord r = adapted_radius - t;
	Coord W = size.width_q6();
	Coord H = size.height_q6();

	polygon.clear();

	// If radius is negativ
	if (adapted_radius <= 0)
	{
		r = -adapted_thickness;
		R = 0;
	}

	// No border thickness is null : fill completly
	if (adapted_thickness == 0)
	{
		if ((borders & Borders::ALL_BORDERS) == Borders::ALL_BORDERS)
		{
			RectRenderer::no_border_rectangle_outer(polygon, W, H, R, borders);
		}
		else
		{
			RectRenderer::rounded_border_rectangle_outer(polygon, W, H, R, r, adapted_thickness, borders);
		}
	}
	// The ends of borders are not rounded
	else if ((borders & Borders::ROUNDED_END) == 0)
	{
		RectRenderer::right_angle_border_rectangle_outer(polygon, W, H, R, r, adapted_thickness, borders);
	}
	else // The end of borders are rounded
	{
		RectRenderer::rounded_border_rectangle_outer(polygon, W, H, R, r, adapted_thickness, borders);
	}
}
