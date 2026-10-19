#include "berialdraw_imp.hpp"

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
	};

	// Parse snapshot output type.
	bool parse_snapshot_request(const char* snapshot, SnapshotRequest& request)
	{
		bool valid = File::parse_location(snapshot, request.location);
		if (valid)
		{
			const char* filename = request.location.filename.c_str();
			if (File::match_pattern("*.svg", filename, true))
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
			else if (PngEncoder::supports(request.location))
			{
				request.format = SnapshotFormat::PNG;
			}
			else if (JpegEncoder::supports(request.location))
			{
				request.format = SnapshotFormat::JPEG;
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

// Render a single window offscreen and return its cropped pixels.
std::unique_ptr<Argb8888> Desktop::render_window_offscreen(Window& window)
{
	std::unique_ptr<Argb8888> result;
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
			window.paint(no_occlusion);
		}

		const Area& bounds = window.backclip();
		int64_t left = scaled_floor(bounds.x_q6(), screen_renderer->scale_q6());
		int64_t top = scaled_floor(bounds.y_q6(), screen_renderer->scale_q6());
		int64_t right = scaled_ceil((int64_t)bounds.x_q6() + bounds.width_q6(), screen_renderer->scale_q6());
		int64_t bottom = scaled_ceil((int64_t)bounds.y_q6() + bounds.height_q6(), screen_renderer->scale_q6());
		if (right > left && bottom > top)
		{
			result = Argb8888::crop(*offscreen_framebuf, (int32_t)left, (int32_t)top,
				(uint32_t)(right - left), (uint32_t)(bottom - top));
		}
	}
	return result;
}

// Encode the cropped window image using the selected raster format.
bool Desktop::encode_raster_snapshot(Window& window, Argb8888& image, const File::Location& location, bool jpeg)
{
	bool exported = false;
	if (jpeg)
	{
		uint32_t background = window.stated_color(window.color());
		exported = JpegEncoder::encode(image, location, background);
	}
	else
	{
		exported = PngEncoder::encode(image, location);
	}
	return exported;
}

// Capture and export the front window as a raster image.
void Desktop::export_raster_snapshot(const File::Location& location, bool jpeg)
{
	Window* window = front();
	if (window != nullptr)
	{
		bool exported = false;
		std::unique_ptr<Argb8888> image = render_window_offscreen(*window);
		if (image)
		{
			exported = encode_raster_snapshot(*window, *image, location, jpeg);
		}

		if (exported == false)
		{
			bd_printf("Unable to encode snapshot: %s\n", location.full_path.c_str());
		}
	}
	else
	{
		bd_printf("Cannot encode raster snapshot without a window\n");
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
		export_raster_snapshot(snapshot_request.location, snapshot_request.format == SnapshotFormat::JPEG);
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

