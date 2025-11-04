#include "berialdraw_imp.hpp"

using namespace berialdraw;

Device      * UIManager::m_device = 0;
Renderer    * UIManager::m_renderer = 0;
Framebuf    * UIManager::m_framebuf = 0;
Exporter    * UIManager::m_exporter = 0;
Notifier    * UIManager::m_notifier = 0;
Invalidator * UIManager::m_invalidator = 0;
Styles      * UIManager::m_styles = 0;
Colors      * UIManager::m_colors = 0;
Fonts       * UIManager::m_fonts  = 0;
Desktop     * UIManager::m_desktop = 0;
ScreenCrc   * UIManager::m_screen_crc = 0;
ArcCache    * UIManager::m_arc_cache = 0;
bool          UIManager::m_initialized= false;


inline Dim adapt_size(Dim size, uint32_t scale)
{
	if (scale < 10)
	{
		scale <<= 6;
	}
	return ((size << 6) / scale);
}

void UIManager::init(Device * device, Dim width, Dim height, enum Framebuf::Type type, uint32_t scale, const berialdraw::String & resource_dir)
{
	if(device && m_device == 0)
	{
		File::resource_dir(resource_dir);
		m_notifier     = new Notifier;
		m_invalidator  = new Invalidator;
		m_styles       = new Styles;
		m_colors       = new Colors;
		m_fonts        = new Fonts;
		m_desktop      = new Desktop;
		m_screen_crc   = new ScreenCrc;
		m_device       = device;
		m_device->size(width, height);
		m_framebuf     = Framebuf::create(width, height, type);
		m_renderer     = new Renderer(adapt_size(width,scale), adapt_size(height,scale), scale);
		m_arc_cache    = new ArcCache;
		m_initialized = true;
	}
	else
	{
		bd_printf("UIManager::init failed");
	}
}

void UIManager::deinit()
{
	delete m_desktop;
	delete m_device;
	delete m_renderer;
	delete m_framebuf;
	delete m_exporter;
	delete m_notifier;
	delete m_invalidator;
	delete m_styles;
	delete m_colors;
	delete m_fonts;
	delete m_screen_crc;
	delete m_arc_cache;

	m_initialized = false;
	m_device      = 0;
	m_renderer    = 0;
	m_framebuf    = 0;
	m_exporter    = 0;
	m_notifier    = 0;
	m_invalidator = 0;
	m_styles      = 0;
	m_colors      = 0;
	m_fonts       = 0;
	m_desktop     = 0;
	m_screen_crc  = 0;
	m_arc_cache   = 0;
}

/** Indicates if the uimanager is initialized or not */
bool UIManager::is_initialized()
{
	return m_initialized;
}


/** Return the svg content */
Exporter * UIManager::exporter()
{
	return m_exporter;
}

// Start the export to svg
void UIManager::exporter(Exporter * export_)
{
	if (m_exporter)
	{
		m_exporter->close(m_framebuf->crc32());
		delete m_exporter;
	}

	m_exporter = export_;
	if(export_)
	{
		export_->size(UIManager::renderer()->size());
	}
}

/** Return the pointer on internal framebuf */
Framebuf * UIManager::framebuf()
{
	if (m_framebuf == 0) bd_printf("UIManager::framebuf no existing");
	return m_framebuf;
}

/** Return the notifier */
Notifier * UIManager::notifier()
{
	if (m_notifier == 0) bd_printf("UIManager::notifier no existing");
	return m_notifier;
}

/** Return the renderer */
Renderer * UIManager::renderer()
{
	if (m_renderer == 0) bd_printf("UIManager::renderer no existing");
	return m_renderer;
}

/** Return the screen device */
Device * UIManager::device()
{
	if (m_device == 0) bd_printf("UIManager::device no existing");
	return m_device;
}

/** Return the fonts manager */
Fonts * UIManager::fonts()
{
	if (m_fonts == 0) bd_printf("UIManager::fonts no existing");
	return m_fonts;
}

/** Return the dirty manager */
Invalidator * UIManager::invalidator()
{
	if (m_invalidator == 0) bd_printf("UIManager::invalidator no existing");
	return m_invalidator;
}

/** Return the styles manager */
Styles * UIManager::styles()
{
	if (m_styles == 0) bd_printf("UIManager::styles no existing");
	return m_styles;
}

/** Return the colors manager */
Colors * UIManager::colors()
{
	if (m_colors == 0) bd_printf("UIManager::colors no existing");
	return m_colors;
}

/** Return the desktop */
Desktop * UIManager::desktop()
{
	if (m_desktop == 0) bd_printf("UIManager::desktop no existing");
	return m_desktop;
}

/** Return the screen crc */
ScreenCrc * UIManager::screen_crc()
{
	if (m_screen_crc == 0) bd_printf("UIManager::screen_crc no existing");
	return m_screen_crc;
}

/** Return the arc cache handle */
ArcCache * UIManager::arc_cache()
{
	if (m_arc_cache == 0) bd_printf("UIManager::arc_cache no existing");
	return m_arc_cache;
}

