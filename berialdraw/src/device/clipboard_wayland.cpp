#include "berialdraw_imp.hpp"
#include "device/clipboard_wayland.hpp"

using namespace berialdraw;

/** Constructor */
berialdraw::ClipboardProviderWayland::ClipboardProviderWayland()
{
	// TODO: Implement Wayland clipboard using wl_data_device
}

/** Destructor */
berialdraw::ClipboardProviderWayland::~ClipboardProviderWayland()
{
}

/** Set text to clipboard */
void berialdraw::ClipboardProviderWayland::set_text(const String & text)
{
	// TODO: Implement using wl_data_device_manager and wl_data_source
	// For now, this is a stub
}

/** Get text from system clipboard */
bool berialdraw::ClipboardProviderWayland::get_text(String & text) const
{
	// TODO: Implement using wl_data_device_manager
	// For now, this is a stub
	return false;
}

/** Check if system clipboard has changed since last read */
bool berialdraw::ClipboardProviderWayland::has_changed() const
{
	// TODO: Implement proper change detection when Wayland support is added
	return false;
}

/** Update clipboard listener */
void berialdraw::ClipboardProviderWayland::update_listener()
{
	// TODO: Implement when Wayland support is added
}
