#pragma once

namespace berialdraw
{
	/** Wayland implementation of ClipboardProvider */
	class ClipboardProviderWayland : public ClipboardProvider
	{
	public:
		/** Constructor */
		ClipboardProviderWayland();

		/** Destructor */
		virtual ~ClipboardProviderWayland();

		/** Set text to clipboard */
		virtual void set_text(const String & text);

		/** Get text from clipboard */
		virtual bool get_text(String & text) const;

		/** Check if clipboard has changed */
		virtual bool has_changed() const;

		/** Update clipboard listener */
		virtual void update_listener();

	private:
		String m_cached_text;	/** Cache for clipboard content */
	};
}
