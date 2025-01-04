#include "berialdraw_imp.hpp"

#if defined(_DEBUG)
static bool trace=false;
#endif

using namespace berialdraw;

Outline::Outline()
{
	memset(&m_outline, 0, sizeof(m_outline));
	resize(4, 1);
}

Outline::Outline(const Outline & other)
{
	set(other.m_outline);
	m_outline.n_points   = other.m_outline.n_points;
	m_outline.n_contours = other.m_outline.n_contours;
	m_contour_opened      = other.m_contour_opened;
	m_zoom                = other.m_zoom;
}

Outline::Outline(const FT_Outline &outline)
{
	set(outline);
}

Outline::~Outline()
{
	if (m_outline.points)
	{
		delete[] m_outline.points;
	}

	if (m_outline.tags)
	{
		delete[] m_outline.tags;
	}

	if (m_outline.contours)
	{
		delete[] m_outline.contours;
	}
}

// Set the freetype outline object
void Outline::set(const FT_Outline & outline)
{
	memset(&m_outline, 0, sizeof(m_outline));
	resize(outline.n_points, outline.n_contours);
	m_outline.n_contours = outline.n_contours;
	m_outline.n_points   = outline.n_points;
	FT_Outline_Copy(&outline, &m_outline);
	m_contour_opened      = false;
}

// Get the freetype outline object
FT_Outline & Outline::get()
{
	if (m_contour_opened)
	{
		next_contour();
	}
	return m_outline;
}

// Resize the FT_Outline buffer
void Outline::resize(uint32_t nb_points, uint32_t nb_contours)
{
	// If the point quantity is not enough
	if (nb_points > m_nb_points)
	{
		// Copy vectors
		FT_Vector *points = new FT_Vector[nb_points]();
		
		if (m_outline.points)
		{
			memcpy(points, m_outline.points, m_outline.n_points * sizeof(FT_Vector));
			delete[] m_outline.points;
		}
		m_outline.points = points;

		// Copy points
		unsigned char * tags = new unsigned char[nb_points]();
		if (m_outline.tags)
		{
			memcpy(tags, m_outline.tags,  m_outline.n_points * sizeof(char));
			delete[] m_outline.tags;
		}
		m_outline.tags = tags;

		m_nb_points = nb_points;
	}

	// If the contour quantity is not enough
	if(nb_contours > m_nb_contours)
	{
		// Copy contours
		unsigned short * contours = new unsigned short[nb_contours]();
		if (m_outline.contours)
		{
			memcpy(contours, m_outline.contours,  nb_contours * sizeof(short));
			delete[] m_outline.contours;
		}
		m_outline.contours = contours;
		m_nb_contours = nb_contours;
	}
}

// Add point
void Outline::add_point(const Point& p)
{
	add(p.x_(), p.y_(), FT_CURVE_TAG_ON);
}

// Add conic curve
void Outline::add_conic(const Point& p)
{
	add(p.x_(), p.y_(), FT_CURVE_TAG_CONIC);
}

// Add bezier curve
void Outline::add_cubic(const Point& p) 
{
	add(p.x_(), p.y_(), FT_CURVE_TAG_CUBIC);
}

// Add point in freetype FT_Outline object
void Outline::add(int32_t x, int32_t y, uint32_t tag)
{
	if(x < -Outline::MAX_COORD)
	{
		x = -Outline::MAX_COORD;
	}
	else if(x > Outline::MAX_COORD)
	{
		x = Outline::MAX_COORD;
	}

	if(y < -Outline::MAX_COORD)
	{
		y = -Outline::MAX_COORD;
	}
	else if(y > Outline::MAX_COORD)
	{
		y = Outline::MAX_COORD;
	}

	if ((uint32_t)m_outline.n_points >= m_nb_points)
	{
		// Increase the quantity to allocate to avoid too frequent reallocations
		resize(m_nb_points + 16, m_nb_contours);
	}

	FT_Vector v;
	v.x = x;
	v.y = y;

#if defined(_DEBUG)
	if(trace) bd_printf("%s (x=%4d,y=%4d)\n",(tag == FT_CURVE_TAG_ON ? "Point" : tag == FT_CURVE_TAG_CONIC ? "Conic" : "Cubic"), x >> 6, y >> 6);
#endif
	m_outline.points[m_outline.n_points] = v;
	m_outline.tags  [m_outline.n_points] = tag;
	m_outline.n_points ++;
	m_contour_opened = true;
}

// Append existing outline to the current outline
void Outline::append(const Outline & other)
{
	resize(m_nb_points + other.m_nb_points, m_nb_contours + other.m_nb_contours);
	int32_t last_point = m_outline.n_points;
	
	for (int32_t i = 0; i < other.m_outline.n_points; i++)
	{
		add(other.m_outline.points[i].x, other.m_outline.points[i].y, other.m_outline.tags[i]);
	}

	for (int32_t i = 0; i < other.m_outline.n_contours; i++)
	{
		m_outline.contours[m_outline.n_contours] = last_point+ other.m_outline.contours[i];
	}
}

// Select next contour (required after each adds else nothing rendered)
void Outline::next_contour()
{
	if ((uint32_t)m_outline.n_contours >= m_nb_contours)
	{
		resize(m_nb_points, m_nb_contours + 1);
	}
		
	m_outline.contours[m_outline.n_contours] = m_outline.n_points - 1;
	m_outline.n_contours ++;
	m_contour_opened = false;
}

// Clear polygon (remove all points)
void Outline::clear()
{
	m_outline.n_points   = 0;
	m_outline.n_contours = 0;
	m_contour_opened = true;
}

/** Set the zoom ratio for the polygon
@param z zoom value shifted by 6 bits */
void Outline::zoom_(uint32_t z)
{
	m_zoom = z;
}

/** Return the zoom ratio for the polygon
@return zoom value shifted by 6 bits */
uint32_t Outline::zoom_()
{
	return m_zoom;
}

#ifdef _DEBUG
void Outline::test()
{
	Window window;
		window.position(0,0);
		window.size(480,480);
		window.color(Color::WHITE);

	Canvas * canvas = new Canvas(&window);
		canvas->position(0,0);
		canvas->size(window.size());

	Rect * rect = new Rect(canvas);

		rect->size(200,50);
		rect->radius(10);
		rect->color(PURPLE);
		rect->thickness(5);
		rect->position(0,0);
		rect->center(0,0);
		rect->angle(0);

	UIManager::desktop()->dispatch();
	const Polygon & poly = rect->polygon();
	
	Outline outline(poly.outline());
	outline.next_contour();

	SvgOut svg;
	svg.add_path(outline, rect->color());
	bd_printf("%s\n",svg.get().c_str());
}
#endif