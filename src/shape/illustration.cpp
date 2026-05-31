#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor for illustration on a canvas */
Illustration::Illustration(Canvas * canvas) :
	Shape(canvas, sizeof(Illustration)),
	m_polygon(0),
	m_parent(canvas)
{
}

/** Constructor for illustration on a widget */
Illustration::Illustration(Widget * widget) :
	Shape(widget, sizeof(Illustration)),
	m_polygon(0),
	m_parent(widget)
{
}

/** Destructor */
Illustration::~Illustration()
{
	if (m_image)
	{
		delete m_image;
	}
	if (m_sketch)
	{
		delete m_sketch;
	}
}

/** Check if filename has a vector extension */
bool Illustration::is_vector_extension(const String & filename)
{
	bool result = false;

	int32_t dot_pos = -1;
	for (int32_t i = (int32_t)filename.size() - 1; i >= 0; i--)
	{
		if (filename[i] == '.')
		{
			dot_pos = i;
			break;
		}
	}

	if (dot_pos >= 0)
	{
		String ext;
		for (int32_t i = dot_pos + 1; i < (int32_t)filename.size(); i++)
		{
			char c = filename[i];
			if (c >= 'A' && c <= 'Z')
			{
				c = c + ('a' - 'A');
			}
			ext += c;
		}

		if (ext == "icn" || ext == "svg")
		{
			result = true;
		}
	}

	return result;
}

/** Detect the type based on file extension and create the appropriate shape */
void Illustration::detect_type()
{
	if (m_modified)
	{
		m_modified = false;
		bool new_is_vector = is_vector_extension(m_filename);

		// Type changed: destroy old shape
		if (new_is_vector != m_is_vector || (m_image == nullptr && m_sketch == nullptr))
		{
			if (m_image)
			{
				delete m_image;
				m_image = nullptr;
			}
			if (m_sketch)
			{
				delete m_sketch;
				m_sketch = nullptr;
			}

			m_is_vector = new_is_vector;

			// Create the appropriate shape (without registering to canvas)
			if (m_is_vector)
			{
				m_sketch = new Sketch((Canvas*)nullptr);
				m_sketch->filename(m_filename);
			}
			else
			{
				m_image = new Image((Canvas*)nullptr);
				m_image->filename(m_filename);
				m_image->fit_mode(m_fit_mode);
				m_image->alpha(m_alpha);
			}
		}
		else
		{
			// Same type: just update the filename
			if (m_sketch)
			{
				m_sketch->filename(m_filename);
			}
			if (m_image)
			{
				m_image->filename(m_filename);
			}
		}
	}
}

/** Get the filename value */
const String & Illustration::filename() const
{
	return m_filename;
}

/** Set filename value with string */
void Illustration::filename(const String & s)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	m_filename = s;
	m_modified = true;
}

/** Get the fit mode */
ImageFitMode Illustration::fit_mode() const
{
	return m_fit_mode;
}

/** Set the fit mode */
void Illustration::fit_mode(ImageFitMode mode)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	m_fit_mode = mode;
	if (m_image)
	{
		m_image->fit_mode(mode);
	}
}

/** Get the alpha value */
uint8_t Illustration::alpha() const
{
	return m_alpha;
}

/** Set the alpha value */
void Illustration::alpha(uint8_t a)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::REDRAW);
	m_alpha = a;
	if (m_image)
	{
		m_image->alpha(a);
	}
}

/** Get the image width in pixels */
uint32_t Illustration::image_width() const
{
	uint32_t result = 0;

	if (m_image)
	{
		result = m_image->image_width();
	}
	else if (m_sketch)
	{
		result = (uint32_t)(m_sketch->resolution().width_() >> 6);
	}

	return result;
}

/** Get the image height in pixels */
uint32_t Illustration::image_height() const
{
	uint32_t result = 0;

	if (m_image)
	{
		result = m_image->image_height();
	}
	else if (m_sketch)
	{
		result = (uint32_t)(m_sketch->resolution().height_() >> 6);
	}

	return result;
}

/** Check if the illustration is loaded */
bool Illustration::is_loaded() const
{
	bool result = false;

	if (m_image)
	{
		result = m_image->is_loaded();
	}
	else if (m_sketch)
	{
		result = m_sketch->resolution().width_() > 0;
	}

	return result;
}

/** Check if the illustration is a vector image */
bool Illustration::is_vector() const
{
	return m_is_vector;
}

/** Load the illustration from filename */
bool Illustration::load()
{
	bool result = false;

	detect_type();

	if (m_image)
	{
		result = m_image->load();
	}
	else if (m_sketch)
	{
		result = m_sketch->load();
	}

	return result;
}

/** Get the content size */
Size Illustration::content_size()
{
	Size result;

	detect_type();

	if (m_image)
	{
		result = m_image->content_size();
	}
	else if (m_sketch)
	{
		if (m_sketch->load())
		{
			result.width_(m_sketch->resolution().width_());
			result.height_(m_sketch->resolution().height_());
		}
	}

	return result;
}

/** Paint the illustration within a clipping area */
void Illustration::paint(const Area & foreclip, const Margin & padding, uint8_t alpha, uint32_t stated_color)
{
	detect_type();

	if (m_image)
	{
		m_image->paint(foreclip, padding, alpha);
	}
	else if (m_sketch)
	{
		m_sketch->size(foreclip.size());
		m_sketch->paint(foreclip, padding, stated_color);
	}
}

/** Paint the illustration on the desktop */
void Illustration::paint(const Point & shift)
{
	detect_type();

	if (m_image)
	{
		m_image->paint(shift);
	}
	else if (m_sketch)
	{
		m_sketch->paint(shift);
	}
}
