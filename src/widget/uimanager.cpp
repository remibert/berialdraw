#include "berialdraw_imp.hpp"
#include <memory>

using namespace berialdraw;

Device      * UIManager::m_device = 0;
std::unique_ptr<Renderer>    UIManager::m_renderer;
std::unique_ptr<Framebuf>    UIManager::m_framebuf;
Exporter    * UIManager::m_exporter = 0;
std::unique_ptr<Notifier>    UIManager::m_notifier;
std::unique_ptr<Invalidator> UIManager::m_invalidator;
std::unique_ptr<Styles>      UIManager::m_styles;
std::unique_ptr<Colors>      UIManager::m_colors;
std::unique_ptr<Fonts>       UIManager::m_fonts;
std::unique_ptr<Desktop>     UIManager::m_desktop;
std::unique_ptr<ScreenCrc>   UIManager::m_screen_crc;
std::unique_ptr<ArcCache>    UIManager::m_arc_cache;
std::unique_ptr<Settings>    UIManager::m_settings;
std::unique_ptr<Clipboard>   UIManager::m_clipboard;
std::unique_ptr<ImageCache>  UIManager::m_image_cache;
bool          UIManager::m_initialized= false;

inline Dim adapt_scale(uint32_t scale)
{
	if (scale == 0)
	{
		scale = 1<<6;
	}
	else if (scale <= 31)
	{
		scale = scale << 6;
	}
	return scale;
}

inline Dim adapt_size(Dim size, uint32_t scale)
{
	return ((size << 6) / scale);
}

void UIManager::init(Device * device, Dim width, Dim height, enum Framebuf::Type type, uint32_t scale, const berialdraw::String & root_dir)
{
	if(device && m_device == 0)
	{
		m_settings     = std::make_unique<Settings>();
		scale = adapt_scale(scale);
		String dir = File::resolve(root_dir);
		if (dir == "")
		{
			dir = ".";
		}
		(*m_settings)["ui"]["root-dir"]  = dir;
		(*m_settings)["ui"]["fonts"]     = "$(ui.root-dir)/fonts";
		(*m_settings)["ui"]["styles"]    = "$(ui.root-dir)/styles";
		(*m_settings)["ui"]["icons"]     = "$(ui.root-dir)/icons";
		(*m_settings)["ui"]["images"]    = "$(ui.root-dir)/images";
		(*m_settings)["ui"]["colors"]    = "$(ui.root-dir)/colors";
		(*m_settings)["ui"]["tests"]     = "$(ui.root-dir)/../test/snapshot";
		m_notifier     = std::make_unique<Notifier>();
		m_invalidator  = std::make_unique<Invalidator>();
		m_styles       = std::make_unique<Styles>();
		m_colors       = std::make_unique<Colors>();
		m_fonts        = std::make_unique<Fonts>();
		m_desktop      = std::make_unique<Desktop>();
		m_screen_crc   = std::make_unique<ScreenCrc>();
		m_device       = device;
		m_clipboard    = std::make_unique<Clipboard>();
		m_device->size(width, height);
		m_framebuf     = std::unique_ptr<Framebuf>(Framebuf::create(width, height, type));
		m_renderer     = std::make_unique<Renderer>(adapt_size(width,scale), adapt_size(height,scale), scale);
		m_arc_cache    = std::make_unique<ArcCache>();
		m_image_cache  = std::make_unique<ImageCache>();
		m_initialized = true;
	}
	else
	{
		bd_printf("UIManager::init failed");
	}
}

void UIManager::deinit()
{
	m_desktop.reset();
	delete m_device;
	m_renderer.reset();
	m_framebuf.reset();
	delete m_exporter;
	m_notifier.reset();
	m_invalidator.reset();
	m_styles.reset();
	m_colors.reset();
	m_fonts.reset();
	m_screen_crc.reset();
	m_arc_cache.reset();
	m_clipboard.reset();
	m_image_cache.reset();
	m_settings.reset();

	m_initialized = false;
	m_device      = 0;
	m_exporter    = 0;
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
	return m_framebuf.get();
}

/** Return the notifier */
Notifier * UIManager::notifier()
{
	if (m_notifier == 0) bd_printf("UIManager::notifier no existing");
	return m_notifier.get();
}

/** Return the renderer */
Renderer * UIManager::renderer()
{
	if (m_renderer == 0) bd_printf("UIManager::renderer no existing");
	return m_renderer.get();
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
	return m_fonts.get();
}

/** Return the dirty manager */
Invalidator * UIManager::invalidator()
{
	if (m_invalidator == 0) bd_printf("UIManager::invalidator no existing");
	return m_invalidator.get();
}

/** Return the styles manager */
Styles * UIManager::styles()
{
	if (m_styles == 0) bd_printf("UIManager::styles no existing");
	return m_styles.get();
}

/** Return the colors manager */
Colors * UIManager::colors()
{
	if (m_colors == 0) bd_printf("UIManager::colors no existing");
	return m_colors.get();
}

/** Return the desktop */
Desktop * UIManager::desktop()
{
	if (m_desktop == 0) bd_printf("UIManager::desktop no existing");
	return m_desktop.get();
}

/** Return the screen crc */
ScreenCrc * UIManager::screen_crc()
{
	if (m_screen_crc == 0) bd_printf("UIManager::screen_crc no existing");
	return m_screen_crc.get();
}

/** Return the arc cache handle */
ArcCache * UIManager::arc_cache()
{
	if (m_arc_cache == 0) bd_printf("UIManager::arc_cache no existing");
	return m_arc_cache.get();
}

/** Return the image cache */
ImageCache * UIManager::image_cache()
{
	if (m_image_cache == 0) bd_printf("UIManager::image_cache no existing");
	return m_image_cache.get();
}

/** Return the settings manager */
Settings * UIManager::settings()
{
	if (m_settings == 0) bd_printf("UIManager::settings no existing");
	return m_settings.get();
}

/** Return the clipboard manager */
Clipboard * UIManager::clipboard()
{
	if (m_clipboard == 0) bd_printf("UIManager::clipboard no existing");
	return m_clipboard.get();
}
