#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Constructor for sketch
Sketch::Sketch(Canvas * canvas) :
	Shape(canvas, sizeof(Sketch)),
	m_polygon(0),
	m_image(canvas),
	m_parent(canvas)
{
}

// Constructor for sketch
Sketch::Sketch(Widget * widget) :
	Shape(widget, sizeof(Sketch)),
	m_polygon(0),
	m_image(widget),
	m_parent(widget)
{
}

// Copy constructor for sketch
Sketch::Sketch(const Sketch & other):
	Shape(other),
	m_polygon(0),
	m_image(other.m_image),
	m_parent(other.m_parent)
{
}

// Render polygon
void Sketch::paint(const Area & foreclip, uint32_t stated_color)
{
	if (m_image.loaded() == false)
	{
		load();
	}

	if (m_image.path_count() > 0)
	{
		Point shift(foreclip.position());
		Point position;
		Dim zoom = 64;

		// Adapt the zoom factor and the position according to the size if it has been defined
		if (m_size.is_width_undefined() == false || m_size.is_height_undefined() == false)
		{
			zoom = m_image.fit_zoom_q6(m_size);
			Area area(m_position, m_size);
			Size content = m_image.content_size(zoom);
			Area foreclip_sketch;

			place_in_layout(area, content, m_margin, EXTEND_NONE, foreclip_sketch, m_align);
			position = foreclip_sketch.position();
		}
		else
		{
			position = m_position;
		}

		m_image.paint(position, shift, m_center, m_angle, zoom, stated_color, &m_polygon);
	}
}

// Render outline
void Sketch::paint(const Point & shift)
{
	Area foreclip;
	foreclip.position(shift);
	Sketch::paint(foreclip, color());
}

// Destructor
Sketch::~Sketch()
{
}

// Add bezier path
void Sketch::add_path(uint32_t color, const String & path)
{
	m_image.add_path(color, path);
}

// Clear all bezier path
void Sketch::clear_paths()
{
	m_image.clear_paths();
}

// Get the resolution
const Size & Sketch::resolution() const
{
	return m_image.resolution();
}

// Set the resolution
void Sketch::resolution(const Size & resolution_)
{
	m_image.resolution(resolution_);
}

// Set the resolution with width and height in pixels
void Sketch::resolution(Dim w, Dim h)
{
	m_image.resolution(w, h);
}

// Set the resolution with a precision of 64th of a pixel
void Sketch::resolution_q6(Dim w, Dim h)
{
	m_image.resolution_q6(w, h);
}


// Get the filename value
const String & Sketch::filename() const
{
	return m_image.filename();
}

// Set filename value with string
void Sketch::filename(const String & s)
{
	m_image.filename(s);
}

// Load the icon from filename
bool Sketch::load()
{
	return m_image.load();
}

// Serialize the content of widget into json
void Sketch::serialize(JsonIterator & it)
{
	m_image.serialize(it);
}

// Unserialize the content of widget from json
void Sketch::unserialize(JsonIterator & it)
{
	m_image.unserialize(it);
}
