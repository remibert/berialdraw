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
	Dim thickness, Dim gap, uint8_t sides, uint32_t backcolor, uint32_t bordercolor, 
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
		rect.sides(sides|Sides::INNER_AREA);
		rect.paint(pos,true);
	}
	if(bordercolor && (thickness || focus_thickness))
	{
		rect.color(bordercolor);
		rect.sides(sides);
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
	Sides sides = (Sides)common_style.sides();
	if (focused && border_style.focus_thickness())
	{
		Area focus_area(area);
		if (sides & Sides::RECTANGULAR_EXTREMITY)
		{
			Dim reduce = (border_style.focus_gap()<<6) +(border_style.focus_thickness()<<6);
			switch((uint8_t)sides & ALL_SIDES)
			{
			case BOTTOM_SIDE | LEFT_SIDE | RIGHT_SIDE:
				focus_area.size().decrease_(0,reduce);
				focus_area.position().move_(0,reduce);
				break;

			case TOP_SIDE    | LEFT_SIDE | RIGHT_SIDE:
				focus_area.size().decrease_(0,reduce);
				break;
		
			case LEFT_SIDE  | BOTTOM_SIDE | TOP_SIDE:
				focus_area.size().decrease_(reduce,0);
				break;

			case RIGHT_SIDE | BOTTOM_SIDE | TOP_SIDE:
				focus_area.size().decrease_(reduce,0);
				focus_area.position().move_(reduce,0);
				break;
	
			case BOTTOM_SIDE | LEFT_SIDE:
				focus_area.position().move_(0,reduce);
				focus_area.size().decrease_(reduce,reduce);
				break;

			case TOP_SIDE    | LEFT_SIDE:
				focus_area.size().decrease_(reduce,reduce);
				break;

			case RIGHT_SIDE  | TOP_SIDE:
				focus_area.position().move_(reduce,0);
				focus_area.size().decrease_(reduce,reduce);
				break;

			case RIGHT_SIDE  | BOTTOM_SIDE:
				focus_area.position().move_(reduce,reduce);
				focus_area.size().decrease_(reduce,reduce);
				break;
			}
		}
		// Draw focus
		Rect::build_polygon(focus_area, border_style.radius_(), border_style.thickness_(), border_style.focus_gap()<<6, sides, focus_color, focus_border_color, border_style.focus_thickness()<<6);
	}
	// Draw background
	Rect::build_polygon(area, border_style.radius_(), border_style.thickness_(), 0, sides, color, border_color);
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
	if (m_sides & RECTANGULAR_EXTREMITY)
	{
		adding_flags = Polygon::FLAG_RECTANGULAR;
	}
	else if ((m_sides & ROUNDED_EXTREMITY) == 0)
	{
		adding_flags = Polygon::FLAG_STRAIGHT;
	}

	if (m_sides & INNER_AREA)
	{
		adding_flags |= Polygon::FLAG_INNER;
	}

	m_polygon.arc_(x,y,radius,thickness,flags|adding_flags);
}

void Rect::create_part()
{
	Coord w = m_size.width_();
	Coord h = m_size.height_();
	Coord radius = m_radius;
	Coord thickness = m_thickness;

	bool radius_standard = true;
	if (m_sides & RECTANGULAR_EXTREMITY)
	{
		Sides sides = (Sides)(m_sides & ALL_SIDES);
		
		switch((uint8_t)sides)
		{
		case BOTTOM_SIDE | LEFT_SIDE | RIGHT_SIDE: 
		case TOP_SIDE    | LEFT_SIDE | RIGHT_SIDE:
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
	
		case LEFT_SIDE  | BOTTOM_SIDE | TOP_SIDE:
		case RIGHT_SIDE | BOTTOM_SIDE | TOP_SIDE:
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

		case BOTTOM_SIDE | LEFT_SIDE: 
		case TOP_SIDE    | LEFT_SIDE:
		case RIGHT_SIDE  | TOP_SIDE:
		case RIGHT_SIDE  | BOTTOM_SIDE:
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
	if(thickness > (Coord)m_size.width_())
	{
		thickness = m_size.width_();
	}
	if(thickness > (Coord)m_size.height_())
	{
		thickness = m_size.height_();
	}

	Coord t = thickness>>1;
	Coord R = radius +t;
	Coord r = radius -t;

	m_polygon.clear();
	{
		if (radius <= 0)
		{
			r = -thickness;
			R = 0;
		}

		// Fill completly
		if (thickness == 0)
		{
			add_corner(-t   ,  R-t,R,R,Polygon::LEFT_TO_TOP);
			add_corner(w+t-R,   -t,R,R,Polygon::TOP_TO_RIGHT);
			add_corner(w+t  ,h+t-R,R,R,Polygon::RIGHT_TO_BOTTOM);
			add_corner(-t+R ,  h+t,R,R,Polygon::BOTTOM_TO_LEFT);
			m_polygon.next_contour();
		}
		// Empty rectangle
		else if ((sides() & ROUNDED_EXTREMITY) == 0)
		{
			switch(sides() & ALL_SIDES)
			{
			case NO_SIDE               : 
				if ((m_sides & INNER_AREA) != 0)
				{
					if (r < 0 || (m_sides & RECTANGULAR_EXTREMITY)) r = 0;
					add_corner(-t   , r-t    , r, r, Polygon::LEFT_TO_TOP);
					add_corner(w+t-r, -t     , r, r, Polygon::TOP_TO_RIGHT);
					add_corner(w+t  , h+t-r  , r, r, Polygon::RIGHT_TO_BOTTOM);
					add_corner(-t+r , h+t    , r, r, Polygon::BOTTOM_TO_LEFT);
					m_polygon.next_contour();
				}
				break;

			// One side
			case TOP_SIDE|BOTTOM_SIDE  : 
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(R-t  , -t, R, thickness, Polygon::TOP_TO_LEFT  | Polygon::FLAG_EXTREMITY);
					add_corner(w+t-R, -t, R, thickness, Polygon::TOP_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_EXTREMITY);
					m_polygon.next_contour();
					add_corner(R-t  , h+t, R, thickness, Polygon::BOTTOM_TO_LEFT  | Polygon::FLAG_EXTREMITY);
					add_corner(w+t-R, h+t, R, thickness, Polygon::BOTTOM_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_EXTREMITY);
					m_polygon.next_contour();
				}
				else
				{
					if ((m_sides & RECTANGULAR_EXTREMITY) || (m_sides & ROUNDED_EXTREMITY) == 0)
					{
						if (r < 0 || (m_sides & RECTANGULAR_EXTREMITY)) r = 0;
						m_polygon.add_point_(-t,t);
						m_polygon.add_point_(w+t,t);
						m_polygon.add_point_(w+t,h-t);
						m_polygon.add_point_(-t,h-t);
						m_polygon.next_contour();
					}
				}
				break;

			case TOP_SIDE              : 
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(R-t  , -t, R, thickness, Polygon::TOP_TO_LEFT  | Polygon::FLAG_EXTREMITY);
					add_corner(w+t-R, -t, R, thickness, Polygon::TOP_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_EXTREMITY);
					m_polygon.next_contour();
				}
				else
				{
					if ((m_sides & RECTANGULAR_EXTREMITY) || (m_sides & ROUNDED_EXTREMITY) == 0)
					{
						if (r < 0 || (m_sides & RECTANGULAR_EXTREMITY)) r = 0;
						add_corner(w+t  , h+t-r  , r, r, Polygon::RIGHT_TO_BOTTOM);
						add_corner(-t+r , h+t    , r, r, Polygon::BOTTOM_TO_LEFT);
						m_polygon.add_point_(-t,t);
						m_polygon.add_point_(w+t,t);
						m_polygon.next_contour();
					}
				}
				break;

			case BOTTOM_SIDE           : 
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(R-t  , h+t, R, thickness, Polygon::BOTTOM_TO_LEFT  | Polygon::FLAG_EXTREMITY);
					add_corner(w+t-R, h+t, R, thickness, Polygon::BOTTOM_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_EXTREMITY);
					m_polygon.next_contour();
				}
				else
				{
					if ((m_sides & RECTANGULAR_EXTREMITY) || (m_sides & ROUNDED_EXTREMITY) == 0)
					{
						if (r < 0 || (m_sides & RECTANGULAR_EXTREMITY)) r = 0;
						add_corner(-t   , r-t    , r, r, Polygon::LEFT_TO_TOP);
						add_corner(w+t-r, -t     , r, r, Polygon::TOP_TO_RIGHT);
						m_polygon.add_point_(w+t,h-t);
						m_polygon.add_point_(-t,h-t);
						m_polygon.next_contour();
					}
				}
				break;

			case LEFT_SIDE|RIGHT_SIDE  :
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(w+t, R-t  , R, thickness, Polygon::RIGHT_TO_TOP    | Polygon::FLAG_EXTREMITY);
					add_corner(w+t, h+t-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_EXTREMITY);
					m_polygon.next_contour();

					add_corner(-t, R-t  , R, thickness, Polygon::LEFT_TO_TOP    | Polygon::FLAG_EXTREMITY);
					add_corner(-t, h+t-R, R, thickness, Polygon::LEFT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_EXTREMITY);
					m_polygon.next_contour();
				}
				else
				{
					if ((m_sides & RECTANGULAR_EXTREMITY) || (m_sides & ROUNDED_EXTREMITY) == 0)
					{
						if (r < 0 || (m_sides & RECTANGULAR_EXTREMITY)) r = 0;
						m_polygon.add_point_(t,-t);
						m_polygon.add_point_(w-t,-t);
						m_polygon.add_point_(w-t,h+t);
						m_polygon.add_point_(t,h+t);
						m_polygon.next_contour();
					}
				}
				break;

			case LEFT_SIDE             : 
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(-t, R-t  , R, thickness, Polygon::LEFT_TO_TOP    | Polygon::FLAG_EXTREMITY);
					add_corner(-t, h+t-R, R, thickness, Polygon::LEFT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_EXTREMITY);
					m_polygon.next_contour();
				}
				else
				{
					if ((m_sides & RECTANGULAR_EXTREMITY) || (m_sides & ROUNDED_EXTREMITY) == 0)
					{
						if (r < 0 || (m_sides & RECTANGULAR_EXTREMITY)) r = 0;
						add_corner(w+t-r, -t     , r, r, Polygon::TOP_TO_RIGHT);
						add_corner(w+t  , h+t-r  , r, r, Polygon::RIGHT_TO_BOTTOM);
						m_polygon.add_point_(t,h+t);
						m_polygon.add_point_(t,-t);
						m_polygon.next_contour();
					}
				}
				break;

			case RIGHT_SIDE            : 
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(w+t, R-t  , R, thickness, Polygon::RIGHT_TO_TOP    | Polygon::FLAG_EXTREMITY);
					add_corner(w+t, h+t-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE | Polygon::FLAG_EXTREMITY);
					m_polygon.next_contour();
				}
				else
				{
					if ((m_sides & RECTANGULAR_EXTREMITY) || (m_sides & ROUNDED_EXTREMITY) == 0)
					{
						if (r < 0 || (m_sides & RECTANGULAR_EXTREMITY)) r = 0;
						add_corner(-t   , r-t    , r, r, Polygon::LEFT_TO_TOP);
						m_polygon.add_point_(w-t,-t);
						m_polygon.add_point_(w-t,h+t);
						add_corner(-t+r , h+t    , r, r, Polygon::BOTTOM_TO_LEFT);
						m_polygon.next_contour();
					}
				}
				break;

			// Two sides corner
			case TOP_SIDE|RIGHT_SIDE        : 
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(w+t-R, t   , r, r        , Polygon::TOP_TO_RIGHT    | Polygon::FLAG_INTERNAL);
					add_corner(w+t  ,h+t-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE| Polygon::FLAG_EXTREMITY);
					add_corner(w+t  , R-t , R, R        , Polygon::RIGHT_TO_TOP);
					add_corner(R-t  , -t  , R, thickness, Polygon::TOP_TO_LEFT     | Polygon::FLAG_EXTREMITY);
					m_polygon.next_contour();
				}
				else
				{
					if ((m_sides & RECTANGULAR_EXTREMITY) || (m_sides & ROUNDED_EXTREMITY) == 0)
					{
						m_polygon.add_point_(-t,t);
						add_corner(w+t-R, t   , r, r        , Polygon::TOP_TO_RIGHT    | Polygon::FLAG_INTERNAL);
						m_polygon.add_point_(w-t,h+t);
						if (r < 0 || (m_sides & RECTANGULAR_EXTREMITY)) r = 0;
						add_corner(-t    , h+t-r  , r, r, Polygon::LEFT_TO_BOTTOM|Polygon::FLAG_REVERSE);
						m_polygon.next_contour();
					}
				}
				break;

			case TOP_SIDE|LEFT_SIDE         : 
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(-t   , h+t-R, R, thickness, Polygon::LEFT_TO_BOTTOM | Polygon::FLAG_EXTREMITY);
					add_corner(t    , R-t  , r, r        , Polygon::LEFT_TO_TOP    | Polygon::FLAG_INTERNAL);
					add_corner(w+t-R, -t   , R, thickness, Polygon::TOP_TO_RIGHT   | Polygon::FLAG_REVERSE| Polygon::FLAG_EXTREMITY);
					add_corner(R-t  , -t   , R, R        , Polygon::TOP_TO_LEFT);
					m_polygon.next_contour();
				}
				else
				{
					if ((m_sides & RECTANGULAR_EXTREMITY) || (m_sides & ROUNDED_EXTREMITY) == 0)
					{
						add_corner(t    , R-t  , r, r        , Polygon::LEFT_TO_TOP    | Polygon::FLAG_INTERNAL);
						m_polygon.add_point_(w+t,t);
						//m_polygon.add_point_(w+t,h+t);
						if (r < 0 || (m_sides & RECTANGULAR_EXTREMITY)) r = 0;
						add_corner(w+t-r, h+t    , r, r, Polygon::BOTTOM_TO_RIGHT|Polygon::FLAG_REVERSE);
						m_polygon.add_point_(t,h+t);
						m_polygon.next_contour();
					}
				}
				break;

			case BOTTOM_SIDE|RIGHT_SIDE     : 
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(w+t  , R-t  , R, thickness, Polygon::RIGHT_TO_TOP    | Polygon::FLAG_EXTREMITY);
					add_corner(w-t  , h+t-R, r, r        , Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_INTERNAL);
					add_corner(R-t  , h+t  , R, thickness, Polygon::BOTTOM_TO_LEFT  | Polygon::FLAG_REVERSE| Polygon::FLAG_EXTREMITY);
					add_corner(w+t-R, h+t  , R, R        , Polygon::BOTTOM_TO_RIGHT);
					m_polygon.next_contour();
				}
				else
				{
					if ((m_sides & RECTANGULAR_EXTREMITY) || (m_sides & ROUNDED_EXTREMITY) == 0)
					{
						add_corner(w-t  , h+t-R, r, r        , Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_INTERNAL);
						m_polygon.add_point_(-t,h-t);
						if (r < 0 || (m_sides & RECTANGULAR_EXTREMITY)) r = 0;
						add_corner(r-t  , -t      , r, r, Polygon::TOP_TO_LEFT|Polygon::FLAG_REVERSE);
						m_polygon.add_point_(w-t,-t);
						m_polygon.next_contour();
					}
				}
				break;

			case BOTTOM_SIDE|LEFT_SIDE      :
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(w+t-R, h+t  , R, thickness, Polygon::BOTTOM_TO_RIGHT| Polygon::FLAG_EXTREMITY);
					add_corner(R-t  , h-t  , r, r        , Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_INTERNAL);
					add_corner(-t   , R-t  , R, thickness, Polygon::LEFT_TO_TOP    | Polygon::FLAG_REVERSE| Polygon::FLAG_EXTREMITY);
					add_corner(-t   , h+t-R, R, R        , Polygon::LEFT_TO_BOTTOM);
					m_polygon.next_contour();
				}
				else
				{
					if ((m_sides & RECTANGULAR_EXTREMITY) || (m_sides & ROUNDED_EXTREMITY) == 0)
					{
						add_corner(R-t  , h-t  , r, r        , Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_INTERNAL);
						m_polygon.add_point_(t,-t);
						if (r < 0 || (m_sides & RECTANGULAR_EXTREMITY)) r = 0;
						add_corner(w+t  , r-t    , r, r, Polygon::RIGHT_TO_TOP|Polygon::FLAG_REVERSE);
						m_polygon.add_point_(w+t,h-t);
						m_polygon.next_contour();
					}
				}
				break;
		
			// Three sides U
			case TOP_SIDE|RIGHT_SIDE|BOTTOM_SIDE : 
				add_corner(R-t  , -t   , R, thickness, Polygon::TOP_TO_LEFT    | Polygon::FLAG_EXTREMITY);
				add_corner(w+t-R, t    , r, r        , Polygon::TOP_TO_RIGHT   | Polygon::FLAG_INTERNAL);
				add_corner(w-t  , h+t-R, r, r        , Polygon::RIGHT_TO_BOTTOM| Polygon::FLAG_INTERNAL);
				add_corner(R-t  , h+t  , R, thickness, Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_REVERSE | Polygon::FLAG_EXTREMITY);
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(w+t-R, h+t  , R, R    , Polygon::BOTTOM_TO_RIGHT);
					add_corner(w+t  , R-t  , R, R    , Polygon::RIGHT_TO_TOP);
				}
				m_polygon.next_contour();
				break;

			case RIGHT_SIDE|BOTTOM_SIDE|LEFT_SIDE: 
				add_corner(-t   , R-t  , R, thickness, Polygon::LEFT_TO_TOP   | Polygon::FLAG_REVERSE | Polygon::FLAG_EXTREMITY);
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(-t   , h+t-R, R, R    , Polygon::LEFT_TO_BOTTOM);
					add_corner(w+t-R, h+t  , R, R    , Polygon::BOTTOM_TO_RIGHT);
				}
				add_corner(w+t  , R-t  , R, thickness, Polygon::RIGHT_TO_TOP   | Polygon::FLAG_EXTREMITY);
				add_corner(w-t  , h+t-R, r, r        , Polygon::RIGHT_TO_BOTTOM| Polygon::FLAG_INTERNAL);
				add_corner(R-t  , h-t  , r, r        , Polygon::BOTTOM_TO_LEFT | Polygon::FLAG_INTERNAL);
				m_polygon.next_contour();
				break;

			case BOTTOM_SIDE|LEFT_SIDE|TOP_SIDE  : 
				add_corner(w+t-R, -t   , R, thickness, Polygon::TOP_TO_RIGHT    | Polygon::FLAG_EXTREMITY);
				add_corner(R-t  , t    , r, r        , Polygon::TOP_TO_LEFT);
				add_corner(t    , h+t-R, r, r        , Polygon::LEFT_TO_BOTTOM);
				add_corner(w+t-R, h+t  , R, thickness, Polygon::BOTTOM_TO_RIGHT | Polygon::FLAG_REVERSE | Polygon::FLAG_EXTREMITY);
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(R-t  , h+t  , R, R    , Polygon::BOTTOM_TO_LEFT);
					add_corner(-t   , R-t  , R, R    , Polygon::LEFT_TO_TOP);
				}
				m_polygon.next_contour();
				break;

			case LEFT_SIDE|TOP_SIDE|RIGHT_SIDE   : 
				add_corner(-t   , h+t-R, R, thickness, Polygon::LEFT_TO_BOTTOM  | Polygon::FLAG_EXTREMITY); 
				add_corner(t    , R-t  , r, r        , Polygon::LEFT_TO_TOP     | Polygon::FLAG_INTERNAL);
				add_corner(w+t-R, t    , r, r        , Polygon::TOP_TO_RIGHT    | Polygon::FLAG_INTERNAL);
				add_corner(w+t  , h+t-R, R, thickness, Polygon::RIGHT_TO_BOTTOM | Polygon::FLAG_REVERSE| Polygon::FLAG_EXTREMITY);
				if ((m_sides & INNER_AREA) == 0)
				{
					add_corner(w+t  , R-t  , R, R        , Polygon::RIGHT_TO_TOP);
					add_corner(R-t  , -t   , R, R        , Polygon::TOP_TO_LEFT);
				}
				m_polygon.next_contour();
				break;

			// All sides
			case ALL_SIDES:
				if ((m_sides & INNER_AREA) == 0)
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
		else
		{
			if ((m_sides & INNER_AREA) == 0)
			{
				add_corner(-t   , R-t    , R, R, Polygon::LEFT_TO_TOP);
				add_corner(w+t-R, -t     , R, R, Polygon::TOP_TO_RIGHT);
				add_corner(w+t  , h+t-R  , R, R, Polygon::RIGHT_TO_BOTTOM);
				add_corner(-t+R , h+t    , R, R, Polygon::BOTTOM_TO_LEFT);
				m_polygon.next_contour();
			}

			Coord dxl = 0;
			Coord dxr = 0;
			Coord dyb = 0;
			Coord dyt = 0;

			Coord rtl = 0;
			Coord rlb = 0;
			Coord rbr = 0;
			Coord rrt = 0;

			Coord mtl = 0;
			Coord mlb = 0;
			Coord mbr = 0;
			Coord mrt = 0;

			if ((m_sides & TOP_SIDE)    == 0) dyt = -(t<<1);
			if ((m_sides & BOTTOM_SIDE) == 0) dyb =  (t<<1);
			if ((m_sides & LEFT_SIDE)   == 0) dxl = -(t<<1);
			if ((m_sides & RIGHT_SIDE)  == 0) dxr =  (t<<1);

			if ((m_sides & (TOP_SIDE|RIGHT_SIDE)   ) == 0) { rrt = t+t; mrt = +t+t;}
			if ((m_sides & (TOP_SIDE|LEFT_SIDE)    ) == 0) { rtl = t+t; mtl = +t+t;}
			if ((m_sides & (BOTTOM_SIDE|RIGHT_SIDE)) == 0) { rbr = t+t; mbr = -t-t;}
			if ((m_sides & (BOTTOM_SIDE|LEFT_SIDE )) == 0) { rlb = t+t; mlb = -t-t;}

			add_corner(R-t  +dxl+mtl, t+dyt        , r+rtl, r+rtl, Polygon::TOP_TO_LEFT);
			add_corner(t    +dxl    , h+t-R+dyb+mlb, r+rlb, r+rlb, Polygon::LEFT_TO_BOTTOM);
			add_corner(w+t-R+dxr+mbr, h-t+dyb      , r+rbr, r+rbr, Polygon::BOTTOM_TO_RIGHT);
			add_corner(w-t  +dxr    , R-t+dyt+mrt  , r+rrt, r+rrt, Polygon::RIGHT_TO_TOP);
			m_polygon.next_contour();
		}
	}
}

#ifdef _DEBUG
void Rect::test1()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());
		canvas->color(Color::YELLOW,32);

	Rect * roundrect = new Rect(canvas);
		roundrect->position(10, 10);
		roundrect->radius(50);
		roundrect->size(460, 460);
		roundrect->thickness(10);
		roundrect->color(0x3FFF0000);

	Rect * fillroundrect = new Rect(canvas);
		fillroundrect->position(30, 30);
		fillroundrect->radius(30);
		fillroundrect->size(420, 420);
		fillroundrect->color(0x3F00FF00);

	Rect * rect = new Rect(canvas);
		rect->position(140, 140);
		rect->size(200, 200);
		rect->color(0x3F0000FF);

	UIManager::desktop()->dispatch("test/out/rect1.svg");
}

void Rect::test2()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());
		canvas->color(Color::YELLOW,32);

	Point center(240,240);

	uint32_t angle = 10;

	Point radius(-120,0);

	Cross * cross = new Cross(canvas);
		cross->position(center);

	Circle * circle = new Circle(canvas);
		circle->radius(120);
		circle->position(center);
		circle->thickness(2);

	for (angle = 0; angle < 360; angle += 15)
	{
		Rect * background = new Rect(canvas);
		Text * label = new Text(canvas);
			label->font_size(20);
			label->text("Hello");
			label->center(radius);
			label->position(center);
			label->color(Color::BLUE);
			label->margin(3,3,3,3);
			label->angle(angle);

			background->center(radius);
			background->position(center);
			background->radius(8);
			background->size(label->marged_size());
			background->color(Color::CYAN, 32);
			background->thickness(0);
			background->angle(angle);
		
		background = new Rect(canvas);
			background->center(radius);
			background->position(center);
			background->radius(8);
			background->size(label->marged_size());
			background->angle(angle);
			background->thickness(2);
			background->color(Color::BLUE);
	}
	UIManager::desktop()->dispatch("test/out/rect2.svg");
}

void Rect::test3()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE_ICE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());

	Point position(0,0);

	Coord row = 10;
	Coord column = 10;
	position.set(column,row);

	// Button normal
	{
		Rect * background = new Rect(canvas);
		Text * label = new Text(canvas);
			label->text("NORMAL");
			label->position(position);
			label->color(Color::WHITE);
			label->margin(10,10,10,10);
			label->font_size(20);

			background->position(position);
			background->radius(4);
			background->size(label->marged_size());
			background->color(Color::DODGER_BLUE);
			background->thickness(0);

		column += label->marged_size().width() + 10;
	}
	UIManager::desktop()->dispatch();

	position.set(column, row);

	// Button pressed
	{
		Rect * background = new Rect(canvas);
		Text * label = new Text(canvas);
			label->text("PRESSED");
			label->position(position);
			label->color(Color::DODGER_BLUE);
			label->margin(10,10,10,10);
			label->font_size(20);

			background->position(position);
			background->radius(4);
			background->size(label->marged_size());
			background->color(Color::SILVER_WHITE);
			background->thickness(0);

		row += label->marged_size().height() + 10;
		column = 10;
	}

	position.set(column, row);

	// Input text
	{
		Text * label = new Text(canvas);
			label->text("Input tex");
			label->position(position);
			label->color(Color::BLACK);
			label->margin(10,10,5,10);
			label->font_size(20);

		Rect * background = new Rect(canvas);
			background->position(position.x(), position.y() + label->marged_size().height());
			background->radius(0);
			background->size(label->marged_size().width(), 1);
			background->color(Color::DODGER_BLUE);
			background->thickness(2);

		Rect * cursor = new Rect(canvas);
			cursor->position(position.x() + label->content_size().width() + label->margin().left(), position.y() + label->margin().top());	
			cursor->size(1, label->content_size().height());
			cursor->color(Color::COLUMBIA_BLUE);

		row += label->marged_size().height() + 10;
		column = 10;
	}
	UIManager::desktop()->dispatch();

	position.set(column, row);

	// Switch off
	{
		Rect * back = new Rect(canvas);
			back->color(Color::GRAY_CLOUD);
			back->size(50, 20);
			back->radius(10);
			back->position(position.x()+5, position.y()+5);

		Circle * switch_ = new Circle(canvas);
			switch_->color(Color::BLACK,16);
			switch_->radius(15);
			switch_->thickness(0);
			switch_->position(position.x() + 15+2, position.y()+15+2);
	
			switch_ = new Circle(canvas);
			switch_->color(Color::PLATINUM);
			switch_->radius(15);
			switch_->thickness(0);
			switch_->position(position.x() + 15, position.y()+15);
	}

	row += 40;
	position.set(column, row);

	// Switch on
	{
		Rect * back = new Rect(canvas);
			back->color(Color::JEANS_BLUE);
			back->size(50, 20);
			back->radius(10);
			back->position(position.x()+5, position.y()+5);

		Circle * switch_ = new Circle(canvas);
			switch_->color(Color::BLACK,32);
			switch_->radius(15);
			switch_->thickness(0);
			switch_->position(position.x() + 15+2+30, position.y()+15+2);
	
			switch_->color(Color::DODGER_BLUE);
			switch_->radius(15);
			switch_->thickness(0);
			switch_->position(position.x() + 15+30, position.y()+15);
	}

	// Footer
	{
		Rect * background = new Rect(canvas);
			background->position(0,425);
			background->color(Color::BLACK);
			background->size(480,55);
			background->thickness(0);

		Triangle * triangle = new Triangle(canvas);
			triangle->color(Color::WHITE);
			triangle->radius(13);
			triangle->position(110,452);
			triangle->thickness(3);
			triangle->angle(90);

		Circle * circle = new Circle(canvas);
			circle->color(Color::WHITE);
			circle->radius(13);
			circle->position(240,452);
			circle->thickness(3);
			circle->angle(90);

		Square * square = new Square(canvas);
			square->color(Color::WHITE);
			square->radius(18);
			square->position(240+130,452);
			square->thickness(3);
			square->angle(90);
	}
 
	UIManager::desktop()->dispatch("test/out/rect3.svg");
}

void Rect::test4()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());

	Rect * rect = new Rect(canvas);
		rect->position(10, 10);
		rect->size(460, 460);
		rect->color(Color::RED,32);

		rect = new Rect(canvas);
		rect->position(5, 5);
		rect->size(470, 470);
		rect->thickness(10);
		rect->color(Color::BLUE,32);

		canvas->clear();

		rect = new Rect(canvas);
		rect->position(10, 10);
		rect->size(460, 460);
		rect->color(Color::RED,32);
		rect->radius(35);

		rect = new Rect(canvas);
		rect->position(5, 5);
		rect->size(470, 470);
		rect->thickness(10);
		rect->radius(40);
		rect->color(Color::BLUE,32);

	UIManager::desktop()->dispatch("test/out/rect4.svg");
}

void Rect::test5()
{
	int row = 0;
	int column = 0;
	Window window;
	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::WHITE);
		canvas->extend(Extend::EXTEND_ALL);
		canvas->clear();
	Rect * rect = 0;
	int id = 0;

	for (Dim thickness = 0; thickness <= 16; )
	{
		for (Dim radius = 0; radius <= 16; )
		{
			row = 0;
			column = 0;
			for (int side = NO_SIDE; side <= ALL_SIDES; side++)
			{
				rect = new Rect(canvas);
					rect->position(15+column*120, 15+row*60);
					rect->size(80, 35);
					rect->color(Color::RED,58);
					rect->thickness(thickness);
					rect->radius(radius);
					
				rect = new Rect(*rect);
					rect->color(Color::BLUE,64);
					rect->sides(side);

				rect = new Rect(*rect);
					rect->color(Color::GREEN,64);
					rect->sides(side | INNER_AREA);


				rect = new Rect(canvas);
					rect->position(15+column*120, 15+(row+4)*58);
					rect->size(80, 35);
					rect->color(Color::RED,64);
					rect->thickness(thickness);
					rect->radius(radius);
					
				rect = new Rect(*rect);
					rect->color(Color::BLUE,64);
					rect->sides(side | RECTANGULAR_EXTREMITY);

				rect = new Rect(*rect);
					rect->color(Color::GREEN,64);
					rect->sides(side | RECTANGULAR_EXTREMITY | INNER_AREA);

				column ++;
				if (column >= 4)
				{
					row ++;
					column = 0;
				}
			}
			if (radius == 0)
			{
				radius = 1;
			}
			else
			{
				radius <<= 1;
			}
			{
				String name;
				name.print("test/out/rect5_%d.svg", ++id);
				UIManager::desktop()->dispatch(name);
			}
			canvas->clear();
		}
		if (thickness == 0)
		{
			thickness = 1;
		}
		else
		{
			thickness <<= 1;
		}
	}
}

void Rect::test6()
{
	int row = 0;
	int column = 0;
	Window window;
	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::WHITE);
		canvas->extend(Extend::EXTEND_ALL);
		canvas->clear();
	Rect * rect = 0;
	int id = 0;

	for (Dim thickness=1; thickness <= 32; )
	{
		for (Dim radius = 0; radius <= 32; )
		{
			row = 0;
			column = 0;
			for (int side = NO_SIDE; side <= ALL_SIDES; side++)
			{
				rect = new Rect(canvas);
					rect->position(15+column*120, 15+row*100);
					rect->size(80, 60);
					rect->thickness(thickness);
					rect->radius(radius);
					rect->color(Color::BLUE,64);
					rect->sides(side| ROUNDED_EXTREMITY);

				rect = new Rect(*rect);
					rect->color(Color::GREEN,64);
					rect->sides(side | INNER_AREA | ROUNDED_EXTREMITY);

				column ++;
				if (column >= 4)
				{
					row ++;
					column = 0;
				}
			}
			if (radius < 8)
			{
				radius ++;
			}
			else
			{
				radius <<= 1;
			}
			{
				String name;
				name.print("test/out/rect6_%d.svg", ++id);
				UIManager::desktop()->dispatch(name);
			}
			canvas->clear();
		}
		if (thickness < 8)
		{
			thickness++;
		}
		else
		{
			thickness <<= 1;
		}
	}
}

void Rect::test7()
{
	int row = 0;
	int column = 0;
	Window window;
	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::WHITE);
		canvas->extend(Extend::EXTEND_ALL);
		canvas->clear();
	Rect * rect = 0;
	int id = 0;

	for (Dim thickness=1; thickness <= 32; )
	{
		for (Dim radius = 0; radius <= 32; )
		{
			row = 0;
			column = 0;
			for (int side = NO_SIDE; side <= ALL_SIDES; side++)
			{
				rect = new Rect(canvas);
					rect->position(15+column*120, 15+row*100);
					rect->size(80, 60);
					rect->thickness(thickness);
					rect->radius(radius);
					rect->color(Color::BLUE,64);
					rect->sides(side);

				rect = new Rect(*rect);
					rect->color(Color::GREEN,64);
					rect->sides(side | INNER_AREA);

				column ++;
				if (column >= 4)
				{
					row ++;
					column = 0;
				}
			}
			if (radius < 8)
			{
				radius ++;
			}
			else
			{
				radius <<= 1;
			}
			{
				//String name;
				//name.print("test/out/rect7_%d.svg", ++id);
				//UIManager::desktop()->dispatch(name);
				UIManager::desktop()->dispatch();
			}
			canvas->clear();
		}
		if (thickness < 8)
		{
			thickness++;
		}
		else
		{
			thickness <<= 1;
		}
	}
}

void Rect::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test7();
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
#endif
