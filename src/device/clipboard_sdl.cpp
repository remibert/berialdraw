#include "berialdraw_imp.hpp"
#include "device/clipboard_sdl.hpp"
#include <SDL3/SDL.h>

using namespace berialdraw;

/** Constructor */
berialdraw::ClipboardProviderSDL::ClipboardProviderSDL()
{
	// Initialize with current clipboard content
	get_system_clipboard_text(m_last_clipboard_content);
}

/** Destructor */
berialdraw::ClipboardProviderSDL::~ClipboardProviderSDL()
{
}

/** Set text to clipboard */
void berialdraw::ClipboardProviderSDL::set_text(const String & text)
{
	SDL_SetClipboardText((const char *)text);
}

/** Get text from system clipboard */
bool berialdraw::ClipboardProviderSDL::get_text(String & text) const
{
	bool success = false;

	const char * clipboard_text = SDL_GetClipboardText();
	if (clipboard_text != nullptr)
	{
		if (clipboard_text[0] != '\0')
		{
			text = clipboard_text;
			success = true;
		}
		SDL_free((void*)clipboard_text);
	}
	
	return success;
}

/** Check if system clipboard has changed since last read */
bool berialdraw::ClipboardProviderSDL::has_changed() const
{
	String current_content;
	get_system_clipboard_text(const_cast<String&>(current_content));
	return current_content != m_last_clipboard_content;
}

/** Update clipboard listener */
void berialdraw::ClipboardProviderSDL::update_listener()
{
	// Update cached clipboard content
	get_system_clipboard_text(m_last_clipboard_content);
}
