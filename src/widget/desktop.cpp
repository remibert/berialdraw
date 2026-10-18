#include "berialdraw_imp.hpp"
#include <cerrno>
#include <cstdlib>

using namespace berialdraw;

namespace
{
	enum class SnapshotFormat
	{
		UNSUPPORTED,
		SVG,
		PNG,
		JPEG
	};

	struct SnapshotRequest
	{
		SnapshotFormat format = SnapshotFormat::UNSUPPORTED;
		File::Location location;
		String svg_filename;
		int png_compression = -1;
		int jpeg_quality = 90;
		uint32_t jpeg_background = 0xFFFFFFFF;
		bool has_jpeg_background = false;
	};

	// Parse a decimal or 0x-prefixed integer option with inclusive bounds.
	bool parse_snapshot_integer(const String& text, long minimum, long maximum, long& value)
	{
		bool valid = false;
		char* end = nullptr;
		long parsed = strtol(text.c_str(), &end, 0);
		if (end != text.c_str() && *end == '\0' && parsed >= minimum && parsed <= maximum)
		{
			value = parsed;
			valid = true;
		}
		return valid;
	}

	bool parse_snapshot_color(const String& text, uint32_t& color)
	{
		bool valid = false;
		char* end = nullptr;
		errno = 0;
		unsigned long long parsed = strtoull(text.c_str(), &end, 0);
		if (end != text.c_str() && *end == '\0' && errno != ERANGE && parsed <= 0xFFFFFFFFULL)
		{
			color = (uint32_t)parsed;
			if (color <= 0x00FFFFFF)
			{
				color |= 0xFF000000;
			}
			valid = true;
		}
		return valid;
	}

	// Read a named query parameter, using the last occurrence when repeated.
	bool snapshot_parameter(const File::Location& location, const char* name, String& value)
	{
		bool found = false;
		for (uint32_t index = 0; index < location.query_parameters.size(); index++)
		{
			if (location.query_parameters[index].name == name)
			{
				value = location.query_parameters[index].value;
				found = true;
			}
		}
		return found;
	}

	bool extension_equals(const char* extension, const char* expected)
	{
		bool equal = extension != nullptr;
		if (equal)
		{
			size_t index = 0;
			while (extension[index] != '\0' && expected[index] != '\0' &&
				tolower((unsigned char)extension[index]) == tolower((unsigned char)expected[index]))
			{
				index++;
			}
			equal = extension[index] == '\0' && expected[index] == '\0';
		}
		return equal;
	}

	// Parse snapshot output type and optional PNG/JPEG encoder settings.
	bool parse_snapshot_request(const char* snapshot, SnapshotRequest& request)
	{
		bool valid = File::parse_location(snapshot, request.location);
		if (valid)
		{
			const char* filename = request.location.filename.c_str();
			const char* extension = strrchr(filename, '.');
			if (extension != nullptr)
			{
				if (extension_equals(extension, ".svg"))
				{
					request.format = SnapshotFormat::SVG;
					const char* path_end = snapshot + strlen(snapshot);
					const char* query = strchr(snapshot, '?');
					const char* fragment = strchr(snapshot, '#');
					if (query != nullptr && query < path_end)
					{
						path_end = query;
					}
					if (fragment != nullptr && fragment < path_end)
					{
						path_end = fragment;
					}
					request.svg_filename = String(snapshot, (size_t)(path_end - snapshot));
				}
				else if (extension_equals(extension, ".png"))
				{
					request.format = SnapshotFormat::PNG;
				}
				else if (extension_equals(extension, ".jpg") || extension_equals(extension, ".jpeg"))
				{
					request.format = SnapshotFormat::JPEG;
				}
			}

			String option;
			long value = 0;
			if (valid && request.format == SnapshotFormat::PNG && snapshot_parameter(request.location, "compression", option))
			{
				valid = parse_snapshot_integer(option, 0, 9, value);
				if (valid)
				{
					request.png_compression = (int)value;
				}
			}
			if (valid && request.format == SnapshotFormat::JPEG && snapshot_parameter(request.location, "quality", option))
			{
				valid = parse_snapshot_integer(option, 1, 100, value);
				if (valid)
				{
					request.jpeg_quality = (int)value;
				}
			}
			if (valid && request.format == SnapshotFormat::JPEG && snapshot_parameter(request.location, "background", option))
			{
				valid = parse_snapshot_color(option, request.jpeg_background);
				if (valid)
				{
					request.has_jpeg_background = true;
				}
			}
		}
		return valid;
	}

	// Convert logical Q6 bounds to physical pixel coordinates, rounding outwards.
	int64_t scaled_floor(int64_t coordinate_q6, Dim scale_q6)
	{
		int64_t value = coordinate_q6 * (int64_t)scale_q6;
		return value >= 0 ? value / 4096 : -((-value + 4095) / 4096);
	}

	int64_t scaled_ceil(int64_t coordinate_q6, Dim scale_q6)
	{
		int64_t value = coordinate_q6 * (int64_t)scale_q6;
		return value >= 0 ? (value + 4095) / 4096 : -((-value) / 4096);
	}

}

Desktop::Desktop()
{
	m_quit = 0;
	m_already_in_dispatch = 0;
}

Desktop::~Desktop()
{
	remove_all();
}

void Desktop::add(Window * window)
{
	m_windows.insert(0,window);
}

/** Places the specified window at the top of its parents canvas stack, above all of its sibling window. */
void Desktop::back(Window * window)
{
	for(uint32_t i = 0; i < m_windows.size();)
	{
		if (m_windows[i] == 0)
		{
			m_windows.remove(i);
		}
		else if(m_windows[i] == window)
		{
			m_windows.remove(i);
			m_windows.push_back(window);
			break;
		}
		else
		{
			i++;
		}
	}
}

/** Places the specified window at the bottom of its parents canvas stack, below all of its sibling window.*/
void Desktop::front(Window * window)
{
	for(uint32_t i = 0; i < m_windows.size(); )
	{
		if (m_windows[i] == 0)
		{
			m_windows.remove(i);
		}
		else if(m_windows[i] == window)
		{
			m_windows.remove(i);
			m_windows.insert(0,window);
			break;
		}
		else
		{
			i++;
		}
	}
}

void Desktop::remove(Window * window)
{
	uint32_t size = m_windows.size();
	for(uint32_t i = 0; i < size; i++)
	{
		if(m_windows[i] == window)
		{
			if(m_windows[i]->is_allocated())
			{
				m_windows.remove(i);
				delete window;
			}
			else
			{
			m_windows.remove(i);
			}
			break;
		}
	}
	size = m_windows.size();
	for(uint32_t i = 0; i < size; i++)
	{
		UIManager::invalidator()->dirty(m_windows[i], Invalidator::REDRAW);
	}
}

void Desktop::remove_all()
{
	for(uint32_t i = 0; i < m_windows.size(); i++)
	{
		if (m_windows[i])
		{
			if(m_windows[i]->is_allocated())
			{
				delete m_windows[i];
				m_windows[i] = 0;
			}
			else
			{
				m_windows[i]->clear();
			}
		}
	}
	m_windows.clear();
}

void Desktop::mainloop()
{
	m_quit = 0;
	while(dispatch() && m_quit == 0)
	{
	}
}

void Desktop::quit()
{
	m_quit = 1;
}

/** Indicates if the mainloop is running */
bool Desktop::is_loop()
{
	return m_quit == 0;
}

bool Desktop::dispatch(const char * snapshot)
{
	bool result = false;

	// Guard: ensure UIManager is initialized
	if (!UIManager::is_initialized())
	{
		bd_printf("Desktop::dispatch - UIManager not initialized");
		return result;
	}

	// Check is test script started
	if (UIManager::notifier()->is_script_started())
	{
		// Goto next step script
		snapshot = UIManager::notifier()->next_step_script();
	}

	// Get event from screen device
	result = UIManager::device()->dispatch();

	// If the screen must be refressed
	if (UIManager::invalidator()->is_dirty() || snapshot != 0)
	{
		// Refresh the display
		refresh(snapshot);
	}

	// Notify all user event to application
	UIManager::notifier()->dispatch();

#if defined(_DEBUG) && defined(USE_COCOA_DEVICE)
	// Get event from screen device
	result = UIManager::device()->dispatch();
#endif

	return result;
}


void Desktop::refresh(const char * snapshot)
{
	SnapshotRequest snapshot_request;
	bool raster_snapshot = false;
	if (snapshot != nullptr && snapshot[0] != '\0')
	{
		if (parse_snapshot_request(snapshot, snapshot_request))
		{
			if (snapshot_request.format == SnapshotFormat::SVG)
			{
				UIManager::exporter(0);
				UIManager::exporter(new SvgOut(snapshot_request.svg_filename.c_str()));
			}
			else if (snapshot_request.format == SnapshotFormat::PNG || snapshot_request.format == SnapshotFormat::JPEG)
			{
				UIManager::exporter(0);
				raster_snapshot = true;
			}
			else
			{
				UIManager::exporter(0);
				bd_printf("Unsupported snapshot extension: %s\n", snapshot_request.location.filename.c_str());
			}
		}
		else
		{
			UIManager::exporter(0);
			bd_printf("Invalid snapshot path or encoder option: %s\n", snapshot);
		}
	}

	UIManager::renderer()->clear();

	if (m_windows.size())
	{
		// Place all widgets
		for(uint32_t i = 0; i < m_windows.size(); i++)
		{
			Window * window = m_windows[i];
			if (window)
			{
				if(UIManager::invalidator()->is_dirty(window))
				{
					// Apply cascade styles before placing
					window->apply_cascade_styles();

					window->place();

					// Flow replacement detected
					if (window->flow_replacement())
					{
						window->place();
					}
				}
			}
		}

		// Paint all widgets
		for(int32_t i = m_windows.size()-1; i >= 0; i--)
		{
			Window * window = m_windows[i];
			if (window)
			{
				Region region;

				// Create region to clip paint
				for(int32_t j = 0; j < i; j++)
				{
					Window * fore_window = m_windows[j];
					if (fore_window)
					{
						region.unite(fore_window->backclip());
					}
				}

				// Paint window
				window->paint(region);

				// Remove all dirty and invalidate state
				window->clean_all();

			}
		}
	}

	if(UIManager::renderer())
	{
		UIManager::device()->refresh();
		UIManager::invalidator()->clear(0);
#ifdef WIN32
		UIManager::device()->dispatch();
#endif
	}
	UIManager::exporter(0);

	// Raster snapshots capture the front window into a transparent offscreen target.
	if (raster_snapshot)
	{
		Window* window = front();
		if (window != nullptr)
		{
			bool exported = false;
			Framebuf* screen_framebuf = UIManager::framebuf();
			Renderer* screen_renderer = UIManager::renderer();
			std::unique_ptr<Framebuf> offscreen_framebuf(
				Framebuf::create(screen_framebuf->width(), screen_framebuf->height(), Framebuf::ARGB8888));
			if (offscreen_framebuf)
			{
				Size render_size = screen_renderer->size();
				Renderer offscreen_renderer(render_size.width(), render_size.height(), screen_renderer->scale_q6());
				Region no_occlusion;
				{
					UIManager::RenderContextScope context(offscreen_renderer, *offscreen_framebuf);
					offscreen_framebuf->clear(0x00000000);
					window->paint(no_occlusion);
				}

				const Area& bounds = window->backclip();
				int64_t left = scaled_floor(bounds.x_q6(), screen_renderer->scale_q6());
				int64_t top = scaled_floor(bounds.y_q6(), screen_renderer->scale_q6());
				int64_t right = scaled_ceil((int64_t)bounds.x_q6() + bounds.width_q6(), screen_renderer->scale_q6());
				int64_t bottom = scaled_ceil((int64_t)bounds.y_q6() + bounds.height_q6(), screen_renderer->scale_q6());
				if (right > left && bottom > top)
				{
					std::unique_ptr<Argb8888> cropped = Argb8888::crop(
						*offscreen_framebuf, (int32_t)left, (int32_t)top,
						(uint32_t)(right - left), (uint32_t)(bottom - top));
					if (cropped)
					{
						if (snapshot_request.format == SnapshotFormat::PNG)
						{
							exported = PngEncoder::encode(*cropped, snapshot_request.location.full_path.c_str(), snapshot_request.png_compression);
						}
						else if (snapshot_request.format == SnapshotFormat::JPEG)
						{
							uint32_t background = snapshot_request.has_jpeg_background ? snapshot_request.jpeg_background : window->stated_color(window->color());
							background |= 0xFF000000;
							exported = JpegEncoder::encode(*cropped, snapshot_request.location.full_path.c_str(), background, snapshot_request.jpeg_quality);
						}
					}
				}
			}

			if (exported == false)
			{
				bd_printf("Unable to encode snapshot: %s\n", snapshot_request.location.full_path.c_str());
			}
		}
		else
		{
			bd_printf("Cannot encode raster snapshot without a window\n");
		}
	}
}

/** Get the front window */
Window * Desktop::front()
{
	Window * result = 0;
	if (m_windows.size() > 0)
	{
		result = m_windows[0];
	}
	return result;
}

/** Get the widget hovered */
Widget * Desktop::hovered(const Point & position)
{
	Widget * result = 0;
	Region region;

	// Search window with hovered
	for(uint32_t window_id = 0; window_id < m_windows.size(); window_id++)
	{
		Window * window = m_windows[window_id];
		if (window)
		{
			// Get the widget hovered
			result = m_windows[window_id]->hovered(region, position);

			if (result)
			{
				break;
			}
			// Add the area of this window to region
			region.unite(window->area());
		}
	}
	return result;
}

