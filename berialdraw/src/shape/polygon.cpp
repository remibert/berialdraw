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


#ifdef _DEBUG
void Polygon::test()
{
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
	UIManager::desktop()->dispatch("test/out/polygon1.svg");
}
#endif