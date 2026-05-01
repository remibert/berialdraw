#include "berialdraw_imp.hpp"

using namespace berialdraw;

Rect::Rect(Canvas * canvas) : 
	Shape(canvas, sizeof(Rect)),
	m_polygon(0)
{
	m_thickness = 0;
}

Rect::Rect(const Rect & other):
	Shape(other),
	RoundStyle(other),
	m_polygon(0)
{
}

Coord round_(Coord value)
{
	if ((value % 64) != 0)
	{
		value = ((value + 33) >> 6) << 6;
	}
	return value;
}

void Rect::build_polygon(const Area & area, Dim radius, 
	Dim thickness, Dim gap, uint8_t borders, uint32_t backcolor, uint32_t bordercolor, 
	Dim focus_thickness)
{
	thickness = min(area.size().width_(), min(area.size().height_(), thickness));

	Size  siz = area.size();
	Point pos = area.position();
	Coord delta;

	Rect rect(0);
		rect.radius_(radius);

	if (focus_thickness)
	{
		delta = thickness>>1;
		pos.move_(-delta - gap-(focus_thickness>>1),-delta - gap-(focus_thickness>>1));

		delta = (thickness)>>7;
		siz.decrease(delta<<1,delta<<1);
		delta = (thickness<<1)+gap+gap+focus_thickness;
		siz.increase_(delta,delta);
		rect.thickness_(focus_thickness);

		rect.radius_(radius +  (thickness>>1) + (focus_thickness>>1) + gap);

		// If the focus thickness is odd
		if ((focus_thickness >> 6) % 2)
		{
			if (((pos.x_() >> 5) & 1) == 0)
			{
				// Shift by half a pixel to have a thinner line
				pos.move_(-32,0);
			}
			if (((pos.y_() >> 5) & 1) == 0)
			{
				// Shift by half a pixel to have a thinner line
				pos.move_(0,-32);
			}
		}
		else
		{
			if (((pos.x_() >> 5) & 1) == 1)
			{
				// Shift by half a pixel to have a thinner line
				pos.move_(-32,0);
			}
			if (((pos.y_() >> 5) & 1) == 1)
			{
				// Shift by half a pixel to have a thinner line
				pos.move_(0,-32);
			}
		}
	}
	else
	{
		rect.thickness_(thickness);
		delta = (thickness)>>7;
		siz.decrease(delta<<1,delta<<1);
		siz.increase_(thickness,thickness);

		// If the thickness is odd
		if ((thickness >> 6) % 2)
		{
			// Shift by half a pixel to have a thinner line
			pos.move_(-32,-32);
		}
	}

	rect.size(siz);

	if(backcolor)
	{
		rect.color(backcolor);
		rect.borders(borders|Borders::INNER_AREA);
		rect.paint(pos,true);
	}
	if(bordercolor && (thickness || focus_thickness))
	{
		rect.color(bordercolor);
		rect.borders(borders);
		rect.paint(pos,true);
	}
}

void Rect::build_focused_polygon(const Area & area, 
	const CommonStyle & common_style,
	const BorderStyle & border_style,
	uint32_t color,
	uint32_t border_color,
	uint32_t focus_color,
	uint32_t focus_border_color,
	bool focused)
{
	Borders borders = (Borders)common_style.borders();
	if (focused && border_style.focus_thickness())
	{
		Area focus_area(area);
		Dim reduce = (border_style.focus_gap()<<6) +(border_style.focus_thickness()<<6);
		switch((uint8_t)borders & ALL_BORDERS)
		{
		case BOTTOM_BORDER | LEFT_BORDER | RIGHT_BORDER:
			focus_area.size().decrease_(0,reduce);
			focus_area.position().move_(0,reduce);
			break;

		case TOP_BORDER    | LEFT_BORDER | RIGHT_BORDER:
			focus_area.size().decrease_(0,reduce);
			break;
		
		case LEFT_BORDER  | BOTTOM_BORDER | TOP_BORDER:
				focus_area.size().decrease_(reduce,0);
			break;

		case RIGHT_BORDER | BOTTOM_BORDER | TOP_BORDER:
			focus_area.size().decrease_(reduce,0);
			focus_area.position().move_(reduce,0);
			break;
	
		case BOTTOM_BORDER | LEFT_BORDER:
			focus_area.position().move_(0,reduce);
			focus_area.size().decrease_(reduce,reduce);
			break;

		case TOP_BORDER    | LEFT_BORDER:
			focus_area.size().decrease_(reduce,reduce);
			break;

		case RIGHT_BORDER  | TOP_BORDER:
			focus_area.position().move_(reduce,0);
			focus_area.size().decrease_(reduce,reduce);
			break;

		case RIGHT_BORDER  | BOTTOM_BORDER:
			focus_area.position().move_(reduce,reduce);
			focus_area.size().decrease_(reduce,reduce);
			break;

		case BOTTOM_BORDER:
			focus_area.size().decrease_(reduce<<1,reduce);
			focus_area.position().move_(reduce,reduce);
			break;

		case TOP_BORDER:
			focus_area.size().decrease_(reduce<<1,reduce);
			focus_area.position().move_(reduce,0);
			break;

		case RIGHT_BORDER:
			focus_area.size().decrease_(reduce,reduce<<1);
			focus_area.position().move_(reduce,reduce);
			break;

		case LEFT_BORDER:
			focus_area.size().decrease_(reduce,reduce<<1);
			focus_area.position().move_(0,reduce);
			break;
		}

		// Draw focus
		Rect::build_polygon(focus_area, border_style.radius_(), border_style.thickness_(), border_style.focus_gap()<<6, borders, focus_color, focus_border_color, border_style.focus_thickness()<<6);
	}
	// Draw background
	Rect::build_polygon(area, border_style.radius_(), border_style.thickness_(), 0, borders, color, border_color);
}


// Render outline
void Rect::paint(const Point & shift)
{
	paint(shift,false);
}

// Render outline
void Rect::paint(const Point & shift, bool in_widget)
{
	if (m_radius == 0 && m_thickness == 0 && m_angle == 0 && 
		m_center.x_() == 0 && m_center.y_() == 0 && UIManager::exporter() == 0)
	{
		Point move(shift);
		move.move(m_position);
		UIManager::renderer()->fillrect(move.x(),move.y(), m_size.width(),m_size.height(), color());
	}
	else
	{
		Point move(shift);
		create_part();
		if (in_widget == false)
		{
			// If the thickness is odd
			if ((m_thickness >> 6) % 2)
			{
				// Shift by half a pixel to have a thinner line
				move.move_(-32,-32);
			}
		}
		UIManager::renderer()->draw(*this, move);
	}
}

void Rect::add_corner(Coord x, Coord y, Coord radius, Dim thickness, uint32_t flags)
{
	uint32_t adding_flags = 0;

	// If the end have a right angle
	if (m_borders & RIGHT_ANGLE_END)
	{
		adding_flags = Polygon::FLAG_RIGHT_ANGLE_END;
	}
	else if ((m_borders & ROUNDED_END) == 0)
	{
		adding_flags = Polygon::FLAG_STRAIGHT;
	}

	if (m_borders & INNER_AREA)
	{
		adding_flags |= Polygon::FLAG_INNER;
	}

	m_polygon.arc_(x,y,radius,thickness,flags|adding_flags);
}

void Rect::no_border_rectangle(Coord w, Coord h, Coord R, Coord t)
{
	add_corner(-t   ,  R-t,R,R,Polygon::LEFT_TO_TOP);
	add_corner(w+t-R,   -t,R,R,Polygon::TOP_TO_RIGHT);
	add_corner(w+t  ,h+t-R,R,R,Polygon::RIGHT_TO_BOTTOM);
	add_corner(-t+R ,  h+t,R,R,Polygon::BOTTOM_TO_LEFT);
	m_polygon.next_contour();
}

void Rect::rounded_border_rectangle(Coord w, Coord h, Coord R, Coord r, Coord t)
{
	// If borders present (not an inner rectangle)
	if ((m_borders & INNER_AREA) == 0 && t > 0)
	{
		no_border_rectangle(w,h,R,t);
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

	if ((m_borders & TOP_BORDER)    == 0) delta_y_top = -(t<<1);
	if ((m_borders & BOTTOM_BORDER) == 0) delta_y_bottom =  (t<<1);
	if ((m_borders & LEFT_BORDER)   == 0) delta_x_left = -(t<<1);
	if ((m_borders & RIGHT_BORDER)  == 0) delta_x_right =  (t<<1);

	if ((m_borders & (TOP_BORDER|RIGHT_BORDER)   ) == 0) 
	{
		if ((m_borders & INNER_AREA) && (m_borders & RIGHT_ANGLE_WITHOUT_BORDER))
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
	if ((m_borders & (TOP_BORDER|LEFT_BORDER)    ) == 0)
	{
		if ((m_borders & INNER_AREA) && (m_borders & RIGHT_ANGLE_WITHOUT_BORDER))
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
	if ((m_borders & (BOTTOM_BORDER|RIGHT_BORDER)) == 0)
	{
		if ((m_borders & INNER_AREA) && (m_borders & RIGHT_ANGLE_WITHOUT_BORDER))
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
	if ((m_borders & (BOTTOM_BORDER|LEFT_BORDER )) == 0)
	{
		if ((m_borders & INNER_AREA) && (m_borders & RIGHT_ANGLE_WITHOUT_BORDER))
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
	add_corner(R-t  +delta_x_left+move_top_left, t+delta_y_top        , r+round_top_left, r+round_top_left, Polygon::TOP_TO_LEFT);

	// Top right
	add_corner(t    +delta_x_left    , h+t-R+delta_y_bottom+move_left_bottom, r+round_left_bottom, r+round_left_bottom, Polygon::LEFT_TO_BOTTOM);

	// Bottom right
	add_corner(w+t-R+delta_x_right+move_bottom_right, h-t+delta_y_bottom      , r+round_bottom_right, r+round_bottom_right, Polygon::BOTTOM_TO_RIGHT);

	// Bottom left
	add_corner(w-t  +delta_x_right    , R-t+delta_y_top+move_right_top  , r+round_right_top, r+round_right_top, Polygon::RIGHT_TO_TOP);

	m_polygon.next_contour();
}

void Rect::right_angle_border_rectangle(Coord w, Coord h, Coord R, Coord r, Coord t)
{
	Coord thickness = t << 1;
	// According to the borders style (remove all other flags)
	switch(m_borders & ALL_BORDERS)
	{
	case NO_BORDER               : 
		if ((m_borders & INNER_AREA) != 0)
		{
			if (r < 0 || (m_borders & RIGHT_ANGLE_END) || (m_borders & RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner(-t   , r-t    , r, r, Polygon::LEFT_TO_TOP);
			add_corner(w+t-r, -t     , r, r, Polygon::TOP_TO_RIGHT);
			add_corner(w+t  , h+t-r  , r, r, Polygon::RIGHT_TO_BOTTOM);
			add_corner(-t+r , h+t    , r, r, Polygon::BOTTOM_TO_LEFT);
			m_polygon.next_contour();
		}
		break;

	// One border
	case TOP_BORDER|BOTTOM_BORDER  : 
		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(R-t  , -t, R, thickness, Polygon::TOP_TO_LEFT  | Polygon::FLAG_END);
			add_corner(w+t-R, -t, R, thickness, Polygon::TOP_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END);
			m_polygon.next_contour();
			add_corner(R-t  , h+t, R, thickness, Polygon::BOTTOM_TO_LEFT  | Polygon::FLAG_END);
			add_corner(w+t-R, h+t, R, thickness, Polygon::BOTTOM_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END);
			m_polygon.next_contour();
		}
		else
		{
			if (r < 0 || (m_borders & RIGHT_ANGLE_END) || (m_borders & RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			m_polygon.add_point_(-t,t);
			m_polygon.add_point_(w+t,t);
			m_polygon.add_point_(w+t,h-t);
			m_polygon.add_point_(-t,h-t);
			m_polygon.next_contour();
		}
		break;

	case TOP_BORDER              : 
		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(R-t  , -t, R, thickness, Polygon::TOP_TO_LEFT  | Polygon::FLAG_END);
			add_corner(w+t-R, -t, R, thickness, Polygon::TOP_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END);
			m_polygon.next_contour();
		}
		else
		{
			if (r < 0 || (m_borders & RIGHT_ANGLE_END) || (m_borders & RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner(w+t  , h+t-r  , r, r, Polygon::RIGHT_TO_BOTTOM);
			add_corner(-t+r , h+t    , r, r, Polygon::BOTTOM_TO_LEFT);
			m_polygon.add_point_(-t,t);
			m_polygon.add_point_(w+t,t);
			m_polygon.next_contour();
		}
		break;

	case BOTTOM_BORDER           : 
		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(R-t  , h+t, R, thickness, Polygon::BOTTOM_TO_LEFT  | Polygon::FLAG_END);
			add_corner(w+t-R, h+t, R, thickness, Polygon::BOTTOM_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END);
			m_polygon.next_contour();
		}
		else
		{
			if (r < 0 || (m_borders & RIGHT_ANGLE_END) || (m_borders & RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner(-t   , r-t    , r, r, Polygon::LEFT_TO_TOP);
			add_corner(w+t-r, -t     , r, r, Polygon::TOP_TO_RIGHT);
			m_polygon.add_point_(w+t,h-t);
			m_polygon.add_point_(-t,h-t);
			m_polygon.next_contour();
		}
		break;

	case LEFT_BORDER|RIGHT_BORDER  :
		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(w+t, R-t  , R, thickness, Polygon::RIGHT_TO_TOP    | Polygon::FLAG_END);
			add_corner(w+t, h+t-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_END);
			m_polygon.next_contour();

			add_corner(-t, R-t  , R, thickness, Polygon::LEFT_TO_TOP    | Polygon::FLAG_END);
			add_corner(-t, h+t-R, R, thickness, Polygon::LEFT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_END);
			m_polygon.next_contour();
		}
		else
		{
			if (r < 0 || (m_borders & RIGHT_ANGLE_END) || (m_borders & RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			m_polygon.add_point_(t,-t);
			m_polygon.add_point_(w-t,-t);
			m_polygon.add_point_(w-t,h+t);
			m_polygon.add_point_(t,h+t);
			m_polygon.next_contour();
		}
		break;

	case LEFT_BORDER             : 
		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(-t, R-t  , R, thickness, Polygon::LEFT_TO_TOP    | Polygon::FLAG_END);
			add_corner(-t, h+t-R, R, thickness, Polygon::LEFT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_END);
			m_polygon.next_contour();
		}
		else
		{
			if (r < 0 || (m_borders & RIGHT_ANGLE_END) || (m_borders & RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner(w+t-r, -t     , r, r, Polygon::TOP_TO_RIGHT);
			add_corner(w+t  , h+t-r  , r, r, Polygon::RIGHT_TO_BOTTOM);
			m_polygon.add_point_(t,h+t);
			m_polygon.add_point_(t,-t);
			m_polygon.next_contour();
		}
		break;

	case RIGHT_BORDER            : 
		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(w+t, R-t  , R, thickness, Polygon::RIGHT_TO_TOP    | Polygon::FLAG_END);
			add_corner(w+t, h+t-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_END);
			m_polygon.next_contour();
		}
		else
		{
			if (r < 0 || (m_borders & RIGHT_ANGLE_END) || (m_borders & RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner(-t   , r-t    , r, r, Polygon::LEFT_TO_TOP);
			m_polygon.add_point_(w-t,-t);
			m_polygon.add_point_(w-t,h+t);
			add_corner(-t+r , h+t    , r, r, Polygon::BOTTOM_TO_LEFT);
			m_polygon.next_contour();
		}
		break;

	// Two borders corner
	case TOP_BORDER|RIGHT_BORDER        : 
		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(w+t-R, t   , r, r        , Polygon::TOP_TO_RIGHT    | Polygon::FLAG_INTERNAL);
			add_corner(w+t  ,h+t-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE| Polygon::FLAG_END);
			add_corner(w+t  , R-t , R, R        , Polygon::RIGHT_TO_TOP);
			add_corner(R-t  , -t  , R, thickness, Polygon::TOP_TO_LEFT     | Polygon::FLAG_END);
			m_polygon.next_contour();
		}
		else
		{
			m_polygon.add_point_(-t,t);
			add_corner(w+t-R, t   , r, r        , Polygon::TOP_TO_RIGHT    | Polygon::FLAG_INTERNAL);
			m_polygon.add_point_(w-t,h+t);
			if (r < 0 || (m_borders & RIGHT_ANGLE_END) || (m_borders & RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner(-t    , h+t-r  , r, r, Polygon::LEFT_TO_BOTTOM|Polygon::FLAG_REVERSE);
			m_polygon.next_contour();
		}
		break;

	case TOP_BORDER|LEFT_BORDER         : 
		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(-t   , h+t-R, R, thickness, Polygon::LEFT_TO_BOTTOM | Polygon::FLAG_END);
			add_corner(t    , R-t  , r, r        , Polygon::LEFT_TO_TOP    | Polygon::FLAG_INTERNAL);
			add_corner(w+t-R, -t   , R, thickness, Polygon::TOP_TO_RIGHT   | Polygon::FLAG_REVERSE| Polygon::FLAG_END);
			add_corner(R-t  , -t   , R, R        , Polygon::TOP_TO_LEFT);
			m_polygon.next_contour();
		}
		else
		{
			add_corner(t    , R-t  , r, r        , Polygon::LEFT_TO_TOP    | Polygon::FLAG_INTERNAL);
			m_polygon.add_point_(w+t,t);
			if (r < 0 || (m_borders & RIGHT_ANGLE_END) || (m_borders & RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner(w+t-r, h+t    , r, r, Polygon::BOTTOM_TO_RIGHT|Polygon::FLAG_REVERSE);
			m_polygon.add_point_(t,h+t);
			m_polygon.next_contour();
		}
		break;

	case BOTTOM_BORDER|RIGHT_BORDER     : 
		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(w+t  , R-t  , R, thickness, Polygon::RIGHT_TO_TOP    | Polygon::FLAG_END);
			add_corner(w-t  , h+t-R, r, r        , Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_INTERNAL);
			add_corner(R-t  , h+t  , R, thickness, Polygon::BOTTOM_TO_LEFT  | Polygon::FLAG_REVERSE| Polygon::FLAG_END);
			add_corner(w+t-R, h+t  , R, R        , Polygon::BOTTOM_TO_RIGHT);
			m_polygon.next_contour();
		}
		else
		{
			add_corner(w-t  , h+t-R, r, r        , Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_INTERNAL);
			m_polygon.add_point_(-t,h-t);
			if (r < 0 || (m_borders & RIGHT_ANGLE_END) || (m_borders & RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner(r-t  , -t      , r, r, Polygon::TOP_TO_LEFT|Polygon::FLAG_REVERSE);
			m_polygon.add_point_(w-t,-t);
			m_polygon.next_contour();
		}
		break;

	case BOTTOM_BORDER|LEFT_BORDER      :
		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(w+t-R, h+t  , R, thickness, Polygon::BOTTOM_TO_RIGHT| Polygon::FLAG_END);
			add_corner(R-t  , h-t  , r, r        , Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_INTERNAL);
			add_corner(-t   , R-t  , R, thickness, Polygon::LEFT_TO_TOP    | Polygon::FLAG_REVERSE| Polygon::FLAG_END);
			add_corner(-t   , h+t-R, R, R        , Polygon::LEFT_TO_BOTTOM);
			m_polygon.next_contour();
		}
		else
		{
			add_corner(R-t  , h-t  , r, r        , Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_INTERNAL);
			m_polygon.add_point_(t,-t);
			if (r < 0 || (m_borders & RIGHT_ANGLE_END) || (m_borders & RIGHT_ANGLE_WITHOUT_BORDER)) r = 0;
			add_corner(w+t  , r-t    , r, r, Polygon::RIGHT_TO_TOP|Polygon::FLAG_REVERSE);
			m_polygon.add_point_(w+t,h-t);
			m_polygon.next_contour();
		}
		break;
		
	// Three borders U
	case TOP_BORDER|RIGHT_BORDER|BOTTOM_BORDER : 
		add_corner(R-t  , -t   , R, thickness, Polygon::TOP_TO_LEFT    | Polygon::FLAG_END);
		add_corner(w+t-R, t    , r, r        , Polygon::TOP_TO_RIGHT   | Polygon::FLAG_INTERNAL);
		add_corner(w-t  , h+t-R, r, r        , Polygon::RIGHT_TO_BOTTOM| Polygon::FLAG_INTERNAL);
		add_corner(R-t  , h+t  , R, thickness, Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_REVERSE | Polygon::FLAG_END);

		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(w+t-R, h+t  , R, R    , Polygon::BOTTOM_TO_RIGHT);
			add_corner(w+t  , R-t  , R, R    , Polygon::RIGHT_TO_TOP);
		}

		m_polygon.next_contour();
		break;

	case RIGHT_BORDER|BOTTOM_BORDER|LEFT_BORDER: 
		add_corner(-t   , R-t  , R, thickness, Polygon::LEFT_TO_TOP   | Polygon::FLAG_REVERSE | Polygon::FLAG_END);

		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(-t   , h+t-R, R, R    , Polygon::LEFT_TO_BOTTOM);
			add_corner(w+t-R, h+t  , R, R    , Polygon::BOTTOM_TO_RIGHT);
		}

		add_corner(w+t  , R-t  , R, thickness, Polygon::RIGHT_TO_TOP   | Polygon::FLAG_END);
		add_corner(w-t  , h+t-R, r, r        , Polygon::RIGHT_TO_BOTTOM| Polygon::FLAG_INTERNAL);
		add_corner(R-t  , h-t  , r, r        , Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_INTERNAL);
		m_polygon.next_contour();
		break;

	case BOTTOM_BORDER|LEFT_BORDER|TOP_BORDER  : 
		add_corner(w+t-R, -t   , R, thickness, Polygon::TOP_TO_RIGHT    | Polygon::FLAG_END);
		add_corner(R-t  , t    , r, r        , Polygon::TOP_TO_LEFT);
		add_corner(t    , h+t-R, r, r        , Polygon::LEFT_TO_BOTTOM);
		add_corner(w+t-R, h+t  , R, thickness, Polygon::BOTTOM_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_END);

		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(R-t  , h+t  , R, R    , Polygon::BOTTOM_TO_LEFT);
			add_corner(-t   , R-t  , R, R    , Polygon::LEFT_TO_TOP);
		}

		m_polygon.next_contour();
		break;

	case LEFT_BORDER|TOP_BORDER|RIGHT_BORDER   : 
		add_corner(-t   , h+t-R, R, thickness, Polygon::LEFT_TO_BOTTOM  | Polygon::FLAG_END); 
		add_corner(t    , R-t  , r, r        , Polygon::LEFT_TO_TOP     | Polygon::FLAG_INTERNAL);
		add_corner(w+t-R, t    , r, r        , Polygon::TOP_TO_RIGHT    | Polygon::FLAG_INTERNAL);
		add_corner(w+t  , h+t-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE| Polygon::FLAG_END);

		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(w+t  , R-t  , R, R        , Polygon::RIGHT_TO_TOP);
			add_corner(R-t  , -t   , R, R        , Polygon::TOP_TO_LEFT);
		}

		m_polygon.next_contour();
		break;

	// All borders
	case ALL_BORDERS:
		// If borders present (not an inner rectangle)
		if ((m_borders & INNER_AREA) == 0)
		{
			add_corner(-t   , R-t    , R, R, Polygon::LEFT_TO_TOP);
			add_corner(w+t-R, -t     , R, R, Polygon::TOP_TO_RIGHT);
			add_corner(w+t  , h+t-R  , R, R, Polygon::RIGHT_TO_BOTTOM);
			add_corner(-t+R , h+t    , R, R, Polygon::BOTTOM_TO_LEFT);
			m_polygon.next_contour();
		}

		add_corner(R-t  , t      , r, r, Polygon::TOP_TO_LEFT);
		add_corner(t    , h+t-R  , r, r, Polygon::LEFT_TO_BOTTOM);
		add_corner(w+t-R, h-t    , r, r, Polygon::BOTTOM_TO_RIGHT);
		add_corner(w-t  , R-t    , r, r, Polygon::RIGHT_TO_TOP);
		m_polygon.next_contour();
		break;
	}
}

void Rect::adapt_radius(Coord & radius)
{
	bool radius_standard = true;
	// If the end have a right angle
	if (m_borders & RIGHT_ANGLE_END)
	{
		Borders borders = (Borders)(m_borders & ALL_BORDERS);
		
		// According to the borders present adapt the radius
		switch((uint8_t)borders)
		{
		case BOTTOM_BORDER | LEFT_BORDER | RIGHT_BORDER: 
		case TOP_BORDER    | LEFT_BORDER | RIGHT_BORDER:
			if(radius > (Coord)m_size.width_()/2)
			{
				radius = m_size.width_()/2;
			}
			if(radius > (Coord)m_size.height_())
			{
				radius = m_size.height_();
			}
			radius_standard = false;
			break;
	
		case LEFT_BORDER  | BOTTOM_BORDER | TOP_BORDER:
		case RIGHT_BORDER | BOTTOM_BORDER | TOP_BORDER:
			if(radius > (Coord)m_size.width_())
			{
				radius = m_size.width_();
			}
			if(radius > (Coord)m_size.height_()/2)
			{
				radius = m_size.height_()/2;
			}
			radius_standard = false;
			break;

		case BOTTOM_BORDER | LEFT_BORDER: 
		case TOP_BORDER    | LEFT_BORDER:
		case RIGHT_BORDER  | TOP_BORDER:
		case RIGHT_BORDER  | BOTTOM_BORDER:
			if(radius > (Coord)m_size.width_())
			{
				radius = m_size.width_();
			}
			if(radius > (Coord)m_size.height_())
			{
				radius = m_size.height_();
			}
			radius_standard = false;
			break;
		}
	}

	// If radius not adaptated previously
	if (radius_standard)
	{
		if(radius > (Coord)m_size.width_()/2)
		{
			radius = m_size.width_()/2;
		}
		if(radius > (Coord)m_size.height_()/2)
		{
			radius = m_size.height_()/2;
		}
	}
}

void Rect::adapt_thickness(Coord & thickness)
{
	// Adapt the thickness according to the size
	if(thickness > (Coord)m_size.width_())
	{
		thickness = m_size.width_();
	}
	if(thickness > (Coord)m_size.height_())
	{
		thickness = m_size.height_();
	}
}

void Rect::create_part()
{
	Coord radius = m_radius;
	Coord thickness = m_thickness;

	adapt_radius(radius);
	adapt_thickness(thickness);

	Coord t = thickness>>1;
	Coord R = radius +t;
	Coord r = radius -t;
	Coord w = m_size.width_();
	Coord h = m_size.height_();

	m_polygon.clear();

	// If radius is negativ
	if (radius <= 0)
	{
		// Remove internal round and external round
		r = -thickness;
		R = 0;
	}

	// No border thickness is null : fill completly
	if (thickness == 0)
	{
		if ((m_borders & ALL_BORDERS) == ALL_BORDERS)
		{
			no_border_rectangle(w,h,R,t);
		}
		else
		{
			rounded_border_rectangle(w, h, R, r, t);
		}
	}
	// The ends of borders are not rounded
	else if ((m_borders & ROUNDED_END) == 0)
	{
		right_angle_border_rectangle(w, h, R, r, t);
	}
	else // The end of borders are rounded
	{
		rounded_border_rectangle(w, h, R, r, t);
	}
}

