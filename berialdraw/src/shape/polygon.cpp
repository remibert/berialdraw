#include "berialdraw_imp.hpp"

using namespace berialdraw;

Polygon::Polygon(Canvas * canvas):
	Shape(canvas, sizeof(Polygon)),
	m_outline(new Outline)
{
}

Polygon::Polygon(const Polygon & other) :
	Shape(other),
	m_outline(new Outline(*other.m_outline))
{
	m_size = other.m_size;
}

Polygon::~Polygon()
{
	delete m_outline;
}

// Resize the FT_Polygon buffer
void Polygon::resize(uint32_t nb_points, uint32_t nb_contours)
{
	m_outline->resize(nb_points, nb_contours);
}

// Add point
void Polygon::add_point(const Point& p)
{
	m_outline->add_point(p);
}

void Polygon::add_point(Coord x, Coord y)
{
	Point p(x,y,true);
	m_outline->add_point(p);
}

void Polygon::add_point_(Coord x, Coord y)
{
//#define FLOAT_PART(value) ((((abs(value)%64)*1000)/64)+5)/10
//bd_printf(";%d.%02d, %d.%02d\n", x/64, FLOAT_PART(x), y/64, FLOAT_PART(y));
	Point p(x,y, false);
	m_outline->add_point(p);
}

// Add conic curve
void Polygon::add_conic(const Point& p)
{
	m_outline->add_conic(p);
}

void Polygon::add_conic(Coord x, Coord y)
{
	Point p(x,y, true);
	m_outline->add_conic(p);
}

void Polygon::add_conic_(Coord x, Coord y)
{
	Point p(x,y, false);
	m_outline->add_conic(p);
}

// Add bezier curve
void Polygon::add_cubic(const Point& p) 
{
	m_outline->add_cubic(p);
}

void Polygon::add_cubic(Coord x, Coord y)
{
	Point p(x,y, true);
	m_outline->add_cubic(p);
}

void Polygon::add_cubic_(Coord x, Coord y)
{
	Point p(x,y, false);
	m_outline->add_cubic(p);
}

// Select next contour (required after each adds else nothing rendered)
void Polygon::next_contour()
{
	m_outline->next_contour();
}

// Clear polygon (remove all points)
void Polygon::clear()
{
	m_outline->clear();
}

// Render polygon
void Polygon::paint(const Point & shift)
{
	UIManager::renderer()->draw(*this, shift);
}


/** Set the zoom ratio for the polygon
@param z zoom value */
void Polygon::zoom(int z)
{
	m_outline->zoom_(abs(z) <<6);
}
		
/** Get the zoom ratio for the polygon
@return zoom zoom value */
uint32_t Polygon::zoom()
{
	return m_outline->zoom_() >> 6;
}

/** Set the zoom ratio for the polygon
@param z zoom value shifted by 6 bits */
void Polygon::zoom_(int z)
{
	m_outline->zoom_(abs(z));
}

/** Get the zoom ratio for the polygon
@return zoom zoom value shifted by 6 bits */
uint32_t Polygon::zoom_()
{
	return m_outline->zoom_();
}

// Draws a rounded arc at a given point with specified parameters.
void Polygon::arc_(Coord x, Coord y, Coord radius, Dim thickness, uint32_t flags_)
{
	// If no thickness and no radius : optimize treatment
	if (thickness == 0 && radius == 0)
	{
		add_point_(x,y);
	}
	else
	{
		Point p1;
		Point d1;
		Point d2;
		Point p2;

		enum Polygon::ArcFlags arc_tip = (enum Polygon::ArcFlags)(flags_ & EDGE_MASK);

		p1.set_(x,y);

		bool right_angle_end   = (flags_ & FLAG_RIGHT_ANGLE_END) != 0;
		bool straight          = (flags_ & FLAG_STRAIGHT) != 0;
		bool end               = (flags_ & FLAG_END) != 0;
		bool reverse           = (flags_ & FLAG_REVERSE) != 0;
		bool intern            = (flags_ & FLAG_INTERNAL) != 0;
		bool inner             = (flags_ & FLAG_INNER) != 0;

		if (right_angle_end)
		{
			if (end == false)
			{
				right_angle_end = false;
			}
		}
		if (radius > -1)
		{
			Coord vec_x       = 0;
			Coord vec_y       = 0;
			Coord handle      = 0;
			Coord intersection = 0;
			Coord move = 0;
			Dim width = 0;

			UIManager::arc_cache()->compute(radius, thickness, width, vec_x, vec_y, handle, intersection);

			if (radius > (Coord)thickness)
			{
				if (right_angle_end || straight)
				{
					move = radius - intersection;
				}
			}

			if (inner && end)
			{
				right_angle_end = true;
			}

			switch(arc_tip)
			{
			case TOP_TO_RIGHT:    case TOP_TO_LEFT:    d1.y_(y);           d2.y_(y + (width-vec_y));        p2.y_(y + width);        break;
			case BOTTOM_TO_RIGHT: case BOTTOM_TO_LEFT: d1.y_(y);           d2.y_(y - (width-vec_y));        p2.y_(y - width);        break;
			case RIGHT_TO_TOP:    case LEFT_TO_TOP:    d1.y_(y - handle);  d2.y_(y - (intersection-vec_x)); p2.y_(y - intersection); break;
			case RIGHT_TO_BOTTOM: case LEFT_TO_BOTTOM: d1.y_(y + handle);  d2.y_(y + (intersection-vec_x)); p2.y_(y + intersection); break;
			default:break;
			}
			switch(arc_tip)
			{
			case BOTTOM_TO_RIGHT: case TOP_TO_RIGHT:    d1.x_(x + handle); d2.x_(x + (intersection-vec_x)); p2.x_(x + intersection); break;
			case BOTTOM_TO_LEFT:  case TOP_TO_LEFT:     d1.x_(x - handle); d2.x_(x - (intersection-vec_x)); p2.x_(x - intersection); break;
			case RIGHT_TO_TOP:    case RIGHT_TO_BOTTOM: d1.x_(x);          d2.x_(x - (width-vec_y));        p2.x_(x - width);        break;
			case LEFT_TO_TOP:     case LEFT_TO_BOTTOM:  d1.x_(x);          d2.x_(x + (width-vec_y));        p2.x_(x + width);        break;
			default:break;
			}

			if (right_angle_end)
			{
				switch(arc_tip)
				{
				case TOP_TO_RIGHT:
				case TOP_TO_LEFT:
				case BOTTOM_TO_RIGHT:
				case BOTTOM_TO_LEFT:
					p1.x_(p2.x_());
					break;
				case RIGHT_TO_TOP:
				case RIGHT_TO_BOTTOM:
				case LEFT_TO_TOP:
				case LEFT_TO_BOTTOM:
					p1.y_(p2.y_());
					break;
					default:break;
				}
			}

			if (move > 0)
			{
				switch(arc_tip)
				{
				case LEFT_TO_TOP:   case RIGHT_TO_TOP   : p1.move_(0,-move); d1.move_(0,-move); d2.move_(0,-move); p2.move_(0,-move); break;
				case LEFT_TO_BOTTOM:case RIGHT_TO_BOTTOM: p1.move_(0,move) ; d1.move_(0,move) ; d2.move_(0,move) ; p2.move_(0,move);  break;
				case TOP_TO_LEFT:   case BOTTOM_TO_LEFT : p1.move_(-move,0); d1.move_(-move,0); d2.move_(-move,0); p2.move_(-move,0); break;
				case TOP_TO_RIGHT:  case BOTTOM_TO_RIGHT: p1.move_(move,0) ; d1.move_(move,0) ; d2.move_(move,0) ; p2.move_(move,0);  break;
				default:break;
				}
			}
			if (reverse)
			{
				Point p22(p2);
				if (radius < (Coord)thickness)
				{
					switch(arc_tip)
					{
					case TOP_TO_RIGHT:   case TOP_TO_LEFT:    p22.move_(0,thickness-radius)  ;break;
					case BOTTOM_TO_RIGHT:case BOTTOM_TO_LEFT: p22.move_(0,0-(thickness-radius));break;
					case RIGHT_TO_TOP:   case RIGHT_TO_BOTTOM:p22.move_(0-(thickness-radius),0);break;
					case LEFT_TO_TOP:    case LEFT_TO_BOTTOM: p22.move_(thickness-radius,0)  ;break;
					default:break;
					}
					add_point(p22);
				}

				add_point(p2);
				if (right_angle_end == false)
				{
					add_cubic(d2);
					add_cubic(d1);
				}
				add_point(p1);
			}
			else
			{
				add_point(p1);
				if (right_angle_end == false)
				{
					add_cubic(d1);
					add_cubic(d2);
				}
				add_point(p2);
				if (radius < (Coord)thickness)
				{
					switch(arc_tip)
					{
					case TOP_TO_RIGHT:   case TOP_TO_LEFT:    p2.move_(0,thickness-radius)  ;break;
					case BOTTOM_TO_RIGHT:case BOTTOM_TO_LEFT: p2.move_(0,0-(thickness-radius));break;
					case RIGHT_TO_TOP:   case RIGHT_TO_BOTTOM:p2.move_(0-(thickness-radius),0);break;
					case LEFT_TO_TOP:    case LEFT_TO_BOTTOM: p2.move_(thickness-radius,0)  ;break;
					default:break;
					}
					add_point(p2);
				}
			}
		}
		else // No rounded end
		{
			switch(arc_tip)
			{
			case TOP_TO_RIGHT:
			case TOP_TO_LEFT:
				if (intern)
					p1.x_(p1.x_()+radius);
				else        
					p1.x_(p1.x_()-radius);
				break;
			case BOTTOM_TO_RIGHT:
			case BOTTOM_TO_LEFT:
				if (intern)
					p1.x_(p1.x_()-radius);
				else        
					p1.x_(p1.x_()+radius);
				break;
			case RIGHT_TO_TOP:
			case RIGHT_TO_BOTTOM:
				if (intern)
					p1.y_(p1.y_()+radius);
				else        
					p1.y_(p1.y_()-radius);
				break;
			case LEFT_TO_TOP:
			case LEFT_TO_BOTTOM:
				if (intern)
					p1.y_(p1.y_()-radius);
				else        
					p1.y_(p1.y_()+radius);
				break;
			default:break;
			}
			add_point(p1);
		}
	}
}

#ifdef _DEBUG
void Polygon::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		Window window;
			window.position(20,20);
			window.size(440,440);
			window.color(Color::WHITE);

		Canvas * canvas = new Canvas(&window);
			canvas->position(0,0);
			canvas->size(window.size());
			canvas->color(Color::YELLOW,32);
	
		{
			Polygon * square = new Polygon(canvas);
				square->color(Color::RED,128);
				square->add_point(100, 100);
				square->add_point(200, 100);
				square->add_point(200, 200);
				square->add_point(100, 200);
				square->position(-100, -100);
		}

		{
			Point center(240, 120);

			Cross * cross;
				cross = new Cross(canvas);
				cross->radius(5);
				cross->thickness(1);
				cross->color(Color::BLACK);
				cross->position(center);

			Polygon * polygon = new Polygon(canvas);
					polygon->color(Color::GREEN,64);
					polygon->add_point(0, 50);
					polygon->add_cubic( 50, 50);
					polygon->add_cubic(-50, -50);
					polygon->add_point(0, -50);
					polygon->center(0, 0);
					polygon->position(center);

			Polygon * clone;

			for(int i = 15; i < 180; i+=15)
			{
				clone = new Polygon(*polygon);
					polygon->angle(i);
			}

			for(int i = 0; i < 180; i+=30)
			{
				cross = new Cross(canvas);
					cross->radius(5);
					cross->thickness(1);
					cross->color(Color::BLACK);
					cross->position(i + 100, i + 200);
				clone = new Polygon(*polygon);
					clone->position(i + 100, i + 200);
					clone->angle(i);
			}
		}
		UIManager::desktop()->dispatch("${tests}/out/polygon1.svg");
	}
}
#endif