#include "berialdraw_imp.hpp"

using namespace berialdraw;

/** Constructor */
Path::Path(void * parent) : m_parent(parent)
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
	UIManager::invalidator()->dirty(m_parent, Invalidator::REDRAW);
	m_color = col;
}

/** Set the back color with alpha */
void Path::color(uint32_t col, uint8_t alpha)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::REDRAW);
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

/** Constructor for sketch
@param canvas Pointer to the canvas on which the sketch is drawn */
Sketch::Sketch(Canvas * canvas) :
	Shape(canvas, sizeof(Sketch)),
	m_polygon(0),
	m_parent(canvas)
{
}

/** Constructor for sketch
@param widget Pointer to the widget on which the sketch is drawn */
Sketch::Sketch(Widget * widget) :
	Shape(widget, sizeof(Sketch)),
	m_polygon(0),
	m_parent(widget)
{
}

/** Copy constructor for sketch
@param other Other sketch object to copy from */
Sketch::Sketch(const Sketch & other):
	Shape(other),
	m_polygon(0)
{
}

// Render polygon
void Sketch::paint(const Area & foreclip, const Margin & padding, uint32_t stated_color)
{
	if (m_loaded == false)
	{
		load();
		m_loaded = true;
	}

	// If icon existing
	if (m_paths.size() > 0)
	{
		Point shift(foreclip.position());
		Point position(foreclip.position());
		

		bool one_color = true;

		// Check if icon has only one color
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

		int zoom = zoom_();

		// Adapt the zoom factor according to the size if it has been defined
		if (m_size.is_width_undefined() == false || m_size.is_height_undefined() == false)
		{
			Dim zoom_width = 0;
			Dim zoom_height = 0;
			Size size(m_size);
			size.decrease_(padding.left_() + padding.right_(), padding.top_() + padding.bottom_());

			if (m_size.is_width_undefined() == false && size.width_() > 0 && m_resolution.width_() != 0)
			{
				zoom_width = (size.width_() << 6)/ m_resolution.width_();
			}
			if (m_size.is_height_undefined() == false && size.height_() > 0 && m_resolution.height_() != 0)
			{
				zoom_height = (size.height_() << 6) / m_resolution.height_();
			}

			if (zoom_width < zoom_height && zoom_width != 0)
			{
				zoom = zoom_width;
				if (size.height_() > m_resolution.height_())
				{
					position.move_(0, (size.height_() - size.width_())>>1);
				}
			}
			else if (zoom_height < zoom_width && zoom_height != 0)
			{
				zoom = zoom_height;
				if (size.width_() > m_resolution.width_())
				{
					position.move_((size.width_() - size.height_())>>1, 0);
				}
			}
			else if (zoom_height == zoom_width && zoom_height != 0)
			{
				zoom = zoom_height;
			}

			Area foreclip(m_position,m_size);
			Size content;
			content.width_ ((m_resolution.width_ () * zoom) >> 6);
			content.height_((m_resolution.height_() * zoom) >> 6);
			Area foreclip_sketch;

			place_in_layout(foreclip, content, m_margin, EXTEND_NONE, foreclip_sketch, (Align)m_align);
			position = foreclip_sketch.position();
		}
		else
		{
			position = m_position;
			position.move_(padding.right_(), padding.top_());
		}


		// Display icon
		for (uint32_t i = 0; i < m_paths.size(); i++)
		{
			Path * path = m_paths[i];

			if (path)
			{
				uint32_t path_color = path->color();

				// If the main color is defined
				if (stated_color != Color::TRANSPARENT)
				{
					// If icon color can be changed
					if (one_color)
					{
						// Change icon color
						path_color = stated_color;
					}
					else
					{
						// The sketch has many color
						uint32_t alpha = stated_color >> 24;

						// Change the alpha according to alpha color selected
						if (alpha < 255)
						{
							uint32_t path_alpha = ((path_color >> 24) * alpha) / 255;
							path_color = (path_color & 0xFFFFFF) | (path_alpha << 24);
						}
					}
				}

				// Apply icon zoom
				m_polygon.zoom_(zoom);
					
				// Parse icon path
				VectorsScript vectors_script(m_polygon);
				vectors_script.select(path->path());
				vectors_script.parse();

				// Paint icon path
				UIManager::renderer()->draw(position, 0, shift, m_center, path_color, m_angle, m_polygon.outline());
			}
		}
	}
}

/** Render outline */
void Sketch::paint(const Point & shift)
{
	Area foreclip;
	foreclip.position(shift);
	const Margin padding;
	Sketch::paint(foreclip, padding, color());
}

/** Destructor */
Sketch::~Sketch()
{
	clear_paths();
}

/** Add bezier path */
void Sketch::add_path(uint32_t color, const String & path)
{
	Path * p = new Path(m_parent);

	if (p)
	{
		p->color(color);
		p->path(path);
		m_paths.push_back(p);
	}
}

/** Clear all bezier path */
void Sketch::clear_paths()
{
	for(int index = 0; index < (int)m_paths.size(); index++)
	{
		delete m_paths[index];
	}
	m_paths.clear();
}

/** Get the resolution */
const Size & Sketch::resolution() const
{
	return m_resolution;
}

/** Set the resolution */
void Sketch::resolution(const Size & resolution_)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	m_resolution = resolution_;
}

/** Set the resolution with width and height in pixels */
void Sketch::resolution(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	m_resolution.set(w,h);
}

/** Set the resolution with a precision of 64th of a pixel */
void Sketch::resolution_(Dim w, Dim h)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	m_resolution.set_(w,h);
}

/** Set the zoom ratio for the icon
@param z zoom value */
void Sketch::zoom(Dim z)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	m_zoom = z <<6;
}
		
/** Get the zoom ratio for the icon
@return zoom zoom value */
Dim Sketch::zoom() const
{
	return m_zoom >> 6;
}

/** Set the zoom ratio for the icon
@param z zoom value shifted by 6 bits */
void Sketch::zoom_(Dim z)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	m_zoom = z;
}

/** Get the zoom ratio for the icon
@return zoom zoom value shifted by 6 bits */
Dim Sketch::zoom_() const
{
	return m_zoom;
}

/** Get the filename value */
const String & Sketch::filename() const
{
	return m_filename;
}

/** Set filename value with string */
void Sketch::filename(const String & s)
{
	UIManager::invalidator()->dirty(m_parent, Invalidator::GEOMETRY);
	clear_paths();
	m_loaded = false;
	m_filename = s;
}

/** Load the icon from filename */
bool Sketch::load()
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
void Sketch::serialize(JsonIterator & it)
{
	it["filename"] = m_filename;
	m_resolution.serialize ("resolution",it);
	int zoom  = m_zoom  != Size::MAX_SIZE && m_zoom  != Size::MAX_SIZE ? m_zoom  : m_zoom;
	it["zoom_"]  = zoom;
	berialdraw::unserialize("zoom_", it, m_zoom);
}

/** Unserialize the content of widget from json */
void Sketch::unserialize(JsonIterator & it)
{
	m_filename        = it["filename"]        | m_filename;
	m_resolution.unserialize("resolution",it);
	Dim zoom  = it["zoom_"]   | Size::MAX_SIZE;

	m_zoom    = (zoom == Size::MAX_SIZE) ? m_zoom  : zoom;

	JsonIterator paths = it["paths"];
	for(paths.first(); paths.exist(); paths.next())
	{
		Path * path = new Path(m_parent);
		JsonIterator child = paths.child();
		path->unserialize(child);
		m_paths.push_back(path);
	}
}

#ifdef _DEBUG
void Sketch::test1()
{
	Window window;
		window.color(Color::BLUE,64);
		window.position(7,5);

	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::RED,64);
		canvas->position(13,29);

	Rect * rect= new Rect(canvas);
		rect->position(57,31);
		rect->size(10,360);
		rect->color(Color::WHITE,64);

	Sketch * maison = new Sketch(canvas);
		maison->position(rect->position());
		maison->size(rect->size());
		maison->color(Color::YELLOW);
		maison->filename("resources/icons/maison.icn");

	Sketch * computer = new Sketch(canvas);
		computer->position(rect->position());
		computer->size(rect->size());
		computer->color(Color::YELLOW);
		computer->filename("resources/icons/computer.icn");

	UIManager::desktop()->dispatch("test/out/sketch1_1.svg");
	rect->size(360,10);
	computer->size(rect->size());
	maison->size(rect->size());
	UIManager::desktop()->dispatch("test/out/sketch1_2.svg");
	rect->size(360,360);
	computer->size(rect->size());
	maison->size(rect->size());
	UIManager::desktop()->dispatch("test/out/sketch1_3.svg");
	rect->size(60,360);
	computer->size(rect->size());
	maison->size(rect->size());
	UIManager::desktop()->dispatch("test/out/sketch1_4.svg");
	rect->size(360,60);
	computer->size(rect->size());
	maison->size(rect->size());
	UIManager::desktop()->dispatch("test/out/sketch1_5.svg");
}

void Sketch::test2()
{
	Window window;

	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::GREEN,64);
		canvas->size(480,480);

	Rect * rect= new Rect(canvas);
		rect->position(240,240);
		rect->size(100,100);
		rect->color(Color::RED,64);
		rect->repeat(Shape::REPEAT_ANGLE, 0, 359, 30);

	Sketch * maison = new Sketch(canvas);
		maison->filename("resources/icons/maison.icn");
		maison->size(rect->size());
		maison->color(Color::WHITE,128);
		maison->position(rect->position());
		maison->repeat(Shape::REPEAT_ANGLE, 0, 359, 30);

	Sketch * computer = new Sketch(canvas);
		computer->position(rect->position());
		computer->size(rect->size());
		computer->color(Color::YELLOW);
		computer->filename("resources/icons/computer.icn");
		computer->repeat(Shape::REPEAT_ANGLE, 0, 359, 30);

	Coord pos = 0;
	maison->center(pos,50);
	rect->center(pos,50);
	computer->center(pos,50);
	computer->color(Color::YELLOW,32);
	UIManager::desktop()->dispatch("test/out/sketch2_1.svg");

	pos -= 100;
	maison->color(Color::BLUE,64);
	maison->center(pos,50);
	rect->center(pos,50);
	computer->center(pos,50);
	computer->color(Color::BLUE,64);
	UIManager::desktop()->dispatch("test/out/sketch2_2.svg");

	pos -= 50;
	maison->center(pos,100);
	maison->color(Color::RED);
	rect->center(pos,100);
	computer->center(pos,100);
	computer->color(Color::RED);
	UIManager::desktop()->dispatch("test/out/sketch2_3.svg");
}

void Sketch::test3()
{
	Window window;

	Canvas * canvas = new Canvas(&window);
		canvas->color(Color::RED,64);
		canvas->extend(Extend::EXTEND_ALL);

	Rect * rect= new Rect(canvas);
		rect->position(57,31);
		rect->size(32,360);
		rect->color(Color::WHITE,64);

	Sketch * maison = new Sketch(canvas);
		maison->position(rect->position());
		maison->size(rect->size());
		maison->filename("resources/icons/maison.icn");
	UIManager::desktop()->dispatch("test/out/sketch3_0.svg");

	maison->align(Align::ALIGN_BOTTOM);
	UIManager::desktop()->dispatch("test/out/sketch3_1.svg");

	maison->align(Align::ALIGN_TOP);
	UIManager::desktop()->dispatch("test/out/sketch3_2.svg");

	maison->align(Align::ALIGN_VERTICAL);
	UIManager::desktop()->dispatch("test/out/sketch3_3.svg");

	rect->size(360,16);
	maison->size(rect->size());
	maison->align(Align::ALIGN_LEFT);
	UIManager::desktop()->dispatch("test/out/sketch3_6.svg");

	maison->align(Align::ALIGN_RIGHT);
	UIManager::desktop()->dispatch("test/out/sketch3_4.svg");

	maison->align(Align::ALIGN_HORIZONTAL);
	UIManager::desktop()->dispatch("test/out/sketch3_5.svg");
}

void Sketch::test4()
{
}

void Sketch::test5()
{
}

void Sketch::test6()
{
}

void Sketch::test()
{
	static bool done = false;
	if (done == false)
	{
		done = true;
		test6();
		test5();
		test4();
		test3();
		test2();
		test1();
	}
}
#endif
