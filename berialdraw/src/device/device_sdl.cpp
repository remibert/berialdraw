#include "berialdraw.hpp"
#include "device/device_sdl.hpp"

using namespace berialdraw; 

DeviceSdl::DeviceSdl(const char * title, Dim width, Dim height, Coord x, Coord y):
	m_width(width),
	m_height(height)
{
	m_title = title;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) == false)
	{
		printf("SDL init failed\n");
		exit(-1);
	}

	// Store the initial position (will be used in open_window)
	// Note: SDL window position will be set after window creation
}

void DeviceSdl::open_window()
{
	bool success = false;

	m_buffer = new uint8_t [m_width * m_height * 3];
	memset(m_buffer, 0xff, m_width * m_height * 3);

	do
	{
		// Create window with SDL3 flags
		m_window = SDL_CreateWindow(m_title.c_str(), m_width, m_height, SDL_WINDOW_HIGH_PIXEL_DENSITY);

		if (m_window == 0)  break;

		m_renderer = SDL_CreateRenderer(m_window, NULL);
		if (m_renderer == 0) break;

		m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, m_width, m_height);
		if (m_texture == 0) break;
		success = true;
	}
	while (0);

	// Check if display is high resolution
	int drawableWidth, drawableHeight;
	SDL_GetWindowSizeInPixels(m_window, &drawableWidth, &drawableHeight);

	// Display have high resolution
	if (drawableWidth > m_width && drawableHeight > m_height)
	{
		m_pixel_ratio = drawableWidth/m_width;
		SDL_SetWindowPosition(m_window,0,0);
		SDL_SetWindowSize(m_window, m_width / m_pixel_ratio, m_height / m_pixel_ratio);
	}
	else
	{
		m_pixel_ratio = 1;
	}

	if (success == false)
	{
		printf("Open window failed\n");
	}
}

void DeviceSdl::close_window()
{
	if (m_texture)  SDL_DestroyTexture(m_texture);
	if (m_renderer) SDL_DestroyRenderer(m_renderer);
	if (m_window)   SDL_DestroyWindow(m_window);
	delete m_buffer;
	m_texture = 0;
	m_renderer = 0;
	m_window = 0;
	m_buffer = 0;
}

DeviceSdl::~DeviceSdl()
{
	close_window();

	// Destroy SDL
	SDL_Quit(); 
	SDL_CleanupTLS();
}

void DeviceSdl::clear()
{
	memset(m_buffer, 0xFF, m_width * m_height * 3);
}

/** Convert sdl key sim into berialdraw key */
wchar_t DeviceSdl::get_key(SDL_Scancode scancode)
{
	wchar_t result = 0;
	switch(scancode)
	{
	case SDL_SCANCODE_RETURN    : result = (wchar_t)ReservedKey::KEY_ENTER;     break;
	case SDL_SCANCODE_ESCAPE    : result = (wchar_t)ReservedKey::KEY_ESCAPE;    break;
	case SDL_SCANCODE_BACKSPACE : result = (wchar_t)ReservedKey::KEY_BACKSPACE; break;
	case SDL_SCANCODE_DELETE    : result = (wchar_t)ReservedKey::KEY_DELETE;    break;
	case SDL_SCANCODE_RIGHT     : result = (wchar_t)ReservedKey::KEY_RIGHT ;    break;
	case SDL_SCANCODE_LEFT      : result = (wchar_t)ReservedKey::KEY_LEFT  ;    break;
	case SDL_SCANCODE_DOWN      : result = (wchar_t)ReservedKey::KEY_DOWN  ;    break;
	case SDL_SCANCODE_UP        : result = (wchar_t)ReservedKey::KEY_UP    ;    break;
	case SDL_SCANCODE_HOME      : result = (wchar_t)ReservedKey::KEY_HOME  ;    break;
	case SDL_SCANCODE_END       : result = (wchar_t)ReservedKey::KEY_END   ;    break;
	case SDL_SCANCODE_PAUSE     : result = (wchar_t)ReservedKey::KEY_PAUSE ;    break;

	default:
		result = SDL_GetKeyFromScancode(scancode, SDL_GetModState(), false);
		break;
	}
	//printf("%04X '%c' %s\n",result,result,SDL_GetKeyName(result));
	if (result > 0xFFFF)
	{
		result = 0;
	}
	return result;
}

KeyEvent::Modifier DeviceSdl::get_modifier()
{
	KeyEvent::Modifier modifier = KeyEvent::MODIFIER_NONE;
	SDL_Keymod mod = SDL_GetModState();
	if (mod & SDL_KMOD_SHIFT)
		modifier = (KeyEvent::Modifier)(KeyEvent::MODIFIER_SHIFT | modifier);
	if (mod & SDL_KMOD_CTRL)
		modifier = (KeyEvent::Modifier)(KeyEvent::MODIFIER_CONTROL | modifier);
	if (mod & SDL_KMOD_ALT)
		modifier = (KeyEvent::Modifier)(KeyEvent::MODIFIER_ALT | modifier);
	return modifier;
}

/** Adapt the ratio of coordinates */
void DeviceSdl::adapt_ratio(Point & position)
{
	position.x_(position.x_()*m_pixel_ratio);
	position.y_(position.y_()*m_pixel_ratio);
}

bool DeviceSdl::dispatch(bool blocking)
{
	// Repaint
	blit();

	// Treat all SDL events
	SDL_WaitEventTimeout(&m_event, 10);

	switch (m_event.type)
	{
	case SDL_EVENT_QUIT:
		UIManager::desktop()->quit();
		UIManager::notifier()->push_event(new KeyEvent((wchar_t)ReservedKey::KEY_EXIT, KeyEvent::KEY_DOWN));
		break;

	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		if (m_event.button.button == SDL_BUTTON_LEFT)
		{
			Point position(m_event.button.x, m_event.button.y);
			adapt_ratio(position);
			position.adapt_scale();
			UIManager::notifier()->push_event(new TouchEvent(position, TouchEvent::TOUCH_DOWN));
			m_mouse_down = true;
			// Capture mouse movements outside the window
			SDL_CaptureMouse(SDL_TRUE);
		}
		break;

	case SDL_EVENT_MOUSE_BUTTON_UP:
		if (m_event.button.button == SDL_BUTTON_LEFT)
		{
			Point position(m_event.button.x, m_event.button.y);
			adapt_ratio(position);
			position.adapt_scale();
			UIManager::notifier()->push_event(new TouchEvent(position, TouchEvent::TOUCH_UP));
			m_mouse_down = false;
			// Release mouse capture
			SDL_CaptureMouse(SDL_FALSE);
		}
		break;

	case SDL_EVENT_MOUSE_MOTION:
		{
			Point position(m_event.motion.x, m_event.motion.y);
			adapt_ratio(position);
			position.adapt_scale();
			UIManager::notifier()->push_event(new TouchEvent(position, TouchEvent::TOUCH_MOVE));
		}
		break;

	case SDL_EVENT_KEY_DOWN:
		{
			wchar_t key = get_key(m_event.key.scancode);
			if (key)
			{
				UIManager::notifier()->push_event(new KeyEvent(key, KeyEvent::KEY_DOWN, get_modifier()));
			}
		}
		break;

	case SDL_EVENT_KEY_UP:
		{
			wchar_t key = get_key(m_event.key.scancode);
			if (key)
			{
				UIManager::notifier()->push_event(new KeyEvent(key, KeyEvent::KEY_UP, get_modifier()));
			}
		}
		break;
	}
	refresh();
	return UIManager::desktop()->is_loop();
}

void DeviceSdl::mainloop()
{
	while (dispatch())
	{
	}
}

/** Get the size of screen device in pixels 
@return the size */
Size DeviceSdl::size() const
{
	Size result(m_width, m_height);
	return result;
}

/** Set the size of screen device in pixels
@param s size of screen */
void DeviceSdl::size(const Size & s)
{
	size(s.width(),s.height());
}

/** Resize the window
@param width The new width of the window
@param height The new height of the window */
void DeviceSdl::size(Dim width, Dim height)
{
	m_width = width;
	m_height = height;
	close_window();
	open_window();
}

/** Get the position of the window
@return the position as a Point */
Point DeviceSdl::position() const
{
	int x, y;
	SDL_GetWindowPosition(m_window, &x, &y);
	
	Coord pos_x = static_cast<Coord>(x);
	Coord pos_y = static_cast<Coord>(y);
	
	Point result(pos_x, pos_y, true);
	return result;
}

/** Set the position of the window
@param p position of the window */
void DeviceSdl::position(const Point & p)
{
	position(p.x(), p.y());
}

/** Move the window
@param x The x position of the window
@param y The y position of the window */
void DeviceSdl::position(Coord x, Coord y)
{
	SDL_SetWindowPosition(m_window, (int)x, (int)y);
}

void DeviceSdl::copy(const uint8_t* buffer, Dim width, Dim height)
{
	m_dirty = true;
	if (m_buffer)
	{
		for (Dim y = 0; y < height; y++)
		{
			for (Dim x = 0; x < width; x++)
			{
				int32_t pos = ((y * width) + x) * 3;
				m_buffer[pos] = buffer[pos];
				m_buffer[pos + 1] = buffer[pos + 1];
				m_buffer[pos + 2] = buffer[pos + 2];
			}
		}
	}
}

void DeviceSdl::blit()
{
	if (m_dirty && m_buffer)
	{
		// Mise a jour de la texture avec le buffer de donnée
		SDL_UpdateTexture(m_texture, nullptr, m_buffer, m_width * 3);

		// Copie de la texture sur l'ecran
		SDL_RenderTexture(m_renderer, m_texture, nullptr, nullptr);

		// show the window 
		SDL_RenderPresent(m_renderer); 
		m_dirty = false;
	}
}

void DeviceSdl::refresh()
{
	Framebuf * framebuf = UIManager::framebuf();
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

	// Function to get the elapsed time in milliseconds between two time points: `begin` and `end`
	uint32_t get_elapsed_ms(uint64_t begin, uint64_t end)
	{
		uint32_t result;
		int64_t tmp = end - begin; // Calculate the difference between end and begin (can be negative).
		short neg = 0; // Flag to track if the difference is negative.

		// If the time difference is larger than the maximum positive value for a 32-bit signed integer (scaled),
		// return the maximum possible 32-bit positive integer (0x7FFFFFFF, or INT_MAX).
		if (tmp > 0x7FFFFFFFLL * 1000000LL)
			return(0x7FFFFFFF);

		// If the time difference is smaller than the minimum negative value for a 32-bit signed integer (scaled),
		// return the minimum possible 32-bit negative integer (0x80000000, or INT_MIN).
		if (tmp < -0x80000000LL * 1000000LL)
			return(0x80000000);

		// If the difference is negative, flag it and take its absolute value.
		if (tmp < 0)
		{
			neg = 1; // Mark as negative.
			tmp = -tmp; // Take absolute value of `tmp`.
		}

		// Reduce the scale by dividing `tmp` by 64 (equivalent to shifting right by 6 bits).
		tmp >>= 6; // Faster than dividing by 64.

		// Convert `tmp` to a 32-bit result. Shift right by 16 bits, effectively scaling it down.
		result = (unsigned long)(tmp >> 16);

		// Divide the result by 15625 to convert to milliseconds, then shift left 16 bits to store it.
		result = (result / 15625) << 16;

		// Remove the portion already accounted for from `tmp` and adjust the result accordingly.
		tmp = tmp - (result * 15625LL);

		// Add the remainder to the result after further dividing by 15625.
		result += ((unsigned long)tmp) / 15625;

		// If the original difference was negative, negate the result to represent a negative time difference.
		if (neg)
			return(-(long)result); // Return the negative result.

		// Return the result as a positive value.
		return((long)result);
	}
}
