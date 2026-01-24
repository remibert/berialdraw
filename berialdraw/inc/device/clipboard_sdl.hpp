#pragma once

namespace berialdraw
{
	/** SDL implementation of ClipboardProvider */
	class ClipboardProviderSDL : public ClipboardProvider
	{
	public:
		/** Constructor */
		ClipboardProviderSDL();

		/** Destructor */
		virtual ~ClipboardProviderSDL();

		/** Set text to clipboard */
		virtual void set_text(const String & text);

		/** Get text from clipboard */
		virtual bool get_text(String & text) const;

		/** Check if clipboard has changed */
		virtual bool has_changed() const;

		/** Update clipboard listener */
		virtual void update_listener();

	private:
		String m_last_clipboard_content;	/** Cache last clipboard content for change detection */
	};
}
