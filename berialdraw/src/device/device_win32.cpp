#include "berialdraw_imp.hpp"
#include "device/device_win32.hpp"
#include "device/clipboard_win32.hpp"
#include "shellscalingapi.h"
#include <windowsx.h>

#ifndef GWL_USERDATA
	#define GWL_USERDATA -21
#endif
using namespace berialdraw;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param);

DeviceWin32::DeviceWin32(const char * title, Dim width, Dim height, Coord x, Coord y):
	m_width(width),
	m_height(height)
{
	m_buffer = 0;
	static wchar_t sz_app_name[256];
	WNDCLASS wndclass;
	m_instance = GetModuleHandle(NULL);
	m_cmdShow = SW_SHOWDEFAULT;

	MultiByteToWideChar(CP_UTF8, 0, title, -1, sz_app_name, sizeof(sz_app_name));

	//memset(m_buffer, 0xff, (int)m_width * (int)m_height * 3);

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = (HINSTANCE) GetModuleHandle(NULL);;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = sz_app_name;

	// Register the window
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, L"Registering the class failed", L"Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

    UINT dpi = 144;

    // Obtenir la taille de l'icône en fonction du DPI
    int iconWidth = GetSystemMetricsForDpi(SM_CXCURSOR, dpi);
    int iconHeight = GetSystemMetricsForDpi(SM_CYICON, dpi);

	SetProcessDPIAware();

	// CreateWindow
	HWND hwnd = CreateWindow(
		sz_app_name,
		sz_app_name,         // Window title
		WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX, // Window style
		(int)x,              // x position - now using parameter
		(int)y,              // y position - now using parameter
		0,                    // Width
		0,                    // Height
		NULL,                 // Parent window
		NULL,                 // Menu
		(HINSTANCE)m_instance,// Instance handle
		NULL);                // Additional parameter

	SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG_PTR)this);

	// Get screen dimensions
	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect);

	// Get window dimensions
	RECT window_rect;
	GetWindowRect(hwnd, &window_rect);

	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	// Get the working area of the screen (excluding taskbar)
	RECT work_area_rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &work_area_rect, 0);

	// Calculate the position to place the window at the bottom above the taskbar
	int x_pos = (int)x;
	int y_pos = (int)y;

	// Move the window to the specified position
	SetWindowPos(hwnd, HWND_TOP, x_pos, y_pos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	if (!hwnd)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_OK);
		exit(0);
	}

	m_hwnd = hwnd;

	ShowWindow(hwnd, SW_SHOWDEFAULT);

	if (m_width != 0 && m_height != 0)
	{
		size(m_width, m_height);
	}
#ifdef _DEBUG
	show_console();
#endif
}

DeviceWin32::~DeviceWin32()
{
	delete m_buffer;
}

void DeviceWin32::quit()
{
	UIManager::desktop()->quit();
}

void DeviceWin32::clear()
{
	memset(m_buffer, 0xFF, (int)m_width * (int)m_height * 3);
}

bool DeviceWin32::dispatch(bool blocking)
{
	MSG msg;
	BOOL message_received = FALSE;
	uint32_t count = 0;

	// Register Win32 clipboard provider with UIManager (only once)
	static bool clipboard_initialized = false;
	if (!clipboard_initialized && UIManager::is_initialized())
	{
		UIManager::clipboard()->set_provider(std::make_unique<ClipboardProviderWin32>());
		clipboard_initialized = true;
	}

	do
	{
		message_received = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (message_received)
		{
			count ++;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	while (message_received);

	// Sync clipboard from system (bidirectional clipboard support)
	if (UIManager::clipboard())
	{
		UIManager::clipboard()->sync_from_system();
	}

	if (count == 0)
	{
		usleep(1);
	}
	return UIManager::desktop()->is_loop();
}

void DeviceWin32::mainloop()
{
	while (dispatch(true));
}

/** Get the size of screen device in pixels 
@return the size */
Size DeviceWin32::size() const
{
	Size result(m_width, m_height);
	return result;
}

/** Set the size of screen device in pixels
@param s size of screen */
void DeviceWin32::size(const Size & s)
{
	size(s.width(), s.height());
}

void DeviceWin32::size(Dim width, Dim height)
{
	RECT client_rect;


	delete m_buffer;
	m_width = width;
	m_height = height;

	m_buffer = new uint8_t [(int)m_width * (int)m_height * 3];
	GetClientRect((HWND)m_hwnd, &client_rect);

	int client_width = client_rect.right - client_rect.left;
	int client_height = client_rect.bottom - client_rect.top;

	RECT window_rect;
	GetWindowRect((HWND)m_hwnd, &window_rect);

	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	SetWindowPos((HWND)m_hwnd, NULL, 0, 0, window_width + (width - client_width), window_height + (height - client_height), SWP_NOMOVE | SWP_NOZORDER);
	UpdateWindow((HWND)m_hwnd);
	clear();
}

/** Get the position of the window
@return the position as a Point */
Point DeviceWin32::position() const
{
	RECT window_rect;
	GetWindowRect((HWND)m_hwnd, &window_rect);
	
	Coord x = static_cast<Coord>(window_rect.left);
	Coord y = static_cast<Coord>(window_rect.top);
	
	Point result(x, y, true);
	return result;
}

/** Set the position of the window
@param p position of the window */
void DeviceWin32::position(const Point & p)
{
	position(p.x(), p.y());
}

/** Move the window
@param x The x position of the window
@param y The y position of the window */
void DeviceWin32::position(Coord x, Coord y)
{
	SetWindowPos((HWND)m_hwnd, NULL, (int)x, (int)y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	UpdateWindow((HWND)m_hwnd);
}

void DeviceWin32::copy(const uint8_t* buffer, Dim width, Dim height)
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
	InvalidateRect((HWND)m_hwnd, NULL, FALSE);
}

void DeviceWin32::blit()
{
	uint8_t* image = (uint8_t*)malloc((int)m_width * (int)m_height * 3);

	if (image)
	{
		HDC hdc = GetDC((HWND)m_hwnd);

		BITMAPINFO bmi = { 0 };
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = m_width;
		bmi.bmiHeader.biHeight = 0 - m_height;  // Negative for top-down display (origin at top left)
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 24;     // 3 bytes per pixel (RGB)
		bmi.bmiHeader.biCompression = BI_RGB;

		for (Dim y = 0; y < m_height; y++) 
		{
			for (Dim x = 0; x < m_width; x++) 
			{
				int32_t pos = ((y * m_width) + x) * 3;
				image[(int)(pos)    ] = m_buffer[(int)(pos) + 2];
				image[(int)(pos) + 1] = m_buffer[(int)(pos) + 1];
				image[(int)(pos) + 2] = m_buffer[(int)(pos)];
			}
		}

		SetDIBitsToDevice(
			hdc,                // Destination HDC
			0,                  // x position of destination
			0,                  // y position of destination
			m_width,            // Width of the image
			m_height,           // Height of the image
			0,                  // x position of upper-left corner of source image
			0,                  // y position of upper-left corner of source image
			0,                  // Starting scan line in the source image
			m_height,           // Number of scan lines to copy
			image,              // Pointer to the RGB image
			&bmi,               // Pointer to the BITMAPINFO structure
			DIB_RGB_COLORS      // Use RGB colors in the image
		);

		ReleaseDC((HWND)m_hwnd, hdc);
		free(image);
	}
}

void DeviceWin32::refresh()
{
	if (UIManager::framebuf()->dirty())
	{
		copy(UIManager::framebuf()->buffer(), UIManager::framebuf()->width(), UIManager::framebuf()->height());
		UIManager::framebuf()->dirty(false);
	}
}

void DeviceWin32::show_console()
{
#if defined(_DEBUG)
	if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole())
	{
		FILE* f_dummy;
		CONSOLE_SCREEN_BUFFER_INFO  screen_info;

		// Allocate console 
		FreeConsole();
		AllocConsole();

		// Set the screen buffer 
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen_info);
		screen_info.dwSize.Y = 32767;

		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), screen_info.dwSize);
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screen_info);

		freopen_s(&f_dummy, "CONIN$",  "r", stdin);
		freopen_s(&f_dummy, "CONOUT$", "w", stderr);
		freopen_s(&f_dummy, "CONOUT$", "w", stdout);
	}
#endif
}

wchar_t get_key(WPARAM w_param, LPARAM l_param)
{
	wchar_t key = 0;

	// Get key value
	switch(w_param)
	{
	case VK_LEFT:   key = (wchar_t)ReservedKey::KEY_LEFT;      break;
	case VK_RIGHT:  key = (wchar_t)ReservedKey::KEY_RIGHT;     break;
	case VK_UP:     key = (wchar_t)ReservedKey::KEY_UP;        break;
	case VK_DOWN:   key = (wchar_t)ReservedKey::KEY_DOWN;      break;
	case VK_RETURN: key = (wchar_t)ReservedKey::KEY_ENTER;     break;
	case VK_ESCAPE: key = (wchar_t)ReservedKey::KEY_ESCAPE;    break;
	case VK_BACK:   key = (wchar_t)ReservedKey::KEY_BACKSPACE; break;
	case VK_DELETE: key = (wchar_t)ReservedKey::KEY_DELETE;    break;
	case VK_HOME:   key = (wchar_t)ReservedKey::KEY_HOME;      break;
	case VK_END:    key = (wchar_t)ReservedKey::KEY_END;       break;
	case VK_PAUSE:  key = (wchar_t)ReservedKey::KEY_PAUSE;     break;

	default:
		{
			WORD ascii=0;
			BYTE kbd_stat[256];

			GetKeyboardState( kbd_stat );

			//Get ascii code
			if( ToAscii((UINT)w_param , (UINT)l_param , kbd_stat, &ascii, 0 ) )
			{
				key = ascii;
			}
		}
	}
	return key;
}

KeyEvent::Modifier get_modifier()
{
	KeyEvent::Modifier modifier = KeyEvent::MODIFIER_NONE;
	if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
		modifier = (KeyEvent::Modifier)(KeyEvent::MODIFIER_SHIFT |modifier); 
	if ((GetAsyncKeyState(VK_MENU) & 0x8000) != 0)
		modifier = (KeyEvent::Modifier)(KeyEvent::MODIFIER_ALT | modifier); 
	if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0)
		modifier = (KeyEvent::Modifier)(KeyEvent::MODIFIER_CONTROL | modifier); 
	return modifier;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
{
	bool treat_event = true;
	wchar_t key;
	DeviceWin32* device = (DeviceWin32*)GetWindowLongPtr(hwnd, GWL_USERDATA);
	static bool mouse_down = false;
	LRESULT result = 0;

	if(device)
	{
		switch (message)
		{
		case WM_PAINT:
			device->blit();
			break;

		case WM_CLOSE:
			UIManager::notifier()->push_event(new KeyEvent((wchar_t)ReservedKey::KEY_EXIT, KeyEvent::KEY_DOWN));
			device->quit();
			treat_event = false;
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			key = get_key(w_param, l_param);
			if(key)
			{
				UIManager::notifier()->push_event(new KeyEvent(key,KeyEvent::KEY_DOWN,get_modifier()));
			}
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			key = get_key(w_param, l_param);
			if(key)
			{
				UIManager::notifier()->push_event(new KeyEvent(key,KeyEvent::KEY_UP,get_modifier()));
			}
			break;

		case WM_MOUSEMOVE:
			{
				Point position((Coord)GET_X_LPARAM(l_param), (Coord)GET_Y_LPARAM(l_param));
				position.adapt_scale();
				UIManager::notifier()->push_event(new TouchEvent(position, TouchEvent::TOUCH_MOVE));
				if (mouse_down)
				{
					// If no button clicked
					if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000 || GetAsyncKeyState(VK_RBUTTON) & 0x8000))
					{
						UIManager::notifier()->push_event(new TouchEvent(position, TouchEvent::TOUCH_UP));
						mouse_down = false;
					}
				}
			}
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			{
				Point position(GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param));
				position.adapt_scale();
				UIManager::notifier()->push_event(new TouchEvent(position, TouchEvent::TOUCH_DOWN));
				mouse_down = true;

				// Capture mouse movements outside the window
				SetCapture(hwnd);
			}
			break;

		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			{
				Point position(GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param));
				position.adapt_scale();
				UIManager::notifier()->push_event(new TouchEvent(position, TouchEvent::TOUCH_UP));
				mouse_down = false;

				// Release mouse capture
				ReleaseCapture();
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		default:
			break;
		}
	}
	else
	{
		//printf("Device not found %s:%d\n",__FILE__,__LINE__);
	}

	if (treat_event)
	{
		result = DefWindowProc(hwnd, message, w_param, l_param);
	}
	return result;
}

namespace berialdraw
{
	uint64_t get_clock()
	{
		LARGE_INTEGER freq, count;

		QueryPerformanceFrequency(&freq);
		if (QueryPerformanceCounter(&count) && freq.QuadPart != 0)
		{
			int64_t result;
			result = (int64_t)((count.QuadPart) * (1000000000 / freq.QuadPart));
			return(result);
		}
		else return(0);
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

		tmp >>= 6; //(/ 64)

		result = (unsigned long)(tmp >> 16);
		result = (result / 15625) << 16;
		tmp = tmp - (result * 15625LL);
		result += ((unsigned long)tmp) / 15625;

		if (neg)
			return(-(long)result);
		return((long)result);
	}
}

