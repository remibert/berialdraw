#pragma once

namespace berialdraw
{
	/** Win32 implementation of ClipboardProvider */
	class ClipboardProviderWin32 : public ClipboardProvider
	{
	public:
		/** Constructor */
		ClipboardProviderWin32();

		/** Destructor */
		virtual ~ClipboardProviderWin32();

		/** Set text to clipboard */
		virtual void set_text(const String & text);

		/** Get text from clipboard */
		virtual bool get_text(String & text) const;

		/** Check if clipboard has changed */
		virtual bool has_changed() const;

		/** Update clipboard listener */
		virtual void update_listener();

	private:
		DWORD m_last_sequence_number;	/** Track clipboard sequence number for change detection */
		String m_last_content;			/** Cache last known content as fallback for change detection */
	};
}
