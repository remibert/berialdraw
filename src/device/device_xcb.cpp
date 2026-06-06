#include "berialdraw_imp.hpp"
#include "device/device_xcb.hpp"
#include "device/clipboard_xcb.hpp"

#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xcb_icccm.h>
#include <cstring>
#include <cstdlib>
#include <poll.h>
#include <time.h>

using namespace berialdraw;

// X11 keysym definitions (from X11/keysymdef.h)
#define XK_BackSpace    0xff08
#define XK_Tab          0xff09
#define XK_Return       0xff0d
#define XK_Pause        0xff13
#define XK_Escape       0xff1b
#define XK_Delete       0xffff
#define XK_Home         0xff50
#define XK_Left         0xff51
#define XK_Up           0xff52
#define XK_Right        0xff53
#define XK_Down         0xff54
#define XK_End          0xff57
#define XK_space        0x0020

// Modifier masks
#define XCB_MOD_SHIFT   0x0001
#define XCB_MOD_CONTROL 0x0004
#define XCB_MOD_ALT     0x0008

DeviceXcb::DeviceXcb(const char * title, Dim width, Dim height, Coord x, Coord y)
	: m_connection(nullptr)
	, m_window(0)
	, m_gc(nullptr)
	, m_key_symbols(nullptr)
	, m_wm_delete_window(0)
	, m_dirty(false)
	, m_width(width)
	, m_height(height)
	, m_buffer(nullptr)
	, m_mouse_down(false)
	, m_running(true)
	, m_title(title)
{
	// Connect to X server
	m_connection = xcb_connect(nullptr, nullptr);
	if (xcb_connection_has_error((xcb_connection_t*)m_connection))
	{
		printf("XCB connection failed\n");
		exit(-1);
	}

	// Initialize key symbols
	m_key_symbols = xcb_key_symbols_alloc((xcb_connection_t*)m_connection);

	if (m_width != 0 && m_height != 0)
	{
		open_window();
	}
}

DeviceXcb::~DeviceXcb()
{
	close_window();

	if (m_key_symbols)
	{
		xcb_key_symbols_free((xcb_key_symbols_t*)m_key_symbols);
	}

	if (m_connection)
	{
		xcb_disconnect((xcb_connection_t*)m_connection);
	}
}

void DeviceXcb::open_window()
{
	xcb_connection_t* conn = (xcb_connection_t*)m_connection;
	
	// Get the first screen
	const xcb_setup_t* setup = xcb_get_setup(conn);
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
	xcb_screen_t* screen = iter.data;

	// Allocate buffer
	m_buffer = new uint8_t[m_width * m_height * 4]; // BGRA format for X11
	memset(m_buffer, 0xFF, m_width * m_height * 4);

	// Create window
	m_window = xcb_generate_id(conn);
	uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	uint32_t values[2];
	values[0] = screen->white_pixel;
	values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
	            XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
	            XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_STRUCTURE_NOTIFY;

	xcb_create_window(conn,
	                  XCB_COPY_FROM_PARENT,
	                  m_window,
	                  screen->root,
	                  0, 0,
	                  m_width, m_height,
	                  0,
	                  XCB_WINDOW_CLASS_INPUT_OUTPUT,
	                  screen->root_visual,
	                  mask, values);

	// Set window title
	xcb_change_property(conn,
	                    XCB_PROP_MODE_REPLACE,
	                    m_window,
	                    XCB_ATOM_WM_NAME,
	                    XCB_ATOM_STRING,
	                    8,
	                    m_title.size(),
	                    (const char*)m_title);

	// Create graphics context
	xcb_gcontext_t gc = xcb_generate_id(conn);
	uint32_t gc_mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	uint32_t gc_values[2] = { screen->black_pixel, 0 };
	xcb_create_gc(conn, gc, m_window, gc_mask, gc_values);
	m_gc = (void*)(uintptr_t)gc;

	// Setup WM_DELETE_WINDOW protocol
	xcb_intern_atom_cookie_t protocols_cookie = xcb_intern_atom(conn, 1, 12, "WM_PROTOCOLS");
	xcb_intern_atom_cookie_t delete_cookie = xcb_intern_atom(conn, 0, 16, "WM_DELETE_WINDOW");
	
	xcb_intern_atom_reply_t* protocols_reply = xcb_intern_atom_reply(conn, protocols_cookie, nullptr);
	xcb_intern_atom_reply_t* delete_reply = xcb_intern_atom_reply(conn, delete_cookie, nullptr);
	
	if (protocols_reply && delete_reply)
	{
		m_wm_delete_window = delete_reply->atom;
		xcb_change_property(conn,
		                    XCB_PROP_MODE_REPLACE,
		                    m_window,
		                    protocols_reply->atom,
		                    XCB_ATOM_ATOM,
		                    32,
		                    1,
		                    &m_wm_delete_window);
	}
	
	free(protocols_reply);
	free(delete_reply);

	// Map (show) the window
	xcb_map_window(conn, m_window);
	xcb_flush(conn);
}

void DeviceXcb::close_window()
{
	xcb_connection_t* conn = (xcb_connection_t*)m_connection;

	if (m_gc)
	{
		xcb_free_gc(conn, (xcb_gcontext_t)(uintptr_t)m_gc);
		m_gc = nullptr;
	}

	if (m_window)
	{
		xcb_destroy_window(conn, m_window);
		m_window = 0;
	}

	delete[] m_buffer;
	m_buffer = nullptr;
}

void DeviceXcb::quit()
{
	m_running = false;
	UIManager::desktop()->quit();
}

void DeviceXcb::clear()
{
	if (m_buffer)
	{
		memset(m_buffer, 0xFF, m_width * m_height * 4);
	}
}

wchar_t DeviceXcb::get_key(uint8_t keycode, uint16_t state)
{
	xcb_keysym_t keysym = xcb_key_symbols_get_keysym((xcb_key_symbols_t*)m_key_symbols, keycode, 0);

	switch (keysym)
	{
	case XK_Return:    return (wchar_t)ReservedKey::KEY_ENTER;
	case XK_Escape:    return (wchar_t)ReservedKey::KEY_ESCAPE;
	case XK_BackSpace: return (wchar_t)ReservedKey::KEY_BACKSPACE;
	case XK_Delete:    return (wchar_t)ReservedKey::KEY_DELETE;
	case XK_Right:     return (wchar_t)ReservedKey::KEY_RIGHT;
	case XK_Left:      return (wchar_t)ReservedKey::KEY_LEFT;
	case XK_Down:      return (wchar_t)ReservedKey::KEY_DOWN;
	case XK_Up:        return (wchar_t)ReservedKey::KEY_UP;
	case XK_Home:      return (wchar_t)ReservedKey::KEY_HOME;
	case XK_End:       return (wchar_t)ReservedKey::KEY_END;
	case XK_Pause:     return (wchar_t)ReservedKey::KEY_PAUSE;
	case XK_Tab:       return (wchar_t)ReservedKey::KEY_TAB;
	default:
		// For regular characters, get the keysym with shift state
		if (state & XCB_MOD_SHIFT)
		{
			keysym = xcb_key_symbols_get_keysym((xcb_key_symbols_t*)m_key_symbols, keycode, 1);
		}
		// Return the keysym as character if it's in printable range
		if (keysym >= 0x20 && keysym <= 0xFFFF)
		{
			return (wchar_t)keysym;
		}
		break;
	}
	return 0;
}

KeyEvent::Modifier DeviceXcb::get_modifier(uint16_t state)
{
	KeyEvent::Modifier modifier = KeyEvent::MODIFIER_NONE;
	
	if (state & XCB_MOD_SHIFT)
		modifier = (KeyEvent::Modifier)(KeyEvent::MODIFIER_SHIFT | modifier);
	if (state & XCB_MOD_CONTROL)
		modifier = (KeyEvent::Modifier)(KeyEvent::MODIFIER_CONTROL | modifier);
	if (state & XCB_MOD_ALT)
		modifier = (KeyEvent::Modifier)(KeyEvent::MODIFIER_ALT | modifier);
	
	return modifier;
}

bool DeviceXcb::dispatch(bool blocking)
{
	xcb_connection_t* conn = (xcb_connection_t*)m_connection;

	// Repaint
	blit();

	// Register XCB clipboard provider with UIManager (only once)
	static bool clipboard_initialized = false;
	if (!clipboard_initialized && UIManager::is_initialized())
	{
		UIManager::clipboard()->set_provider(std::make_unique<ClipboardProviderXcb>(m_connection, m_window));
		clipboard_initialized = true;
	}

	// Sync clipboard from system
	if (UIManager::clipboard())
	{
		UIManager::clipboard()->sync_from_system();
	}

	// Get event (blocking or non-blocking)
	xcb_generic_event_t* event;
	if (blocking)
	{
		// Wait for event with timeout using poll
		struct pollfd pfd;
		pfd.fd = xcb_get_file_descriptor(conn);
		pfd.events = POLLIN;
		
		if (poll(&pfd, 1, 10) > 0) // 10ms timeout
		{
			event = xcb_poll_for_event(conn);
		}
		else
		{
			event = nullptr;
		}
	}
	else
	{
		event = xcb_poll_for_event(conn);
	}

	if (event)
	{
		uint8_t response_type = event->response_type & ~0x80;

		switch (response_type)
		{
		case XCB_EXPOSE:
			m_dirty = true;
			blit();
			break;

		case XCB_CLIENT_MESSAGE:
			{
				xcb_client_message_event_t* cm = (xcb_client_message_event_t*)event;
				if (cm->data.data32[0] == m_wm_delete_window)
				{
					m_running = false;
					UIManager::desktop()->quit();
					UIManager::notifier()->push_event(new KeyEvent((wchar_t)ReservedKey::KEY_EXIT, KeyEvent::KEY_DOWN));
				}
			}
			break;

		case XCB_BUTTON_PRESS:
			{
				xcb_button_press_event_t* bp = (xcb_button_press_event_t*)event;
				if (bp->detail == 1) // Left button
				{
					Point position(bp->event_x, bp->event_y);
					position.adapt_scale();
					UIManager::notifier()->push_event(new TouchEvent(position, TouchEvent::TOUCH_DOWN));
					m_mouse_down = true;
				}
			}
			break;

		case XCB_BUTTON_RELEASE:
			{
				xcb_button_release_event_t* br = (xcb_button_release_event_t*)event;
				if (br->detail == 1) // Left button
				{
					Point position(br->event_x, br->event_y);
					position.adapt_scale();
					UIManager::notifier()->push_event(new TouchEvent(position, TouchEvent::TOUCH_UP));
					m_mouse_down = false;
				}
			}
			break;

		case XCB_MOTION_NOTIFY:
			{
				xcb_motion_notify_event_t* motion = (xcb_motion_notify_event_t*)event;
				Point position(motion->event_x, motion->event_y);
				position.adapt_scale();
				UIManager::notifier()->push_event(new TouchEvent(position, TouchEvent::TOUCH_MOVE));
			}
			break;

		case XCB_KEY_PRESS:
			{
				xcb_key_press_event_t* kp = (xcb_key_press_event_t*)event;
				wchar_t key = get_key(kp->detail, kp->state);
				if (key)
				{
					UIManager::notifier()->push_event(new KeyEvent(key, KeyEvent::KEY_DOWN, get_modifier(kp->state)));
				}
			}
			break;

		case XCB_KEY_RELEASE:
			{
				xcb_key_release_event_t* kr = (xcb_key_release_event_t*)event;
				wchar_t key = get_key(kr->detail, kr->state);
				if (key)
				{
					UIManager::notifier()->push_event(new KeyEvent(key, KeyEvent::KEY_UP, get_modifier(kr->state)));
				}
			}
			break;
		}

		free(event);
	}

	refresh();
	return m_running && UIManager::desktop()->is_loop();
}

void DeviceXcb::mainloop()
{
	while (dispatch())
	{
	}
}

Size DeviceXcb::size() const
{
	Size result(m_width, m_height);
	return result;
}

void DeviceXcb::size(const Size & s)
{
	size(s.width(), s.height());
}

void DeviceXcb::size(Dim width, Dim height)
{
	m_width = width;
	m_height = height;
	close_window();
	open_window();
}

Point DeviceXcb::position() const
{
	xcb_connection_t* conn = (xcb_connection_t*)m_connection;
	
	xcb_get_geometry_cookie_t geom_cookie = xcb_get_geometry(conn, m_window);
	xcb_get_geometry_reply_t* geom = xcb_get_geometry_reply(conn, geom_cookie, nullptr);
	
	Point result(0, 0, true);
	if (geom)
	{
		// Translate coordinates to root window
		xcb_translate_coordinates_cookie_t trans_cookie = 
			xcb_translate_coordinates(conn, m_window, geom->root, 0, 0);
		xcb_translate_coordinates_reply_t* trans = 
			xcb_translate_coordinates_reply(conn, trans_cookie, nullptr);
		
		if (trans)
		{
			result = Point(trans->dst_x, trans->dst_y, true);
			free(trans);
		}
		free(geom);
	}
	
	return result;
}

void DeviceXcb::position(const Point & p)
{
	position(p.x(), p.y());
}

void DeviceXcb::position(Coord x, Coord y)
{
	xcb_connection_t* conn = (xcb_connection_t*)m_connection;
	
	uint32_t values[2] = { (uint32_t)x, (uint32_t)y };
	xcb_configure_window(conn, m_window, 
	                     XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, 
	                     values);
	xcb_flush(conn);
}

void DeviceXcb::copy(const uint8_t* buffer, Dim width, Dim height)
{
	m_dirty = true;
	if (m_buffer && buffer)
	{
		// Convert RGB to BGRA (X11 format)
		for (Dim y = 0; y < height; y++)
		{
			for (Dim x = 0; x < width; x++)
			{
				uint32_t src_pos = ((y * width) + x) * 3;
				uint32_t dst_pos = ((y * width) + x) * 4;
				m_buffer[dst_pos + 0] = buffer[src_pos + 2]; // B
				m_buffer[dst_pos + 1] = buffer[src_pos + 1]; // G
				m_buffer[dst_pos + 2] = buffer[src_pos + 0]; // R
				m_buffer[dst_pos + 3] = 0xFF;                // A
			}
		}
	}
}

void DeviceXcb::blit()
{
	if (m_dirty && m_buffer && m_connection && m_window)
	{
		xcb_connection_t* conn = (xcb_connection_t*)m_connection;
		
		// Get the screen to get the visual
		const xcb_setup_t* setup = xcb_get_setup(conn);
		xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
		xcb_screen_t* screen = iter.data;
		
		// Create an image and put it to the window
		xcb_image_t* image = xcb_image_create_native(
			conn,
			m_width,
			m_height,
			XCB_IMAGE_FORMAT_Z_PIXMAP,
			screen->root_depth,
			m_buffer,
			m_width * m_height * 4,
			m_buffer);
		
		if (image)
		{
			xcb_image_put(conn, m_window, (xcb_gcontext_t)(uintptr_t)m_gc, image, 0, 0, 0);
			xcb_image_destroy(image);
		}
		
		xcb_flush(conn);
		m_dirty = false;
	}
}

void DeviceXcb::refresh()
{
	Framebuf* framebuf = UIManager::framebuf();
	if (framebuf)
	{
		if (framebuf->dirty())
		{
			copy(framebuf->buffer(), framebuf->width(), framebuf->height());
			framebuf->dirty(false);
		}
	}
}

namespace berialdraw
{
	uint64_t get_clock()
	{
		struct timespec ts;

		if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
		{
			return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
		}
		else
		{
			return 0;
		}
	}

	uint32_t get_elapsed_ms(uint64_t begin, uint64_t end)
	{
		uint32_t result;
		int64_t tmp = end - begin;
		short neg = 0;

		if (tmp > 0x7FFFFFFFLL * 1000000LL)
			return(0x7FFFFFFF);

		if (tmp < -0x80000000LL * 1000000LL)
			return(0x80000000);

		if (tmp < 0)
		{
			neg = 1;
			tmp = -tmp;
		}

		tmp >>= 6;
		result = (unsigned long)(tmp >> 16);
		result = (result / 15625) << 16;
		tmp = tmp - (result * 15625LL);
		result += ((unsigned long)tmp) / 15625;

		if (neg)
			return(-(long)result);

		return((long)result);
	}
}
