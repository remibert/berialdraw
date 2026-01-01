#include "berialdraw_imp.hpp"
#include "device/device_wayland.hpp"

#include <wayland-client.h>
#include <cstring>

// Add for input handling
#include <linux/input-event-codes.h>

using namespace berialdraw;

namespace {
struct WaylandContext {
	struct wl_display* display = nullptr;
	struct wl_registry* registry = nullptr;
	struct wl_compositor* compositor = nullptr;
	struct wl_surface* surface = nullptr;
	struct wl_seat* seat = nullptr;
	struct wl_pointer* pointer = nullptr;
	struct wl_keyboard* keyboard = nullptr;
	uint8_t* buffer = nullptr;
	Dim width = 0;
	Dim height = 0;
	bool dirty = false;
	bool running = true;
	// Mouse state
	int mouse_x = 0;
	int mouse_y = 0;
	bool mouse_down = false;
};
}

// Forward declarations for listeners
static void pointer_enter(void*, wl_pointer*, uint32_t, wl_surface*, wl_fixed_t, wl_fixed_t) {}
static void pointer_leave(void*, wl_pointer*, uint32_t, wl_surface*) {}
static void pointer_motion(void* data, wl_pointer*, uint32_t, wl_fixed_t sx, wl_fixed_t sy) {
	WaylandContext* ctx = static_cast<WaylandContext*>(data);
	ctx->mouse_x = wl_fixed_to_int(sx);
	ctx->mouse_y = wl_fixed_to_int(sy);
	// Send mouse move event to UIManager
	if (UIManager::notifier()) {
		berialdraw::Point pos(ctx->mouse_x, ctx->mouse_y);
		UIManager::notifier()->push_event(new TouchEvent(pos, TouchEvent::TOUCH_MOVE));
	}
}
static void pointer_button(void* data, wl_pointer*, uint32_t, uint32_t, uint32_t button, uint32_t state) {
	WaylandContext* ctx = static_cast<WaylandContext*>(data);
	if (button == BTN_LEFT) {
		ctx->mouse_down = (state == 1);
		// Send mouse down/up event to UIManager
		if (UIManager::notifier()) {
			berialdraw::Point pos(ctx->mouse_x, ctx->mouse_y);
			UIManager::notifier()->push_event(new TouchEvent(
				pos,
				state == 1 ? TouchEvent::TOUCH_DOWN : TouchEvent::TOUCH_UP
			));
		}
	}
}
static void pointer_axis(void*, wl_pointer*, uint32_t, wl_fixed_t) {}

static const struct wl_pointer_listener pointer_listener = {
	pointer_enter,
	pointer_leave,
	pointer_motion,
	pointer_button,
	pointer_axis
};

static void keyboard_keymap(void*, wl_keyboard*, uint32_t, int32_t, uint32_t) {}
static void keyboard_enter(void*, wl_keyboard*, uint32_t, wl_array*) {}
static void keyboard_leave(void*, wl_keyboard*, uint32_t, wl_array*) {}
static void keyboard_key(void* data, wl_keyboard*, uint32_t, uint32_t, uint32_t key, uint32_t state) {
	WaylandContext* ctx = static_cast<WaylandContext*>(data);
	if (UIManager::notifier()) {
		wchar_t keycode = wayland_reserved_key(key);
		KeyEvent::KeyState keystate = (state == 1) ? KeyEvent::KEY_DOWN : KeyEvent::KEY_UP;
		if (keycode) {
			UIManager::notifier()->push_event(new KeyEvent(keycode, keystate));
		} else {
			// Optionally, handle printable keys (simple fallback)
			if (state == 1 && key >= KEY_A && key <= KEY_Z) {
				UIManager::notifier()->push_event(new KeyEvent((wchar_t)('a' + (key - KEY_A)), keystate));
			}
			if (state == 1 && key >= KEY_1 && key <= KEY_0) {
				static const char num_map[] = {'1','2','3','4','5','6','7','8','9','0'};
				UIManager::notifier()->push_event(new KeyEvent((wchar_t)num_map[key - KEY_1], keystate));
			}
		}
	}
}
static void keyboard_modifiers(void*, wl_keyboard*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) {}
static void keyboard_repeat_info(void*, wl_keyboard*, int32_t, int32_t) {}

static const struct wl_keyboard_listener keyboard_listener = {
	keyboard_keymap,
	keyboard_enter,
	keyboard_leave,
	keyboard_key,
	keyboard_modifiers,
	keyboard_repeat_info
};

static void seat_capabilities(void* data, wl_seat* seat, uint32_t caps) {
	WaylandContext* ctx = static_cast<WaylandContext*>(data);
	if (caps & WL_SEAT_CAPABILITY_POINTER) {
		ctx->pointer = wl_seat_get_pointer(seat);
		wl_pointer_add_listener(ctx->pointer, &pointer_listener, ctx);
	}
	if (caps & WL_SEAT_CAPABILITY_KEYBOARD) {
		ctx->keyboard = wl_seat_get_keyboard(seat);
		wl_keyboard_add_listener(ctx->keyboard, &keyboard_listener, ctx);
	}
}
static void seat_name(void*, wl_seat*, const char*) {}

static const struct wl_seat_listener seat_listener = {
	seat_capabilities,
	seat_name
};

static void registry_global(void* data, wl_registry* registry, uint32_t name, const char* interface, uint32_t) {
	WaylandContext* ctx = static_cast<WaylandContext*>(data);
	if (strcmp(interface, "wl_compositor") == 0) {
		ctx->compositor = (wl_compositor*)wl_registry_bind(registry, name, &wl_compositor_interface, 1);
	} else if (strcmp(interface, "wl_seat") == 0) {
		ctx->seat = (wl_seat*)wl_registry_bind(registry, name, &wl_seat_interface, 1);
		wl_seat_add_listener(ctx->seat, &seat_listener, ctx);
	}
}
static void registry_global_remove(void*, wl_registry*, uint32_t) {}

static const struct wl_registry_listener registry_listener = {
	registry_global,
	registry_global_remove
};

DeviceWayland::DeviceWayland(const char * title, Dim width, Dim height)
	: m_display(nullptr)
	, m_surface(nullptr)
	, m_buffer(nullptr)
	, m_width(width)
	, m_height(height)
	, m_dirty(false)
{
	WaylandContext* ctx = new WaylandContext();
	ctx->display = wl_display_connect(nullptr);
	if (!ctx->display) {
		delete ctx;
		return;
	}
	ctx->registry = wl_display_get_registry(ctx->display);
	wl_registry_add_listener(ctx->registry, &registry_listener, ctx);
	wl_display_roundtrip(ctx->display);

	if (ctx->compositor) {
		ctx->surface = wl_compositor_create_surface(ctx->compositor);
	}
	ctx->width = width;
	ctx->height = height;
	ctx->dirty = false;
	ctx->running = true;
	ctx->buffer = new uint8_t[width * height * 3]();
	m_display = ctx;
}

DeviceWayland::~DeviceWayland()
{
	WaylandContext* ctx = static_cast<WaylandContext*>(m_display);
	if (ctx) {
		if (ctx->pointer) wl_pointer_destroy(ctx->pointer);
		if (ctx->keyboard) wl_keyboard_destroy(ctx->keyboard);
		if (ctx->seat) wl_seat_destroy(ctx->seat);
		if (ctx->buffer) delete[] ctx->buffer;
		if (ctx->surface) wl_surface_destroy(ctx->surface);
		if (ctx->compositor) wl_compositor_destroy(ctx->compositor);
		if (ctx->registry) wl_registry_destroy(ctx->registry);
		if (ctx->display) wl_display_disconnect(ctx->display);
		delete ctx;
	}
	m_display = nullptr;
}

bool DeviceWayland::dispatch(bool blocking)
{
	WaylandContext* ctx = static_cast<WaylandContext*>(m_display);
	if (!ctx || !ctx->display) return false;
	if (blocking) {
		wl_display_dispatch(ctx->display);
	} else {
		wl_display_dispatch_pending(ctx->display);
	}
	return ctx->running;
}

void DeviceWayland::mainloop()
{
	while (dispatch(true)) {
		// Main event loop
	}
}

Size DeviceWayland::size() const
{
	return Size(m_width, m_height);
}

void DeviceWayland::size(const Size & s)
{
	size(s.width(), s.height());
}

void DeviceWayland::size(Dim width, Dim height)
{
	WaylandContext* ctx = static_cast<WaylandContext*>(m_display);
	m_width = width;
	m_height = height;
	if (ctx) {
		if (ctx->buffer) delete[] ctx->buffer;
		ctx->buffer = new uint8_t[width * height * 3]();
		ctx->width = width;
		ctx->height = height;
		ctx->dirty = true;
	}
}

void DeviceWayland::clear()
{
	WaylandContext* ctx = static_cast<WaylandContext*>(m_display);
	if (ctx && ctx->buffer) {
		std::memset(ctx->buffer, 0xFF, ctx->width * ctx->height * 3);
		ctx->dirty = true;
	}
	m_dirty = true;
}

void DeviceWayland::copy(const uint8_t* buffer, Dim width, Dim height)
{
	WaylandContext* ctx = static_cast<WaylandContext*>(m_display);
	if (ctx && ctx->buffer && buffer) {
		Dim copy_width = (width < ctx->width) ? width : ctx->width;
		Dim copy_height = (height < ctx->height) ? height : ctx->height;
		for (Dim y = 0; y < copy_height; y++) {
			const uint8_t* src_row = buffer + (y * width * 3);
			uint8_t* dst_row = ctx->buffer + (y * ctx->width * 3);
			std::memcpy(dst_row, src_row, copy_width * 3);
		}
		ctx->dirty = true;
	}
	m_dirty = true;
}

void DeviceWayland::refresh()
{
	// No framebuffer integration in this stub
}

void DeviceWayland::blit()
{
	WaylandContext* ctx = static_cast<WaylandContext*>(m_display);
	if (ctx && ctx->dirty) {
		// In a real implementation, you would create a wl_buffer and attach it to the surface
		// Here, we just reset the dirty flag
		ctx->dirty = false;
	}
	m_dirty = false;
}

void DeviceWayland::quit()
{
	WaylandContext* ctx = static_cast<WaylandContext*>(m_display);
	if (ctx) {
		ctx->running = false;
	}
}

static wchar_t wayland_reserved_key(uint32_t key)
{
	// Map evdev keycodes to ReservedKey values
	switch (key)
	{
		case KEY_ENTER:      return (wchar_t)ReservedKey::KEY_ENTER;
		case KEY_ESC:        return (wchar_t)ReservedKey::KEY_ESCAPE;
		case KEY_BACKSPACE:  return (wchar_t)ReservedKey::KEY_BACKSPACE;
		case KEY_DELETE:     return (wchar_t)ReservedKey::KEY_DELETE;
		case KEY_RIGHT:      return (wchar_t)ReservedKey::KEY_RIGHT;
		case KEY_LEFT:       return (wchar_t)ReservedKey::KEY_LEFT;
		case KEY_DOWN:       return (wchar_t)ReservedKey::KEY_DOWN;
		case KEY_UP:         return (wchar_t)ReservedKey::KEY_UP;
		case KEY_HOME:       return (wchar_t)ReservedKey::KEY_HOME;
		case KEY_END:        return (wchar_t)ReservedKey::KEY_END;
		case KEY_PAUSE:      return (wchar_t)ReservedKey::KEY_PAUSE;
		case KEY_TAB:        return '\t';
		case KEY_SPACE:      return ' ';
		default:             return 0;
	}
}
