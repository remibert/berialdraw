#include "berialdraw_imp.hpp"

using namespace berialdraw;

// Constructor
Path::Path(void * parent) : m_parent(parent)
{
}

// Destructor
Path::~Path()
{
}

// Gets the path value
const String & Path::path() const
{
	return m_path;
}

// Sets the path value
void Path::path(const String & path_)
{
	m_path = path_;
}

// Get the back color
uint32_t Path::color() const
{
	return m_color;
}

// Set the back color
void Path::color(uint32_t col)
{
	if (m_parent)
	{
		UIManager::invalidator()->dirty(m_parent, Invalidator::REDRAW);
	}
	m_color = col;
}

// Set the back color with alpha
void Path::color(uint32_t col, uint8_t alpha)
{
	if (m_parent)
	{
		UIManager::invalidator()->dirty(m_parent, Invalidator::REDRAW);
	}
	m_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

// Serialize the content of widget into json
void Path::serialize(JsonIterator & it)
{
	it[StyleNames::SKETCH_COLOR] = m_color;
	it[StyleNames::SKETCH_PATH]  = m_path;
}

// Unserialize the content of widget from json
void Path::unserialize(JsonIterator & it)
{
	m_color = (int)(it[StyleNames::SKETCH_COLOR] | (int)m_color);
	m_path  = it[StyleNames::SKETCH_PATH]        | m_path;
}

// Constructor
VectorImage::VectorImage(void * parent) :
	m_parent(parent)
{
	m_image_size.set(24, 24);
}

// Copy constructor
VectorImage::VectorImage(const VectorImage & other) :
	m_resolution(other.m_resolution),
	m_image_size(other.m_image_size),
	m_image_color(other.m_image_color),
	m_filename(other.m_filename),
	m_loaded(other.m_loaded),
	m_parent(other.m_parent)
{
	for (uint32_t i = 0; i < other.m_paths.size(); i++)
	{
		const Path * p = other.m_paths[i];
		if (p)
		{
			add_path(p->color(), p->path());
		}
	}
}

// Destructor
VectorImage::~VectorImage()
{
	clear_paths();
}

// Serialize filename and resolution into json
void VectorImage::serialize(JsonIterator & it)
{
	it[StyleNames::SKETCH_FILENAME] = m_filename;
	m_resolution.serialize(StyleNames::SKETCH_RESOLUTION, it);
}

// Unserialize filename, resolution and paths from json
void VectorImage::unserialize(JsonIterator & it)
{
	m_filename = it[StyleNames::SKETCH_FILENAME] | m_filename;
	m_resolution.unserialize(StyleNames::SKETCH_RESOLUTION, it);

	JsonIterator paths = it[StyleNames::SKETCH_PATHS];
	bool cleared = false;
	for (paths.first(); paths.exist(); paths.next())
	{
		// Replace previous paths only if the json provides new ones
		if (cleared == false)
		{
			clear_paths();
			cleared = true;
		}
		Path * path = new Path(m_parent);
		JsonIterator child = paths.child();
		path->unserialize(child);
		m_paths.push_back(path);
	}
}

// Add bezier path
void VectorImage::add_path(uint32_t color, const String & path)
{
	Path * p = new Path(m_parent);

	if (p)
	{
		p->color(color);
		p->path(path);
		m_paths.push_back(p);
	}
}

// Clear all bezier path
void VectorImage::clear_paths()
{
	for (int index = 0; index < (int)m_paths.size(); index++)
	{
		delete m_paths[index];
	}
	m_paths.clear();
}

// Number of paths
uint32_t VectorImage::path_count() const
{
	return (uint32_t)m_paths.size();
}

// Get the resolution
const Size & VectorImage::resolution() const
{
	return m_resolution;
}

// Get the inline text size
const Size & VectorImage::image_size() const
{
	return m_image_size;
}

// Set the inline text size
void VectorImage::image_size(const Size & size)
{
	m_image_size = size;
}

// Get the inline text color override
uint32_t VectorImage::image_color() const
{
	return m_image_color;
}

// Set the inline text color override
void VectorImage::image_color(uint32_t color)
{
	m_image_color = color;
}

// Set the resolution
void VectorImage::resolution(const Size & resolution_)
{
	if (m_parent)
	{
		UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	}
	m_resolution = resolution_;
}

// Set the resolution with width and height in pixels
void VectorImage::resolution(Dim w, Dim h)
{
	if (m_parent)
	{
		UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	}
	m_resolution.set(w, h);
}

// Set the resolution with a precision of 64th of a pixel
void VectorImage::resolution_q6(Dim w, Dim h)
{
	if (m_parent)
	{
		UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	}
	m_resolution.set_q6(w, h);
}

// Get the filename value
const String & VectorImage::filename() const
{
	return m_filename;
}

// Set filename value (drops loaded paths)
void VectorImage::filename(const String & s)
{
	if (m_parent)
	{
		UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	}
	clear_paths();
	m_loaded = false;
	m_filename = s;
}

// Same as load(), keeps the parsed json so the caller can read extra fields
bool VectorImage::load()
{
	bool result = false;
	m_loaded = true;

	if (m_paths.size() == 0)
	{
		File file;
		String filename = m_filename;
		filename.strip();
		Json json;
		JsonIterator it(json);

		if (file.open(filename, "rb") != -1)
		{
			try
			{
				json.unserialize(file);
				JsonIterator it(json);
				unserialize(it);
				result = true;
			}
			catch (...)
			{
				bd_printf("Unable to decode the sketch file '%s'\n", filename.c_str());
			}
		}
	}
	return result;
}

// True once a load has been attempted for the current filename
bool VectorImage::loaded() const
{
	return m_loaded;
}

// Zoom (Q6) fitting the resolution into size
Dim VectorImage::fit_zoom_q6(const Size & size) const
{
	Dim zoom = 0;

	if (size.is_width_undefined() == false || size.is_height_undefined() == false)
	{
		Dim zoom_width = 0;
		Dim zoom_height = 0;

		if (size.is_width_undefined() == false && size.width_q6() > 0 && m_resolution.width_q6() != 0)
		{
			zoom_width = (size.width_q6() << 6) / m_resolution.width_q6();
		}
		if (size.is_height_undefined() == false && size.height_q6() > 0 && m_resolution.height_q6() != 0)
		{
			zoom_height = (size.height_q6() << 6) / m_resolution.height_q6();
		}

		// Keep the smallest ratio so the image fits in both directions
		if (zoom_width < zoom_height && zoom_width != 0)
		{
			zoom = zoom_width;
		}
		else if (zoom_height < zoom_width && zoom_height != 0)
		{
			zoom = zoom_height;
		}
		else if (zoom_height == zoom_width && zoom_height != 0)
		{
			zoom = zoom_height;
		}
		if (zoom == 0)
		{
			if (zoom_height == 0 && zoom_width != 0)
			{
				zoom = zoom_width;
			}
			else if (zoom_width == 0 && zoom_height != 0)
			{
				zoom = zoom_height;
			}
		}
	}
	return zoom;
}

// Size of the image once zoomed
Size VectorImage::content_size(Dim zoom_q6) const
{
	Size content;
	content.width_q6((m_resolution.width_q6() * zoom_q6) >> 6);
	content.height_q6((m_resolution.height_q6() * zoom_q6) >> 6);
	return content;
}

// Paint all paths
void VectorImage::paint(const Point & position, const Point & shift, const Point & center, Coord angle, Dim zoom_q6, uint32_t stated_color, Polygon * scratch)
{
	if (m_paths.size() > 0)
	{
		Polygon local(0);
		Polygon & polygon = scratch ? *scratch : local;
		bool one_color = true;

		// Check if image has only one color
		for (uint32_t i = 1; i < m_paths.size(); i++)
		{
			Path * path = m_paths[i];
			Path * path_prev = m_paths[i-1];

			if (path && path_prev)
			{
				if (path->color() != path_prev->color())
				{
					one_color = false;
					break;
				}
			}
		}

		for (uint32_t i = 0; i < m_paths.size(); i++)
		{
			Path * path = m_paths[i];

			if (path)
			{
				uint32_t path_color = path->color();

				// If the main color is defined
				if (stated_color != Color::TRANSPARENT)
				{
					if (one_color)
					{
						path_color = stated_color;
					}
					else
					{
						// Multi-color image: only apply the alpha
						uint32_t alpha = stated_color >> 24;
						if (alpha < 255)
						{
							uint32_t path_alpha = ((path_color >> 24) * alpha) / 255;
							path_color = (path_color & 0xFFFFFF) | (path_alpha << 24);
						}
					}
				}

				polygon.zoom_q6(zoom_q6);

				// Parse path
				VectorsScript vectors_script(polygon);
				vectors_script.select(path->path());
				vectors_script.parse();

				UIManager::renderer()->draw_outline(position, 0, shift, center, path_color, angle, polygon.outline());
			}
		}
	}
}
