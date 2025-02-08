#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
Path::Path(IconStyle * parent) : m_parent(parent)
{
}

/** Destructor */
Path::~Path()
{
}

/** Gets the path value */
const String & Path::path() const
{
	return m_path;
}

/** Sets the path value */
void Path::path(const String & path_)
{
	m_path = path_;
}

/** Get the back color */
uint32_t Path::color() const
{
	return m_color;
}

/** Set the back color */
void Path::color(uint32_t col)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::REPAINT);
	m_color = col;
}

/** Set the back color with alpha */
void Path::color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::REPAINT);
	m_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}

/** Serialize the content of widget into json */
void Path::serialize(JsonIterator & it)
{
	it["color"] = m_color;
	it["path"]  = m_path;
}

/** Unserialize the content of widget from json */
void Path::unserialize(JsonIterator & it)
{
	m_color = (int)(it["color"] | (int)m_color);
	m_path  = it["path"]        | m_path;
}




/** Constructor */
IconStyle::IconStyle()
{
}

/** Destructor */
IconStyle::~IconStyle()
{
	clear_paths();
}

/** Add bezier path */
void IconStyle::add_path(uint32_t color, const String & path)
{
	Path * p = new Path(this);

	if (p)
	{
		p->color(color);
		p->path(path);
		m_paths.push_back(p);
	}
}

/** Clear all bezier path */
void IconStyle::clear_paths()
{
	for(int index = 0; index < (int)m_paths.size(); index++)
	{
		delete m_paths[index];
	}
	m_paths.clear();
}

/** Get the resolution */
const Size & IconStyle::resolution() const
{
	return m_resolution;
}

/** Set the resolution */
void IconStyle::resolution(const Size & resolution_)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_resolution = resolution_;
}

/** Set the resolution with width and height in pixels */
void IconStyle::resolution(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_resolution.set(w,h);
}

/** Set the resolution with a precision of 64th of a pixel */
void IconStyle::resolution_(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_resolution.set_(w,h);
}

/** Set the zoom ratio for the icon
@param z zoom value */
void IconStyle::zoom(Dim z)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_zoom = z <<6;
}
		
/** Get the zoom ratio for the icon
@return zoom zoom value */
Dim IconStyle::zoom()
{
	return m_zoom >> 6;
}

/** Set the zoom ratio for the icon
@param z zoom value shifted by 6 bits */
void IconStyle::zoom_(Dim z)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_zoom = z;
}
		
/** Get the zoom ratio for the icon
@return zoom zoom value shifted by 6 bits */
Dim IconStyle::zoom_()
{
	return m_zoom;
}

/** Get the filename value */
const String & IconStyle::filename()
{
	return m_filename;
}

/** Set filename value with string */
void IconStyle::filename(const String & s)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	clear_paths();
	m_filename = s;
}



/** Get the icon_padding */
const Margin & IconStyle::icon_padding() const
{
	return m_icon_padding;
}

/** Set the icon_padding */
void IconStyle::icon_padding(const Margin & m)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_icon_padding = m;
}

/** Set the icon_padding in pixels */
void IconStyle::icon_padding(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_icon_padding.set(top,left,bottom,right);
}

/** Set the icon_padding with a precision of 64th of a pixel */
void IconStyle::icon_padding_(Dim top, Dim left, Dim bottom, Dim right)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_icon_padding.set_(top,left,bottom,right);
}

/** Get the back icon_color */
uint32_t IconStyle::icon_color() const
{
	return m_icon_color;
}

/** Set the back icon_color */
void IconStyle::icon_color(uint32_t col)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_icon_color = col;
}

/** Set the back icon_color with alpha */
void IconStyle::icon_color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(this, Invalidator::REPLACE);
	m_icon_color = (col & 0xFFFFFF) | (((uint32_t)(alpha)) << 24);
}


/** Load the icon from filename */
bool IconStyle::load()
{
	bool result = false;
	if (m_paths.size() == 0)
	{
		File file;
		if (file.open(m_filename,"rb") != -1)
		{
			Json json;
			try
			{
				json.unserialize(file);
				JsonIterator it(json);
				unserialize(it);
				result = true;
			}
			catch(...)
			{
			}
		}
	}
	return result;
}


/** Serialize the content of widget into json */
void IconStyle::serialize(JsonIterator & it)
{
	it["filename"] = m_filename;
	it["icon-color"]        = m_icon_color;
	m_resolution.serialize ("resolution",it);
	//int zoom  = m_zoom  != Size::MAX_SIZE && m_zoom  != Size::MAX_SIZE ? m_zoom  : m_zoom;
	//it["zoom_"]  = zoom;
	berialdraw::unserialize("zoom_", it, m_zoom);

	m_icon_padding.serialize ("icon-padding",it);
}

/** Unserialize the content of widget from json */
void IconStyle::unserialize(JsonIterator & it)
{
	m_filename        = it["filename"]        | m_filename;
	m_icon_color       = (int)(it["icon-color"]        | (int)m_icon_color);
	m_resolution.unserialize("resolution",it);
	Dim zoom  = it["zoom_"]   | Size::MAX_SIZE;

	m_icon_padding.unserialize ("icon-padding",it);
	m_zoom    = (zoom == Size::MAX_SIZE) ? m_zoom  : zoom;

	JsonIterator paths = it["paths"];
	for(paths.first(); paths.exist(); paths.next())
	{
		Path * path = new Path(this);
		JsonIterator child = paths.child();
		path->unserialize(child);
		m_paths.push_back(path);
	}
}

/** Create new paths */
Style * IconStyle::create()
{
	return new IconStyle;
}
