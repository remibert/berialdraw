#include "berialdraw_imp.hpp"
#include "device/clipboard_xcb.hpp"

#include <xcb/xcb.h>
#include <cstring>
#include <cstdlib>
#include <time.h>

using namespace berialdraw;

/** Constructor */
ClipboardProviderXcb::ClipboardProviderXcb(void* connection, uint32_t window)
	: m_connection(connection)
	, m_window(window)
	, m_clipboard_atom(0)
	, m_utf8_string_atom(0)
	, m_targets_atom(0)
	, m_property_atom(0)
	, m_owns_clipboard(false)
{
	xcb_connection_t* conn = (xcb_connection_t*)m_connection;
	
	// Get CLIPBOARD atom
	xcb_intern_atom_cookie_t clipboard_cookie = xcb_intern_atom(conn, 0, 9, "CLIPBOARD");
	xcb_intern_atom_reply_t* clipboard_reply = xcb_intern_atom_reply(conn, clipboard_cookie, nullptr);
	if (clipboard_reply)
	{
		m_clipboard_atom = clipboard_reply->atom;
		free(clipboard_reply);
	}
	
	// Get UTF8_STRING atom
	xcb_intern_atom_cookie_t utf8_cookie = xcb_intern_atom(conn, 0, 11, "UTF8_STRING");
	xcb_intern_atom_reply_t* utf8_reply = xcb_intern_atom_reply(conn, utf8_cookie, nullptr);
	if (utf8_reply)
	{
		m_utf8_string_atom = utf8_reply->atom;
		free(utf8_reply);
	}
	
	// Get TARGETS atom
	xcb_intern_atom_cookie_t targets_cookie = xcb_intern_atom(conn, 0, 7, "TARGETS");
	xcb_intern_atom_reply_t* targets_reply = xcb_intern_atom_reply(conn, targets_cookie, nullptr);
	if (targets_reply)
	{
		m_targets_atom = targets_reply->atom;
		free(targets_reply);
	}
	
	// Get property atom for data transfer
	xcb_intern_atom_cookie_t prop_cookie = xcb_intern_atom(conn, 0, 13, "BERIALDRAW_CB");
	xcb_intern_atom_reply_t* prop_reply = xcb_intern_atom_reply(conn, prop_cookie, nullptr);
	if (prop_reply)
	{
		m_property_atom = prop_reply->atom;
		free(prop_reply);
	}
	
	// Initialize content cache
	get_text(m_last_content);
}

/** Destructor */
ClipboardProviderXcb::~ClipboardProviderXcb()
{
}

/** Set text to clipboard */
void ClipboardProviderXcb::set_text(const String & text)
{
	xcb_connection_t* conn = (xcb_connection_t*)m_connection;
	
	// Store the content we want to provide
	m_owned_content = text;
	m_owns_clipboard = true;
	
	// Take ownership of the clipboard
	xcb_set_selection_owner(conn, m_window, m_clipboard_atom, XCB_CURRENT_TIME);
	xcb_flush(conn);
	
	// Verify we got ownership
	xcb_get_selection_owner_cookie_t owner_cookie = xcb_get_selection_owner(conn, m_clipboard_atom);
	xcb_get_selection_owner_reply_t* owner_reply = xcb_get_selection_owner_reply(conn, owner_cookie, nullptr);
	
	if (owner_reply)
	{
		if (owner_reply->owner != m_window)
		{
			m_owns_clipboard = false;
		}
		free(owner_reply);
	}
}

/** Get text from clipboard */
bool ClipboardProviderXcb::get_text(String & text) const
{
	xcb_connection_t* conn = (xcb_connection_t*)m_connection;
	
	// Check if we own the clipboard
	xcb_get_selection_owner_cookie_t owner_cookie = xcb_get_selection_owner(conn, m_clipboard_atom);
	xcb_get_selection_owner_reply_t* owner_reply = xcb_get_selection_owner_reply(conn, owner_cookie, nullptr);
	
	if (owner_reply)
	{
		if (owner_reply->owner == m_window && m_owns_clipboard)
		{
			// We own the clipboard, return our content
			text = m_owned_content;
			free(owner_reply);
			return !m_owned_content.empty();
		}
		free(owner_reply);
	}
	
	// Request the clipboard content from the owner
	xcb_convert_selection(conn, m_window, m_clipboard_atom, m_utf8_string_atom, 
	                      m_property_atom, XCB_CURRENT_TIME);
	xcb_flush(conn);
	
	// Wait for selection notify event (with timeout)
	bool success = false;
	int timeout = 100; // 100ms timeout
	
	while (timeout > 0)
	{
		xcb_generic_event_t* event = xcb_poll_for_event(conn);
		if (event)
		{
			uint8_t response_type = event->response_type & ~0x80;
			if (response_type == XCB_SELECTION_NOTIFY)
			{
				xcb_selection_notify_event_t* sel = (xcb_selection_notify_event_t*)event;
				if (sel->property != XCB_NONE)
				{
					// Get the property value
					xcb_get_property_cookie_t prop_cookie = xcb_get_property(
						conn, 1, m_window, m_property_atom, XCB_ATOM_ANY, 0, 1024);
					xcb_get_property_reply_t* prop_reply = xcb_get_property_reply(conn, prop_cookie, nullptr);
					
					if (prop_reply)
					{
						if (xcb_get_property_value_length(prop_reply) > 0)
						{
							char* data = (char*)xcb_get_property_value(prop_reply);
							int len = xcb_get_property_value_length(prop_reply);
							text = String(data, len);
							success = true;
						}
						free(prop_reply);
					}
				}
				free(event);
				break;
			}
			free(event);
		}
		
		// Small delay
		struct timespec ts = { 0, 1000000 }; // 1ms
		nanosleep(&ts, nullptr);
		timeout--;
	}
	
	return success;
}

/** Check if clipboard has changed */
bool ClipboardProviderXcb::has_changed() const
{
	String current_content;
	get_text(const_cast<String&>(current_content));
	return current_content != m_last_content;
}

/** Update clipboard listener */
void ClipboardProviderXcb::update_listener()
{
	get_text(m_last_content);
}
